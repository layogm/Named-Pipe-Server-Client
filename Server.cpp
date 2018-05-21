#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "datatypes.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <thread>

int client_to_server;
int server_to_client;
std::unordered_map<int,Employee> allEmployees;


#define FIFO_FILE_1  "/tmp/client_to_server_fifo"
#define FIFO_FILE_2  "/tmp/server_to_client_fifo"

#define MBUFSIZ 5000

std::string DONE = "Done";

std::vector<int> lastfibs;
std::vector<int> curfibs;

void readData(void* buf, int length = MBUFSIZ);
void writeData(void* buf, int length = MBUFSIZ);
void getEmplyeeDetailFromClient(Employee& employee);
void editEmplyeeDetailFromClient();
void sendEmplyeeDetailFromClient();
void startFibonacci();
void getCurrentFib();
void deleteEmployee();
int getEmployeeId();

int main()
{
    char buf[MBUFSIZ];

    /* create the FIFO (named pipe) */
    mkfifo(FIFO_FILE_1, 0666);
    mkfifo(FIFO_FILE_2, 0666);

    printf("Server Started...\n");

    /* open, read, and display the message from the FIFO */
    client_to_server = open(FIFO_FILE_1, O_RDONLY);
    server_to_client = open(FIFO_FILE_2, O_WRONLY);

    while (1)
    {

        std::string client_choices = "Choose an option: \n1)Create new Employee\n2)Edit Employee Data\n3)Retrieve Employee\n4)Delete Employee\n5)Start New Fibinacci Series\n6)Get Current Fibonacci\n0)Exit";
        // write(server_to_client, client_choices.c_str(), client_choices.length());
        writeData((char*)client_choices.c_str(), client_choices.length());;

        read(client_to_server, buf, MBUFSIZ);
        int choice = atoi(buf);

        std::cout << "Option choosen by client : "<<choice<<std::endl;
        if (choice == 1)
        {
            Employee employee;
            getEmplyeeDetailFromClient(employee);
            employee.PrintEmployeeDetails();
            allEmployees.emplace(employee.getId(),employee);
            writeData((char*)DONE.c_str(),DONE.length());
        }
        else if (choice == 2)
        {
            editEmplyeeDetailFromClient();
            writeData((char*)DONE.c_str(),DONE.length());
        }
        else if (choice == 3)
        {
            sendEmplyeeDetailFromClient();
        }
        else if (choice == 4)
        {
            deleteEmployee();
            writeData((char*)DONE.c_str(),DONE.length());
        }
        else if(choice == 5)
        {
            std::string response = "Starting new fibonnaci series with id : "+std::to_string(lastfibs.size());
            writeData((char*)response.c_str());
            std::thread asyncCalls(startFibonacci);
        }
        else if(choice == 6)
        {
            getCurrentFib();
        }
        else if (choice == 0)
        {
            printf("Server OFF.\n");
            break;
        }

    }

    close(client_to_server);
    close(server_to_client);

    unlink(FIFO_FILE_1);
    unlink(FIFO_FILE_2);
    return 0;
}

void getEmplyeeDetailFromClient(Employee& employee)
{
    char buf[MBUFSIZ];
    std::string client_choices = "Please Enter employee id : ";
    writeData((char*)client_choices.c_str(), client_choices.length());;
    readData(buf);
    employee.setId(atoi(buf));

    client_choices = "Please Enter employee name : ";
    writeData((char*)client_choices.c_str(), client_choices.length());;
    readData(buf);
    employee.setName(std::string(buf));

    client_choices = "Please Enter employee designation : ";
    writeData((char*)client_choices.c_str(), client_choices.length());;
    readData(buf);
    employee.setDesignation(std::string(buf));

    client_choices = "Please Enter employee Salary : ";
    writeData((char*)client_choices.c_str(), client_choices.length());;
    readData(buf);
    employee.setSalary(atoi(buf));
}

void editEmplyeeDetailFromClient()
{
    int id = getEmployeeId();
    if(id == -1 )
        return;
    

    char buf[MBUFSIZ];
    Employee& employee = allEmployees.at(id);
    int choice = -1;
    while(1)
    {
        std::string client_choices = "Choose an option :\n0)Done Changes\n1)Change name\n2)Change Designation\n3)Change Salary";
        writeData((char*)client_choices.c_str(), client_choices.length());;
        readData(buf);
        choice = atoi(buf);

        if(choice == 0)
        {
            break;
        }
        else if(choice == 1)
        {
            client_choices = "Please Enter employee's new name : ";
            writeData((char*)client_choices.c_str(), client_choices.length());;
            readData(buf);
            employee.setName(std::string(buf));
        }
        else if(choice == 2)
        {
            client_choices = "Please Enter employee's new designation : ";
            writeData((char*)client_choices.c_str(), client_choices.length());;
            readData(buf);
            employee.setDesignation(std::string(buf));
        }
        else if(choice == 3)
        {
            client_choices = "Please Enter employee's new Salary : ";
            writeData((char*)client_choices.c_str(), client_choices.length());;
            readData(buf);
            employee.setSalary(atoi(buf));
        }
        
    }
    employee.PrintEmployeeDetails();
}

void sendEmplyeeDetailFromClient()
{
    int employee_id = getEmployeeId();
    std::cout<<"Id : "<<employee_id<<std::endl;

    if(employee_id>0)
    {
        Employee& employee = allEmployees.at(employee_id);
        writeData(&employee, sizeof(Employee));
    }
}

void deleteEmployee()
{
    int employee_id = getEmployeeId();
    if(employee_id>0)
    {
        std::unordered_map<int, Employee>::iterator it = allEmployees.find(employee_id);
        allEmployees.erase(it);
    }
}


void startFibonacci()
{
    int position = lastfibs.size();
    lastfibs.push_back(1);
    curfibs.push_back(1);
    int& lastfib = lastfibs.at(position);
    int& curfib = curfibs.at(position);
    sleep(2);
    while(true)
    {
        int tmp = curfib;
        curfib += lastfib;
        lastfib = tmp;
        sleep(1);
    }
}

void getCurrentFib()
{
    char buf[MBUFSIZ];
    std::string client_choices = "Please Enter Series id : ";
    writeData((char*)client_choices.c_str(), client_choices.length());
    readData(buf);
    int id = atoi(buf);

    std::string response = "";
    while(id > (int) lastfibs.size()-1 && id != -1)
    {
        response = "Incorrect id. Enter correct id (or -1 to go back) : ";
        writeData((char*)response.c_str(), response.length());;
        readData(buf);
        id = atoi(buf);
    }

    if(id > 0)
    {
        response = std::to_string(curfibs.at(id));
    }
    
    writeData((char*)response.c_str(),response.length());

}

int getEmployeeId()
{
    char buf[MBUFSIZ];
    std::string client_choices = "Please Enter Employee id : ";
    writeData((char*)client_choices.c_str(), client_choices.length());;
    readData(buf);
    int id = atoi(buf);
    std::string response = "";
    while(allEmployees.find(id) == allEmployees.end() && id != -1)
    {
        response = "Incorrect id. Enter correct id (or -1 to go back) : ";
        writeData((char*)response.c_str(), response.length());;
        readData(buf);
        id = atoi(buf);
        memset(buf, 0, MBUFSIZ);        
    }

    return id;
}


void readData(void* buf, int length)
{
    /* clean buf from any data */
    memset(buf, 0, length);
    if(read(client_to_server,buf,MBUFSIZ) < 0){
        perror("Read:"); //error check
        exit(-1);
    }    
}

void writeData(void* buf, int length)
{
    if(write(server_to_client, buf, length) < 0)
    {
        perror("Write:");//print error
        exit(-1);
    }    
    /* clean buf from any data */
    memset(buf, 0, MBUFSIZ);
}
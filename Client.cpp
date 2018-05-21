#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include "datatypes.h"

#define FIFO_FILE_1  "/tmp/client_to_server_fifo"
#define FIFO_FILE_2  "/tmp/server_to_client_fifo"

#define MBUFSIZ 5000

int client_to_server;
int server_to_client;

void newEmployee();
void readData(void* buf);
void writeData(void* buf);
int getEmployeeId();
void retreiveEmployeeDetails();

int main()
{
    char str[MBUFSIZ];

    /* write str to the FIFO */
    client_to_server = open(FIFO_FILE_1, O_WRONLY);
    server_to_client = open(FIFO_FILE_2, O_RDONLY);

    while(1)
    {       
        // std::string client_choices = "Choose an option: \n1)Create new Employee\n2)Edit Employee Data\n3)Retrieve Employee\n4)Delete Employee\n5)Start Fibinacci Series\n6)Get Current Fibonacci\n0)Exit\n";
        if(read(server_to_client,str, MBUFSIZ) < 0){
            perror("Read:"); //error check
            exit(-1);
        }
        printf("%s",str); 
        memset(str, 0, MBUFSIZ);
        std::cin>>str;
        if(write(client_to_server, str,MBUFSIZ) < 0)
        {
            perror("Write:");//print error
            exit(-1);
        }
        int choice = atoi(str);

        std::cout<<"Choice : "<<choice<<std::endl;
        if (choice == 1)
        {
            newEmployee();
        }
        else if (choice == 2)
        {
            int id = getEmployeeId();
            while(1)
            {
                readData(str);
                if(std::strcmp(str, "Done") == 0)
                {
                    break;
                }
                std::cout<<str;
                /* clean buf from any data */
                memset(str, 0, MBUFSIZ);
                std::cin>>str;
                id = atoi(str);
                writeData(str);
            }
        }
        else if (choice == 3)
        {
            retreiveEmployeeDetails();
        }
        else if (choice == 4)
        {
            // int id = 0;
            while(1)
            {
                readData(str);
                if(std::strcmp(str, "Done") == 0)
                {
                    break;
                }
                std::cout<<str;
                /* clean buf from any data */
                memset(str, 0, MBUFSIZ);
                std::cin>>str;
                writeData(str);
            }

        }
        else if(choice == 5)
        {
            readData(str);
            std::cout<<str;
        }
        else if(choice == 6)
        {
            while(1)
            {
                readData(str);
                if(std::strstr(str, "Incorrect") == nullptr)
                {
                    break;
                }
                std::cout<<str;
                /* clean buf from any data */
                memset(str, 0, MBUFSIZ);
                std::cin>>str;
                writeData(str);
            }
        }
        else if (choice == 0)
        {
            printf("Client OFF.\n");
            break;
        }


    }

    close(client_to_server);
    close(server_to_client);

   /* remove the FIFO */

   return 0;
}


void newEmployee()
{
    char str[MBUFSIZ];
    while(1)
    {
        readData(str);
        if(std::strcmp(str,"Done") == 0 )
        {
            break;
        }
        std::cout<<str;
        /* clean buf from any data */
        memset(str, 0, MBUFSIZ);
        scanf("%s",str);
        // std::cin>>str;
        writeData(str);
    }
    std::cout<<"Employee Data Added..";
}

int getEmployeeId()
{
    char str[MBUFSIZ];
    int id = 0;
    while(1)
    {
        readData(str);
        if(std::strcmp(str, "Done") == 0)
        {
            break;
        }
        std::cout<<str;
        /* clean buf from any data */
        memset(str, 0, MBUFSIZ);
        std::cin>>id;
        writeData(&id);
    }
    return id;
}


void retreiveEmployeeDetails()
{
    int employee_id = getEmployeeId();
    std::cout<<"Id : "<<employee_id<<std::endl;
    if(employee_id == -1)
        return;
    writeData(&employee_id);
    char str[MBUFSIZ];
    readData(str);
    Employee* employee = (Employee*) str;
    printf("\nEmployee received from the server: ");
    employee->PrintEmployeeDetails();
}

void readData(void* buf)
{
    /* clean buf from any data */
    memset(buf, 0, MBUFSIZ);
    if(read(server_to_client,buf,MBUFSIZ) < 0){
        perror("Read:"); //error check
        exit(-1);
    }    
}

void writeData(void* buf)
{
    if(write(client_to_server, buf, MBUFSIZ) < 0)
    {
        perror("Write:");//print error
        exit(-1);
    }    
    /* clean buf from any data */
    memset(buf, 0, MBUFSIZ);
}
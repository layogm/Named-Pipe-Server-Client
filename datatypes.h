#include <cstring>
#include <iostream>

class Employee
{
private:
    /* data */
    int id;
    std::string name;
    int salary;
    std::string designation;


public:
Employee(/* args */)
{
}

~Employee()
{
}

int getId()
{
    return id;
}

std::string getName()
{
    return name;    
}

int getSalary()
{
    return salary;
}

std::string getDesignation()
{
    return designation;
}



void setId(int new_id)
{
    id = new_id;
}

void setName(std::string new_name)
{
    name = new_name;    
}

void setSalary(int new_salary)
{
    salary = new_salary;
}

void setDesignation(std::string new_designation)
{
    designation = new_designation;
}


void PrintEmployeeDetails()
{
    std::cout<<"Id : "<< id <<", Name : "<<name << ", Designation : "<< designation <<", Salary : "<<salary << std::endl;
}

};



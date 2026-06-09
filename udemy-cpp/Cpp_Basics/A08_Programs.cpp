/*
    Author: Aryan Singh
    Topic: Practice Programs
*/
/*
P1) WAP to calculate Area of Circle.(Take radius as input)
//Formula -> area = PI*r^2

P2) WAP to calculate Net Salary.(Inputs: Basic Salary, Percentage of allowances, Percentage of Deduction)
//Formula ->
Basic Salary + (basic salary*Percentage of allowances)/100 - (basic salary*Percentage of Deduction)/100
*/
#include<iostream>

using namespace std;

int main()
{
    //P1 - Area of a circle
    float radius, area_of_Circle;
    cout << "Enter Radius: ";
    cin >> radius;

    area_of_Circle = 3.1425f*radius*radius; 
    cout << "Area of Circle is: " << area_of_Circle << endl;

    //P2 - Net Salary
    int Basic_Salary, Percentage_allowances, Percentage_Deduction;
    double Net_Salary;

    cout << "Enter Basic Salary: ";
    cin >> Basic_Salary;

    cout << "Enter Percentage Allowances: ";
    cin >> Percentage_allowances;

    cout << "Enter Percentage Deduction: ";
    cin >> Percentage_Deduction;

    Net_Salary = Basic_Salary + ((Basic_Salary*Percentage_allowances)/100) - ((Basic_Salary*Percentage_Deduction)/100);

    cout << "Net Salary is: " << Net_Salary << endl;

    return 0;
}
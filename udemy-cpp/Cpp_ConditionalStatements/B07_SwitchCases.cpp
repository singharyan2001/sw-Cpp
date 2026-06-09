/*
    Author:
    Topic:
*/
#include<iostream>
#include<stdint.h>

using namespace std;

void DayName(int day);

int main()
{
    DayName(5);
    return 0;
}

void DayName(int day)
{
    //Display day using switch
    switch(day){
        case 1:
            cout<<"sun";
            break;
        case 2:
            cout<<"mon";
            break;
        case 3:
            cout<<"tue";
            break;
        case 4:
            cout<<"wed";
            break;
        case 5:
            cout<<"thu";
            break;
        case 6:
            cout<<"fri";
            break;
        case 7:
            cout<<"sat";
            break;
        default:
        cout<<"unknown";
    }
}
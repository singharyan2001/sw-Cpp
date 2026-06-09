/*
    Author:
    Topic: Else if
*/
#include<iostream>
using namespace std;

int main()
{
    int day;
    cout << "Enter day number: ";
    cin >> day;
    if(day == 1){
        cout << "Monday\n" << endl;
    }
    else if(day == 2){
        cout << "Tuesday\n" << endl;
    }
    else if(day == 3){
        cout << "Wednesday\n" << endl;
    }
    else if(day == 4){
        cout << "Thursday\n" << endl;
    }
    else if(day == 5){
        cout << "Friday\n" << endl;
    }
    else if(day == 6){
        cout << "Saturday\n" << endl;
    }
    else if(day == 7){
        cout << "Sunday\n" << endl;
    }
    else{
        cout << "Data imputed after scope" << endl;
    }
    return 0;
}
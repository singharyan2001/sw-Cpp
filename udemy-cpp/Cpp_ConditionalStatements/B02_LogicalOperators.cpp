/*
    Author: Aryan Singh
    Topic: Logical Operators
            -> && (AND), || (OR), ! (NOT)
*/
#include<iostream>

using namespace std;

int main()
{
    //Compound statements
    //Working or Leisure Hour
    int WorkingHour_start = 9;
    int WorkingHour_End = 18;
    int Hour;

    cout << "Enter Current Hour: ";
    cin >> Hour;

    if((Hour >= WorkingHour_start) && (Hour <= WorkingHour_End)){
        cout << "Working" << endl;
    }
    else{
        cout << "Leisure" << endl;
    }

    //Check valid age range
    int age_check1 = 12;
    int age_check2 = 50;
    int min_age = 0;
    int max_age = 110;
    int age;

    cout << "Enter Age: ";
    cin >> age;

    //Check if young
    if((age >= min_age) && (age <= max_age)){
        if((age >= 12) && (age <= 50)){
            cout << "Young" << endl;
        }
        else{
            cout << "Not Young" << endl;
        }
    }
    else{
        cout << "Not Valid" << endl;
    }

    //Check for Eligibility   
    if((age >= min_age) && (age <= max_age)){
        if((age < 12) || (age > 50)){
            cout << "Eligible" << endl;
        }
        else{
            cout << "Not Eligible" << endl;;
        }
    }
    else{
        cout << "Not Valid" << endl;
    }

    return 0;
}
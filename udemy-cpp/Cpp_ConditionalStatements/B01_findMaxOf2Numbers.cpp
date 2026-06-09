/*
    Author: Aryan Singh
    Topic: Conditional Statements
    WAP to: 
        - find the maximum of 2 numbers.
        - check if number is +ve or -ve.
        - check if even or odd.
        - if a & b are divided, check if denominator is not 0.
*/
#include<iostream>

using namespace std;

int main()
{
    int a, b;
    cout << "Enter two numbers a & b: ";
    cin >> a >> b;

    //Condition to check if the denominator is Zero.
    if(b == 0){
        cout << "Division by Zero" << endl;
    }
    else{
        cout << "Condition checked! No Issues found. Continue!" << endl;
    }

    //Condition to check which number is Greater.
    if(a>b){
        cout << "Max Number is: " << a << endl;

        //Condition to check number is +ve or -ve
        if(a >= 0){
            cout << "positive" << endl;
        }
        else{
            cout << "negative" << endl;
        }

         //Condition to check even or odd??
        if(a%2 == 0){
            cout << "even" << endl;
        }
        else{
            cout << "odd" << endl;
        }
    }
    else{
        cout << "Max Number is: " << b << endl;

        //Condition to check number is +ve or -ve??
        if(b >= 0){
            cout << "positive" << endl;
        }
        else{
            cout << "negative" << endl;
        }
        
        //Condition to check even or odd??
        if(b%2 == 0){
            cout << "even" << endl;
        }
        else{
            cout << "odd" << endl;
        }
    }

    return 0;
}
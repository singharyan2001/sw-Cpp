/*
    Author:
    Topic: short circuit mechanism && Dynamic Declaration
*/
#include<iostream>
using namespace std;
int main()
{
    /*Short Circuit Mechanism*/
    int a = 15;
    int b = 12;
    int c = 17;
    
    if(a > b && a > c){
        cout  <<"Execute!" << endl;
    }

    if(a > b || a > c){
        //
        cout <<"Execute!!"<<endl;
    }

    /*Dynamic Declaration*/
    if(int k = a; k > b){
        cout << "It works!" <<endl;
    }
    else{
        cout << "It doesn't works!" <<endl;
    }
    return 0;
}
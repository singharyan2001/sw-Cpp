/*
    Author: Aryan Singh
    Topic: 
    Compound Assignment operator
        -> +=, -=, *=, /=, %=, &=, |=, <<=, >>=
    Increment & Decrement operators
        -> x++, x--, ++x, --x
    Overflow Concept
*/
#include<iostream>
using namespace std;
int main()
{
    int a = 10, b = 5, c = 15;
    int sum = 5;
    sum += a+b+c;
    cout << "+= method -> " << sum << endl;

    int prod = 5;
    prod *= a*b*c;
    cout << "*= method -> " << prod << endl;

    //Increment and Decrement operators: 
    int xx = 5, xx1 = 5, yy = 10, yy1 =10, z1, z2;
    z1 = ++xx * yy;
    cout << "Pre Increment -> " << z1 << endl;

    z2 = xx++ *yy;
    cout << "Post Increment -> " << z1 << endl;

    //have same effect if used together or at once

    //Overflow:
    char x3 = 127;
    x3++;
    cout << (int)x3 << endl;

    return 0;
}
/*
    Author: Aryan Singh
    Topic: Enums and Typedef
*/
/*
- Enum is used if you want to declare a set of constants, and have the ability to assign these constants with a value or code.
- Enum is used for defining a group of constants under one name that is related constants under one name.
- Typedef is used for defining our own datatype or giving alias to some data type so that we can make the program more readable.
*/

#include<iostream>

using namespace std;

//enum day {mon, tue, wed, thu, fri, sat, sun};
enum day {mon = 10, tue, wed, thu = 17, fri, sat, sun};
enum dept {CS = 0x03, ECE = 0x80, IT = 0xB1, CIVIL = 0xEE, MECH = 0xFF}; // so you can assign hex codes

typedef int marks, rollno;  //these are integer type variables.

int main()
{
    day d, d1;
    d = mon;
    d1 = fri;
    cout << d << endl;
    cout << d1 << endl;
    cout << wed << endl;
    cout << endl;
    cout << mon << endl;
    cout << tue << endl;
    cout << wed << endl;
    cout << thu << endl;
    cout << fri << endl;
    cout << sat << endl;
    cout << sun << endl;

    dept A, A1;
    A = ECE;
    cout << A << endl;
    cout << endl;
    cout << CS << endl;
    cout << ECE << endl;
    cout << IT << endl;
    cout << CIVIL << endl;
    cout << MECH << endl;

    A1 = MECH;
    int value = 0xff;
    if(A1 == value){
        cout << "OxFF received" << endl;
    }

    marks m1,m2,m3;
    rollno r1,r2,r3;
    m1 = 90;
    r1 = 10;

    m2 = 44;
    r2 = 11;

    m3 = 98;
    r3 = 12; 

    cout << r1 << " " << m1 << endl;
    return 0;
}
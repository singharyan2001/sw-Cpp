/*
    Author: Aryan Singh
    Topic: Data Types, size, range of data types and Variables.
*/
#include<iostream>

using namespace std;

int main()
{
    //Data Types in Cpp (Examples)
    int a;
    float c;
    double d;
    long double aa;
    unsigned int q;
    long int xx;
    long long int w;
    bool e;
    char r1;
    wchar_t r2;
    char16_t r3;
    char32_t r4;

    cout << "int: " << sizeof(a) << endl;
    cout << "float: " << sizeof(c) << endl;
    cout << "double: " << sizeof(d) << endl;
    cout << "long double: " << sizeof(aa) << endl;
    cout << "unsigned int: " << sizeof(q) << endl;
    cout << "long int: " << sizeof(xx) << endl;
    cout << "long long int: " << sizeof(w) << endl;
    cout << "bool: " << sizeof(e) << endl;
    cout << "char: " << sizeof(r1) << endl;
    cout << "wchar_t: " << sizeof(r2) << endl;
    cout << "char16_t: " << sizeof(r3) << endl;
    cout << "char32_t: " << sizeof(r4) << endl;
    cout << "string: " << sizeof(string) << endl;

    //Built in Constants in Cpp
    cout << CHAR_MIN << " to " << CHAR_MAX << endl;
    cout << UCHAR_MAX << endl;
    cout << INT_MIN << " to " << INT_MAX << endl;
    cout << INT8_MIN << " to " << INT8_MAX << endl;
    cout << INT16_MIN << " to " << INT16_MAX << endl;
    cout << INT32_MIN << " to " << INT32_MAX << endl;
    cout << INT64_MIN << " to " << INT64_MAX << endl;
    
    //Variables and Literals (Examples)
    int roll_no = 1;
    char group = 'A';

    float price = 12.75f; //float type variable, float literal 'f' or 'F'
    long distance = 65839L; //Variable type long, long type literal

    float price2 = 12.5; //by default this literal is double, therefore append 'f' or 'F'
    float price3 = 12.5f;
    float cost = 1.72e4f; // e is scientific notation
    double weight = 2.53e7L; //Long type double literal

    char section = 'A';
    char section1 = 65;
    char section2 = 65.5; //Coersion - value is internally converted into a required data type;

    bool b = true;
    bool bb = 1;

    string name = "Aryan"; // string is not a primitive data type, but a class provided by Cpp. & you can use it tor create variables.

    cout << "price: " << price << endl;
    cout << "Distance: " << distance << endl;
    cout << "price: " << price << endl;
    cout << "price2: " << price2 << endl;
    cout << "cost: " << cost << endl;
    cout << "weight: " << weight << endl;
    cout << "section: " << section << endl;
    cout << "section1: " << section1 << endl;
    cout << "section2: " << section2 << endl;
    cout << "bool: " << b << endl;
    cout << "bool: " << bb << endl;
    cout << "String: " << name << endl;

    //Different types of initilizing variables
    int day = 1;
    int dday(3);
    int day1 = (4);
    int day2{7};
    int day3 = {9};

    cout << "day: " << day << endl;
    cout << "dday: " << dday << endl;
    cout << "day1: " << day1 << endl;
    cout << "day2: " << day2 << endl;
    cout << "day3: " << day3 << endl;

    return 0;
}

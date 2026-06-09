/*
    Author: Aryan Singh
    Topic: Input and Output in Cpp
*/

#include<iostream>

using namespace std;

int main(){
    string str;
    cout << "May i know your name: ";
    getline(cin, str);
    cout<<"Welcome " << str << endl;
    return 0;
}

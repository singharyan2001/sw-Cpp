/*
    Author:
    Topic:
*/
#include<iostream>

using namespace std;

int main()
{
    int n1,n2,n3;
    cout << "enter 3 numbers: ";
    cin >> n1 >> n2 >> n3;

    if(n1 > n2){
        if(n1>n3){
            cout << "n1 is the maximum number" << endl;
        }
        else{
            cout << "n3 is the maximum number" << endl;
        }
    }
    else{
        if(n2 > n3){
            cout << "n2 is the maximum number" << endl;
        }
        else{
            cout << "n3 is the maximum number" << endl;
        }
    }

    if(n1 > n2 && n1 > n3){
        cout << n1 << endl;
    }
    else{
        if(n2 > n3){
            cout << n2 << endl;
        }
        else{
            cout << n3 << endl;
        }
    }

    return 0;
}

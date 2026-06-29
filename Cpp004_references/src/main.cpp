#include<iostream>

using namespace std;

#define LOG_MSG(x) cout << x << endl
#define LOG_DATA(x,y) cout << x <<  y << endl

void increment(int& value);

int main(){
    LOG_MSG("TOPIC: References in C++");
    
    LOG_MSG("===================== Generic usage of references in C++ =====================");
    int var = 10;
    int& ref = var; // Referencing var via reference variable ref.
    LOG_DATA("Value stored by the reference variable: ",ref);

    int var2 = 15;
    LOG_DATA("Value stored by reference variable a: ", ref);
    ref = var2; // Note this will not change the reference from var1 to var1, it would just assign the value of var2 to var1 i.e. (referenced vai ref) var1 = var2
    LOG_DATA("Value stored by reference variable a: ", ref);

    LOG_MSG("===================== Pass by reference in C++ =====================");
    LOG_DATA("Value stored by reference variable \"ref\" is: ", ref);
    increment(ref);
    LOG_DATA("Updated Value stored by reference variable \"ref\" is: ", ref);
}

void increment(int& value){
    value++;
}
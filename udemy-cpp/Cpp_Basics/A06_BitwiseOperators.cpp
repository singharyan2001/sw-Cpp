/*
    Author: Aryan Singh
    Topic: Bitwise Operators
*/
#include<iostream>
using namespace std;
int main()
{
    //Bitwise AND Operation
    int x = 11, y = 5; //thes numbers are stored in binary form.
    // x = 11 --->> 0b00001011
    // y = 5  --->> 0b00000101

    int z1, z2, z3, z4;
    z1 = x & y;
    cout << "Bitwise AND operation with x and y is: " << z1 << endl;
    /*
    Ex1
    // x = 11 --->>   0b00001011
    // y = 5  --->> & 0b00000101
    // z1 ------->>   0b00000001 -->solution for bitwise AND operation i.e. 1
    Ex2
    // y = 7; new value
    // x = 11 --->>   0b00001011
    // y = 7  --->> & 0b00000111
    // z1 ------->>   0b00000011 -->solution for bitwise AND operation i.e. 2
    */
    int z11, Ynew = 7;
    z11 = x & Ynew;
    cout << "Bitwise AND operation with x and Ynew is: " << z11 << endl;

    //Bitwise OR Operation 
    z2 = x | Ynew;
    cout << "Bitwise OR operation with x and Ynew is: " << z2 << endl;
    /*
    // x = 11 --->>      0b00001011
    // Ynew = 7  --->> | 0b00000111
    // z2 ------->>      0b00001111 --> solution of bitwise OR operation i.e 15
    */

    //Bitwise NOT Operation
    char xx = 5;
    //x --> 00000101; //NOTE: the MSB is the sign bit which is currently Zero, so after NOT operation, the number is Negative.
    /*
    x --> 0b00000101
    ~x -> 0b11111010 --> take 2's compliment of this number to get the value. MSB is 1 i.e -ve number
    ~x -> 0b11111010 ---> 1's C -> 0b00000101 --> 2's C:
    0b00000101
            +1
    0b00000110 --> solution i.e -6 (since the result of NOT operation, the MSB was 1, therefore the number is -ve)
    */
    z3 = ~xx;
    cout << "Bitwise NOT operation with xx and yy is: " << z3 << endl;
    
    //Bitwise XOR Operation
    int x7 = 11, y7 = 7, z7;

    z7 = x7^y7;
    cout << "Bitwise XOR Operation with x7 and y7 is: " << z7 << endl;

    //Bit Shifting Operations

    //Left Shifting i.e <<
    // x * 2^(n)
    int x11 = 5, y11;
    y11 = x<<1;
    cout << "(left)Bit Shifitng Operation with x11 is: " << y11 << endl;

    //Right Shifting i.e <<
    // x / 2^(i)
    int x12 = 5, y12;
    y12 = x>>1;
    cout << "(Right)Bit Shifitng Operation with x11 is: " << y12 << endl;

    return 0;
}
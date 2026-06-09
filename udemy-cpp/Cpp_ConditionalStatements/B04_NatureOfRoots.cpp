/*
    Author: Aryan Singh
    Topic: Nature of Roots

    Quadratic Equation: ax^2 + bx + c = 0
    roots = (-b +- sqrt(b^2 - 4ac))/ (2a)
    discriminant: d = b^2 - 4ac
    if d = 0 , real and equal
    if d > 0 , real and unequal
    if d < 0 , imaginary
*/

#include<iostream>
#include<cmath>

using namespace std;

void Roots(float a,float b,float c);

int main()
{
    float a,b,c,d,r1,r2;
    cout << "Enter a, b, c: ";
    cin >> a >> b >> c;

    d = (b*b) - (4*a*c);
    r1 = ((-b + sqrt(d))/(2*a));
    r2 = ((-b - sqrt(d))/(2*a));

    if(d==0){
        cout << "roots are real and equal " << r1 << endl;
    }
    else{
        if(d > 0){
            cout << "roots are real and unequal " << r1 << " " << r2 << endl;
        }
        else{
            cout << "Imaginary";
        }
    }
    return 0;
}

void Roots(float a,float b,float c)
{
    float d,r1,r2;
    
    //calculate d
    d = (b*b) - (4*a*c);
    r1 = ((-b + sqrt(d))/(2*a));
    r2 = ((-b - sqrt(d))/(2*a));
    // print nature of roots using nested if
    if(d==0){
        cout << "real and equal " << r1;
    }
    else{
        if(d > 0){
            cout << "real and unequal " << r1 << " " << r2;
        }
        else{
            cout << "imaginary";
        }
    }
}


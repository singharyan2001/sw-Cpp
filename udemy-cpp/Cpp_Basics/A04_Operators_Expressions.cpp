/*
    Author: Aryan Singh
    Topic: Operators and Expressions
*/
#include<iostream>
#include<cmath>

using namespace std;

int main()
{
    //Area of Triangle
    int b = 7, h = 5;
    float area_of_triangle;
    area_of_triangle = (float)(b*h)/2;
    cout << "Area of triangle is: " << area_of_triangle << endl;
    
    //Finding the sum of first n natural numbers
    int n, sum = 0;
    cout << "Enter the value of n: ";
    cin >> n;

    sum = (n*(n+1))/2;
    cout << "The sum is: " << sum << endl;
    
    //Find the roots of a quadratic equation
    int a, bb,c;
    float r1, r2;
    a = 10; bb = 5; c = 2;
    
    r1 = (-bb + sqrt(pow(b,2) - 4*a*c))/(2*a);
    r1 = (-bb - sqrt(pow(b,2) - 4*a*c))/(2*a);

    cout << "The root r1 is: " << r1 << endl;
    cout << "The root r2 is: " << r2 << endl;

    //Area of a circle
    float r, area_of_Circle;
    cout << "Enter Radius: ";
    cin >> r;

    area_of_Circle = 3.1425f*r*r; 
    cout << "Area is: " << area_of_Circle << endl;

    //Find the Speed
    int u, v, aa;
    float speed;
    cout << "enter values for u, v, a: ";
    cin >> u >> v >> aa;

    speed = (v*v - u*u)/(2*aa);

    cout << "Speed is: " << speed << endl;

    //Calculate the Simple Interest
    int P, T, R;
    float SI;

    cout << "Enter the value for P, T, R: ";
    cin >> P >> T >> R;

    SI = (P*R*T)/100;
    cout << "Simple Interest: " << SI << endl;

    //Calculate Volume of a Cylinder
    int radius, height;
    float volume;

    cout << "Enter the value for radius and height: ";
    cin >> radius >> height;

    volume = 3.14*radius*radius*height;
    cout << "Volume of Cylinder is: " << volume << endl; 

    //Find the distance between two points
    int x1, x2, y1, y2;
    float dist;

    cout << "Enter the value of x1, x2, y1, y2: ";
    cin >> x1 >> x2 >> y1 >> y2;

    dist = (float)sqrt(pow((x2-x1),2) + pow((y2-y1),2));
    cout << "Distance between two points is: " << dist << endl;     

    return 0;
}
/*
    Author:
    Topic:
*/
#include<iostream>
using namespace std;

void Grades(int m1,int m2,int m3);
float Amount_discounted(float TotalAmount);

int main()
{
    float Bill_Amount, Discounted_amount;
    cout << "Enter Bill Amount: ";
    cin >> Bill_Amount; 
    Discounted_amount = Amount_discounted(Bill_Amount);
    cout << "Bill Amount: " << Bill_Amount << endl;
    cout << "Discount: " << Discounted_amount << endl;
    cout << "Discounted Amount: " << Bill_Amount - Discounted_amount << endl;
    
    Grades(10,11,12);
    return 0;
}

void Grades(int m1,int m2,int m3)
{
    float Total,Avg;
    Avg = (m1+m2+m3)/3;
    
    //write nested if statements to print grades
    if( Avg >= 60){
        cout << 'A';
    }
    else{
        if(Avg >= 35 && Avg < 60){
            cout << 'B';
        }
        else{
            cout << 'C';
        }
    }
}

float Amount_discounted(float TotalAmount){
    float result;
    if(TotalAmount >= 500){
        result = TotalAmount * 20/100;
        return result;
    }
    else if(TotalAmount >= 100 && TotalAmount < 500){
        result = TotalAmount * 10/100;
        return result;
    }
    else{
        return TotalAmount;
    }
}


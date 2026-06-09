/*
    Author: Aryan
    Topic: Loops in CPp
*/
#include<iostream>

using namespace std;

void Generate_MultiplicationTable();
void Generate_Sum_of_N_Numbers();
void Find_Factorial(int F_sum);
void Find_Factors();
void Find_Perfect_number();
void Find_Prime_number();
bool Check_Prime_number(int x);
void display_digits();
void check_armstrong_number();
int Rev_number(int number);
void check_Palindrome(int number, int RevNumber);
void Find_GDC_2_Nos();


int main()
{
    // //Program to print multiplication Table
    // Generate_MultiplicationTable();
    // cout << endl;

    // //Generate Sum of N numbers
    // Generate_Sum_of_N_Numbers();
    // cout << endl;

    // //Factorial of n
    // int sum;
    // Find_Factorial(sum);
    // cout << endl;

    // //Factors of a number
    // Find_Factors();
    // cout << endl;

    // //Perfect numbers
    // Find_Perfect_number();
    // cout << endl;

    // //Check whether the number is a prime number or not
    // Find_Prime_number();
    // cout << endl;

    // //display digits
    // display_digits();
    
    // //Armstrong number
    // check_armstrong_number();

    // //Reverse a number
    // int n, x;
    // cout << "Enter number: ";
    // cin >> n;
    // x = Rev_number(n);

    // //Check whether a number is Palindrome or not.
    // check_Palindrome(n,x);
    
    //FInd the GCD of two numbers
    Find_GDC_2_Nos();
    

    return 0;
}

//For loops
void Generate_MultiplicationTable(){
    //Program to print multiplication Table
    int Number;
    cout << "Program to print the Multiplication Number" << endl;
    cout<< "Enter n: ";
    cin >> Number;
    for(int initialValue = 1; initialValue <= 10;initialValue++){
        cout << Number << "X" << initialValue << "=" << initialValue*Number << endl;
    }
}

void Generate_Sum_of_N_Numbers(){
    //Generate Sum of N numbers
    int Number, Result;
    cout << "Program to generate sum of n numbers" << endl;
    cout<< "Enter N: ";
    cin >> Number;
    for(int InitialValue = 1; InitialValue<=Number; InitialValue++){
        Result = Result + InitialValue;
    }
    cout << "Sum of N numbers is: " << Result << endl; 
}

void Find_Factorial(int F_sum){
    //Factorial of n
    int n;
    F_sum = 0;
    int InitialValue = 1;
    cout << "Program to generate the factorial of n" << endl;
    cout << "Enter N: ";
    cin >> n;
    for(InitialValue = 1; InitialValue<=n; InitialValue++){
        F_sum = F_sum*InitialValue;
    }
    cout << "Factorial of " << n << " is " << F_sum << endl;
}

void Find_Factors(){
    //Factors of a number
    int Number, sum;
    cout << "Program to generate the factors of a Number" << endl;
    cout << "Enter Number: ";
    cin >> Number;
    for(int InitialValue = 1; InitialValue<=Number; InitialValue++){
        if(Number % InitialValue == 0){
            cout << "Factors of " << Number << " are " << InitialValue << endl;
        }
    }
}

void Find_Perfect_number(){
    //Perfect number
    int number, sum;
    cout << "Program to get the Perfect Number" << endl;
    cout << "Enter n:";
    cin >> number;
    sum = 0;
    for(int initial_value = 1; initial_value <= number; initial_value++){
        if(number % initial_value == 0){
            sum = sum + initial_value;
        }
    }
    if((number*2) == sum){
        cout << number << " is a Perfect number" << endl;
    }
    else{
        cout << number << " is not a Perfect number" << endl;
    }
}

void Find_Prime_number(){
    //Prime number
    int number, sum;
    cout << "Program to find whether the number is a prime number or not" << endl;
    cout << "Enter Number:";
    cin >> number;
    sum = 0;
    for(int initial_value = 1; initial_value < number; initial_value++){
        if(number % initial_value == 0){
            sum ++;
        }
    }
    if(sum == 1){
        cout << number << " is a prime number" << endl;
    }
    else{
        cout << number << " is not a prime number" << endl;
    }
}


bool Check_Prime_number(int x){
    //Prime number
    int number = x;
    int sum = 0;
    for(int initial_value = 1; initial_value < number; initial_value++){
        if(number % initial_value == 0){
            sum ++;
        }
    }
    if(sum == 1){
        cout << number << " is a prime number" << endl;
        return true;
    }
    else{
        cout << number << " is not a prime number" << endl;
        return false;
    }
}


//While loops
void display_digits(){
    int num, r;
    cout << "Enter n: ";
    cin >> num;
    while(num>0){
        r = num%10;
        num = num/10;
        cout << r;
    }
    cout << endl;
}

void check_armstrong_number(){
    int m, num, rem, as_num;
    cout << "enter number: ";
    cin >> num;
    m = num;
    as_num = 0;
    while(num > 0){
        rem = num % 10;
        as_num += rem*rem*rem;
        num = num / 10;
    }
    cout << as_num << endl;
    if(m == as_num){
        cout << m << " is an Armstrong number" << endl;
    }
    else{
        cout << m << " is not an Armstrong number" << endl;
    }
}

int Rev_number(int n){
    int r, rev = 0;
    while(n > 0){
        r = n % 10;
        //cout << r << endl;
        n = n / 10;
        //cout << n << endl;
        rev = rev * 10 + r;
        //cout << rev << endl;
    }
    cout << rev << endl;
    return rev;
}

void check_Palindrome(int number, int RevNumber){
    if(RevNumber == number){
        cout << "palindrome" << endl;
    }
    else{
        cout << "not a palindrome" << endl;
    }
}

void Find_GDC_2_Nos(){
    int a, b;
    cout << "enter two numbers: ";
    cin >> a;
    cin >> b;
    bool C1 = Check_Prime_number(a);
    bool C2 = Check_Prime_number(b);

    while(a != b){
            if(a>b){
                a = a-b;
            }
            else if(b > a){
                b = b - a;
            }
        }
    cout << a << " " << b << endl;
}

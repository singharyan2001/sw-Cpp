//C++ Arrays
#include<iostream>

using namespace std;

int main()
{
	//Initilizing Array A
    int A[5];
    A[0] = 12;
    A[1] = 15;
    A[2] = 25;
	
	// integer size - 4; array size - 5; Memory - 5*4 = 20
    cout<<"Memory Size of Array A is "<<sizeof(A)<<endl; //size - 20
    
    // print using indices
    cout<<A[1]<<endl;
    
    // Print in C Language
    printf("%d\n", A[2]);
    
    //Initilizing Array
    int B[6] = {2,4,6,8,10,12};
    int C[] = {1,3,5,7,9,11,13,15};
    int D[10] = {2,4,6,8,10,12};
    
    //Mmemory Size of the Array
    cout<<"Memory Size of Array B is "<<sizeof(B)<<endl; //size - 24
    cout<<"Memory Size of Array C is "<<sizeof(C)<<endl; //size - 32
	cout<<"Memory Size of Array D is "<<sizeof(D)<<endl; //size - 40
	
	//Printing using indices
	cout<<"5th index of Array D: "<<D[5]<<endl;
	cout<<"8th Index of Array D: "<<D[8]<<endl;
	//Note: if you have  an array of some size and intialized few elements, then the rest of the elements will be zero.
	
	//Displaying an Array
	int i;
	for(i=0; i<6;i++)
	{
		cout<<B[i]<<endl;
	}
	cout<<endl;
	
	for(i=0; i<sizeof(C)/4;i++)
	{
		cout<<C[i]<<endl;
	}
	cout<<endl;
	
	for(i=0; i<sizeof(D)/4;i++)
	{
		cout<<D[i]<<endl;
	}
	cout<<endl;
	
//	for(int x : B)
//	{
//		cout<<x<<endl;
//	}
//	cout<<endl;
	
	//Inputing an Array
	int n;
	cout<<"Enter size of an array: ";
	cin>>n;
	int Arr[n];//this array will currently have garbage Values
	//Inputing elements
	cout<<"Inputing elements"<<endl;
	for(i = 0; i<n;i++)
	{
		cin>>Arr[i];
	}
	cout<<endl;
	//printing the array
	cout<<"Printing the array"<<endl;
	for(i = 0; i<n;i++)
	{
		cout<<Arr[i]<<endl;;
	}
	
    return 0;
}

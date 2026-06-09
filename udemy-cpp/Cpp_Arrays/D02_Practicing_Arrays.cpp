#include <iostream>

using namespace std;

void calculate_sum_array();
void max_element_array();
void linear_search_array(int key);
void binary_search_array(int key);
int find_size_array(int *array);

int main(){
    calculate_sum_array();
    max_element_array();
    linear_search_array(8);
    int AX[] = {12,33,44,55,66,77,88,99};
    cout << sizeof(AX) << endl;
    cout << sizeof(int) << endl;
    cout << sizeof(AX)/sizeof(int) << endl;

    binary_search_array(25);

    return 0;
}

//Find the sum of all the elements in an array

void calculate_sum_array(){
    int A[] = {1,2,3,4,5,6,7,8,9,2,4,6,8,0,1,3,5,7,9};
    int sum = 0;
    for(auto x:A){
        sum += x;
    }
    cout << "Sum of all the elements in the array is " << sum << endl;
}

void max_element_array(){
    int A[] = {1,2,3,4,5,6,7,8,9,2,4,6,8,0,1,3,5,7,9};
    int max = 0;
    for(auto x:A){
        if(max < x){
            max = x;
        }
    }
    cout << "Max value in the array is: " << max << endl;
}

int find_size_array(int *array){
    int arr_size = 0;
    arr_size = sizeof(array)/sizeof(int);
    //print array size and then return it.
    cout << "Array size is " << arr_size << endl;
    return arr_size;
}

void linear_search_array(int key){
    int A[] = {1,2,3,4,5,6,7,8,9,2,4,6,8,0,1,3,5,7,9};
    int count = 0;
    int is_found = -1;
    for(auto x:A){
        if(key == x){
            cout << "key found! at element " << count << " " << endl;
            is_found = 1;
            break;
        }
        else{
            count++;
        }
    }
    if(is_found == -1){
        cout << "Unsuccessful, key not found!" << endl;
    }
}

void binary_search_array(int key){
    int A[] = {6,8,13,17,20,22,25,28,30,35};
    int L = 0, H = 0, mid = 0, x = 0;
    int arr_size = sizeof(A)/sizeof(int);
    H = arr_size - 1;
    for(int i = 0; i < 10; i++){
        mid = (L+H)/2;
        cout << "mid value " << mid << endl;
        cout << A[mid] << endl;
        if(A[mid] < key){
            mid += 1;
            L = mid;
        }
        else if(A[mid] > key){
            mid -= 1;
            H = mid;
        }
        else if(A[mid] == key){
            cout << "Key found at element " << mid << endl;
            break;
        }
    }
    cout << "key not found!" << endl; 
}



#include <iostream>

using namespace std;

int main(){
    int A[5] = {2,4,6,8,10};

    // for(int i = 0; i< 5;i++){
    //     cout << A[i] << endl;
    // }

    //using for - each loop ; using this method we dont need to know the size of an array. it will iterate through all elements
    for(int x:A){
        cout << x << endl;
    }

    float B[] = {2.5f, 5.6f, 9, 8, 7};

    for(int x:B){
        cout << x << endl;
    }

    for(float x:B){
        cout << x << endl;
    }

    for(auto x:B){
        cout << x << endl;
    }

    return 0;
}

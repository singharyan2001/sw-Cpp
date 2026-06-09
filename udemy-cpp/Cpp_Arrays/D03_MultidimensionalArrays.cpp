#include <iostream>

using namespace std;

int main(){
    /*Nested for loop*/
    for(int i = 0; i<=3;i++){
        for(int j = 0; j<=5; j++){
            cout << "(" << i << "," << j << ")";
        }
        cout << endl;
    }
    return 0;
}

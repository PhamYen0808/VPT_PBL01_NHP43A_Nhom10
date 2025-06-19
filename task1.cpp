#include <iostream>
#include <math.h>
using namespace std;
int main () {
    int *p; int X=[30];
    int *p=X; 
    for (i=0; i<30; i++) {
        *(p+i)=(2*i);   
        cout << *(p+i) << " ";
    }
    cout << endl;
    X[0]=1; X[1]=2;
    for (i=2; i<30; i++) {
        X[i]= X[i-1]+2*X[x-2];
        cout << X[i] << "";
        }
        cout << endl;
        cout << X[0] << " " << X[1]<< " ";
    for (i=2; i<30; i++) {
        *(p+i)= (*(p+i-1))+2*(*(p+i-2));
        cout << *(p+i) << " ";
    }
    return 0;
}

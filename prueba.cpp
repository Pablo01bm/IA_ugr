#include<iostream>

using namespace std;



int main(){

    int m[3][3];
    int n=0;

    for (int i=0; i < 3 ; i++){
        for(int j=0; j<3; j++){
            m[i][j]= n++;
        }
    }

        for (int i=0; i < 3 ; i++){
        for(int j=0; j<3; j++){
            cout << m[i][j] << " ";
        }
        cout << endl;
    }

cout << endl;

    for (int i=1; i<3; i++) {
        for(int j=1; j<=i; j++){
            cout << m[i-j][j] << " ";
        }
        cout << endl;
    }
cout << "OTRO"<< endl;

    for (int i=1; i<3; i++) {
        for(int j=1; j<=3-i-1; j++){
            cout << m[2-j][j+1+i] << " ";
        }
        cout << endl;
    }

  
    



}
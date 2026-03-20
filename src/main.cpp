#include "part6.hpp"

int main(){
    model model_=loadmodel();
    matrix x=part6(model_);
    for(int i=0;i<x.rows;i++){
        for(int j=0;j<x.cols;j++){
            cout<<x.data[i][j]<<endl;
        }
    }

    return 0;
}
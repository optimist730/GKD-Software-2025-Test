#include "part6.hpp"

matrix part6(model model_){
    Mat img = imread("nums/0.png",IMREAD_GRAYSCALE);
    if(img.empty()){
        cerr<<"Cannot read image!"<<endl;
        return matrix(1, 10);
    }
    Mat reduce;
    resize(img,reduce,Size(28,28));
    matrix x = matrix (1,784);
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            unsigned char temp=reduce.at<unsigned char>(i,j);
            x.data[0][i*28+j]=static_cast<double>(temp)/255.0;
        }
    }
    matrix x_=model_.forward(x);
    return x_;
}
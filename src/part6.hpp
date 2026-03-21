#ifndef PART6_HPP
#define PART6_HPP

#include "part3.hpp"
#include "part2.hpp"
#include <opencv2/opencv.hpp>
#include <ctime>
#include <chrono>


using namespace cv;
using namespace std;

template<class T>
matrix<T> part6(model<T> model_,vector<unsigned char> apply) {
/*
    Mat img = imread("../nums/9.png",IMREAD_GRAYSCALE);
    if(img.empty()){
        cerr<<"Cannot read image!"<<endl;
        matrix<T> result(1, 10);
        return result;
    }
    Mat reduce;
    resize(img,reduce,Size(28,28));
    matrix<T> x = matrix<T> (1,784);
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            unsigned char temp=reduce.at<unsigned char>(i,j);
            x.data[0][i*28+j]=static_cast<T>(temp)/255.0;
        }
    }
*/
    matrix<T> x = matrix<T> (1,784);
    for(int i=0;i<784;i++){
        x.data[0][i]=static_cast<double>(apply[i])/255.0;
    }
    auto start =chrono::high_resolution_clock::now();
    matrix<T> x_=model_.forward(x);
    auto end =chrono::high_resolution_clock::now();
    auto duration =chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout<<"time:"<<duration<<"ms"<<endl;
    return x_;
}

#endif
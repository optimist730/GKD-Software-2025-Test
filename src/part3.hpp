#ifndef PART3_HPP
#define PART3_HPP

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

template<class T>
class matrix {
public:
    matrix() {
        this->rows = 0;
        this->cols = 0;
        data = std::vector<std::vector<T>>();
    }
    
    matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        data = std::vector<std::vector<T>> (rows, std::vector<T>(cols, 0));
    }

    void add(matrix<T> other) {
        for(int i = 0;i<rows;i++){
            for(int j = 0;j<cols;j++){
                data[i][j] += other.data[i][j];
            }
        }
    }

    matrix<T> mul(matrix<T> other) {
        matrix<T> result(rows, other.cols);
        for(int i=0;i<rows;i++){
            for(int j=0;j<other.cols;j++){
                for(int k=0;k<cols;k++){
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }
    
    int rows, cols;
    vector<vector<T>> data;
};

template<class T>
matrix<T> RELU(matrix<T> a) {
    matrix<T> result(a.rows, a.cols);
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++){
            result.data[i][j] = max(static_cast<T>(0), a.data[i][j]);
        }
    }
    return result;
}

template<class T>
matrix<T> SoftMax(matrix<T> a) {
    matrix<T> result(a.rows, a.cols);
    for(int i=0;i<a.rows;i++){
        T sum = 0;
        for(int j=0;j<a.cols;j++){
            sum += exp(a.data[i][j]);
        }
        for(int j=0;j<a.cols;j++){
            result.data[i][j] = exp(a.data[i][j]) / sum;
        }
    }
    return result;
}

template<class T>
class model{
public:
    model(int w1r,int w1c,int b1r,int b1c,int w2r,int w2c,int b2r,int b2c) {
        w1 = matrix<T>(w1r, w1c);
        b1 = matrix<T>(b1r, b1c);
        w2 = matrix<T>(w2r, w2c);
        b2 = matrix<T>(b2r, b2c);
    }

    matrix<T> forward(matrix<T> x) {
        matrix<T> first=x.mul(w1);
        first.add(b1);
        matrix<T> second=RELU(first);
        matrix<T> third=second.mul(w2);
        third.add(b2);
        matrix<T> result=SoftMax(third);
        return result;
    }

    matrix<T> w1,b1,w2,b2;
};

#endif
#ifndef PART1_HPP
#define PART1_HPP

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class matrix {
public:
    matrix();
    matrix(int rows, int cols);

    void add(matrix other);

    matrix mul(matrix other);
    int rows, cols;
    vector<vector<float>> data;
};

matrix RELU(matrix a);

matrix SoftMax(matrix a);//规定为行向量

class model{
public:
    model(int w1r,int w1c,int b1r,int b1c,int w2r,int w2c,int b2r,int b2c);

    matrix forward(matrix x);

    matrix w1,b1,w2,b2;
};

#endif

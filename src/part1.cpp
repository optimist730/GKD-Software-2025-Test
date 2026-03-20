#include "part1.hpp"

matrix::matrix(){
    this->rows = 0;
    this->cols = 0;
    data = std::vector<std::vector<float>>();
}

matrix::matrix(int rows,int cols){
    this->rows = rows;
    this->cols = cols;
    data = std::vector<std::vector<float>> (rows, std::vector<float>(cols, 0));
}

void matrix::add(matrix other){
    for(int i = 0;i<rows;i++){
        for(int j = 0;j<cols;j++){
            data[i][j] += other.data[i][j];
        }
    }
}

matrix matrix::mul(matrix other){
    matrix result(rows, other.cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<other.cols;j++){
            for(int k=0;k<cols;k++){
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

matrix RELU(matrix a){
    matrix result(a.rows, a.cols);
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++){
            result.data[i][j] = max(0.0f, a.data[i][j]);
        }
    }
    return result;
}

matrix SoftMax(matrix a){
    matrix result(a.rows,a.cols);
    float sum=0;
    for(int i=0;i<a.cols;i++){
        sum+=exp(a.data[0][i]);
    }
    for(int i=0;i<a.cols;i++){
        result.data[0][i]=exp(a.data[0][i])/sum;
    }
    return result;
}

model::model(int w1r,int w1c,int b1r,int b1c,int w2r,int w2c,int b2r,int b2c){
    this->w1 = matrix(w1r, w1c);
    this->b1 = matrix(b1r, b1c);
    this->w2 = matrix(w2r, w2c);
    this->b2 = matrix(b2r, b2c);
}

matrix model::forward(matrix x){
    matrix first=x.mul(w1);
    first.add(b1);
    matrix second=RELU(first);
    matrix third=second.mul(w2);
    third.add(b2);
    matrix result=SoftMax(third);
    return result;

}
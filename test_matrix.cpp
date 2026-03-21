#include "src/part3.hpp"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main() {
    // 创建测试矩阵
    int rows = 500;
    int cols = 500;
    int other_cols = 500;
    
    matrix<double> a(rows, cols);
    matrix<double> b(cols, other_cols);
    
    // 初始化矩阵数据
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            a.data[i][j] = i + j;
        }
    }
    
    for(int i = 0; i < cols; i++) {
        for(int j = 0; j < other_cols; j++) {
            b.data[i][j] = i * j;
        }
    }
    
    // 测试单线程版本
    cout << "Testing single-threaded version..." << endl;
    auto start = high_resolution_clock::now();
    
    // 临时启用单线程代码
    matrix<double> result_single(rows, other_cols);
    for(int i=0; i<rows; i++) {
        for(int j=0; j<other_cols; j++) {
            for(int k=0; k<cols; k++) {
                result_single.data[i][j] += a.data[i][k] * b.data[k][j];
            }
        }
    }
    
    auto end = high_resolution_clock::now();
    auto duration_single = duration_cast<milliseconds>(end - start);
    cout << "Single-threaded time: " << duration_single.count() << " ms" << endl;
    
    // 测试多线程版本
    cout << "Testing multi-threaded version..." << endl;
    start = high_resolution_clock::now();
    matrix<double> result_multi = a.mul(b);
    end = high_resolution_clock::now();
    auto duration_multi = duration_cast<milliseconds>(end - start);
    cout << "Multi-threaded time: " << duration_multi.count() << " ms" << endl;
    
    // 验证结果正确性
    bool correct = true;
    for(int i = 0; i < rows && correct; i++) {
        for(int j = 0; j < other_cols && correct; j++) {
            if(abs(result_single.data[i][j] - result_multi.data[i][j]) > 1e-6) {
                correct = false;
                cout << "Error at (" << i << ", " << j << "): " 
                     << result_single.data[i][j] << " vs " << result_multi.data[i][j] << endl;
            }
        }
    }
    
    if(correct) {
        cout << "Results are correct!" << endl;
    } else {
        cout << "Results are incorrect!" << endl;
    }
    
    return 0;
}
#include <fstream>
#include "json.hpp"
#include "part3.hpp"

using namespace std;

template<class T>
model<T> loadmodel() {
    ifstream file{"mnist-fc/meta.json",ios::in};
    if(!file){
        cerr<<"文件无法打开"<<endl;
        file.close();
        return model<T>(784, 500, 1, 500, 500, 10, 1, 10);
    }
    nlohmann::json data_;
    file >> data_;
    file.close();
    auto model_ = model<T> (
        data_["fc1.weight"][0].get<int>(),
        data_["fc1.weight"][1].get<int>(),
        data_["fc1.bias"][0].get<int>(),
        data_["fc1.bias"][1].get<int>(),
        data_["fc2.weight"][0].get<int>(),
        data_["fc2.weight"][1].get<int>(),
        data_["fc2.bias"][0].get<int>(),
        data_["fc2.bias"][1].get<int>()
    );

    loadmatrix(model_.w1, "mnist-fc/fc1.weight");
    loadmatrix(model_.b1, "mnist-fc/fc1.bias");
    loadmatrix(model_.w2, "mnist-fc/fc2.weight");
    loadmatrix(model_.b2, "mnist-fc/fc2.bias");
    return model_;
}

template<class T>
void loadmatrix(matrix<T>& a, const string& filename) {
    ifstream file{filename,ios::in | ios::binary};
    if(!file){
        cerr<<"矩阵文件无法打开！"<<endl;
        file.close();
        return;
    }
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++){
            T temp;
            file.read(reinterpret_cast<char*>(&temp),sizeof(T));
            a.data[i][j]=temp;
        }
    }
    file.close();
}

template<>
model<float> loadmodel<float>() {
    ifstream file{"mnist-fc/meta.json",ios::in};
    if(!file){
        cerr<<"文件无法打开"<<endl;
        file.close();
        return model<float>(784, 500, 1, 500, 500, 10, 1, 10);
    }
    nlohmann::json data_;
    file >> data_;
    file.close();
    auto model_ = model<float> (
        data_["fc1.weight"][0].get<int>(),
        data_["fc1.weight"][1].get<int>(),
        data_["fc1.bias"][0].get<int>(),
        data_["fc1.bias"][1].get<int>(),
        data_["fc2.weight"][0].get<int>(),
        data_["fc2.weight"][1].get<int>(),
        data_["fc2.bias"][0].get<int>(),
        data_["fc2.bias"][1].get<int>()
    );

    loadmatrix(model_.w1, "mnist-fc/fc1.weight");
    loadmatrix(model_.b1, "mnist-fc/fc1.bias");
    loadmatrix(model_.w2, "mnist-fc/fc2.weight");
    loadmatrix(model_.b2, "mnist-fc/fc2.bias");
    return model_;
}

template<>
model<double> loadmodel() {
    ifstream file{"mnist-fc-plus/meta.json",ios::in};
    if(!file){
        cerr<<"文件无法打开"<<endl;
        file.close();
        return model<double>(784, 1000, 1, 1000, 1000, 10, 1, 10);
    }
    nlohmann::json data_;
    file >> data_;
    file.close();
    auto model_ = model<double> (
        data_["fc1.weight"][0].get<int>(),
        data_["fc1.weight"][1].get<int>(),
        data_["fc1.bias"][0].get<int>(),
        data_["fc1.bias"][1].get<int>(),
        data_["fc2.weight"][0].get<int>(),
        data_["fc2.weight"][1].get<int>(),
        data_["fc2.bias"][0].get<int>(),
        data_["fc2.bias"][1].get<int>()
    );

    loadmatrix(model_.w1, "mnist-fc-plus/fc1.weight");
    loadmatrix(model_.b1, "mnist-fc-plus/fc1.bias");
    loadmatrix(model_.w2, "mnist-fc-plus/fc2.weight");
    loadmatrix(model_.b2, "mnist-fc-plus/fc2.bias");
    return model_;
}
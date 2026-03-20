#include "part2.hpp"
#include "json.hpp"



model loadmodel(){
    ifstream file{"mnist-fc/meta.json",ios::in};
    if(!file){
        cerr<<"文件无法打开"<<endl;
        file.close();
        return model(784, 500, 1, 500, 500, 10, 1, 10);
    }
    nlohmann::json data_;
    file >> data_;
    file.close();
    auto model_ = model (
        data_["fc1.weight"][0].get<int>(),
        data_["fc1.weight"][1].get<int>(),
        data_["fc1.bias"][0].get<int>(),
        data_["fc1.bias"][1].get<int>(),
        data_["fc2.weight"][0].get<int>(),
        data_["fc2.weight"][1].get<int>(),
        data_["fc2.bias"][0].get<int>(),
        data_["fc2.bias"][1].get<int>()
    );

    loadmatrix(model_.w1);
    loadmatrix(model_.b1);
    loadmatrix(model_.w2);
    loadmatrix(model_.b2);
    return model_;
}

void loadmatrix(matrix& a){
    ifstream file{"mnist-fc/fc1.weight",ios::in | ios::binary};
    if(!file){
        cerr<<"矩阵文件无法打开！"<<endl;
        file.close();
    }
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++){
            float temp;
            file.read(reinterpret_cast<char*>(&temp),sizeof(float));
            a.data[i][j]=temp;
        }
    }
}

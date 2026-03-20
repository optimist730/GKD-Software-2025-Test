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

    loadmatrix(model_.w1, "mnist-fc/fc1.weight");
    loadmatrix(model_.b1, "mnist-fc/fc1.bias");
    loadmatrix(model_.w2, "mnist-fc/fc2.weight");
    loadmatrix(model_.b2, "mnist-fc/fc2.bias");
    return model_;
}

void loadmatrix(matrix& a, const string& filename){
    ifstream file{filename,ios::in | ios::binary};
    if(!file){
        cerr<<"矩阵文件无法打开！"<<endl;
        file.close();
        return;
    }
    for(int i=0;i<a.rows;i++){
        for(int j=0;j<a.cols;j++){
            float temp;
            file.read(reinterpret_cast<char*>(&temp),sizeof(float));
            a.data[i][j]=temp;
        }
    }
    file.close();
}

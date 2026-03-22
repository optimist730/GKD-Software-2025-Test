#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
using namespace cv;

int main(){
    Mat img = imread("../nums/9.png",IMREAD_GRAYSCALE);
    if(img.empty()){
        cerr<<"Cannot read image!"<<endl;
        return -1;
    }
    Mat reduce;
    resize(img,reduce,Size(28,28));
    vector<unsigned char> send_;
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            unsigned char temp=reduce.at<unsigned char>(i,j);
            send_.emplace_back(temp);
        }
    }



     WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Winsock初始化失败!" << endl;
        return 1;
    }
    SOCKET applysocket = socket(AF_INET, SOCK_STREAM, 0);
    if(applysocket==INVALID_SOCKET){
        cerr<<"applysocket failure!code: "<<WSAGetLastError()<<endl;
        WSACleanup();
        return -1;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8888);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(applysocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr <<"connect failure!code: "<< WSAGetLastError() << endl;
        closesocket(applysocket);
        WSACleanup();
        return -1;
    }

    if (send(applysocket,reinterpret_cast<const char*>(send_.data()), send_.size(), 0) == SOCKET_ERROR) {
                
        cerr <<"send failure!code: " <<WSAGetLastError() << endl;
        closesocket(applysocket);
        WSACleanup();
        return -1;           
    }
    vector<double> recv_(10);
    int total_bytes = 10 * sizeof(double);
    int total_received = 0;
    //cout<<"2"<<endl;
    while (total_received < total_bytes) {
        int received = recv(applysocket, reinterpret_cast<char*>(recv_.data()) + total_received,total_bytes-total_received, 0);
        if (received <= 0) {
            break;
        }
        total_received += received;
    }
    //cout<<"3"<<endl;
    if (total_received == total_bytes) {
        // 处理接收到的数据
        for (double val : recv_) {
            std::cout << val <<endl;
        }
    }
    //cout<<"1"<<endl;

    return 0;
}
#include <WinSock2.h>
#include "part6.hpp"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define type double

using namespace std;

int main(){
    WSAData wsadata;
    if(WSAStartup(MAKEWORD(2,2),&wsadata)!=0){
        cerr<<"WSAData failure!code: "<<WSAGetLastError()<<endl;
        return -1;
    }
    SOCKET listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listensocket==INVALID_SOCKET){
        cerr<<"listensocket failure!code: "<<WSAGetLastError()<<endl;
        WSACleanup();
        return -1;
    }
    sockaddr_in severaddr;
    severaddr.sin_family=AF_INET;
    severaddr.sin_port=htons(8888);
    severaddr.sin_addr.s_addr=INADDR_ANY;
    if(::bind(listensocket,(sockaddr*)&severaddr,sizeof(severaddr)) == SOCKET_ERROR){
        cerr<<"bind failure!code: "<<WSAGetLastError()<<endl;
        closesocket(listensocket);
        WSACleanup();
        return -1;
    }
    if(listen(listensocket,10)==SOCKET_ERROR){
        cerr<<"listen failure!code: "<<WSAGetLastError()<<endl;
        closesocket(listensocket);
        WSACleanup();
        return -1;
    }

    sockaddr_in applyaddr;
    int applylen=sizeof(applyaddr);
    SOCKET applysocket = accept(listensocket,(sockaddr*)&applyaddr,&applylen);
    if(applysocket==INVALID_SOCKET){
        cerr<<"accept failure!code: "<<WSAGetLastError()<<endl;
        closesocket(listensocket);
        WSACleanup();
        return -1;
    }
    vector<unsigned char> apply_matrix(784);
    while(true){
        int recvlen=recv(applysocket,reinterpret_cast<char*>(apply_matrix.data()),apply_matrix.size()-0,0);
        if(recvlen>0){
            model<type> model_=loadmodel<type>();
            matrix<type> x=part6(model_,apply_matrix);
            vector<double> send_(10);
            int total_bytes = 10 * sizeof(double);
            for(int i=0;i<10;i++){
                send_[i]=x.data[0][i];
            }
            send(listensocket, reinterpret_cast<const char*>(send_.data()), total_bytes, 0);
        }else{
            cerr<<"recv failure"<<endl;
            break;
        }
    }
    closesocket(listensocket);
    WSACleanup();
/*
    model<type> model_=loadmodel<type>();
    matrix<type> x=part6(model_);

    for(int i=0;i<x.rows;i++){
        for(int j=0;j<x.cols;j++){
            cout<<x.data[i][j]<<endl;
        }
    }
*/
    return 0;
}
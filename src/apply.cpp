#include <opencv2/opencv.hpp>
#include <iostream>
#include <ctime>
#include <vector>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace cv;
using namespace std;

bool is_drawing = false;  // 是否正在绘画
Point prev_point;         // 上一次的鼠标坐标
Scalar draw_color = Scalar(0, 0, 0);  // 绘画颜色（BGR，红色）
int draw_thickness = 250;    // 画笔粗细
time_t lastMouseUpTime = 0;   // 鼠标松开的时间
bool needRefresh = false;  // 是否需要刷新

void onMouse(int event, int x, int y, int flags, void* userdata) {
    Mat& canvas = *(Mat*)userdata;

    // 左键按下：开始绘画，记录起点
    if (event == EVENT_LBUTTONDOWN) {
        is_drawing = true;
        prev_point = Point(x, y);
        circle(canvas, prev_point, draw_thickness / 2, draw_color, -1);
        needRefresh = false;  
    }
    else if (event == EVENT_MOUSEMOVE && is_drawing) {
        Point curr_point = Point(x, y);
        line(canvas, prev_point, curr_point, draw_color, draw_thickness, LINE_AA);
        prev_point = curr_point;
    }
    // 左键松开：结束绘画，记录时间
    else if (event == EVENT_LBUTTONUP) {
        is_drawing = false;
        lastMouseUpTime = time(NULL);  // 记录鼠标松开的时间
        needRefresh = true;  // 标记需要刷新
    }
}

// 拼接两个图像（水平/垂直）
Mat mergeTwoImages(const Mat& img1, const Mat& img2) {
    if (img1.empty() || img2.empty()) {
        cerr << "错误：输入图像为空！" << endl;
        return Mat();
    }

    Mat mergedImg;
    
    int maxHeight = max(img1.rows, img2.rows);
    int totalWidth = img1.cols + img2.cols+100;
    mergedImg = Mat::zeros(maxHeight, totalWidth, img1.type());
    img1.copyTo(mergedImg(Rect(0, 0, img1.cols, img1.rows)));
    img2.copyTo(mergedImg(Rect(img1.cols+30, 0, img2.cols, img2.rows)));
    
    return mergedImg;
}

int main() {
    

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

    
    string img1Path = "../picture/white.jpeg";
    string img2Path = "../picture/black.jpg";
    Mat img1 = imread(img1Path);
    Mat img2 = imread(img2Path);
    
    
    if (img1.empty()||img2.empty()) {
        cerr << "Failed to read image: " << endl;
        return -1;
    }

    Mat grayimg;
    cvtColor(img1, grayimg, COLOR_BGR2GRAY);
    vector<unsigned char> send_;
    Mat reduce;
/*
    resize(grayimg,reduce,Size(28,28));
    vector<unsigned char> send_;
    for(int i=0;i<28;i++){
        for(int j=0;j<28;j++){
            unsigned char temp=reduce.at<unsigned char>(i,j);
            send_.emplace_back(temp);
        }
    }
    
    if (send(applysocket,reinterpret_cast<const char*>(send_.data()), send_.size(), 0) == SOCKET_ERROR) {
                
        cerr <<"send failure!code: " <<WSAGetLastError() << endl;
        closesocket(applysocket);
        WSACleanup();
        return -1;           
    }
*/
    // 2. 拼接图像（水平拼接）
    Mat mergedImg = mergeTwoImages(img1, img2);
    
    // 3. 创建窗口
    namedWindow("all", WINDOW_NORMAL);
    resizeWindow("all", 1000, 800);

    // 4. 设置鼠标回调
    setMouseCallback("all", onMouse, &mergedImg);

    // 5. 主循环
    while (true) {
        time_t currentTime = time(NULL);
        
        if (needRefresh && (currentTime - lastMouseUpTime >= 2)) {
            needRefresh = false;
            mergedImg = mergeTwoImages(img1, img2);
            setMouseCallback("all", onMouse, &mergedImg);
        }

        Mat displayImg = mergedImg.clone();
        
        Mat currentImg = displayImg(Rect(0, 0, img1.cols, img1.rows));
        cvtColor(currentImg, grayimg, COLOR_BGR2GRAY);
        resize(grayimg,reduce,Size(28,28));
        send_.clear();
        for(int i=0;i<28;i++){
            for(int j=0;j<28;j++){
                unsigned char temp=reduce.at<unsigned char>(i,j);
                send_.emplace_back(temp);
            }
        }
        if (send(applysocket,reinterpret_cast<const char*>(send_.data()), send_.size(), 0) == SOCKET_ERROR) {
            cerr <<"send failure!code: " <<WSAGetLastError() << endl;
            closesocket(applysocket);
            WSACleanup();
            return -1;           
        }
        
        // 输出发送数据的前几个值，用于调试99
        cout << "发送数据前10个值: ";
        for (int i = 0; i < 10 && i < send_.size(); i++) {
            cout << (int)send_[i] << " ";
        }
        cout << endl;

        //接收传回来的向量;
        vector<double> recv_(10);
        int total_bytes = 10 * sizeof(double);
        int total_received = 0;
        while (total_received < total_bytes) {
            int received = recv(applysocket, reinterpret_cast<char*>(recv_.data()) + total_received,total_bytes-total_received, 0);
            if (received <= 0) {
                break;
            }
            total_received += received;
        }
        
        // 检查是否接收到了完整的数据
        if (total_received != total_bytes) {
            cerr << "failure: " << total_received << " bytes" << endl;
        }
        
        // 输出接收到的数据，用于调试
/*
        cout << "接收到的预测数据: ";
        for (double val : recv_) {
            cout << val << " ";
        }
        cout << endl;
*/
        double max=0;
        int max_i=0;
        for(int i=0;i<10;i++){
            Point pos(img1.cols+10,i*200+100);
            string temp=to_string((int)(recv_[i]*100));
            string temp_=to_string(i)+" "+temp+"%";
            putText(displayImg, temp_, pos, FONT_HERSHEY_SIMPLEX, 3, Scalar(0,0,255), 5);
            cv::rectangle(img2,Point(img1.cols+100,i*200+50),Point(img1.cols+100+img2.cols*recv_[i],i*200+150),Scalar(0,255,0),-1,LINE_AA);

            if(recv_[i]>max){
                max=recv_[i];
                max_i=i;
            }
        }
        for (double val : recv_) {
            std::cout << val <<endl;
        }
        string text1 = "prediction: "+to_string(max_i);
        Point pos1(100, 200); // 文字左上角坐标 (x,y)
        int fontFace = FONT_HERSHEY_SIMPLEX; // 字体
        double fontScale = 5.0; // 字体大小缩放比例
        Scalar color1(0, 255, 0); // 文字颜色（BGR）：绿色
        int thickness = 10; // 文字线条粗细
        putText(displayImg, text1, pos1, fontFace, fontScale, color1, thickness);

        imshow("all", displayImg);
        if (waitKey(1) == 27) {
            break;
        }

    }
    cv::destroyAllWindows();
    return 0;
}
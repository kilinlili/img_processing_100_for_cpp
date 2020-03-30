#include<iostream>
#include<opencv2/opencv.hpp>

int main(){
    cv::Mat img = cv::imread("imori.jpg");
    cv::imshow("sample", img);
    cv::waitKey(0); 

    return 0;

}

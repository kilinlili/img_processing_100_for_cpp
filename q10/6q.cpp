#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;

void bgr256tobgr64(cv::Mat &src, cv::Mat &dst);

int main()
{
    cv::Mat src = cv::imread("./img/imori.jpg");
    cv::Mat dst0 = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);//!
    
    bgr256tobgr64(src, dst0);
    cv::imshow("test", dst0);
    cv::waitKey(0);
    
    cv::imwrite("./img/6q_ans.jpg", dst0);
    return 0;
}

void bgr256tobgr64(cv::Mat &src, cv::Mat &dst)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //B,G,R
            for(int i = 0; i < 3; i++){
                if(src.at<cv::Vec3b>(y, x)[i] < 64){
                    dst.at<cv::Vec3b>(y, x)[i] = 32;
                }else if(src.at<cv::Vec3b>(y, x)[i] < 128){
                    dst.at<cv::Vec3b>(y, x)[i] = 96;
                }else if(src.at<cv::Vec3b>(y, x)[i] < 192){
                    dst.at<cv::Vec3b>(y, x)[i] = 160;
                }else if(src.at<cv::Vec3b>(y, x)[i] < 256){
                    dst.at<cv::Vec3b>(y, x)[i] = 224;
                }
                
            }
        }
    }
}



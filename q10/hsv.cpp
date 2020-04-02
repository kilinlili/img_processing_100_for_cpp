#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;

void rgb2hsv(cv::Mat &src, cv::Mat &dst);

int main()
{
    cv::Mat src = cv::imread("./img/imori.jpg");
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    rgb2hsv(src, dst);

    cv::imshow("sample", dst);
    cv::waitKey(0);

    cv::imwrite("./img/hsv.jpg", dst);
    return 0;
}

void rgb2hsv(cv::Mat &src, cv::Mat &dst)
{
    //dst = cv::Mat::zeros((src.rows(), src.cols()), CV_8UC3);
    double value_max = 0.0, value_min = 0.0;
    //int max_flag, 
    int min_flag;
    double h, v, s;

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //B,G,R
            value_min = src.at<cv::Vec3b>(y, x)[0];
            value_max = src.at<cv::Vec3b>(y, x)[0];
            //max_flag = 0;
            min_flag = 0;
            //min check
            if(value_min > src.at<cv::Vec3b>(y, x)[1]){
                value_min = src.at<cv::Vec3b>(y, x)[1];
                min_flag = 1;
            }
            if(value_min > src.at<cv::Vec3b>(y, x)[2]){
                value_min = src.at<cv::Vec3b>(y, x)[2];
                min_flag = 2;
            }
            //max check
            if(value_max < src.at<cv::Vec3b>(y, x)[1]){
                value_max = src.at<cv::Vec3b>(y, x)[1];
                //max_flag = 1;
            }
            if(value_max < src.at<cv::Vec3b>(y, x)[2]){
                value_max = src.at<cv::Vec3b>(y, x)[2];
                //max_flag = 2;
            }

            //Hue:色相
            if(min_flag == 0){
                //B
                h = 60 + 60 * (src.at<cv::Vec3b>(y, x)[1] - src.at<cv::Vec3b>(y, x)[2]) / (value_max - value_min);
                //h += 180;
            }else if(min_flag == 1){
                //G
                h = 300 + 60 * (src.at<cv::Vec3b>(y, x)[2] - src.at<cv::Vec3b>(y, x)[0]) / (value_max - value_min);
                //h += 180;
            }else{
                //R
                h = 180 + 60 * (src.at<cv::Vec3b>(y, x)[0] - src.at<cv::Vec3b>(y, x)[1]) / (value_max - value_min);
                //h += 180;
            }

            //Value:明度
            v = value_max;

            //Saturation:彩度
            s = value_max - value_min;

            //dst
            dst.at<cv::Vec3b>(y, x)[0] = h;
            dst.at<cv::Vec3b>(y, x)[1] = s;
            dst.at<cv::Vec3b>(y, x)[2] = v;
        }
    }
}
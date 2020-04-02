#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;

void bgr2hsv(cv::Mat &src, cv::Mat &dst);
void hsv2rgb(cv::Mat &src, cv::Mat &dst);


int main()
{
    cv::Mat src = cv::imread("./img/imori.jpg");
    cv::Mat dst0 = cv::Mat::zeros(src.rows, src.cols, CV_32FC3);//!
    cv::Mat dst1 = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);//!
    bgr2hsv(src, dst0);
    hsv2rgb(dst0, dst1);   
    cv::imshow("hoge", dst1);
    cv::waitKey(0);
    
    cv::imwrite("./img/5q_failed_ans.jpg", dst1);
    return 0;
}

void bgr2hsv(cv::Mat &src, cv::Mat &dst)
{
    double value_max, value_min;
    float r, g, b;
    float h, v, s;

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //B,G,R
            r = (float)src.at<cv::Vec3b>(y, x)[2]/255;
            g = (float)src.at<cv::Vec3b>(y, x)[1]/255;
            b = (float)src.at<cv::Vec3b>(y, x)[0]/255;

            value_min = fmin(r, fmin(g, b));
            value_max = fmax(r, fmax(g, b));

            //Hue:色相
            if(value_min == b){
                h =  60 * (g - r) / (value_max - value_min) + 60;
            }else if(value_min == g){
                h =  60 * (r - b) / (value_max - value_min) + 300;
            }else if(value_min == r){
                h =  60 * (b - g) / (value_max - value_min) + 180;
            }

            //Value:明度
            v = value_max;

            //Saturation:彩度
            s = value_max - value_min;//円錐モデル
            //dst
            dst.at<cv::Vec3f>(y, x)[0] = h;
            dst.at<cv::Vec3f>(y, x)[1] = s;
            dst.at<cv::Vec3f>(y, x)[2] = v;
        }
    }
}


void hsv2rgb(cv::Mat &src, cv::Mat &dst)
{
    float c;
    float _h;
    float _x;
    float r, g, b;

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            c = src.at<cv::Vec3f>(y, x)[1];
            _h = src.at<cv::Vec3f>(y, x)[0]/60;
            _x = c * (1 - abs(fmod(_h, 2.0) - 1));

            //dst
            r = g = b = src.at<cv::Vec3f>(y, x)[2] - c;

            if(_h < 1){
                r += c;
                g += _x;
            }else if(_h < 2){
                r += _x;
                g += c;
            }else if(_h < 3){
                b += _x;
                g += c;
            }else if(_h < 4){
                g += _x;
                b += c;
            }else if(_h < 5){
                r += _x;
                b += c;
            }else if(_h < 6){
                b += _x;
                r += c;
            }
            
            dst.at<cv::Vec3b>(y, x)[2] = (uint8_t)(r*255);
            dst.at<cv::Vec3b>(y, x)[1] = (uint8_t)(g*255);
            dst.at<cv::Vec3b>(y, x)[0] = (uint8_t)(b*255);
        }
    }
}
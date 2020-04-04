#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

void average_pooling8x8(cv::Mat &src, cv::Mat &dst)
{
    float value;
    for(int y = 0; y < src.rows; y += 8){
        for(int x = 0; x < src.cols; x += 8){

            for(int c = 0; c < src.channels(); c++){
                value = 0; 
                for(int dy = 0; dy < 8; dy++){                
                    for(int dx = 0; dx < 8; dx++){
                        value += src.at<cv::Vec3b>(y + dy, x + dx)[c];
                    }
                }
                value = value / 64.0;
                //dst
                for(int dy = 0; dy < 8; dy++){                
                    for(int dx = 0; dx < 8; dx++){
                        dst.at<cv::Vec3b>(y + dy, x + dx)[c] = (uint8_t)value;
                    }
                }

            }
        }
    }
}

int main(void)
{
    cv::Mat src = cv::imread("./img/imori.jpg");
    cv::Mat dst0 = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);//!
    
    average_pooling8x8(src, dst0);
    cv::imshow("test", dst0);
    cv::waitKey(0);
    
    cv::imwrite("./img/7q_ans.jpg", dst0);
    return 0;
}

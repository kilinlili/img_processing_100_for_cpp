#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void motion_filter(cv::Mat &src, cv::Mat &dst, int size)
{
    //float kernel[size][size];
    int s = size/2;
    double temp;
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                temp = 0;
                for(int dy = -s; dy <= s; dy++){
                    for(int dx = -s; dx <= s; dx++){
                        if(dy == dx){
                            temp += src.at<cv::Vec3b>(y + dy, x + dx)[c]/size;
                        }else{
                            temp += 0;
                        }
                    }
                }
                dst.at<cv::Vec3b>(y, x)[c] = temp;

            }
        }
    }
}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);
    cv::imshow("sample", img);
    cv::waitKey(0);
    motion_filter(img, dst, 5);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/12q_ans.jpg", dst);
    
    return 0;
}
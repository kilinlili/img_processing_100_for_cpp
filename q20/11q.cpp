#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void average_filter(cv::Mat &src, cv::Mat &dst, int size)
{
    int s = size/2;
    double sum;
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                sum = 0;
                for(int dy = -s; dy <= s; dy++){
                    for(int dx = -s; dx <= s; dx++){
                        if(y + dy >= 0 && x + dx >= 0){
                            sum += (int)src.at<cv::Vec3b>(y + dy, x + dx)[c];
                        }
                    }
                }
                dst.at<cv::Vec3b>(y, x)[c] = sum/(size*size);
                //sum /=(size*size);
                //dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)sum;
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
    average_filter(img, dst, 3);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/11q_ans.jpg", dst);
    
    return 0;
}
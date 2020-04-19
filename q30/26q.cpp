#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

cv::Mat bi_linear(cv::Mat &src, double rx, double ry)
{
    //目標サイズ
    int resize_x = (int)(src.cols * rx);
    int resize_y = (int)(src.rows * ry);
    cv::Mat dst = cv::Mat::zeros(resize_y, resize_x, CV_8UC3);

    double temp;
    int src_y, src_x;//元画像サイズの取得
    double distance_y, distance_x;//距離dy = y'(イテレータ)/ ry - src_y;

    //dst
    for(int y = 0; y < resize_y; y++){
        src_y = (int)floor(y/ry);
        distance_y = y/ry - src_y;

        for(int x = 0; x < resize_x; x++){
            src_x = (int)floor(x/rx);
            distance_x = x/rx - src_x;

            for(int c = 0; c < src.channels(); c++){
                temp = (1. - distance_x)*(1. - distance_y) * src.at<cv::Vec3b>(src_y, src_x)[c] + 
                distance_x*(1. - distance_y) * src.at<cv::Vec3b>(src_y, src_x + 1)[c] + 
                (1. - distance_x)*distance_y * src.at<cv::Vec3b>(src_y + 1, src_x)[c] + 
                distance_x*distance_y * src.at<cv::Vec3b>(src_y + 1, src_x + 1)[c];

                dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)temp;
            }
        }
    }
    return dst;
}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::imshow("sample", img);
    cv::waitKey(0);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    cv::Mat dst = bi_linear(img, 1.5, 1.5);//bi_linear
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/26q_ans.jpg", dst);
    
    return 0;
}
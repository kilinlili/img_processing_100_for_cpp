#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

void hist_equalize(cv::Mat &src, cv::Mat &dst)
{
    
    int s = src.rows * src.cols * src.channels();
    double sum = 0;
    int temp;
    
    int h[256];
    for(int i = 0; i < 256; i++){
        h[i] = 0;
    }

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                temp = (uint8_t)src.data[y*src.step + x*src.channels() + c];
                //temp = (int)src.at<cv::Vec3b>(y, x)[c];
                h[temp] ++;
            }
        }
    }

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                temp = (uint8_t)src.data[y*src.step + x*src.channels() + c];
                temp = (int)src.at<cv::Vec3b>(y, x)[c];
                
                sum = 0;
                for(int i = 0; i <= temp; i++){
                    sum += h[i];
                }
                dst.data[y*src.step + x*src.channels() + c] = (uint8_t)(255.0/s * sum);//255/sで時間を溶かした．
                //dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)(255.0/s * sum);
            }
        }
    }
}



int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);
    //cv::imshow("sample", img);
    //cv::waitKey(0);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    hist_equalize(img, dst);//平均,標準偏差
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/23q_ans.jpg", dst);
    
    return 0;
}
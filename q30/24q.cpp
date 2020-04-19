#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

void gamma_correction(cv::Mat &src, cv::Mat &dst, float _c, float g)
{
    double temp;
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                temp = src.data[y*dst.step + x*src.channels() + c];
                dst.data[y*dst.step + x*src.channels() + c] = 255.0 *pow(temp / 255, 1.0/g);
            }
        }
    }
}



int main(void)
{
    cv::Mat img = cv::imread("imori_gamma.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);
    cv::imshow("sample", img);
    cv::waitKey(0);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    gamma_correction(img, dst, 1, 2.2);
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/24q_ans.jpg", dst);
    
    return 0;
}
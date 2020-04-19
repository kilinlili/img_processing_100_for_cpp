#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

cv::Mat nearest_neighbor(cv::Mat &src, double dx, double dy)
{
    //目標サイズ
    int resize_x = (int)(src.cols * dx);
    int resize_y = (int)(src.rows * dy);
    cv::Mat dst = cv::Mat::zeros(resize_y, resize_x, CV_8UC3);

    //dst
    int src_x, src_y;
    for(int y = 0; y < resize_y; y++){
        src_y = (int)round(y/dy);//y' = y * 拡大率
        //std::cout << y/dy << std::endl;
        for(int x = 0; x < resize_x; x++){
            src_x = (int)round(x/dx);//x; = x * 拡大率
            for(int c = 0; c < src.channels(); c++){
                dst.at<cv::Vec3b>(y, x)[c] = src.at<cv::Vec3b>(src_y, src_x)[c];
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
    cv::Mat dst = nearest_neighbor(img, 1.5, 1.5);//二アレストネイバー法
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/25q_ans.jpg", dst);
    
    return 0;
}
//floor ... 小数点以下切り捨て
//round ... 四捨五入
//拡大後の画素を拡大率で除算．その値を四捨五入して得られた座標の画素をそのまま利用．最も近い座標をそのまま利用してる．
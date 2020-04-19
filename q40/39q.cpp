#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>
#include <complex>

void bgrtoYCbCr(cv::Mat &src, cv::Mat &dst, double parm)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //Y
            dst.at<cv::Vec3f>(y, x)[0] =(float) (parm * (0.299 * src.at<cv::Vec3b>(y, x)[2] \
                                    + 0.5870 * src.at<cv::Vec3b>(y, x)[1] \
                                    + 0.114 * src.at<cv::Vec3b>(y, x)[0]));
            //Cb
            dst.at<cv::Vec3f>(y, x)[1] = (float)(-0.1687 * src.at<cv::Vec3b>(y, x)[2] \
                                    - 0.3313 * src.at<cv::Vec3b>(y, x)[1] \
                                    + 0.5 * src.at<cv::Vec3b>(y, x)[0] + 128);
            //Cr
            dst.at<cv::Vec3f>(y, x)[2] = (float)(0.5 * src.at<cv::Vec3b>(y, x)[2] \
                                    - 0.4187 * src.at<cv::Vec3b>(y, x)[1] \
                                    - 0.0813 * src.at<cv::Vec3b>(y, x)[0] + 128);
        }
    }
}


void YCbCrtobgr(cv::Mat &src, cv::Mat &dst)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //R
            dst.at<cv::Vec3b>(y, x)[2] = (uint8_t)(src.at<cv::Vec3f>(y, x)[0] \
                            + (src.at<cv::Vec3f>(y, x)[2] - 128) * 1.402);
            //G
            dst.at<cv::Vec3b>(y, x)[1] = (uint8_t)(src.at<cv::Vec3f>(y, x)[0] \
                            - (src.at<cv::Vec3f>(y, x)[1] - 128) * 0.3441 \
                            - (src.at<cv::Vec3f>(y, x)[2] - 128) * 0.7139);
            //B
            dst.at<cv::Vec3b>(y, x)[0] = (uint8_t)(src.at<cv::Vec3f>(y, x)[0] \
                            + (src.at<cv::Vec3f>(y, x)[1] - 128) * 1.7718);
        }
    }
}

int main(void)
{
    cv::Mat src = cv::imread("imori.jpg");
    cv::Mat temp = cv::Mat::zeros(src.rows, src.cols, CV_32FC3);
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    //cv::waitKey(0);
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    bgrtoYCbCr(src, temp, 0.7);
    YCbCrtobgr(temp, dst);


    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/39q_ans.jpg", dst);
    
    return 0;
}

//cv::circle()
//って関数を使ってもいいのか...?
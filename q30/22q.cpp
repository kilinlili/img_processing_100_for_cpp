#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

void hist_operation(cv::Mat &src, cv::Mat &dst, int mu, int sig)
{

    double ave = 0, sigma = 0, squate_sum = 0;
    double temp = 0;
    std::cout << src.step << std::endl;//384 (= 128 pixel * 3)
    std::cout << src.elemSize() << std::endl;//3

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                //temp = src.data[y*src.step + x*src.elemSize() + c];//OK//理解できんが
                temp = src.data[y*src.step + x*src.channels() + c];
                //temp = src.at<cv::Vec3b>(y, x)[c];//OK
                ave += temp;
                squate_sum += temp*temp;
            }
        }
    }

    ave /= src.rows*src.cols*src.channels();
    sigma = sqrt(squate_sum / (src.rows*src.cols*src.channels()) - ave*ave );

    std::cout << "ave :" << ave << std::endl;
    std::cout << "sigma :" << sigma << std::endl;

    //dst
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                dst.data[y*src.step + x*src.channels() + c] = (uint8_t) (sig/sigma)*(src.data[y*src.step + x*src.elemSize() + c] - ave) + mu;
                //dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)(sig/sigma) * (src.at<cv::Vec3b>(y, x)[c] - ave) + mu;
            }
        }
    }
}



int main(void)
{
    cv::Mat img = cv::imread("imori_dark.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);

    struct timeval t1, t2;

    gettimeofday(&t1, NULL);
    hist_operation(img, dst, 128, 52);//平均,標準偏差
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/22q_ans.jpg", dst);
    
    return 0;
}

/*
at:0.001213 [sec]
ave :97.729
sigma :12.7473*/

/*
↑だけdata
data:0.001258
ave :97.729
sigma :12.7473

両方ともdata
ave :97.729
sigma :12.7473
Time : 0.001190 [sec]


気持ちはやいかなーくらい．0.0001 [sec]
*/
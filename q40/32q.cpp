#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>
#include <complex>


void bgrtogray(cv::Mat &src, cv::Mat &dst)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //dst.data[y*dst.step + x*]
            dst.at<uint8_t>(y, x) = (uint8_t)(0.2126 * src.at<cv::Vec3b>(y, x)[2] + \
                0.7152 * src.at<cv::Vec3b>(y, x)[1] + \
                0.0722 * src.at<cv::Vec3b>(y, x)[0]);
        }
    }
}

struct fourier{
    std::complex<double> fft[128][128];
};

void dft(cv::Mat &src, struct fourier *f)
{   
    //パワースペクトルチェック
    cv::Mat ps_dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    double _x, temp;
    std::complex<double> ready;
    std::complex<double> csum;

    //周波数領域(dst)
    for(int l = 0; l < src.rows; l++){
        for(int k = 0; k < src.cols; k++){
            ready = 0;
            //空間領域(src)
            for(int y = 0; y < src.rows; y++){
                for(int x = 0; x < src.cols; x++){
                    _x = (double)src.at<uint8_t>(y, x);
                    //temp = -2 * M_PI * ( ((double)(k*x)/src.cols) + ((double)(l*y)/src.rows) );
                    temp = -2 * M_PI * ( (double)k * (double)x/ (double)src.cols + (double)l * (double)y / (double)src.rows );

                    //std::complex<double> csum(std::cos(temp), std::sin(temp));//OK
                    //csum.real() = std::cos(temp);//NO
                    //csum.imag() = std::sin(temp)//NO
                    csum = std::complex<double>(std::cos(temp), std::sin(temp));//OK
                    ready += csum*_x;
                }
            }
            //A
            //ready /= sqrt(src.rows * src.cols);//ここの計算意義が曖昧
            //B
            ready /= src.rows * src.cols;//こちらでもOK//資料の数式に従えばこれでいいはず．
            //C
            //なにもしない
            f->fft[l][k] = ready;
            ps_dst.at<uint8_t>(l, k) = (uint8_t)(fmin(fmax(std::abs(ready), 0), 255));
        }
    }
    
    //cv::imshow("sample", ps_dst);
    //cv::waitKey(0);
    cv::imwrite("img/fft_resB.jpg", ps_dst);
}


struct fourier_str {
  std::complex<double> coef[128][128];
};



void idft(cv::Mat &src, struct fourier *f)
{
    //空間領域(dst)
    double temp;
    std::complex<double> _X;
    std::complex<double> csum, ready;
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //周波数領域(src)
            ready = 0;//これ忘れて事故
            for(int l = 0; l < src.rows; l++){
                for(int k = 0; k < src.cols; k++){
                    _X = f->fft[l][k];
                    //temp = 2 * M_PI * ( ((double)(k*x)/src.cols) + ((double)(l*y)/src.rows) );
                    temp = 2 * M_PI * ( (double)k * (double)x/ (double)src.cols + (double)l * (double)y / (double)src.rows );
                    csum = std::complex<double>(std::cos(temp), std::sin(temp));
                    ready += csum*_X;
                }
            }
            //A
            //double result = std::abs(ready) / sqrt(src.rows * src.cols);//ここのsqrt理由が曖昧
            //B
            double result = std::abs(ready) / src.rows * src.cols;//こちらでもOK
            //C
            //double result = std::abs(ready);
            src.at<uint8_t>(y, x) = (uint8_t)(fmin(fmax(result, 0), 255));
        }
    }
}



int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat out = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);


    //cv::imshow("sample", img);
    //cv::waitKey(0);
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    struct fourier fft_result;
    struct fourier *p;
    p = &fft_result;

    bgrtogray(img, out);
    dft(out, p);
    idft(out, p);

    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", out);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/32q_ansC.jpg", out);
    
    return 0;
}
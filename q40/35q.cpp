#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>
#include <complex>

struct fourier_str {
  std::complex<double> coef[128][128];
};

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

void dft(cv::Mat &src, struct fourier_str *f)
{   
    //cv::Mat ps_dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

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
                    csum = std::complex<double>(std::cos(temp), std::sin(temp));//OK
                    ready += csum*_x;
                }
            }
            ready /= src.rows * src.cols;
            f->coef[l][k] = ready;
            //ps_dst.at<uint8_t>(l, k) = (uint8_t)(fmin(fmax(std::abs(ready), 0), 255));
        }
    }
    //cv::imwrite("img/34dft_res.jpg", ps_dst);
}




void idft(cv::Mat &src, struct fourier_str *f)
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
                    _X = f->coef[l][k];
                    //temp = 2 * M_PI * ( ((double)(k*x)/src.cols) + ((double)(l*y)/src.rows) );
                    temp = 2 * M_PI * ( (double)k * (double)x/ (double)src.cols + (double)l * (double)y / (double)src.rows );
                    csum = std::complex<double>(std::cos(temp), std::sin(temp));
                    ready += csum*_X;
                }
            }
            double result = std::abs(ready) / src.rows * src.cols;
            src.at<uint8_t>(y, x) = (uint8_t)(fmin(fmax(result, 0), 255));
        }
    }
}



void bpf(fourier_str *src, double lr, double hr)
{
    
    //円形にくり抜いて画素値0でいいはず．
    //int x0 = dst.cols/2;
    //int y0 = dst.rows/2;
    double hdr = 64 * hr;
    double ldr = 64 * lr;

    for ( int j = 0; j < 128 / 2; j++){
        for ( int i = 0; i < 128 / 2; i++){
            if (sqrt(i * i + j * j) <= hdr){
                if (sqrt(i * i + j * j) >= ldr){
                    src->coef[j][i] = 0;
                    src->coef[j][128 - i] = 0;
                    src->coef[128 - j][i] = 0;
                    src->coef[128 - j][128 - i] = 0;
                }
            }
        }
    }

    //cv::imwrite("img/34hpf_res.jpg", dst);//dstはこれ用．dstをここで宣言したくなかった．
}





int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat out = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    cv::Mat zero = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    
    //cv::imshow("sample", img);
    //cv::waitKey(0);
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    struct fourier_str fft_result;
    struct fourier_str *p;
    p = &fft_result;

    bgrtogray(img, out);
    dft(out, p);

    bpf(p, 0.1, 0.5);

    idft(zero, p);
    //idft_for_lpf(out, p);

    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", zero);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/35q_ans.jpg", zero);
    
    return 0;
}

//cv::circle()
//って関数を使ってもいいのか...?
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
    cv::imwrite("img/33gray_res.jpg", dst);    
}


void dft(cv::Mat &src, struct fourier_str *f)
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
            f->coef[l][k] = ready;
            ps_dst.at<uint8_t>(l, k) = (uint8_t)(fmin(fmax(std::abs(ready), 0), 255));
        }
    }
    
    //cv::imshow("sample", ps_dst);
    //cv::waitKey(0);
    cv::imwrite("img/33dft_res.jpg", ps_dst);
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



//fourier lowpassf(struct fourier *src, double r)
void lowpassf(struct fourier_str *src, double r)
{
    
    //円形にくり抜いて画素値0でいいはず．
    //int x0 = 128/2;
    //int y0 = 128/2;
    /*
    for(int y = 0; y < 128; y++){
        for(int x = 0; x < 128; x++){
            if(pow(x - x0, 2) + pow(y - y0, 2) >= pow(r, 2)){
                src->fft[y][x] = 0;
            }
        }
    }*/

    double dr = 64 * r;
    for ( int j = 0; j < 128 / 2; j++){
        for ( int i = 0; i < 128 / 2; i++){
            if (sqrt(i * i + j * j) >= dr){
                src->coef[j][i] = 0;
                src->coef[j][128 - i] = 0;
                src->coef[128 - j][i] = 0;
                src->coef[128 - j][128 - i] = 0;
            }
        }
    }



    //cv::imwrite("img/33lpf_res.jpg", dst);//dstはこれ用．dstをここで宣言したくなかった．
    //return src;
}


void check_pic(cv::Mat &pic1, cv::Mat &pic2)
{
    double vmax = 0;
    for(int y = 0; y < pic1.rows; y++){
        for(int x = 0; x < pic1.cols; x++){
            if(pic1.at<uint8_t>(y, x) != pic2.at<uint8_t>(y, x)){
                printf("(y = %d, x = %d ) pic1 = %d, pic2 = %d\n", y, x, pic1.at<uint8_t>(y, x), pic2.at<uint8_t>(y, x));
                vmax = fmax(vmax,   abs( (double)pic1.at<uint8_t>(y, x) - (double)pic2.at<uint8_t>(y, x) ));
                printf("vmax : %f\n", vmax);
            }
        }
    }
}



int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat out = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    cv::Mat zero = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    cv::Mat g = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
    
    //cv::imshow("sample", img);
    //cv::waitKey(0);
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    struct fourier_str fft_result;
    struct fourier_str *p;
    p = &fft_result;

    bgrtogray(img, out);
    g = out.clone();
    dft(out, p);//Mat型に複素数型のデータを格納する方法ってどうしたらいいんだ...

    //struct fourier ans = 
    lowpassf(p, 0.5);

    idft(zero, p);
    //idft_for_lpf(out, p);

    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);


    //cv::Mat gray = cv::imread("img/33gray_res.jpg");
    check_pic(g, zero);

    cv::imshow("sample", zero);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/33q_ans.jpg", zero);
    
    return 0;
}

//cv::circle()
//って関数を使ってもいいのか...?
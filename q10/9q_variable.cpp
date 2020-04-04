#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;

void gaussian_filter3x3(cv::Mat &src, cv::Mat &dst, double sigma)
{
    //making kernel
    double kernel[3][3];
    double kernel_sum = 0.0;

    for(int y = -1; y <= 1; y++){
        for(int x = -1; x <= 1; x++){
            kernel[y+1][x+1] = 1/(2*M_PI*sigma*sigma)*exp(-(y*y + x*x)/(2*sigma*sigma));
            kernel_sum += kernel[y+1][x+1];
        }
    }
    //正規化1
    for(int y = -1; y <= 1; y++){
        for(int x = -1; x <= 1; x++){
            kernel[y+1][x+1] /=kernel_sum;
        }
    }
    //kernel check
    for(int y = -1; y <= 1; y++){
        for(int x = -1; x <= 1; x++){
           std::cout <<  kernel[y+1][x+1] ;
        }
        std::cout << std::endl;
    }

    //dst
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                float sum = 0.0;

                for(int dy = -1; dy <= 1; dy++){
                    for(int dx = -1; dx <= 1; dx++){
                        //範囲外srcはs0?
                        sum += kernel[dy+1][dx+1] * (float)src.at<cv::Vec3b>(y + dy, x + dx)[c];
                    }
                }
                dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)sum;
            }
        }
    }
}

int main()
{
    cv::Mat src = cv::imread("./img/imori_noise.jpg");
    cv::Mat dst0 = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);

    cv::imshow("hoge", src);
    cv::waitKey(0);
    //標準偏差1.3の時にガウス分布に完全に従う実装．
    gaussian_filter3x3(src, dst0, 1.3);
    cv::imshow("hoge", dst0);
    cv::waitKey(0);
    
    cv::imwrite("./img/9q_ans.jpg", dst0);
    return 0;
}


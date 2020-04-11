#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void LoG_filter5x5(cv::Mat &src, cv::Mat &dst, float sigma, int size)
{
    //to Gray
    cv::Mat img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            img.at<uint8_t>(y, x) = 0.2126 * src.at<cv::Vec3b>(y, x)[2] + 
                0.7152 * src.at<cv::Vec3b>(y, x)[1] + 
                0.0722 * src.at<cv::Vec3b>(y, x)[0];
        }
    }

    cv::imshow("sample", img);
    cv::waitKey(0);

    float kernel[size][size];
    int s = size/2;
    float kernel_sum = 0;

    //making kernel
    for(int y = -s; y <= s; y++){
        for(int x = -s; x <= s; x++){
            kernel[y + s][x + s] = (x*x + y*y - sigma*sigma)/ (2*M_PI*pow(sigma, 6)) * exp(-(x*x + y*y)/(2*sigma*sigma));
            kernel_sum += kernel[y + s][x + s];
        }   
    }
    //正規化
    for(int y = -s; y <= s; y++){
        for(int x = -s; x <= s; x++){
            kernel[y + s][x + s] /= kernel_sum;
        }   
    }

    float temp;
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
                temp = 0;
                for(int dy = -2; dy <= 2; dy++){
                    for(int dx = -2; dx <= 2; dx++){
                        temp += img.at<uint8_t>(y + dy, x + dx) * kernel[dy + s][dx + s];
                    }
                }
                dst.at<uint8_t>(y, x) = fmax(0, fmin(temp, 255));
        }
    }

}

int main(void)
{
    cv::Mat img = cv::imread("imori_noise.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC1);
    
    LoG_filter5x5(img, dst, 3, 5);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/19q_ans.jpg", dst);
    
    return 0;
}

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void hist_nomalize(cv::Mat &src, cv::Mat &dst, int a, int b)
{
    cv::imshow("sample", src);
    cv::waitKey(0);

    //全画素探索
    int vmin = 255, vmax = 0;//vmin = c, vmax = d
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                if(vmin > src.at<cv::Vec3b>(y, x)[c]) vmin = src.at<cv::Vec3b>(y, x)[c];
                else if(vmax < src.at<cv::Vec3b>(y, x)[c]) vmax = src.at<cv::Vec3b>(y, x)[c];
            }
        }
    }

    std::cout << "vmin :" << vmin << std::endl;
    std::cout << "vmax :" << vmax << std::endl;

    //dst
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                if(src.at<cv::Vec3b>(y, x)[c] < vmin)
                    dst.at<cv::Vec3b>(y, x)[c] = a;
                else if(vmin <= src.at<cv::Vec3b>(y, x)[c] && src.at<cv::Vec3b>(y, x)[c] < vmax)
                    dst.at<cv::Vec3b>(y, x)[c] = (b - a)/(vmax - vmin) * (src.at<cv::Vec3b>(y, x)[c] - vmin) + a;
                else              
                    dst.at<cv::Vec3b>(y, x)[c] = b;
            }
        }
    }
}

int main(void)
{
    cv::Mat img = cv::imread("imori_dark.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);
    
    hist_nomalize(img, dst, 0, 255);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/21q_ans.jpg", dst);
    
    return 0;
}

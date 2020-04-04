#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
using namespace std;

void gaussian_filter3x3(cv::Mat &src, cv::Mat &dst)
{
    float kernel[3][3] = {
        {1/16.0, 2/16.0, 1/16.0},
        {2/16.0, 4/16.0, 2/16.0},
        {1/16.0, 2/16.0, 1/16.0},
    };

    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                float sum = 0.0;

                for(int dy = 0; dy < 3; dy++){
                    for(int dx = 0; dx < 3; dx++){
                        //範囲外srcはs0?
                        sum += kernel[dy][dx] * (float)src.at<cv::Vec3b>(y + dy, x + dx)[c];
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
    //標準偏差1.3のときのカーネルをハードコーディングした場合の実装
    gaussian_filter3x3(src, dst0);
    cv::imshow("hoge", dst0);
    cv::waitKey(0);
    
    cv::imwrite("./img/9q_ans.jpg", dst0);
    return 0;
}





#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void differential_filter(cv::Mat &src, cv::Mat &dst, bool flag)
{
    //今回は手動でカーネル生成


    float kernel[3][3] = {
        {0, -1, 0},
        {0, 1, 0},
        {0, 0, 0}
    };
    int temp;
    //3チャンネルで強引に微分フィルタをかませた場合の処理．これだと訳のわからない画像が生成される．
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                //
                temp = 0;
                for(int dy = -1; dy <= 1; dy++){
                    for(int dx = -1; dx <= 1; dx++){
                        temp += src.at<cv::Vec3b>(y + dy, x + dx)[c]*kernel[dy+1][dx+1];
                    }
                }
                dst.at<cv::Vec3b>(y, x)[c] = temp;
            }
        }
    }

}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC3);
    
    differential_filter(img, dst, 0);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/14q_failed.jpg", dst);
    
    return 0;
}
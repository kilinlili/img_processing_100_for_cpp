#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>
#include  <vector>

void median_filter3x3(cv::Mat *src, cv::Mat *dst, int size)
{
    //std::vector<uint8_t> v;
    int s = size/2;

    for(int y = 0; y < src->rows; y++){
        for(int x = 0; x < src->cols; x++){
            for(int c = 0; c < src->channels(); c++){
                //vector initialize
                std::vector<uint8_t> v;//too late...

                for(int dy = -s; dy <= s; dy++){
                    for(int dx = -s; dx <= s; dx++){
                        v.push_back(src->at<cv::Vec3b>(y + dy, x + dx)[c]);
                    }
                }

                std::sort(v.begin(), v.end());

                dst->at<cv::Vec3b>(y, x)[c] = v[5];
                
            }
        }
    }

}

int main(void){
    cv::Mat src = cv::imread("./img/imori_noise.jpg");
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    cv::imshow("test", src);
    cv::waitKey(0);
    median_filter3x3(&src, &dst, 3);
    cv::imshow("test", dst);
    cv::waitKey(0);

    cv::imwrite("./img/10q_ans.jpg", dst);
    return 0;
}

#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void differential_filter(cv::Mat &src, cv::Mat &dst, bool flag)
{
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

    float kernel[3][3] = {
        {0, -1, 0},
        {0, 1, 0},
        {0, 0, 0}
    };

    //flagで横微分に変更．
    if(flag == true){
        kernel[0][1] = 0;
        kernel[1][0] = -1;
    }

    int temp;
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
            temp = 0;
            for(int dy = -1; dy <= 1; dy++){
                for(int dx = -1; dx <= 1; dx++){
                    temp += img.at<uint8_t>(y + dy, x + dx)*kernel[dy+1][dx+1];
                }
            }
            dst.at<uint8_t>(y, x) = fmax(0, fmin(temp, 255));//0~255
        }
    }

}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC1);
    
    differential_filter(img, dst, 0);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/14q_ans.jpg", dst);
    
    return 0;
}
/*-----><-----*/
/*
横方向微分でそれに垂直方向成分がはっきり見える．(縦方向が見える．)
縦方向微分でも同様，横方向がはっきり見える．
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

void maxmin_filter(cv::Mat &src, cv::Mat &dst, int size)
{
    //Gray scale　変換からするなら...
    cv::Mat img = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            img.at<uint8_t>(y, x) = 0.2126 * (float)src.at<cv::Vec3b>(y, x)[2] \
                + 0.7152 * (float)src.at<cv::Vec3b>(y, x)[1] \
                + 0.0722 * (float)src.at<cv::Vec3b>(y, x)[0];
        }   
    }

    cv::imshow("sample", img);
    cv::waitKey(0);


    int s = size/2;
    uint8_t min,max;
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
                min = 255;
                max = 0;
                for(int dy = -s; dy <= s; dy++){
                    for(int dx = -s; dx <= s; dx++){
                        //if( (y + dy) >= 0 && (x + dx) >= 0 && (y + dy) < img.rows && (x + dx) < img.cols){
                            if(max < img.at<uint8_t>(y + dy, x + dx)){
                                max = img.at<uint8_t>(y + dy, x + dx);
                            }
                            if(min > img.at<uint8_t>(y + dy, x + dx)){
                                min = img.at<uint8_t>(y + dy, x + dx);
                            }
                        //}
                    }
                }
                dst.at<uint8_t>(y, x) = (uint8_t)max - min;
        }
    }
}
//コーナーケースの処理を行う場合，コーナー周辺の画素値が低めになる．
// 行わ無いと逆にコーナー周辺は画素値が高めになる．なんか直観的でないが...．
// 画像全体で見るとクリティカルに効いてくる処理では無い．

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst = cv::Mat::zeros(img.cols, img.rows, CV_8UC1);
    //cv::cvtColor(img, img, CV_BGR2GRAY);//めんどくさいのでGRAYスケール変換はこれ．

    maxmin_filter(img, dst, 3);
    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/13q_ans.jpg", dst);
    
    return 0;
}
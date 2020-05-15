#include <iostream>
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cstdlib>
#include <cmath>

void bgrtogray(cv::Mat &src, cv::Mat &dst)
{
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            dst.at<uint8_t>(y, x) = (uint8_t)(0.2126 * (double)src.at<cv::Vec3b>(y, x)[2] + 
                0.7152 * (double)src.at<cv::Vec3b>(y, x)[1] + 
                0.0722 * (double)src.at<cv::Vec3b>(y, x)[0]);
        }
    }
}

void otu_algorithm(cv::Mat src, cv::Mat &dst)
{
    
    int pixel_num0, pixel_num1;
    int pixel_sum0, pixel_sum1;//class sum
    double ave0, ave1;//class average

    int pixel_value = 0;
    int thres = 0;
    double between_class_variance = 0.0;
    double max_between_class_variance = 0.0;

    for(int t = 0; t < 256; t++){
        pixel_num0 = 0;
        pixel_num1 = 0;

        pixel_sum0 = 0;
        pixel_sum1 = 0;
        ave0 = 0.0;
        ave1 = 0.0;

        //making histgram
        for(int y = 0; y < src.rows; y++){
            for(int x = 0; x < src.cols; x++){

                pixel_value = src.at<uint8_t>(y, x);

                if(pixel_value < t){
                    pixel_num0++;
                    pixel_sum0 += pixel_value;
                }else{
                    pixel_num1++;
                    pixel_sum1 += pixel_value;
                }
            }
        }

        //check between-class variance
        if(pixel_num0 == 0 || pixel_num1 == 0) continue;

        //各クラスの平均
        ave0 = pixel_sum0 / pixel_num0;
        ave1 = pixel_sum1 / pixel_num1;

        
        //クラス間分散
        //これの最大値 = 分離度最大
        between_class_variance = pixel_num0*pixel_num1* pow((ave0 - ave1), 2);
        if(max_between_class_variance < between_class_variance){
            max_between_class_variance = between_class_variance;
            thres = t;
            //std::cout << "threshold is " << thres << "!" << std::endl;
        }
    }


    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            if(src.at<uint8_t>(y, x) > thres){
                dst.at<uint8_t>(y, x) = 255;
            }else{
                //dst(y, x) = 0;//動かない．
                dst.at<uint8_t>(y, x) = 0;
            }
        }
    }
}

//膨張
void morphology_dilation(cv::Mat &src, cv::Mat &dst, int n)
{
    int top, bottom, left, right;
    cv::Mat dst_base = src.clone();


    for(int i = 0; i < n; i++){
        for(int y = 0; y < dst_base.rows; y++){
            for(int x = 0; x < dst_base.cols; x++){  
                top = dst_base.at<uint8_t>(y - 1, x);
                bottom = dst_base.at<uint8_t>(y + 1, x);
                left = dst_base.at<uint8_t>(y, x - 1);
                right = dst_base.at<uint8_t>(y, x + 1);
                if(y -1 == -1){ top = 0;}
                if(y + 1 == dst_base.rows){ bottom = 0;}
                if(x - 1 == -1){ left = 0;}
                if(x + 1 == dst_base.cols){ right = 0;}

                if(top == 255 || bottom == 255 || left == 255 || right == 255)
                {
                    dst.at<uint8_t>(y, x) = 255;
                }
            }
        }

        dst_base = dst.clone();
    }
}

//縮小
void morphology_erosion(cv::Mat &src, cv::Mat &dst, int n)
{
    cv::Mat dst_base = src.clone();
    dst = src.clone();
    for(int i = 0; i < n; i++){
        for(int y = 0; y < dst_base.rows; y++){
            for(int x = 0; x < dst_base.cols; x++){
                //top
                if((0 < y - 1) && dst_base.at<uint8_t>(y - 1, x) == 0){
                    //dst.data[y*src.step + x*src.channels()] = 0;
                    dst.at<uint8_t>(y, x) = 0;
                    continue;
                }
                //bottom
                if((0 < y + 1) && dst_base.at<uint8_t>(y + 1, x) == 0){
                    //dst.data[y*src.step + x*src.channels()] = 0;
                    dst.at<uint8_t>(y, x) = 0;
                    continue;
                }
                //left
                if((0 < x - 1) && dst_base.at<uint8_t>(y, x - 1) == 0){
                    //dst.data[y*src.step + x*src.channels()] = 0;
                    dst.at<uint8_t>(y, x) = 0;
                    continue;
                }
                //right
                if((0 < x + 1) && dst_base.at<uint8_t>(y, x + 1) == 0){
                    //dst.data[y*src.step + x*src.channels()] = 0;
                    dst.at<uint8_t>(y, x) = 0;
                    continue;
                }
            }
        }

        cv::imshow("sample", dst);
        cv::waitKey(0);
        dst_base = dst.clone();
    }
}



cv::Mat image_diff(cv::Mat src0, cv::Mat src1)
{
    cv::Mat dst = cv::Mat::zeros(src1.rows, src1.cols, CV_8UC1);
    for(int y = 0; y < src1.rows; y++){
        for(int x = 0; x < src1.cols; x++){
            int value = src0.at<uint8_t>(y, x) - src1.at<uint8_t>(y, x);
            dst.at<uint8_t>(y, x) = std::min(std::max(std::abs(value), 0), 255);
        }
    }
    return dst;
}




int main(int argc, char *argv[])
{
    cv::Mat src = cv::imread("imori.jpg");
    cv::Mat gray = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat otu = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat dilation = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat erosion = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    bgrtogray(src, gray);
    cv::imshow("sample", gray);
    cv::waitKey(0);

    otu_algorithm(gray, otu);
    cv::imshow("sample", otu);
    cv::waitKey(0);

    //オープニング
    morphology_erosion(otu, erosion, 3);//収縮
    cv::imshow("sample", erosion);
    cv::waitKey(0);
    morphology_dilation(erosion, dilation, 3);//膨張
    cv::imshow("sample", dilation);
    cv::waitKey(0);

    
    //cv::Mat result = image_diff(otu, dilation);//完成度低いように見えるが...
    cv::Mat result = image_diff(dilation, otu);//闇．何故．==>absで何とか．それでも完成度低いように見える．
    cv::imshow("sample", result);
    cv::waitKey(0);
    cv::imwrite("./img/52q_ans.jpg", result);

    return 0;
}
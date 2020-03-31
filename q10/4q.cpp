#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat Otu_algo(cv::Mat src);

int main(){
    cv::Mat img;
    cv::imread("./img/imori.jpg", CV_LOAD_IMAGE_GRAYSCALE).copyTo(img);
    //cv::Mat img = cv::imread("./img/imori.jpg");
    cv::Mat dst;
    //大津の二値化
    /*
    http://www.thothchildren.com/chapter/5bc358ec51d9305189029edb
    別名:判別分析法
    分離度を最大となる閾値を求めて，自動的に二値化を行う手法．
    クラス間分散とクラス内分散の比で分離度を求める．
    その最大値を閾値とし，分離する手法．
    まずヒストグラム作るところから始まる．
    縦:画素数,横:画素値
    入力はグレースケール画像．
    THRESH_OTSU
    //https://docs.opencv.org/3.2.0/d7/d1b/group__imgproc__misc.html#gaa9e58d2860d4afa658ef70a9b1115576
    cv::threshold(img_gray, dst, 0, 255,  CV_THRESH_OTSU | CV_THRESH_BINARY );
    //今回は手動．
    */
    dst = Otu_algo(img);

    cv::imshow("sample", dst);
    cv::waitKey(0); 
    
    cv::imwrite("./img/4q_ans.jpg", dst);
    return 0;
}

cv::Mat Otu_algo(cv::Mat src)
{
    //src is GRAY SCALE!!
    int width = src.cols;
    int height = src.rows;
    //check

    std::cout << "width:" << width << std::endl;
    std::cout << "height:" << height << std::endl;
    
    int pixel_num0, pixel_num1;
    int pixel_sum0, pixel_sum1;//class sum
    double ave0, ave1;//class average

    int pixel_value = 0;
    int thres = 0;//threshold    
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
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
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

        ave0 = pixel_sum0 / pixel_num0;
        ave1 = pixel_sum1 / pixel_num1;

        between_class_variance = pixel_num0*pixel_num1* pow((ave0 - ave1), 2);
        if(max_between_class_variance < between_class_variance){
            max_between_class_variance = between_class_variance;
            thres = t;
            std::cout << "threshold is " << thres << "!" << std::endl;
        }
    }

    //apply
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(src.at<uint8_t>(y, x) > thres){
                dst.at<uint8_t>(y, x) = 255;
            }else{
                //dst(y, x) = 0;//動かない．
                dst.at<uint8_t>(y, x) = 0;
            }
        }
    }

    return dst;
}
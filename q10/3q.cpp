#include <iostream>
#include <opencv2/opencv.hpp>

int main(){
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst;
    //Binarization
    //https://docs.opencv.org/3.2.0/d7/d1b/group__imgproc__misc.html#gae8a4a146d1ca78c626a53577199e9c57
    //double cv::threshold(src, dst, thresh(閾値), maxval(最大値), type(cv::ThresholdTypes));
    //cv::ThresholdTypes:https://docs.opencv.org/3.2.0/d7/d1b/group__imgproc__misc.html#gaa9e58d2860d4afa658ef70a9b1115576
    
    
    //cv::threshold(img, dst, 128, 255, cv::THRESH_BINARY);//3q_ans_faild.jpg
    //1.BGR2GRAY
    cv::cvtColor(img, dst, CV_BGR2GRAY);
    //2.Binarization
    cv::threshold(dst, dst, 128, 255, cv::THRESH_BINARY);

    cv::imshow("sample", dst);
    cv::waitKey(0); 
    
    cv::imwrite("3q_ans.jpg", dst);
    return 0;

}

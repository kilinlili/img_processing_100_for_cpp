#include <iostream>
#include <opencv2/opencv.hpp>

int main(){
    cv::Mat img = cv::imread("imori.jpg");
    //imread("imori.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat dst;
    //BGR2GRAY
    cv::cvtColor(img, dst, CV_BGR2GRAY);

    cv::imshow("sample", dst);
    cv::waitKey(0); 
    
    cv::imwrite("2q_ans.jpg", dst);
    return 0;

}

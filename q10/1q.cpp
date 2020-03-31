#include <iostream>
#include <opencv2/opencv.hpp>

int main(){
    cv::Mat img = cv::imread("imori.jpg");
    cv::Mat dst;
    //BGR2RGB
    cv::cvtColor(img, dst, CV_BGR2RGB);

    cv::imshow("sample", dst);
    cv::waitKey(0); 
    
    cv::imwrite("1q_ans.jpg", dst);
    return 0;

}

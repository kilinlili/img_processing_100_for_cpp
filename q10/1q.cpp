#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>


cv::Mat bgrtorgb(cv::Mat src)
{
      //int width = img.cols;
  //int height = img.rows;
    //cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC3);
   // int temp;
    //printf("rows : %d\n", src.rows);
    //printf("cols : %d\n", src.cols);
    
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            //temp = src.at<cv::Vec3b>(y, x)[2];
            //src.at<cv::Vec3b>(y, x)[2] = src.at<cv::Vec3b>(y, x)[0];
            //src.at<cv::Vec3b>(y, x)[0] = temp;
            
            dst.at<cv::Vec3b>(y, x)[0] = src.at<cv::Vec3b>(y, x)[2];
            dst.at<cv::Vec3b>(y, x)[1] = src.at<cv::Vec3b>(y, x)[1];
            dst.at<cv::Vec3b>(y, x)[2] = src.at<cv::Vec3b>(y, x)[0];
        }
    }
    /*
    for (int y = 0; y < src.rows; y++){
        for (int x = 0; x < src.cols; x++){
      // R -> B
      dst.at<cv::Vec3b>(y, x)[0] = src.at<cv::Vec3b>(y, x)[2];
      // B -> R
      dst.at<cv::Vec3b>(y, x)[2] = src.at<cv::Vec3b>(y, x)[0];
      // G -> G
      dst.at<cv::Vec3b>(y, x)[1] = src.at<cv::Vec3b>(y, x)[1];
    }
  }*/
    return dst;
}


int main(){
    cv::Mat img = cv::imread("imori.jpg");
    //cv::Mat dst;
    //BGR2RGB
    //cv::cvtColor(img, dst, CV_BGR2RGB);
    cv::Mat dst = bgrtorgb(img);

    cv::imshow("sample", dst);
    cv::waitKey(0); 
    
    //cv::imwrite("./img/1q_ans.jpg", dst);
    return 0;

}

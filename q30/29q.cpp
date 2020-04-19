#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>


cv::Mat affine(cv::Mat &src, double a, double b, double c, double d, double tx, double ty)
{
    //affine_kernel
    //double kernel[3][3] = {
    //    {a, b, tx},
    //    {c, d, ty},
    //    {0, 0, 1}
    //};

    int resize_y = (int)(d * src.rows);//1.3
    int resize_x = (int)(a * src.cols);//0.8
    int src_y, src_x;
    
    cv::Mat dst = cv::Mat::zeros(resize_y, resize_x, CV_8UC3);

    double det = a*d - b*c;

    for(int y = 0; y < resize_y; y++){
        for(int x = 0; x < resize_x; x++){
            for(int ch = 0; ch < src.channels(); ch++){
                //元画像の画素を取得してそれをdstに適用する
                src_x = (int)((d * x - b * y) /det - tx);
                src_y = (int)((-c * x + a * y) /det - ty); 

                if ((src_x < 0) || (src.cols <= src_x) || (src_y < 0) || ( src.rows <= src_y)){
                    dst.at<cv::Vec3b>(y, x)[ch] = 255;//範囲外を白く．
                    continue;
                }
                dst.at<cv::Vec3b>(y, x)[ch] = src.at<cv::Vec3b>(src_y, src_x)[ch];
            }
        }
    }
    return dst;
}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    //cv::Mat dst = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
    cv::imshow("sample", img);
    cv::waitKey(0);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    cv::Mat dst = affine(img, 1.3, 0, 0, 0.8, 30, -30);
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/29q_ans2.jpg", dst);
    
    return 0;
}
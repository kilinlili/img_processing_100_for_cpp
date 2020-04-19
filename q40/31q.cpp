#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>


cv::Mat affine(cv::Mat &src, double a, double b, double c, double d, double tx, double ty, double theta, double dx, double dy)
{
    //X-shearing
    //Y-shearing
    int resize_y = (int)(d * src.rows + dy);//!
    int resize_x = (int)(a * src.cols + dx);//!
    int src_y, src_x;
    
    cv::Mat dst = cv::Mat::zeros(resize_y, resize_x, CV_8UC3);

    double det = a*d - b*c;

    if(theta){//回転
        double rad = theta / 180.0 * M_PI;
        
        a = std::cos(rad);
        b = - std::sin(rad);
        c = std::sin(rad);
        d = std::cos(rad);
        tx = 0;
        ty = 0;
        //更新
        det = a*d - b*c;

        //tx, tyで並行移動で中心合わせを行う．
        //現在の中心位置(座標でも可)
        double center_x = src.cols/2.0;
        double center_y = src.rows/2.0;
        //その中心位置から回転処理を施した後の中心位置．
        double now_center_x = (d* center_x - b*center_y)/det;
        double now_center_y = (-c* center_x + a*center_y)/det;
        //平行移動
        tx = now_center_x - center_x;
        ty = now_center_y - center_y;
    }


    for(int y = 0; y < resize_y; y++){
        for(int x = 0; x < resize_x; x++){
            //元画像の画素を取得してそれをdstに適用する
            //X-shearing
            b = dx/resize_y;
            c = dy/resize_x;
            src_x = (int)((d * x - b * y) /det - tx);
            src_y = (int)((-c * x + a * y) /det - ty); 

            if ((src_x < 0) || (src.cols <= src_x) || (src_y < 0) || ( src.rows <= src_y)){
                dst.at<cv::Vec3b>(y, x)[0] = 0;//B
                dst.at<cv::Vec3b>(y, x)[1] = 0;//G
                dst.at<cv::Vec3b>(y, x)[2] = 0;//R
                continue;
            }
            dst.at<cv::Vec3b>(y, x)[0] = src.at<cv::Vec3b>(src_y, src_x)[0];
            dst.at<cv::Vec3b>(y, x)[1] = src.at<cv::Vec3b>(src_y, src_x)[1];
            dst.at<cv::Vec3b>(y, x)[2] = src.at<cv::Vec3b>(src_y, src_x)[2];        
        }
    }
    return dst;
}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    cv::Mat dst = affine(img, 1, 0, 0, 1, 0, 0, 0, 30, 30);
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::destroyAllWindows();
    cv::imwrite("./img/31q_ans3.jpg", dst);
    
    return 0;
}
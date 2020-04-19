#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/time.h>
#include <cmath>

double h(double t){
    if(fabs(t) <= 1){
        return pow(fabs(t), 3) - 2 * pow(fabs(t), 2) +1 ;
    }else if(fabs(t) <= 2){
        return - pow(fabs(t), 3) + 5 * pow(fabs(t), 2) - 8* abs(t) + 4;
    }else{
        return 0;
    }
    
}


cv::Mat bi_cubic(cv::Mat &src, double rx, double ry)
{
    //目標サイズ
    int resize_x = (int)(src.cols * rx);
    int resize_y = (int)(src.rows * ry);
    cv::Mat dst = cv::Mat::zeros(resize_y, resize_x, CV_8UC3);


    double temp;
    double w_sum;
    //dst
    for(int y = 0; y < resize_y; y++){
        //int src_y = (int)floor(y/ry);

        for(int x = 0; x < resize_x; x++){

            //元画像の画素位置を生成したい画像位置から取得(double)
            double dy = y/ry;
            double dx = x/rx;
            //元画像の基点(int)
            int src_y = (int)floor(dy);
            int src_x = (int)floor(dx);

            for(int c = 0; c < src.channels(); c++){
                temp = 0;
                w_sum = 0;

                //距離から重みを計算．そして周辺16画素の取得．
                for(int i = -1; i <= 2; i++){
                    for(int j = -1; j <= 2; j++){
                        //元画像画素位置周辺16画素取得用
                        int current_y = src_y + i;
                        int current_x = src_x + j;

                        //距離取得
                        double dist_y = fabs(current_y - dy);//int - double
                        double dist_x = fabs(current_x - dx);

                        //重み計算(y, xそれぞれで重みを計算してかけ合わせ．)
                        double w_y = h(dist_y);
                        double w_x = h(dist_x);
                        
                        //元画像画素のある位置における重みを累積していく．
                        temp += (double)src.at<cv::Vec3b>(current_y, current_x)[c] * w_y * w_x;
                        w_sum += w_y * w_x;
                    }
                }
                temp /= w_sum;//正規化(?)
                dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)fmax(0, fmin(255,temp));
            }
        }
    }
    return dst;
}

int main(void)
{
    cv::Mat img = cv::imread("imori.jpg");
    cv::imshow("sample", img);
    cv::waitKey(0);

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    cv::Mat dst = bi_cubic(img, 1.5, 1.5);//bi_linear
    gettimeofday(&t2, NULL);
 
    double t = (double)(t2.tv_sec - t1.tv_sec) + (double)(t2.tv_usec - t1.tv_usec)/1000000;
    printf("Time : %lf [sec]\n", t);

    cv::imshow("sample", dst);
    cv::waitKey(0);
    cv::imwrite("./img/27q_ans.jpg", dst);
    
    return 0;
}
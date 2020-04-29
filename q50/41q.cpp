#include <iostream>
#include <cmath>
#include <algorithm>
#include <opencv2/opencv.hpp>
using namespace std;


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

void gaussian_filter5x5(cv::Mat &src, cv::Mat &dst, double sigma)
{
    //making kernel
    double kernel[5][5];
    double kernel_sum = 0.0;

    for(int y = -2; y <= 2; y++){
        for(int x = -2; x <= 2; x++){
            kernel[y+2][x+2] = 1/(2*M_PI*sigma*sigma)*exp(-(y*y + x*x)/(2*sigma*sigma));
            kernel_sum += kernel[y+2][x+2];
        }
    }
    //正規化1
    for(int y = -2; y <= 2; y++){
        for(int x = -2; x <= 2; x++){
            kernel[y+2][x+2] /=kernel_sum;
        }
    }
    //kernel check
    for(int y = -2; y <= 2; y++){
        for(int x = -2; x <= 2; x++){
           std::cout <<  kernel[y+2][x+2] ;
        }
        std::cout << std::endl;
    }

    //dst(3channels)
    /*
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
            for(int c = 0; c < src.channels(); c++){
                float sum = 0.0;

                for(int dy = -1; dy <= 1; dy++){
                    for(int dx = -1; dx <= 1; dx++){
                        //範囲外srcはs0?
                        sum += kernel[dy+1][dx+1] * (float)src.at<cv::Vec3b>(y + dy, x + dx)[c];
                    }
                }
                dst.at<cv::Vec3b>(y, x)[c] = (uint8_t)sum;
            }
        }
    }*/

    cv::Mat temp = cv::Mat::zeros(dst.rows, dst.cols, CV_8UC1);

    float sum;
    for(int y = 0; y < src.rows; y++){
        for(int x = 0; x < src.cols; x++){
                sum = 0.0;

                for(int dy = -2; dy <= 2; dy++){//5x5でカーネル作って3x3で畳み込みはないわぁ
                    for(int dx = -2; dx <= 2; dx++){
                        //範囲外srcはs0?
                        sum += kernel[dy+2][dx+2] * (float)src.at<uint8_t>(y + dy, x + dx);
                    }
                }
                dst.at<float>(y, x) = std::clamp(sum, 0.0f, 255.0f);
                temp.at<uint8_t>(y, x) = std::clamp((int)sum, 0, 255);
        }
    }
    
    cv::imshow("sample", temp);
    cv::waitKey(0);
    cv::destroyAllWindows();
}


void sobel_filter3x3(cv::Mat img, cv::Mat dst, bool flag)
{
    float kernelY[3][3] = {//Y方向微分-->横方向の輪郭
        {1, 2, 1},  
        {0, 0, 0},
        {-1, -2, -1}
    };

    float kernelX[3][3] = {//X方向微分-->縦方向の輪郭
        {1, 0, -1},
        {2, 0, -2},
        {1, 0, -1}
    };

    float (*kernel)[3];//OK.二次元配列のアドレス格納用．//float型3要素のポインタになる．

    //check
    if(flag){
        printf("X\n");
        kernel = kernelX;
    }else{
        printf("Y\n");
        kernel = kernelY;
    }

    double temp;
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){

            temp = 0;
            for(int dy = -1; dy <= 1; dy++){
                for(int dx = -1; dx <= 1; dx++){
                    if (((y + dy) >= 0) && (( x + dx) >= 0) && ((y + dy) < img.rows) && ((x + dx) < img.cols)){
                        temp += img.at<float>(y + dy, x + dx)*kernel[dy+1][dx+1];
                    }
                }
            }
            dst.at<float>(y, x) = fmax(0., fmin(temp, 255.0));
        }
    }

}


void get_edge(cv::Mat &fx, cv::Mat &fy, cv::Mat &dst)
{

    for(int y = 0; y < dst.rows; y++){
        for(int x = 0; x < dst.cols; x++){
            int tx = fx.at<float>(y, x);//途中計算は全部floatで殴るのが正解かもわからん．こっちのが精度高い気がしてる．
            int ty = fy.at<float>(y, x);
            dst.at<uint8_t>(y, x) = (uint8_t)std::clamp( (int)sqrt(tx*tx + ty*ty), 0, 255);//c++17
            //fmax(fmin(sqrt(pow(fx.at<uint8_t>(y, x), 2) + pow(fy.at<uint8_t>(y, x), 2)), 255), 0);
        }
    }
}

void get_angle(cv::Mat &fx, cv::Mat &fy, cv::Mat &dst)
{
    double tx;
    double ty;

    double ans;
    for(int y = 0; y < dst.rows; y++){
        for(int x = 0; x < dst.cols; x++){
            tx = (double)fx.at<float>(y, x);
            ty = (double)fy.at<float>(y, x);
            if(tx == 0) tx = 0.0000001;//分母0回避
            std::cout << tx << "::" << ty << std::endl;
            
            ans = std::atan2(ty, tx);//[-pi, pi]の範囲//radで返ってきてる．
            ans = (ans * 180)/M_PI;

            //-22.5~157.5に修正
            if(ans < -22.5){
                ans += 180;
            }else if(157.5 < ans){
                ans -= 180;
            }
            std::cout << ans << " ";
            //output
            if(ans <= 22.5){
                ans = 0;
            }else if(ans <=67.5){
                ans = 45;
            }else if(ans <= 112.5){
                ans = 90;
            }else{
                ans = 135;
            }
            dst.at<uint8_t>(y, x) = (uint8_t)ans;
        }
    }
}


int canny_step1(void)
{
    cv::Mat src = cv::imread("./imori.jpg");
    cv::Mat dst = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat gau_dst = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat fx = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);
    cv::Mat fy = cv::Mat::zeros(src.rows, src.cols, CV_32FC1);

    bgrtogray(src, dst);
    gaussian_filter5x5(dst, gau_dst, 1.4);
    sobel_filter3x3(gau_dst, fx, true);
    sobel_filter3x3(gau_dst, fy, false);

    //sobelフィルタ結果のx,y方向微分結果から勾配の大きさと勾配の角度を求める．//Step1
    cv::Mat edge = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);
    cv::Mat angle = cv::Mat::zeros(src.rows, src.cols, CV_8UC1);

    get_edge(fx, fy, edge);
    cv::imshow("sample1", edge);
    cv::waitKey(0);
    cv::imwrite("./img/41q_ans1.jpg", edge);


    get_angle(fx, fy, angle);
    cv::imshow("sample2", angle);
    cv::waitKey(0);    
    cv::imwrite("./img/41q_ans2.jpg", angle);
    return 0;
}

int main(int argc, char *argv[])
{
    canny_step1();
    return 0;
}
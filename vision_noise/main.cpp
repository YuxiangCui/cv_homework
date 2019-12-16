//
//  vision_noise.cpp
//  noise
//
//  Created by 崔瑜翔 on 2019/12/1.
//  Copyright © 2019年 崔瑜翔. All rights reserved.
//


// 自选一张清晰图像
// 增加高斯噪声
// 分别用高斯滤波和双边滤波器对两类噪声图像去噪
// 比较滤波器对各类噪声的去噪效果


#include "cv.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;


//为图像添加椒盐噪声
Mat add_Salt_Noise(const Mat srcImage, int n);
//产生高斯噪声
double generate_Gaussian_Noise();
//为图像添加高斯噪声
Mat add_Gaussian_Noise(Mat& srcImage);

int main()
{
    //读入图像
    Mat srcImage = imread("/home/cyx/桌面/cv_homework/vision_noise/input/1.jpg");
    resize(srcImage,srcImage,Size(512,334),CV_INTER_AREA);
    if (!srcImage.data)
    {
        cout << "读入图像有误！" << endl;
        system("pause");
        return -1;
    }
    imshow("srcImage", srcImage);

    //增添椒盐噪声
    Mat dstImage1 = add_Salt_Noise(srcImage, 2000);
    imshow("salt_pepper_Image", dstImage1);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/salt_pepper_Image.jpg", dstImage1);

    //增添高斯噪声
    Mat dstImage2 = add_Gaussian_Noise(srcImage);
    imshow("gaussian_Image", dstImage2);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/gaussian_Image.jpg", dstImage2);



    //均值滤波1

    Mat blur_out_0;
    blur(dstImage1,blur_out_0,Size(3, 3));
    imshow("blur_out_0", blur_out_0);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/blur_out_0.jpg", blur_out_0);

    Mat blur_out_1;
    blur(dstImage1,blur_out_1,Size(5, 5));
    imshow("blur_out_1", blur_out_1);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/blur_out_1.jpg", blur_out_1);

    //均值滤波2
    Mat blur_out_2;
    blur(dstImage2,blur_out_2,Size(5, 5));
    imshow("blur_out_2", blur_out_2);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/blur_out_2.jpg", blur_out_2);

    //高斯滤波1
    Mat gaussian_blur_out_1;
    GaussianBlur(dstImage1, gaussian_blur_out_1, Size(3, 3), 0, 0);
    imshow("gaussian_blur_out_1", gaussian_blur_out_1);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/gaussian_blur_out_1.jpg", gaussian_blur_out_1);

    Mat gaussian_blur_out_0;
    GaussianBlur(dstImage2, gaussian_blur_out_0, Size(7, 7), 0, 0);
    imshow("gaussian_blur_out_0", gaussian_blur_out_0);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/gaussian_blur_out_0.jpg", gaussian_blur_out_0);

    //高斯滤波2
    Mat gaussian_blur_out_2;
    GaussianBlur(dstImage2, gaussian_blur_out_2, Size(3, 3), 0, 0);
    imshow("gaussian_blur_out_2", gaussian_blur_out_2);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/gaussian_blur_out_2.jpg", gaussian_blur_out_2);

    //中值滤波1
    Mat median_blur_out_1;
    medianBlur(dstImage1, median_blur_out_1, 3);
    imshow("median_blur_out_1", median_blur_out_1);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/median_blur_out_1.jpg", median_blur_out_1);

    Mat median_blur_out_0;
    medianBlur(dstImage1, median_blur_out_0, 5);
    imshow("median_blur_out_1", median_blur_out_0);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/median_blur_out_0.jpg", median_blur_out_0);

    //中值滤波2
    Mat median_blur_out_2;
    medianBlur(dstImage2, median_blur_out_2, 3);
    imshow("median_blur_out_2", median_blur_out_2);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/median_blur_out_2.jpg", median_blur_out_2);

    //双边滤波
    Mat bilateral_blur_out_1;
    bilateralFilter(dstImage1, bilateral_blur_out_1, 5, 50, 10);
    imshow("bilateral_blur_out_1", bilateral_blur_out_1);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/bilateral_blur_out_1.jpg", bilateral_blur_out_1);

    //双边滤波
    Mat bilateral_blur_out_2;
    bilateralFilter(dstImage2, bilateral_blur_out_2, 5, 50, 10);
    imshow("bilateral_blur_out_2", bilateral_blur_out_2);
    imwrite("/home/cyx/桌面/cv_homework/vision_noise/output/bilateral_blur_out_2.jpg", bilateral_blur_out_2);

    waitKey();
    return 0;
}


//椒盐噪声包含两种噪声，一种是盐噪声（salt noise），另一种是胡椒噪声（pepper noise）。
//盐噪声为白色，椒噪声为黑色。前者是高灰度噪声，灰度值为255，后者属于低灰度噪声，灰度值为0。
//一般两种噪声同时出现，呈现在图像上就是黑白杂点。
//椒盐噪声往往由图像切割引起，去除脉冲干扰及椒盐噪声最常用的算法是中值滤波。
Mat add_Salt_Noise(const Mat srcImage, int n)
{
    Mat dstImage = srcImage.clone();
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1)
        {
            dstImage.at<uchar>(i, j) = 255;       //盐噪声
        }
        else
        {
            //留下彩色图片通道，方便后续代码拓展
            dstImage.at<Vec3b>(i, j)[0] = 255;
            dstImage.at<Vec3b>(i, j)[1] = 255;
            dstImage.at<Vec3b>(i, j)[2] = 255;
        }
    }
    for (int k = 0; k < n; k++)
    {
        //随机取值行列
        int i = rand() % dstImage.rows;
        int j = rand() % dstImage.cols;
        //图像通道判定
        if (dstImage.channels() == 1)
        {
            dstImage.at<uchar>(i, j) = 0;     //椒噪声
        }
        else
        {
            //留下彩色图片通道，方便后续代码拓展
            dstImage.at<Vec3b>(i, j)[0] = 0;
            dstImage.at<Vec3b>(i, j)[1] = 0;
            dstImage.at<Vec3b>(i, j)[2] = 0;
        }
    }
    return dstImage;
}

//高斯噪声是指它的概率密度函数服从高斯分布（即正态分布）的一类噪声。
//常见的高斯噪声包括起伏噪声、宇宙噪声、热噪声和散粒噪声等等。
//除常用抑制噪声的方法外，对高斯噪声的抑制方法常常采用数理统计方法。
//如果一个噪声，它的幅度分布服从高斯分布，而它的功率谱密度又是均匀分布的，则称它为高斯白噪声。
double generate_Gaussian_Noise()
{
    //定义小值
    const double epsilon = numeric_limits<double>::min();
    static double z0, z1;
    static bool flag = false;
    flag = !flag;
    //构造高斯随机变量X
    if (!flag) {
        //printf("%f\n", z1);
        return z1;
    }
    double u1, u2;
    do
    {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);
    //构造高斯随机变量Y
    z0 = sqrt(-2.0*log(u1))*cos(2 * CV_PI * u2);
    z1 = sqrt(-2.0*log(u1))*sin(2 * CV_PI * u2);
    //printf("%f\n",z0);
    return z0;
}


//为图像添加高斯噪声
Mat add_Gaussian_Noise(Mat& srcImage)
{
    Mat resultImage = srcImage.clone();
    int channels = resultImage.channels();    //获取图像的通道
    int nRows = resultImage.rows;    //图像的行数
    int nCols = resultImage.cols*channels;   //图像的总列数
    //判断图像的连续性
    if (resultImage.isContinuous())    //判断矩阵是否连续，若连续相当于只需要遍历一个一维数组
    {
        nCols *= nRows;
        nRows = 1;
    }
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {   //添加高斯噪声
            int val = resultImage.ptr<uchar>(i)[j] + (generate_Gaussian_Noise()*0.8+2) * 20;
            //防溢出
            if (val < 0)
                val = 0;
            if (val > 255)
                val = 255;
            resultImage.ptr<uchar>(i)[j] = (uchar)val;
        }
    }
    return resultImage;
}

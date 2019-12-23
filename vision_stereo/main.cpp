//
//  vision_stereo.cpp
//  noise
//
//  Created by 崔瑜翔 on 2019/12/23.
//  Copyright © 2019年 崔瑜翔. All rights reserved.
//


// 任选一种方法，计算ＴＵＫＵＢＡ图像的稠密视差，给出视差图


#include "cv.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv) {

    //读入图片
    Mat left = cv::imread("/home/cyx/桌面/cv_homework/vision_stereo/INPUT/tsukuba_l.png");
    Mat right = cv::imread("/home/cyx/桌面/cv_homework/vision_stereo/INPUT/tsukuba_r.png");
    if (left.empty() || right.empty()) {
        cout << "Failed to load image\n" << endl;
        return -1;
    }

    //转为灰度图
    cv::cvtColor(left, left, CV_BGR2GRAY);
    cv::cvtColor(right, right, CV_BGR2GRAY);

    //设置参数
    int numberOfDisparities = 64;
    int SADWindowSize = 11;
    int uniquenessRatio = 15;
    int speckleWindowSize = 50;
    int speckleRange = 32;
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(0, numberOfDisparities, SADWindowSize);
    int cn = left.channels();
    sgbm->setP1(8 * cn * SADWindowSize * SADWindowSize);
    sgbm->setP2(32 * cn * SADWindowSize * SADWindowSize);
    sgbm->setPreFilterCap(63);
    sgbm->setUniquenessRatio(uniquenessRatio);
    sgbm->setSpeckleWindowSize(speckleWindowSize);
    sgbm->setSpeckleRange(speckleRange);
    sgbm->setDisp12MaxDiff(1);

    //输出图片
    Mat disp;
    sgbm->compute(left, right, disp);
    disp.convertTo(disp, CV_8U, 255 / (numberOfDisparities*16.));
    cv::imshow("disparity", disp);
    cv::imwrite("/home/cyx/桌面/cv_homework/vision_stereo/OUTPUT/tsukuba_disparity.png", disp);
    cv::waitKey();

    return 0;
}


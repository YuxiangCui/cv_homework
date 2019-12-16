//
//  img_3.h
//  img_3
//
//  Created by 崔瑜翔 on 2018/6/14.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//


//1. Image logarithmic operation for visibility enhancement
//2. Histogram equalization


#ifndef img_3_h
#define img_3_h

#include <stdio.h>
#include <stdlib.h>
#include "math.h"


typedef unsigned char BYTE;//1
typedef unsigned short WORD;//2
typedef unsigned int DWORD;//4
typedef int LONG;//4

typedef struct bitmap_file_header {
    WORD    bfType;
    WORD    bfSize_1;
    WORD    bfSize_2;//否则自动字节对齐
    WORD    bfReserved1;
    WORD    bfReserved2;
    WORD    bfOffBits_1;
    WORD    bfOffBits_2;//否则自动字节对齐
} BMPFILEHEADER;//文件头

typedef struct bitmap_info_header {
    DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BMPINFOHEADER;//信息头

typedef struct _bgr{
    BYTE blue;
    BYTE green;
    BYTE red;
}BGR_Pixel;//BGR像素

typedef struct _yuv {
    BYTE y;
    BYTE u;
    BYTE v;
}YUV_Pixel;//YUV像素

typedef struct image {
    char* path;
    BMPFILEHEADER* bmpfileheader;
    BMPINFOHEADER* bmpinfoheader;
    BGR_Pixel* pixel;
}BMP_image;//bmp图片结构，包含文件头，信息头，像素以及路径


    //RGB->YUV
YUV_Pixel* BGRtoYUV(BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel);

    //Color to gray: gray=Y in YUV color space,转化为灰度图
YUV_Pixel* YUVToGray(YUV_Pixel* yuv, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel);

    //Rearrange gray intensity to lie between[0,255]，将灰度图均衡化，使得灰度值均匀分布在0~255间
YUV_Pixel* rearrange_gray_intensity(YUV_Pixel* gray0, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel);



//Image logarithmic operation for visibility enhancement for gray
YUV_Pixel* logarithmic_operation(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader);
//Histogram equalization for gray
YUV_Pixel* histogram_equalization(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader);
//Image logarithmic operation for visibility enhancement for color
void logarithmic_operation_color(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader);
//Histogram equalization for color
void histogram_equalization_color(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader);

//自定义函数，对于出现的溢出情况进行处理
BYTE rearrange(int num) {
    
    if (num > 255) num = 255;
    if (num < 0) num = 0;
    return (BYTE)(num);
}

#endif /* img_3_h */

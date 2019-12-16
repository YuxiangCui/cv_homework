////
////  img_1.h
////  img_1
////
////  Created by 崔瑜翔 on 2018/6/2.
////  Copyright © 2018年 崔瑜翔. All rights reserved.
////
//
#ifndef img_1_h
#define img_1_h

#include <stdio.h>
#include <stdlib.h>


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

//RGB->YUV
YUV_Pixel* BGRtoYUV(const BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel);

//Color to gray: gray=Y in YUV color space,转化为灰度图
YUV_Pixel* YUVToGray(YUV_Pixel* yuv, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel);

//Rearrange gray intensity to lie between[0,255]，将灰度图均衡化，使得灰度值均匀分布在0~255间
void rearrange_gray_intensity(YUV_Pixel* gray0, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel);

//Change the luminance value Y，改变图片亮度
void change_luminance(YUV_Pixel* yuv, int magnitude,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel);

//YUV->BGR
void YUVToBGR(const YUV_Pixel* yuv, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel);

//自定义函数，方便对于RGB<->YUV间转换时出现的溢出情况进行处理
BYTE rearrange(int num) {

    if (num > 255) num = 255;
    if (num < 0) num = 0;
    return (BYTE)(num);
}


#endif /* img_1_h */


//
//  img_2.h
//  img_2
//
//  Created by 崔瑜翔 on 2018/6/4.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

#ifndef img_2_h
#define img_2_h

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

typedef struct image {
    char* path;
    BMPFILEHEADER* bmpfileheader;
    BMPINFOHEADER* bmpinfoheader;
    BGR_Pixel* pixel;
}BMP_image;//bmp图片结构，包含文件头，信息头，像素以及路径

typedef struct kernel {
    int width;
    int height;
    BYTE* ker;
}Kernel;//核


//二值化处理
int binarize(BMP_image* image, BMP_image* binarized);
//腐蚀处理
int erode(BMP_image* image, BMP_image* eroded, Kernel* kernel) ;
//膨胀处理
int dilate(BMP_image* image, BMP_image* dilated, Kernel* kernel) ;
//开运算先进行腐蚀再进行膨胀
int opening(BMP_image* image, BMP_image* opening, Kernel* kernel);
//闭运算先进行膨胀再进行腐蚀
int closing(BMP_image* image, BMP_image* closing, Kernel* kernel);

//自定义函数，对于出现的溢出情况进行处理
BYTE rearrange(int num) {
    
    if (num > 255) num = 255;
    if (num < 0) num = 0;
    return (BYTE)(num);
}

#endif /* img_2_h */

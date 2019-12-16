//
//  img_4.h
//  img_4
//
//  Created by 崔瑜翔 on 2018/6/20.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

#ifndef img_4_h
#define img_4_h


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

void translation(BGR_Pixel* bgr,FILE * in_file,FILE * out_file,int dx,int dy);
void rotation(BGR_Pixel* bgr,FILE* in_file,FILE* out_file,double angle);
void scale(BGR_Pixel* bgr, FILE* in_file,FILE* out_file,double ratio_x,double ration_y);
void shear(BGR_Pixel* bgr, FILE* in_file,FILE* out_file,double dx,double dy);
void mirror(BGR_Pixel* bgr,FILE * in_file,FILE * out_file);

//自定义函数，对于出现的溢出情况进行处理
BYTE rearrange(int num) {
    
    if (num > 255) num = 255;
    if (num < 0) num = 0;
    return (BYTE)(num);
}


#endif /* img_4_h */

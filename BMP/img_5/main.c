//
//  main.c
//  img_5
//
//  Created by 崔瑜翔 on 2018/6/23.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

//􏰀Image mean filtering
//􏰀Laplacian image enhancement

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "img_5.h"

#define PI 3.1415926

int main(int argc, const char * argv[]) {
    
//文件路径
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/图片/stars/stars.bmp", "rb");
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/图片/ironman/ironman.bmp", "rb");
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/图片/game_of_thrones/game_of_thrones.bmp", "rb");
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/图片/moon/moon.bmp", "rb");
    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/图片/children/children.bmp", "rb");
    
    FILE *out_file_laplace_filter_unrearranged = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/laplace_filter_unrearranged.bmp", "wb");
    FILE *out_file_laplace_filter = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/laplace_filter.bmp", "wb");
    FILE *out_file_mean_filter = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/mean_filter.bmp", "wb");
    FILE *out_file_laplace_filter_mask3 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/laplace_filter_mask3.bmp", "wb");
    FILE *out_file_laplace_filter_unrearranged_mask3 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_5/laplace_filter_unrearranged_mask3.bmp", "wb");
    
        //分配空间给文件头&信息头
    BMPFILEHEADER *bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(file, 0L, SEEK_SET);
    fread(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fread(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    
    int numofpixel = (*bmpinfoheader).biWidth * (*bmpinfoheader).biHeight;
    
    BGR_Pixel* bgr = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*(numofpixel));
    fread(bgr, sizeof(BGR_Pixel), (numofpixel), file);
    
    
    int mask1[9]={1,1,1,
                  1,1,1,
                  1,1,1};//mean_filter
    
    int mask2[9]={-1,-1,-1,
                  -1,8,-1,
                  -1,-1,-1};//laplace_filter
    
    int mask3[9]={1,1,1,
                  1,-8,1,
                  1,1,1};//laplace_filter
    
    mean_filter(bgr,bmpfileheader,bmpinfoheader,numofpixel,out_file_mean_filter,mask1);
    laplace_filter(bgr,bmpfileheader,bmpinfoheader,numofpixel,out_file_laplace_filter_unrearranged,mask2);
    laplace_filter_rearranged(bgr,bmpfileheader,bmpinfoheader,numofpixel,out_file_laplace_filter,mask2);
    
    laplace_filter(bgr,bmpfileheader,bmpinfoheader,numofpixel,out_file_laplace_filter_unrearranged_mask3,mask3);
    laplace_filter_rearranged(bgr,bmpfileheader,bmpinfoheader,numofpixel,out_file_laplace_filter_mask3,mask3);
    
    fseek(file,54L,0);
    
    fclose(file);
    
}


void mean_filter(BGR_Pixel* bgr, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel,FILE* out_file,int* mask){
   
    YUV_Pixel* yuv =  BGRtoYUV(bgr, bmpfileheader, bmpinfoheader, numofpixel);
    
    YUV_Pixel* mean_filter_yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    BGR_Pixel* mean_filter_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * numofpixel);
    
    int width = (*bmpinfoheader).biWidth;
    int height = (*bmpinfoheader).biHeight;
    unsigned char y = 0;
    unsigned char u,v;

    int temp_y=0;
    for (int i=0;i < height;i++)
        for (int j = 0; j < width; j++){
            if (i >= 1 && j >= 1 && i <= height - 2 && j <= width - 2) {
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        temp_y += yuv[m * width + n].y * mask[(m - i + 1) * 3 + n - j + 1];
                    }
                }
                temp_y /= 9;
                y = temp_y;
                if (temp_y>255) {
                    y=255;
                }
                if (temp_y<0) {
                    y=0;
                }
               
            }

            else
                y = yuv[i * width + j].y;
            
            u = yuv[i * width + j].u;
            v = yuv[i * width + j].v;

            mean_filter_yuv[i * width + j].y = y;
            mean_filter_yuv[i * width + j].u = u;
            mean_filter_yuv[i * width + j].v = v;

        }
    mean_filter_done = YUVToBGR(mean_filter_yuv, bmpfileheader, bmpinfoheader, numofpixel,out_file);
}

void laplace_filter(BGR_Pixel* bgr, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel,FILE* out_file,int* mask){
    
    YUV_Pixel* yuv =  BGRtoYUV(bgr, bmpfileheader, bmpinfoheader, numofpixel);
    
    YUV_Pixel* laplace_filter_yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    BGR_Pixel* laplace_filter_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * numofpixel);
    
    int width = (*bmpinfoheader).biWidth;
    int height = (*bmpinfoheader).biHeight;
    
    unsigned char y = 0;
    unsigned char u,v;
    unsigned char center_y = 0;
    
    int mingray = 255;
    int maxgray = 0;
    
    int temp_y;
    
        //找到灰度极值
    for (int i = 0;i < height;i++)
        for (int j = 0; j < width; j++){
            if (i >= 1 && j >= 1 && i <= height - 2 && j <= width - 2) {
                temp_y = 0;
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        temp_y += yuv[m * width + n].y * mask[(m - i + 1) * 3 + n - j + 1];
                    }
                }
                if (mingray > temp_y) {
                    mingray = temp_y;
                }
                if (maxgray < temp_y) {
                    maxgray = temp_y;
                }
            }
            
        }
    
    for (int i = 0;i < height;i++)
        for (int j = 0; j < width; j++){
            temp_y = 0;
            if (i >= 1 && j >= 1 && i <= height - 2 && j <= width - 2) {
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        temp_y += yuv[m * width + n].y * mask[(m - i + 1) * 3 + n - j + 1];
                    }
                }
                
                center_y = yuv[i * width + j].y;
                
                int value = temp_y;

                
                int dy = 1.0 * (value * mask[0] * -1) / 7;
                
                y = center_y + dy;
                
                if ((center_y + dy) > 255) {
                    y = 255;
                }
                if ((center_y + dy) <= 0) {
                    y = 0;
                }
            }
            else{
                y = yuv[i * width + j].y;
            }
            
            
            u = yuv[i * width + j].u;
            v = yuv[i * width + j].v;
            
            laplace_filter_yuv[i * width + j].y = y;
            laplace_filter_yuv[i * width + j].u = u;
            laplace_filter_yuv[i * width + j].v = v;
        }
    laplace_filter_done = YUVToBGR(laplace_filter_yuv, bmpfileheader, bmpinfoheader, numofpixel,out_file);
    
}

void laplace_filter_rearranged(BGR_Pixel* bgr, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel,FILE* out_file,int* mask){
    
    YUV_Pixel* yuv =  BGRtoYUV(bgr, bmpfileheader, bmpinfoheader, numofpixel);
    
    YUV_Pixel* laplace_filter_yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    BGR_Pixel* laplace_filter_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * numofpixel);
    
    int width = (*bmpinfoheader).biWidth;
    int height = (*bmpinfoheader).biHeight;
    
    unsigned char y = 0;
    unsigned char u,v;
    unsigned char center_y = 0;
    
    int mingray = 255;
    int maxgray = 0;
    
    int temp_y;
    
        //找到灰度极值
    for (int i = 0;i < height;i++)
        for (int j = 0; j < width; j++){
            if (i >= 1 && j >= 1 && i <= height - 2 && j <= width - 2) {
                temp_y = 0;
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        temp_y += yuv[m * width + n].y * mask[(m - i + 1) * 3 + n - j + 1];
                    }
                }
                if (mingray > temp_y) {
                    mingray = temp_y;
                }
                if (maxgray < temp_y) {
                    maxgray = temp_y;
                }
            }
            
        }
    
    for (int i = 0;i < height;i++)
        for (int j = 0; j < width; j++){
            temp_y = 0;
            if (i >= 1 && j >= 1 && i <= height - 2 && j <= width - 2) {
                //除了边缘像素，进行卷积
                for (int m = i - 1; m <= i + 1; m++) {
                    for (int n = j - 1; n <= j + 1; n++) {
                        temp_y += yuv[m * width + n].y * mask[(m - i + 1) * 3 + n - j + 1];
                    }
                }
                
                center_y = yuv[i * width + j].y;
                
                int value = 1.0 * (temp_y - mingray) / (maxgray - mingray) * 255;//rearrange
                
                int dy = (value * mask[0] * -1) / 7;//根据正负判断加减
                
                y = center_y + dy;
                
                if ((center_y + dy) > 255) {
                    y = 255;
                }
                if ((center_y + dy) <= 0) {
                    y = 0;
                }
            }
            else{
                y = yuv[i * width + j].y;
            }
            
            
            u = yuv[i * width + j].u;
            v = yuv[i * width + j].v;
            
            laplace_filter_yuv[i * width + j].y = y;
            laplace_filter_yuv[i * width + j].u = u;
            laplace_filter_yuv[i * width + j].v = v;
        }
    laplace_filter_done = YUVToBGR(laplace_filter_yuv, bmpfileheader, bmpinfoheader, numofpixel,out_file);
    
}


YUV_Pixel* BGRtoYUV(const BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel) {
    
    YUV_Pixel* yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    for (int i = 0; i < numofpixel; i++) {
            //利用公式实现BGR->YUV
        yuv[i].y = rearrange((int)(bgr[i].blue * 0.114 + bgr[i].green * 0.587 + bgr[i].red * 0.299));
        yuv[i].u = rearrange((int)(bgr[i].blue * 0.436 - bgr[i].green * 0.289 - bgr[i].red * 0.147) + 128);
        yuv[i].v = rearrange((int)(-bgr[i].blue * 0.1 - bgr[i].green * 0.515 + bgr[i].red * 0.615) + 128);
    }
    return yuv;
}

YUV_Pixel* YUVToGray(YUV_Pixel* yuv, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel) {
    
    YUV_Pixel* gray = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    for (int i = 0; i < numofpixel; i++) {
            //将YUV的值全部变为Y的值
        gray[i].y = yuv[i].y;
        gray[i].u = yuv[i].y;
        gray[i].v = yuv[i].y;
    }
    return gray;
}

BGR_Pixel* YUVToBGR(const YUV_Pixel* yuv,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel, FILE* out_file) {
    BGR_Pixel* bgr = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*numofpixel);
    for (int i = 0; i < numofpixel; i++) {

        bgr[i].red = rearrange((int)(yuv[i].y + (yuv[i].v-128) * 1.4075));
        bgr[i].green = rearrange((int)(yuv[i].y - (yuv[i].u-128) * 0.3455 - (yuv[i].v-128) * 0.7169));
        bgr[i].blue = rearrange((int)(yuv[i].y + (yuv[i].u-128) * 1.779));
    }
        //保存文件
    fseek(out_file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    fwrite(bgr, sizeof(BGR_Pixel), numofpixel, out_file);
    fclose(out_file);
    return bgr;
}

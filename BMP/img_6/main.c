//
//  main.c
//  img_6
//
//  Created by 崔瑜翔 on 2018/6/25.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "img_6.h"

#define PI 3.1415926

int main(int argc, const char * argv[]) {
    
        //文件路径
        //    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/stars.bmp", "rb");
        //    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/ironman.bmp", "rb");
            FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/game_of_thrones.bmp", "rb");
  //  FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/oldman.bmp", "rb");
    
    FILE *out_file_bilateral_filter_1 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_1.bmp", "wb");
    FILE *out_file_bilateral_filter_2 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_2.bmp", "wb");
    FILE *out_file_bilateral_filter_3 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_3.bmp", "wb");
    FILE *out_file_bilateral_filter_4 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_4.bmp", "wb");
    FILE *out_file_bilateral_filter_5 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_5.bmp", "wb");
    FILE *out_file_bilateral_filter_6 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/out_file_bilateral_filter_6.bmp", "wb");
    
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
    
    YUV_Pixel* yuv =  BGRtoYUV(bgr, bmpfileheader, bmpinfoheader, numofpixel);
    
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 2, 0.5, out_file_bilateral_filter_1);
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 2, 10, out_file_bilateral_filter_2);
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 2, 50, out_file_bilateral_filter_3);
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 2, 100, out_file_bilateral_filter_4);
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 20, 10, out_file_bilateral_filter_5);
    bilateral_filter(yuv, bmpfileheader, bmpinfoheader, 10, 200, 10, out_file_bilateral_filter_6);
    
    fclose(file);
    
}


void bilateral_filter(const YUV_Pixel* yuv,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int window_r,double space_sigma,double range_sigma,FILE* out_file){
    
    
    
    int width = (*bmpinfoheader).biWidth;
    int height = (*bmpinfoheader).biHeight;
    
    YUV_Pixel* bilateral_filter = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * width * height);
    
    double space_weight[2 * window_r + 1][2 * window_r + 1];
    
    
    for (int i = -window_r; i <= window_r; i++) {
        for (int j = -window_r; j <= window_r; j++) {
            space_weight[i + window_r][j + window_r]=exp(-0.5 * (i * i + j * j) / space_sigma / space_sigma);
        }
    }
    
    fseek(out_file, 54L, 0);
    int result_y = 0;
    for (int i = 0;i < height;i++)
        for (int j = 0; j < width; j++){
            int y = yuv[i * width + j].y;

                double w_sum=0;
                double w_coefficient_sum=0;
                
                for (int w_col = -window_r; w_col <= window_r; w_col++){
                    for (int w_row = -window_r; w_row <= window_r; w_row++){
                        int row,col;
                        row = w_row + j;
                        col = w_col + i;

                        int temp_y;
                        if(row < 0||col < 0||row >= width||col >= height)
                            temp_y = 0;
                        else
                            temp_y = yuv[(col) * width + (row)].y;
                        
                        double range_coefficient = exp(-0.5*(temp_y - y)*(temp_y - y) / range_sigma / range_sigma);
                        double total_coefficient = range_coefficient * space_weight[w_row + window_r][w_col + window_r];
                        
                        w_coefficient_sum += total_coefficient;
                        w_sum += temp_y * total_coefficient;
                        
                    }
                }
                
                result_y = w_sum / w_coefficient_sum;
                y = result_y;

            bilateral_filter[i * width + j].y = y;
            bilateral_filter[i * width + j].u = y;
            bilateral_filter[i * width + j].v = y;
        
            
    }
    
    fseek(out_file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    fwrite(bilateral_filter, sizeof(YUV_Pixel), width * height, out_file);
    fclose(out_file);
    
}


YUV_Pixel* BGRtoYUV(const BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel) {
    
    YUV_Pixel* yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    for (int i = 0; i < numofpixel; i++) {
            //利用公式实现BGR->YUV
        yuv[i].y = rearrange((int)(bgr[i].blue * 0.114 + bgr[i].green * 0.587 + bgr[i].red * 0.299));
        yuv[i].u = rearrange((int)(bgr[i].blue * 0.436 - bgr[i].green * 0.289 - bgr[i].red * 0.147) + 128);
        yuv[i].v = rearrange((int)(-bgr[i].blue * 0.1 - bgr[i].green * 0.515 + bgr[i].red * 0.615) + 128);
    }
    
    YUVToGray(yuv, bmpfileheader, bmpinfoheader, numofpixel);
    
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
 
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_6/GRAY.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    return gray;
}

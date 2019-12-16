//
//  main.c
//  img_3
//
//  Created by 崔瑜翔 on 2018/6/14.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

//1. Image logarithmic operation for visibility enhancement
//2. Histogram equalization

#include <stdio.h>
#include "img_3.h"
#include "math.h"



int main(int argc, const char * argv[]) {
        //文件路径
    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars.bmp", "rb");
    
        //分配空间给文件头&信息头
    BMPFILEHEADER *bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(file, 0L, SEEK_SET);
    fread(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fread(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    
//    int width = (*bmpinfoheader).biWidth;
//    int height = (*bmpinfoheader).biHeight;
//
//    int * maxgrey;
//    int * mingrey;
   
    
    int numofpixel = (*bmpinfoheader).biWidth * (*bmpinfoheader).biHeight;
    
    BGR_Pixel* bgr = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*(numofpixel));
    fread(bgr, sizeof(BGR_Pixel), (numofpixel), file);
    fclose(file);
    
    //RGB->YUV
    YUV_Pixel* yuv = BGRtoYUV(bgr, bmpfileheader, bmpinfoheader,numofpixel);
    
    //Color to gray: gray=Y in YUV color space,转化为灰度图
    YUV_Pixel* yuvtogray = YUVToGray(yuv,bmpfileheader, bmpinfoheader,numofpixel);
    
    //Rearrange gray intensity to lie between[0,255]，将灰度图均衡化，使得灰度值均匀分布在0~255间
    //YUV_Pixel* rearranged_gray = rearrange_gray_intensity(yuvtogray, bmpfileheader, bmpinfoheader,numofpixel);
    
    //Image logarithmic operation for visibility enhancement for gray
    logarithmic_operation(yuvtogray,numofpixel,bmpfileheader,bmpinfoheader);
    
    //Histogram equalization for gray
    histogram_equalization(yuvtogray,numofpixel,bmpfileheader,bmpinfoheader);
    
    //Image logarithmic operation for visibility enhancement for color
    logarithmic_operation_color(yuv,numofpixel,bmpfileheader,bmpinfoheader);
    
    //Histogram equalization for color
    histogram_equalization_color(yuv,numofpixel,bmpfileheader,bmpinfoheader);
    
}



YUV_Pixel* BGRtoYUV(BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel) {
    
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
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_GRAY.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    return gray;

}

YUV_Pixel* rearrange_gray_intensity(YUV_Pixel* gray0, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel) {
    
    YUV_Pixel* rearrange_gray = (YUV_Pixel*)malloc(sizeof(YUV_Pixel)* numofpixel);
    
        //先对像素进行遍历，找到灰度级的最大值和最小值
    int low = 255;
    int high = 0;
    for (int i = 0; i < numofpixel; i++) {
        if (low > gray0[i].y) low = gray0[i].y;
        if (high < gray0[i].y) high = gray0[i].y;
    }
    
//    * maxgrey = high;
//    * mingrey = low;
        //得到相应的转换比例
    float ratio = (high - low) / 255.0f;
    
        //按比例更新灰度级
    for (int i = 0; i < numofpixel; i++) {
        rearrange_gray[i].y = (BYTE)((gray0[i].y - low) / ratio);
        rearrange_gray[i].u = (BYTE)((gray0[i].y - low) / ratio);
        rearrange_gray[i].v = (BYTE)((gray0[i].y - low) / ratio);
        
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_REARRANGE_GRAY.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(rearrange_gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    return rearrange_gray;
    
}

YUV_Pixel* logarithmic_operation(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader){

    YUV_Pixel* logarithmic_operation_gray = (YUV_Pixel*)malloc(sizeof(YUV_Pixel)* numofpixel);
    
    //遍历找到灰度的最大最小值，便于后面带入公式
    int low = 255;
    int high = 0;
    for (int i = 0; i < numofpixel; i++) {
        if (low > yuv[i].y) low = yuv[i].y;
        if (high < yuv[i].y) high = yuv[i].y;
    }
    
    for (int i = 0; i < numofpixel; i++) {
        //利用公式对灰度值进行对数增强，保留u、v值
        logarithmic_operation_gray[i].y = rearrange(255 * (log2(yuv[i].y+1)-log2(low+1))/(log2(high+1)-log2(low+1)));
        logarithmic_operation_gray[i].u = logarithmic_operation_gray[i].y;
        logarithmic_operation_gray[i].v = logarithmic_operation_gray[i].y;
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_logarithmic_operation_gray.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(logarithmic_operation_gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    
    return logarithmic_operation_gray;
    
}

YUV_Pixel* histogram_equalization(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader){
    
    YUV_Pixel* histogram_equalization_gray = (YUV_Pixel*)malloc(sizeof(YUV_Pixel)* numofpixel);
    
    double original_gray[256]={0};//原有各灰度值个数数组
    double cdf_gray[256]={0};//累计概率数组
    double int_cdf_gray[256]={0};//根据累计概率数组进行转变得到直方图均衡化后的灰度划分
    
    for (int i = 0; i < numofpixel; i++) {
            original_gray[yuv[i].y]++;//统计各个灰度级的总数
    }
    for (int i = 0; i<256; i++) {
        original_gray[i] /= (numofpixel);//转变为各个灰度级的概率
    }
    
    //统计累计分布概率
    cdf_gray[0] = original_gray[0];
    for (int i =1; i<256; i++) {
        cdf_gray[i]=cdf_gray[i-1]+original_gray[i];
    }
    
    for (int i = 0; i<256; i++) {
        int_cdf_gray[i] = round(255*cdf_gray[i]);//根据概率划分新的灰度值
    }
    for (int i = 0; i < numofpixel; i++) {
        histogram_equalization_gray[i].y = int_cdf_gray[yuv[i].y];
        histogram_equalization_gray[i].u = int_cdf_gray[yuv[i].y];
        histogram_equalization_gray[i].v = int_cdf_gray[yuv[i].y];
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_histogram_equalization_gray.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(histogram_equalization_gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    
    return histogram_equalization_gray;
    
}

void logarithmic_operation_color(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader){
    
    BGR_Pixel* logarithmic_operation_color = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)* numofpixel);
    
    int low = 255;
    int high = 0;
    for (int i = 0; i < numofpixel; i++) {
        if (low > yuv[i].y) low = yuv[i].y;
        if (high < yuv[i].y) high = yuv[i].y;
    }
    int y;
    for (int i = 0; i < numofpixel; i++) {
        
        y = rearrange(255 * (log2(yuv[i].y+1)-log2(low+1))/(log2(high+1)-log2(low+1)));
        //根据新的y值重新反算RGB
        logarithmic_operation_color[i].red = rearrange((int)(y + (yuv[i].v-128) * 1.4075));
        logarithmic_operation_color[i].green = rearrange((int)(y - (yuv[i].u-128) * 0.3455 - (yuv[i].v-128) * 0.7169));
        logarithmic_operation_color[i].blue = rearrange((int)(y + (yuv[i].u-128) * 1.779));
        
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_logarithmic_operation_color.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(logarithmic_operation_color, sizeof(BGR_Pixel), numofpixel, file);
    fclose(file);
    
}

void histogram_equalization_color(YUV_Pixel * yuv,int numofpixel,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader){
    
    BGR_Pixel* histogram_equalization_color = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)* numofpixel);
    
    double original_gray[256]={0};
    double cdf_gray[256]={0};
    double int_cdf_gray[256]={0};
    for (int i = 0; i < numofpixel; i++) {
        original_gray[yuv[i].y]++;//统计各个灰度级的总数
    }
    for (int i = 0; i<256; i++) {
        original_gray[i] /= (numofpixel);//转变为各个灰度级的概率
    }
    
        //统计累计分布概率
    cdf_gray[0] = original_gray[0];
    for (int i =1; i<256; i++) {
        cdf_gray[i]=cdf_gray[i-1]+original_gray[i];
    }
    
    for (int i = 0; i<256; i++) {
        int_cdf_gray[i] = round(255*cdf_gray[i]);//get the result grey value
    }
    int y;
    for (int i = 0; i < numofpixel; i++) {
        y = int_cdf_gray[yuv[i].y];
        //根据新的y值重新反算RGB
        histogram_equalization_color[i].red = rearrange((int)(y + (yuv[i].v-128) * 1.4075));
        histogram_equalization_color[i].green = rearrange((int)(y - (yuv[i].u-128) * 0.3455 - (yuv[i].v-128) * 0.7169));
        histogram_equalization_color[i].blue = rearrange((int)(y + (yuv[i].u-128) * 1.779));
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_3/stars_histogram_equalization_color.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(histogram_equalization_color, sizeof(BGR_Pixel), numofpixel, file);
    fclose(file);

    
}

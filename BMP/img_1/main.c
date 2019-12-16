////
////  main.c
////  img_1
////
////  Created by 崔瑜翔 on 2018/6/2.
////  Copyright © 2018年 崔瑜翔. All rights reserved.
////
//
#include <stdio.h>
#include "img_1.h"

int main(int argc, char** argv) {
    //文件路径
    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman.bmp", "rb");
    
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
    fclose(file);

    //RGB->YUV
    YUV_Pixel* yuv = BGRtoYUV(bgr, bmpfileheader, bmpinfoheader,numofpixel);
    
    //Color to gray: gray=Y in YUV color space,转化为灰度图
    YUV_Pixel* yuvtogray = YUVToGray(yuv,bmpfileheader, bmpinfoheader,numofpixel);
    
    //Rearrange gray intensity to lie between[0,255]，将灰度图均衡化，使得灰度值均匀分布在0~255间
    rearrange_gray_intensity(yuvtogray, bmpfileheader, bmpinfoheader,numofpixel);
    
    //Change the luminance value Y，改变图片亮度
    change_luminance(yuv,60,bmpfileheader, bmpinfoheader, numofpixel);
    
    //YUV->BGR
    YUVToBGR(yuv, bmpfileheader, bmpinfoheader, numofpixel);

}

YUV_Pixel* BGRtoYUV(const BGR_Pixel* bgr,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel) {

    YUV_Pixel* yuv = (YUV_Pixel*)malloc(sizeof(YUV_Pixel) * numofpixel);
    for (int i = 0; i < numofpixel; i++) {
        //利用公式实现BGR->YUV
        yuv[i].y = rearrange((int)(bgr[i].blue * 0.114 + bgr[i].green * 0.587 + bgr[i].red * 0.299));
        yuv[i].u = rearrange((int)(bgr[i].blue * 0.436 - bgr[i].green * 0.289 - bgr[i].red * 0.147) + 128);
        yuv[i].v = rearrange((int)(-bgr[i].blue * 0.1 - bgr[i].green * 0.515 + bgr[i].red * 0.615) + 128);
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman_YUV.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(yuv, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
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
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman_GRAY.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
    return gray;
}

void rearrange_gray_intensity(YUV_Pixel* gray0, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel) {

    YUV_Pixel* rearrange_gray = (YUV_Pixel*)malloc(sizeof(YUV_Pixel)* numofpixel);
    
    //先对像素进行遍历，找到灰度级的最大值和最小值
    int low = 255;
    int high = 0;
    for (int i = 0; i < numofpixel; i++) {
        if (low > gray0[i].y) low = gray0[i].y;
        if (high < gray0[i].y) high = gray0[i].y;
    }
    //得到相应的转换比例
    float ratio = (high - low) / 255.0f;
    
    //按比例更新灰度级
    for (int i = 0; i < numofpixel; i++) {
        rearrange_gray[i].y = (BYTE)((gray0[i].y - low) / ratio);
        rearrange_gray[i].u = (BYTE)((gray0[i].y - low) / ratio);
        rearrange_gray[i].v = (BYTE)((gray0[i].y - low) / ratio);
        
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman_REARRANGE_GRAY.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(rearrange_gray, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);

}



void change_luminance(YUV_Pixel *yuv, int magnitude, BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader,int numofpixel) {

    YUV_Pixel* change = (YUV_Pixel*)malloc(sizeof(YUV_Pixel)* numofpixel);
    //根据输入的改变幅值对Y进行修改，并且保证仍然位于0~255间
    for (int i = 0; i < numofpixel; i++) {
        change[i].y = rearrange(yuv[i].y + magnitude);
        change[i].u = yuv[i].u;
        change[i].v = yuv[i].v;
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman_CHANGE_LUMINANCE.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(change, sizeof(YUV_Pixel), numofpixel, file);
    fclose(file);
}

void YUVToBGR(const YUV_Pixel* yuv,BMPFILEHEADER *bmpfileheader,BMPINFOHEADER *bmpinfoheader, int numofpixel) {
    BGR_Pixel* bgr = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*numofpixel);
    for (int i = 0; i < numofpixel; i++) {
        //利用公式实现YUV->BGR
//        bgr[i].red = rearrange((int)(yuv[i].y + (yuv[i].v-128) * 1.13983f));
//        bgr[i].green = rearrange((int)(yuv[i].y + (yuv[i].u-128) * -0.39465f + (yuv[i].v-128) * -0.5806f));
//        bgr[i].blue = rearrange((int)(yuv[i].y + (yuv[i].u-128) * 2.03211f));
        bgr[i].red = rearrange((int)(yuv[i].y + (yuv[i].v-128) * 1.4075));
        bgr[i].green = rearrange((int)(yuv[i].y - (yuv[i].u-128) * 0.3455 - (yuv[i].v-128) * 0.7169));
        bgr[i].blue = rearrange((int)(yuv[i].y + (yuv[i].u-128) * 1.779));
    }
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_1/oldman_BACKTO_BGR.bmp", "wb");
    fseek(file, 0L, SEEK_SET);
    fwrite(bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(bgr, sizeof(BGR_Pixel), numofpixel, file);
    fclose(file);
}


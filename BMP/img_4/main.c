//
//  main.c
//  img_4
//
//  Created by 崔瑜翔 on 2018/6/20.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

//Simple geometric transformation
//▪ Translation
//▪ Rotation
//▪ Scale
//▪ Shear
//▪ Mirror

#include <stdio.h>
#include <math.h>
#include <float.h>
#include "img_4.h"

#define PI 3.1415926

int main(int argc, const char * argv[]) {
    
        //文件路径
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/stars.bmp", "rb");
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/ironman.bmp", "rb");
//    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/game_of_thrones.bmp", "rb");
    FILE *file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/oldman.bmp", "rb");
    
    FILE *out_translation_1 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/translation_1.bmp", "wb");
    FILE *out_translation_2 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/translation_2.bmp", "wb");
    FILE *out_translation_3 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/translation_3.bmp", "wb");
    FILE *out_translation_4 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/translation_4.bmp", "wb");
    FILE *out_rotation_30 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/rotation_30.bmp", "wb");
    FILE *out_rotation_minus30 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/rotation_minus30.bmp", "wb");
    FILE *out_rotation_90 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/rotation_90.bmp", "wb");
    FILE *out_rotation_minus90 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/rotation_minus90.bmp", "wb");
    
    FILE *out_scale_0_5_0_5 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/scale_0_5_0_5.bmp", "wb");
    FILE *out_scale_1_0_0_5 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/scale_1_0_0_5.bmp", "wb");
    FILE *out_scale_0_5_1_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/scale_0_5_1_0.bmp", "wb");
    FILE *out_scale_2_0_0_5 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/scale_2_0_0_5.bmp", "wb");
    FILE *out_scale_0_5_2_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/scale_0_5_2_0.bmp", "wb");
    
    FILE *out_shear_0_0_1_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/shear_0_0_1_0.bmp", "wb");
    FILE *out_shear_re_0_0_1_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/shear_re_0_0_1_0.bmp", "wb");
    FILE *out_shear_1_0_0_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/shear_1_0_0_0.bmp", "wb");
    FILE *out_shear_re_1_0_0_0 = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/shear_re_1_0_0_0.bmp", "wb");
    
    FILE *out_mirror = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_4/mirror.bmp", "wb");

    
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
    
    
    translation(bgr, file, out_translation_1, 40, 40);// + +
    translation(bgr, file, out_translation_2, -40, 40);// - +
    translation(bgr, file, out_translation_3, 40, -40);// + -
    translation(bgr, file, out_translation_4, -40, -40);// - -
    
    rotation(bgr, file, out_rotation_30, 30);
    rotation(bgr, file, out_rotation_minus30, -30);
    rotation(bgr, file, out_rotation_90, 90);
    rotation(bgr, file, out_rotation_minus90, -90);
    
    scale(bgr, file, out_scale_0_5_0_5, 0.5, 0.5);
    scale(bgr, file, out_scale_1_0_0_5, 1.0, 0.5);
    scale(bgr, file, out_scale_0_5_1_0, 0.5, 1.0);
    scale(bgr, file, out_scale_2_0_0_5, 2.0, 0.5);
    scale(bgr, file, out_scale_0_5_2_0, 0.5, 2.0);
    
    shear(bgr, file, out_shear_0_0_1_0, 0, 1.0);
    shear(bgr, file, out_shear_1_0_0_0, 1.0, 0);
    shear(bgr, file, out_shear_re_0_0_1_0, 0, -1.0);
    shear(bgr, file, out_shear_re_1_0_0_0, -1.0, 0);
    
    mirror(bgr, file, out_mirror);
    
    fclose(file);
    
}

//双线性插值
unsigned char bilinear_interpolation(unsigned char d0, unsigned char d1,unsigned char d2,unsigned char d3,double dx, double dy){
    int x1 = dx * d0 + (1 - dx) * d1;
    int x2 = dx * d2 + (1 - dx) * d3;
    unsigned char result = dy*x1+(1-dy)*x2;
    return result;
}

void translation(BGR_Pixel* bgr,FILE * in_file,FILE * out_file,int dx,int dy){
    
    BMPFILEHEADER *new_bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *new_bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(in_file, 0L, 0);
    fread(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, in_file);
    fread(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, in_file);
    
    int width = (*new_bmpinfoheader).biWidth;
    int height = (*new_bmpinfoheader).biHeight;
    
    int new_width = width + abs(dx) + 3;
    new_width = new_width / 4 * 4;//4字节对齐
    int new_height = height + abs(dy) + 3;
    new_height = new_height / 4 * 4;//4字节对齐
    
    new_bmpinfoheader->biHeight = new_height;
    new_bmpinfoheader->biWidth = new_width;
    new_bmpinfoheader->biSizeImage = 3 * new_height * new_width;
    
    BGR_Pixel* translation_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * new_width * new_height);
    
    //保存文件
    fseek(out_file, 0L, 0);
    fwrite(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    
    unsigned int bfSize = 3 * new_height * new_width + 54;
    fseek(out_file, 2L, 0);
    fwrite(&bfSize, 4, 1, out_file);
    
    fseek(out_file, 54L, 0);
    
    int r,g,b;
    r = b = g = 255;
    
    
    for (int i = 0;i < new_height;i++)
        for (int j = 0; j < new_width; j++){
            
            int count = i * new_width + j;
            int option = 0;
            if (dx >= 0 && dy >= 0) {
                option=0;                   // + +
            }else if(dx < 0 && dy > 0){
                option = 1;                 // - +
            }else if(dx > 0 && dy < 0){
                option = 2;                 // + -
            }else if(dx < 0 && dy < 0){
                option = 3;                 // - -
            }
            
            switch (option) {
                case 0:
                    if (i < dy || j < dx || (i-dy) == height) {
                        b = r = g = 0;
                    }
                    else {
                        b = bgr[(i-dy) * width + (j-dx)].blue;
                        g = bgr[(i-dy) * width + (j-dx)].green;
                        r = bgr[(i-dy) * width + (j-dx)].red;
                    }
                    
                    break;
                case 1:
                    if (i < dy || j >= width || (i-dy) == height) {
                        b = r = g = 0;
                    }
                    else {
                        b = bgr[(i-dy) * width + j].blue;
                        g = bgr[(i-dy) * width + j].green;
                        r = bgr[(i-dy) * width + j].red;
                    }
                    break;
                case 2:
                    if (i >= height || j < dx) {
                        b = r = g = 0;
                    }
                    else {
                        b = bgr[i * width + j - dx].blue;
                        g = bgr[i * width + j - dx].green;
                        r = bgr[i * width + j - dx].red;
                    }
                    break;
                case 3:
                    if (i > height || j >= width) {
                        b = r = g = 0;
                    }
                    else {
                        b = bgr[i * width + j].blue;
                        g = bgr[i * width + j].green;
                        r = bgr[i * width + j].red;
                    }
                    
                    break;
                default:
                    break;
            }
            translation_done[count].blue = b;
            translation_done[count].green = g;
            translation_done[count].red = r;
        }
    
    fwrite(translation_done, sizeof(BGR_Pixel), new_height * new_width, out_file);
    fclose(out_file);
    
}

void rotation(BGR_Pixel* bgr,FILE* in_file,FILE* out_file,double angle){
    
        //x2 = xcosa - ysina
        //y2 = xsina + ycosa
        //逆变换：
        //x = x2cosa + y2sina
        //y = y2cosa - x2sina
    
    angle = angle / 180 * PI;//弧度制
    
    BMPFILEHEADER *new_bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *new_bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(in_file, 0L, 0);
    fread(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, in_file);
    fread(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, in_file);
    
    int width = (*new_bmpinfoheader).biWidth;
    int height = (*new_bmpinfoheader).biHeight;
    

    int new_width = width * fabs(cos(angle)) + height * fabs(sin(angle)) + 3;
    new_width = new_width / 4 * 4;
    int new_height = width * fabs(sin(angle)) + height * fabs(cos(angle)) + 3;
    new_height = new_height / 4 * 4;
    
    new_bmpinfoheader->biHeight = new_height;
    new_bmpinfoheader->biWidth = new_width;
    new_bmpinfoheader->biSizeImage = 3 * new_height * new_width;
    
    BGR_Pixel* rotation_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * new_width * new_height);
    
        //保存文件
    fseek(out_file, 0L, 0);
    fwrite(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    
    unsigned int bfSize = 3 * new_height * new_width + 54;
    fseek(out_file, 2L, 0);
    fwrite(&bfSize, 4, 1, out_file);
    
    fseek(out_file, 54L, 0);

    
    int xoffset = width / 2;
    int yoffset = height / 2;
    
    int new_xoffset = new_width / 2;
    int new_yoffset = new_height / 2;
    
    int new_x,new_y;
    int x,y;

    for (int i = -new_yoffset; i < new_yoffset; i++) {
        for (int j = -new_xoffset; j < new_xoffset; j++) {
            
            double tx,ty;

            new_x = j;
            new_y = i;
            tx = new_x * cos(angle) + new_y * sin(angle);
            ty = -new_x * sin(angle) + new_y * cos(angle);
            new_x = j;
            new_y = i;
            x = new_x * cos(angle) + new_y * sin(angle);
            y = -new_x * sin(angle) + new_y * cos(angle);
            double dx = fabs(tx - x);
            double dy = fabs(ty - y);//求小数部分
            
            if (x == (xoffset - 1)||(y == yoffset - 1)) {
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
            }
            
            if (x < (xoffset - 1) && x >= -xoffset && y < (yoffset - 1) && y >= -yoffset) {

                if ( fabs(tx - (int)tx) <= FLT_EPSILON && fabs(ty - (int)ty) <= FLT_EPSILON){
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
                }
                else{
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].blue, bgr[(y + yoffset) * width + x + xoffset + 1].blue, bgr[(y + yoffset + 1) * width + x + xoffset].blue, bgr[(y + yoffset + 1) * width + x + xoffset + 1].blue, dx, dy);
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].green, bgr[(y + yoffset) * width + x + xoffset + 1].green, bgr[(y + yoffset + 1) * width + x + xoffset].green, bgr[(y + yoffset + 1) * width + x + xoffset + 1].green, dx, dy);
                rotation_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].red, bgr[(y + yoffset) * width + x + xoffset + 1].red, bgr[(y + yoffset + 1) * width + x + xoffset].red, bgr[(y + yoffset + 1) * width + x + xoffset + 1].red, dx, dy);
                    
                }
                
            }
        }
    }
    
    fseek(out_file, 54L, 0);
    fwrite(rotation_done, 1, new_height*new_width*3, out_file);

}

void scale(BGR_Pixel* bgr, FILE* in_file,FILE* out_file,double ratio_x,double ration_y){
    
        //            x2 = x * ratio_w
        //            y2 = y * ratio_h
        //            逆变换：
        //            x = x2 / ratio_w
        //            y = y2 / ratio_h
    
    BMPFILEHEADER *new_bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *new_bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(in_file, 0L, 0);
    fread(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, in_file);
    fread(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, in_file);
    
    int width = (*new_bmpinfoheader).biWidth;
    int height = (*new_bmpinfoheader).biHeight;
    
    int new_width = width * ratio_x + 3;
    new_width = (int)(new_width)/4*4;//4字节对齐
    int new_height = height*ration_y+3;
    new_height = (int)(new_height)/4*4;
    
    
    new_bmpinfoheader->biHeight = new_height;
    new_bmpinfoheader->biWidth = new_width;
    new_bmpinfoheader->biSizeImage = 3 * new_height * new_width;
    
    
    BGR_Pixel* scale_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * new_width * new_height);
    
        //保存文件
    fseek(out_file, 0L, 0);
    fwrite(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    
    unsigned int bfSize = 3 * new_height * new_width + 54;
    fseek(out_file, 2L, 0);
    fwrite(&bfSize, 4, 1, out_file);
    
    int xoffset = width / 2;
    int yoffset = height / 2;
    
    int new_xoffset = new_width / 2;
    int new_yoffset = new_height / 2;
    
    int new_x,new_y;
    int x,y;
    
    for (int i = -new_yoffset; i < new_yoffset; i++) {
        for (int j = -new_xoffset; j < new_xoffset; j++) {
            
            double tx,ty;
            
            new_x = j;
            new_y = i;
            tx = 1.0 * new_x / ratio_x;
            ty = 1.0 * new_y / ration_y;
            x = new_x / ratio_x;
            y = new_y / ration_y;
                //bilinear interpolation method
            double dx = fabs(tx - x);
            double dy = fabs(ty - y);
            if (x == (xoffset - 1)||y == (yoffset-1)) {
                scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
            }
            if (x < (xoffset - 1) && x >= -xoffset && y < (yoffset - 1) && y >= -yoffset) {
                
                if ( fabs(tx - (int)tx) <= FLT_EPSILON && fabs(ty - (int)ty) <= FLT_EPSILON){
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
                }

                else {
                    
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].blue, bgr[(y + yoffset) * width + x + xoffset + 1].blue, bgr[(y + yoffset + 1) * width + x + xoffset].blue, bgr[(y + yoffset + 1) * width + x + xoffset + 1].blue, dx, dy);
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].green, bgr[(y + yoffset) * width + x + xoffset + 1].green, bgr[(y + yoffset + 1) * width + x + xoffset].green, bgr[(y + yoffset + 1) * width + x + xoffset + 1].green, dx, dy);
                    scale_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].red, bgr[(y + yoffset) * width + x + xoffset + 1].red, bgr[(y + yoffset + 1) * width + x + xoffset].red, bgr[(y + yoffset + 1) * width + x + xoffset + 1].red, dx, dy);
                }
                
            }

        }
    }
    
    fseek(out_file, 54L, 0);
    fwrite(scale_done, 1, new_height * new_width * 3, out_file);
    
}

void shear(BGR_Pixel* bgr, FILE* in_file,FILE* out_file,double dx,double dy){
    BMPFILEHEADER *new_bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *new_bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(in_file, 0L, 0);
    fread(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, in_file);
    fread(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, in_file);
    
    int width = (*new_bmpinfoheader).biWidth;
    int height = (*new_bmpinfoheader).biHeight;
    
    int new_width = width * 1 + height * fabs(dx)+3;
    new_width = (new_width) / 4 * 4;//4字节对齐
    int new_height = height * 1 + width * fabs(dy) + 3;
    new_height = (new_height) / 4 * 4;
    
    new_bmpinfoheader->biHeight = new_height;
    new_bmpinfoheader->biWidth = new_width;
    new_bmpinfoheader->biSizeImage = 3 * new_height * new_width;
    
    BGR_Pixel* shear_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * new_width * new_height);
    
        //保存文件
    fseek(out_file, 0L, 0);
    fwrite(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    
    unsigned int bfSize = 3 * new_height * new_width + 54;
    fseek(out_file, 2L, 0);
    fwrite(&bfSize, 4, 1, out_file);

    int xoffset = width / 2;
    int yoffset = height / 2;
    
    int new_xoffset = new_width / 2;
    int new_yoffset = new_height/2;
    
    int new_x,new_y;
    int x,y;
    
    for (int i = -new_yoffset; i < new_yoffset; i++) {
        for (int j = -new_xoffset; j < new_xoffset; j++) {
            
            new_x = j;
            new_y = i;

            double tx,ty;
            tx = 1.0 * (new_x - dx * new_y);
            ty = 1.0 * (new_y - dy * new_x);
            x = (new_x - dx * new_y);
            y = (new_y - dy * new_x);
            
            double dx = fabs(tx - x);
            double dy = fabs(ty - y);
            
            if (x == (xoffset - 1)|| y == (yoffset - 1)) {
                shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
            }
            if (x < (xoffset - 1) && x >= -xoffset && y < (yoffset - 1) && y >= -yoffset) {
                
                if ( fabs(tx - (int)tx) <= FLT_EPSILON&&fabs(ty - (int)ty) <= FLT_EPSILON){
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bgr[(y + yoffset) * width + x + xoffset].blue;
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bgr[(y + yoffset) * width + x + xoffset].green;
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bgr[(y + yoffset) * width + x + xoffset].red;
                }
                else {
                    
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].blue = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].blue, bgr[(y + yoffset) * width + x + xoffset + 1].blue, bgr[(y + yoffset + 1) * width + x + xoffset].blue, bgr[(y + yoffset + 1) * width + x + xoffset + 1].blue, dx, dy);
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].green = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].green, bgr[(y + yoffset) * width + x + xoffset + 1].green, bgr[(y + yoffset + 1) * width + x + xoffset].green, bgr[(y + yoffset + 1) * width + x + xoffset + 1].green, dx, dy);
                    shear_done[(new_y + new_yoffset) * new_width + new_x + new_xoffset].red = bilinear_interpolation(bgr[(y + yoffset) * width + x + xoffset].red, bgr[(y + yoffset) * width + x + xoffset + 1].red, bgr[(y + yoffset + 1) * width + x + xoffset].red, bgr[(y + yoffset + 1) * width + x + xoffset + 1].red, dx, dy);
                }
                
            }
            
        }
    }
    
    fseek(out_file, 54L, 0);
    fwrite(shear_done, 1, new_height * new_width * 3, out_file);
    
    
}

void mirror(BGR_Pixel* bgr,FILE * in_file,FILE * out_file){

    BMPFILEHEADER *new_bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    BMPINFOHEADER *new_bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    
        //读取文件信息
    fseek(in_file, 0L, 0);
    fread(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, in_file);
    fread(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, in_file);
    
    int width = (*new_bmpinfoheader).biWidth;
    int height = (*new_bmpinfoheader).biHeight;
    
    fseek(out_file, 0L, 0);
    fwrite(new_bmpfileheader, sizeof(BMPFILEHEADER), 1, out_file);
    fwrite(new_bmpinfoheader, sizeof(BMPINFOHEADER), 1, out_file);
    
    BGR_Pixel* mirror_done = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * width * height);
    
    for (int i = 0;i< height;i++)
        for (int j = 0; j < width; j++){
           int count = i * width + j;
            
            mirror_done[count].blue = bgr[i * width + width - j].blue;
            mirror_done[count].green = bgr[i * width + width - j].green;
            mirror_done[count].red = bgr[i * width + width - j].red;
            
        }
    fwrite(mirror_done, sizeof(BGR_Pixel), height * width, out_file);
    fclose(out_file);
    
}

//
//  main.c
//  img_2
//
//  Created by 崔瑜翔 on 2018/6/4.
//  Copyright © 2018年 崔瑜翔. All rights reserved.
//

#include "img_2.h"

int main(int argc, char** argv) {
    //读取文件信息
    BMP_image* image = (BMP_image*)malloc(sizeof(BMP_image));
    
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2.bmp", "rb");
    //文件头&信息头
    image->bmpfileheader = (BMPFILEHEADER*)malloc(sizeof(BMPFILEHEADER));
    image->bmpinfoheader = (BMPINFOHEADER*)malloc(sizeof(BMPINFOHEADER));
    fread(image->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fread(image->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    //像素信息
    image->pixel = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)* (image->bmpinfoheader->biWidth) * (image->bmpinfoheader->biHeight));
    fread(image->pixel, sizeof(BGR_Pixel), (image->bmpinfoheader->biWidth) * (image->bmpinfoheader->biHeight), file);
    fclose(file);
    
    //二值化
    BMP_image* binarized = (BMP_image*)malloc(sizeof(BMP_image));
    binarize(image, binarized);

    //设定kernel，这里我们选用的是3x3方格，其中除角点外点被选中
    Kernel* kernel = (Kernel*)malloc(sizeof(Kernel));
    kernel->width  = 3;
    kernel->height = 3;
    kernel->ker = (BYTE*)malloc(sizeof(BYTE) * kernel->width * kernel->height);
    for (int y = 0; y < kernel->height; y++) {
        for (int x = 0; x < kernel->width; x++) {
            int count = y*kernel->width + x;
            if (count == 1 || count == 3 || count == 4 || count == 5 || count == 7)
                kernel->ker[count] = (BYTE)255;
            else kernel->ker[count] = (BYTE)0;
        }
    }
    
    //选定操作模式
    int choice = 0;
    printf("选择处理模式\n1:erode  2:dilate  3:opening  4:closing\n");
    scanf("%d",&choice);
    printf("done!\n");
    switch (choice) {
        case 1:{
            //腐蚀处理
            BMP_image* eroded_image = (BMP_image*)malloc(sizeof(BMP_image));
            erode(binarized, eroded_image, kernel);
            free(eroded_image);}
            break;
        case 2:{
            //膨胀处理
            BMP_image* dilated_image = (BMP_image*)malloc(sizeof(BMP_image));
            dilate(binarized, dilated_image, kernel);
            free(dilated_image);}
            break;
        case 3:{
            //开运算
            BMP_image* opened_image = (BMP_image*)malloc(sizeof(BMP_image));
            opening(binarized, opened_image, kernel);
            free(opened_image);}
            break;
        case 4:{
            //闭运算
            BMP_image* closed_image = (BMP_image*)malloc(sizeof(BMP_image));
            closing(binarized, closed_image, kernel);
            free(closed_image);}
            break;
 
        default:
            break;
    }
    free(image);
    free(binarized);
 

    return 0;
}


//二值化处理
int binarize(BMP_image* image, BMP_image* binarized) {
    
    //读取图片信息
    binarized->path = "/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2_binarized.bmp";
    binarized->bmpfileheader = image->bmpfileheader;
    binarized->bmpinfoheader = image->bmpinfoheader;
    binarized->pixel = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*image->bmpinfoheader->biWidth*image->bmpinfoheader->biHeight);
    
    BGR_Pixel* graypixels = (BGR_Pixel*)malloc(sizeof(BGR_Pixel)*image->bmpinfoheader->biWidth*image->bmpinfoheader->biHeight);
    
    BGR_Pixel* imagepixels = image->pixel;
    
    //找到最大最小的灰度级，求灰度级总和
    int min = 255; int max = 0;
    int sum = 0;
    for (int y = 0; y < image->bmpinfoheader->biHeight; y++) {
        for (int x = 0; x < image->bmpinfoheader->biWidth; x++) {
            int count = y * image->bmpinfoheader->biWidth + x;
            graypixels[count].blue = rearrange(imagepixels[count].blue * 0.114 + imagepixels[count].green * 0.587 + imagepixels[count].red * 0.299);
            if (graypixels[count].blue > max) max = graypixels[count].blue;
            if (graypixels[count].blue < min) min = graypixels[count].blue;
            sum += graypixels[count].blue;
        }
    }
    
//    方法一，利用线性求平均的方法，迭代找到最合适的阈值
//    int threshold0 = (max+min)/2;
//    int threshold = 0;
//    int larger = 0;
//    int smaller = 0;
//    int sumlarger = 0;
//    int sumsmaller = 0;
//    while (1) {
//         larger = 0;
//         smaller = 0;
//         sumlarger = 0;
//         sumsmaller = 0;
//        for (int y = 0; y < image->bmpinfoheader->biHeight; y++) {
//            for (int x = 0; x < image->bmpinfoheader->biWidth; x++) {
//                int count = y * image->bmpinfoheader->biWidth + x;
//                graypixels[count].blue = rearrange(imagepixels[count].blue * 0.114 + imagepixels[count].green * 0.587 + imagepixels[count].red * 0.299);
//                if (graypixels[count].blue >= threshold0) {
//                    larger++;
//                    sumlarger += graypixels[count].blue;
//                }
//                if (graypixels[count].blue < threshold0) {
//                    smaller++;
//                    sumsmaller += graypixels[count].blue;
//
//                }
//            }
//        }
//        threshold = (sumlarger/larger+sumsmaller/smaller)/2;
//        if (threshold0 == threshold) {
//            break;
//        }
//        threshold0 = threshold;
//    }

    //方法二，利用最大津算法求得最合适的阈值
    float average = (float)sum / (image->bmpinfoheader->biWidth*image->bmpinfoheader->biHeight);

    int threshold = 0;
    int maxVariance = 0;
    for (int i = min; i < max; i++) {
        int larger = 0, smaller = 0;
        int sumlarger = 0, sumsmaller = 0;
        for (int y = 0; y < image->bmpinfoheader->biHeight; y++) {
            for (int x = 0; x < image->bmpinfoheader->biWidth; x++) {
                int count = y * image->bmpinfoheader->biWidth + x;
                if (graypixels[count].blue >= i) {
                    larger++;
                    sumlarger += graypixels[count].blue;
                }
                else {
                    smaller++;
                    sumsmaller += graypixels[count].blue;
                }
            }
        }
        float average_larger = (float)sumlarger / larger;
        float average_smaller = (float)sumsmaller / smaller;
        float Variance = larger*(average_larger - average) * (average_larger - average) + smaller* (average_smaller - average) * (average_smaller - average);
        if (Variance > maxVariance) {
            maxVariance = Variance;
            threshold = i;
        }
    }
    //利用求得阈值对目标图像进行赋值
    for (int y = 0; y < image->bmpinfoheader->biHeight; y++) {
        for (int x = 0; x < image->bmpinfoheader->biWidth; x++) {
            int i = y*image->bmpinfoheader->biWidth + x;
            if (graypixels[i].blue > threshold) {
                binarized->pixel[i].blue = binarized->pixel[i].green = binarized->pixel[i].red = 255;
            }else
                binarized->pixel[i].blue = binarized->pixel[i].green = binarized->pixel[i].red = 0;
        }
    }
    free(graypixels);
    
    //保存文件
    FILE* file = fopen(binarized->path, "wb");
    
    fwrite(binarized->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(binarized->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(binarized->pixel, sizeof(BGR_Pixel), binarized->bmpinfoheader->biWidth * binarized->bmpinfoheader->biHeight, file);
    fclose(file);
    return 0;
}

//膨胀处理
int erode(BMP_image* image, BMP_image* eroded, Kernel* kernel) {

    eroded->path = "/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2_eroded.bmp";
    eroded->bmpfileheader = image->bmpfileheader;
    eroded->bmpinfoheader = image->bmpinfoheader;
    eroded->pixel = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * image->bmpinfoheader->biWidth * image->bmpinfoheader->biHeight);
    
    for (int y = 1; y < image->bmpinfoheader->biHeight - 1; y++) {
        for (int x = 1; x < image->bmpinfoheader->biWidth - 1; x++) {
            int i = y * image->bmpinfoheader->biWidth + x;
            int flag = 1;
            for (int ky = 0; ky < kernel->height; ky++) {
                for (int kx = 0; kx < kernel->width; kx++) {
                    int indexX = x + (kx - (kernel->width / 2));
                    int indexY = y + (ky - (kernel->height / 2));
                    int index_all = indexY * image->bmpinfoheader->biWidth + indexX;
                    int index_kernel = ky*kernel->width + kx;
                    //找到白色点，也即背景像素点
                    if (image->pixel[index_all].blue != 0 && (kernel->ker[index_kernel] == 255)) {
                        flag = 0;
                        break;
                    }
                }
                if (!flag) break;
            }
            //与找到的背景点进行同化处理
            if (flag) {
                eroded->pixel[i].blue = eroded->pixel[i].green = eroded->pixel[i].red= 0;
            } else {
                eroded->pixel[i].blue = eroded->pixel[i].green = eroded->pixel[i].red = 255;
            }
        }
    }
    //保存文件
    FILE* file = fopen(eroded->path, "wb");
    
    fwrite(eroded->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(eroded->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(eroded->pixel, sizeof(BGR_Pixel), eroded->bmpinfoheader->biWidth * eroded->bmpinfoheader->biHeight, file);
    fclose(file);
    return 0;
}

int dilate(BMP_image* image, BMP_image* dilated, Kernel* kernel) {

    dilated->path = "/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2_dilated.bmp";
    dilated->bmpfileheader = image->bmpfileheader;
    dilated->bmpinfoheader = image->bmpinfoheader;
    dilated->pixel = (BGR_Pixel*)malloc(sizeof(BGR_Pixel) * image->bmpinfoheader->biWidth * image->bmpinfoheader->biHeight);
    
    for (int y = 1; y < image->bmpinfoheader->biHeight - 1; y++) {
        for (int x = 1; x < image->bmpinfoheader->biWidth - 1; x++) {
            int i = y * image->bmpinfoheader->biWidth + x;
            int flag = 0;
            for (int ky = 0; ky < kernel->height; ky++) {
                for (int kx = 0; kx < kernel->width; kx++) {
                    int indexX = x + (kx - (kernel->width / 2));
                    int indexY = y + (ky - (kernel->height / 2));
                    int index_all = indexY * image->bmpinfoheader->biWidth + indexX;
                    int index_kernel = ky*kernel->width + kx;
                    //找到黑色点，也即前景像素点
                    if (image->pixel[index_all].blue == 0 && (kernel->ker[index_kernel] == 255)) {
                        flag = 1;
                        break;
                    }
                }
                if (flag) break;
            }
            //与找到的前景点进行同化处理
            if (flag) {
                dilated->pixel[i].blue = dilated->pixel[i].green = dilated->pixel[i].red = 0;
            }
            else {
                dilated->pixel[i].blue = dilated->pixel[i].green= dilated->pixel[i].red = 255;
            }
        }
    }
    //保存文件
    FILE* file = fopen(dilated->path, "wb");
    
    fwrite(dilated->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(dilated->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(dilated->pixel, sizeof(BGR_Pixel), dilated->bmpinfoheader->biWidth * dilated->bmpinfoheader->biHeight, file);
    fclose(file);
    return 0;
}

int opening(BMP_image* image, BMP_image* opening, Kernel* kernel) {
    BMP_image* temp = (BMP_image*)malloc(sizeof(BMP_image));
    //先腐蚀再膨胀
    erode(image, temp, kernel);
    dilate(temp, opening, kernel);
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2_opening.bmp", "wb");
    
    fwrite(opening->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(opening->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(opening->pixel, sizeof(BGR_Pixel), opening->bmpinfoheader->biWidth * opening->bmpinfoheader->biHeight, file);
    fclose(file);
    free(temp);
    return 0;
}

int closing(BMP_image* image, BMP_image* closing, Kernel* kernel) {
    BMP_image* temp = (BMP_image*)malloc(sizeof(BMP_image));
    //先膨胀再腐蚀
    dilate(image, temp, kernel);
    erode(temp, closing, kernel);
    
    //保存文件
    FILE* file = fopen("/Users/cuiyuxiang/Desktop/图像信息处理/HOMEWORK/img_2/ironman2_closing.bmp", "wb");
    
    fwrite(closing->bmpfileheader, sizeof(BMPFILEHEADER), 1, file);
    fwrite(closing->bmpinfoheader, sizeof(BMPINFOHEADER), 1, file);
    fwrite(closing->pixel, sizeof(BGR_Pixel), closing->bmpinfoheader->biWidth * closing->bmpinfoheader->biHeight, file);
    fclose(file);
    free(temp);
    return 0;
}



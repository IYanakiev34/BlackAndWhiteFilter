#ifndef IMAGE_H
#define IMAGE_H

#include<stdio.h>
#include<stdlib.h>

// Bitmap file header
typedef struct BMP_Header{
    char name[2];
    int size;
    unsigned short res1;
    unsigned short res2;
    int offset;
}BMP_Header;

// DIB header
typedef struct imageHeader{
    unsigned int headerSize; // the size of the header
    unsigned int width; // the width in pixels of the image
    unsigned int height; // the height in pixels of the image
    unsigned short int colorPanes; //must be one
    unsigned short int bitsPerPixel; // numbers of bit per pixel : typical values are 1,2,4,8,16,24,32;
    unsigned int compression; // compression method used (in our case no compression)
    unsigned int imageSize; // the size of the raw bitmap data

    int temp[25]; // in our case we have version 5 bitmap so we need 124 bits
    //unsigned int horRes; // horizontal resolution
   // unsigned int verRes; //vertical resolution
    //unsigned int numOfColors; // num of colors = 0 or 2^n;
    //unsigned int numOfImpColors;  // 0 is every color is imp , generally ignored

}imageHeader;

// RGB : 24 bits per pixels => 3 bytes
typedef struct RGB{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}RGB;

// the image
typedef struct Image{
    int width;
    int height;
    RGB **rgb;
}Image;

Image newImage(FILE *fp,int width,int height);
void freeImage(Image image);
BMP_Header getBMP_Header(FILE *fp);
imageHeader getImageHeader(FILE *fp);
Image getImage(FILE *fp,int offset,int width,int height);
unsigned char greyScalePixel(RGB rgb);
void imageToGrayScale(Image image);
void flipImage(Image image);
void createBlackAndWhiteImage(Image image,BMP_Header bmp_header,imageHeader image_header);
void createFlippedImage(Image image,BMP_Header bmp_header,imageHeader image_header);
#endif
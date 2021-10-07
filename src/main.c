#include<stdio.h>
#include<stdlib.h>

#include "image.h"

int main(int argc,char *argv[]){
    char filename[100];
    scanf("%s",filename);

    FILE *fp = fopen(filename,"rb");
    BMP_Header bmp_header = getBMP_Header(fp);
    imageHeader imgHeader = getImageHeader(fp);
    Image img = getImage(fp,bmp_header.offset,imgHeader.width,imgHeader.height);


    //createBlackAndWhiteImage(img,bmp_header,imgHeader);
    createFlippedImage(img,bmp_header,imgHeader);
    freeImage(img);
    return 0;
}
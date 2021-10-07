#include<stdio.h>
#include<stdlib.h>

typedef struct BMP_Header{
    char name[2];
    int size;
    unsigned short res1;
    unsigned short res2;
    int offset;
}BMP_Header;

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

typedef struct RGB{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}RGB;

typedef struct Image{
    int width;
    int height;
    RGB **rgb;
}Image;

Image newImage(FILE *fp,int width,int height);
void freeImage(Image image);
void openbmpfile(char filename[100]);
unsigned char greyScalePixel(RGB rgb);
void imageToGrayScale(Image image);
void createBlackAndWhiteImage(Image image,BMP_Header bmp_header,imageHeader image_header);

//you will need a bmp image that has a header of 124 bytes, prob if you are running on a newer window
// this will  be the case
int main(int argc,char *argv[]){
    char filename[100];
    scanf("%s",filename);
    openbmpfile(filename);
    return 0;
}

void openbmpfile(char filename[100]){
    FILE *fp = fopen(filename,"rb");
    BMP_Header header;
    imageHeader imageHeader;

    fread(&header.name,2*sizeof(char),1,fp);
    fread(&header.size,3*sizeof(int),1,fp);

    fread(&imageHeader,1*sizeof(struct imageHeader),1,fp);

    if(header.name[0]!='B' || header.name[1]!='M'){
        fclose(fp);
        printf("File is not in the correct format! Supply a BMP file.\n");
        return;
    }
    if(imageHeader.headerSize!=124 || imageHeader.colorPanes!=1){
        fclose(fp);
        printf("Header size not correct!\n");
        return;
    }

    fseek(fp,header.offset,SEEK_SET);
    Image image = newImage(fp,imageHeader.width,imageHeader.height);
    createBlackAndWhiteImage(image,header,imageHeader);

    fclose(fp);
    freeImage(image);
}


Image newImage(FILE *fp,int width,int height){
    Image image;
    image.height = height;
    image.width = width;
    image.rgb =(struct RGB**) malloc(height*sizeof(void *));
    for(int i =height-1; i >=0 ;i--){
        image.rgb[i] = (struct RGB*) malloc(width*sizeof(struct RGB));
        fread(image.rgb[i],width,sizeof(struct RGB),fp);
    }

    return image;
}

unsigned char greyScalePixel(RGB rgb){
    return ((rgb.red*0.3)+(rgb.green*0.6)+(rgb.blue*0.1));
}

void imageToGrayScale(Image image){
    for(int i = 0;i<image.height;i++){
        for(int j=0;j<image.width;j++){
            image.rgb[i][j].red = greyScalePixel(image.rgb[i][j]);
            image.rgb[i][j].blue = greyScalePixel(image.rgb[i][j]);
            image.rgb[i][j].green = greyScalePixel(image.rgb[i][j]);
        }
    }

}

void createBlackAndWhiteImage(Image image,BMP_Header bmp_header,imageHeader image_header){
    FILE *fpw = fopen("bw-pic.bmp","wb");
    if(fpw == NULL){
        return;
    }

    imageToGrayScale(image);

    fwrite(bmp_header.name,2*sizeof(char),1,fpw);
    fwrite(&bmp_header.size,3*sizeof(int),1,fpw);

    fwrite(&image_header,sizeof(struct imageHeader),1,fpw);


    for(int i=image.height-1; i>=0;i--){
        fwrite(image.rgb[i],image.width,sizeof(struct RGB),fpw);
    }
    fclose(fpw);
}

void freeImage(Image image){
    for(int i=image.height-1;i>=0;i--){
        free(image.rgb[i]);
    }
    free(image.rgb);
}
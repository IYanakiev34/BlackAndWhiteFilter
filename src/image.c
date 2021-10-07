#include "image.h"
BMP_Header getBMP_Header(FILE *fp){
    BMP_Header header;      
    fread(&header.name,2*sizeof(char),1,fp);
    fread(&header.size,3*sizeof(int),1,fp);

    if(header.name[0]!='B' || header.name[1]!='M'){
        fclose(fp);
        printf("File is not in the correct format! Supply a BMP file.\n");
        exit(-1);
    }

    return header;
}

imageHeader getImageHeader(FILE *fp){
    imageHeader imageHeader;
    fread(&imageHeader,1*sizeof(struct imageHeader),1,fp);

    if(imageHeader.headerSize!=124 || imageHeader.colorPanes!=1){
        fclose(fp);
        printf("Header size not correct!\n");
        exit(-1);
    }
    return imageHeader;
}
Image getImage(FILE *fp,int offset,int width,int height){
   
    fseek(fp,offset,SEEK_SET);
    Image image = newImage(fp,width,height);
    fclose(fp);
    return image;
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
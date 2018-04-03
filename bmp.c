/**
 * @file bmp.c
 *
 * @author Brendan Wilson
 *
 * @date 3/20/17
 *
 * Assignment: Bitmap Project
 *
 * @brief A simple bitmap manipulation program that utilizes multiple optargs to
 * do things such as change colors in 24bpp images or reverse grayscale 8bpp
 * images.
 *
 * @details Reads in the File Header (14 bytes), Image Header (40 bytes), Color
 * Table (8bpp images only, variable size calculated by offset in file header),
 * and Pixel Data (offset - rest of file) to get multiple types of information
 * about the image. Mutiple options are available for the 8bpp and 24bpp
 * images such as color manipulation or reversal of colors.
 *
 * @bugs none
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "apue.h"
#include "bmp.h"
#define EXIT_SUCCESS 0

//reverse images (grayscale) colors are calculated by abs(255 - color)

int main(int argc, char *argv[])
{
        int opt = 0;
        char *file = malloc(30);
        assert(file);
        char *outputFile = malloc(30);
        assert(outputFile);
        int outputSet = 0;
        int headerInfoSimple = 0; //-i
        int headerInfoFull = 0; // -d
        int reverseGrayscale = 0; //only on 8bpp image
        int red = -1; //if set, rgb can be any color from 0-255 so -1 must be set for checking
        int green = -1; //see red
        int blue = -1; //see red
        //rgb cannot be used in conjunction with 8bpp image or -R set
        while ((opt = getopt(argc, argv, "o:idRr:g:b:h")) != -1) {
                switch (opt) {
                        case 'o': //output file needed
                                outputFile = optarg;
                                outputSet = 1;
                                break;
                        case 'i': //simple header info
                                headerInfoSimple = 1;
                                break;
                        case 'd': //expanded header info
                                headerInfoFull = 1;
                                break;
                        case 'R': //reverse grayscale images
                                reverseGrayscale = 1;
                                break;
                        case 'r':
                                red = atoi(optarg);
                                red = checkColor(red);
                                break;
                        case 'g':
                                green = atoi(optarg);
                                green = checkColor(green);
                                break;
                        case 'b':
                                blue = atoi(optarg);
                                blue = checkColor(blue);
                                break;
                        case 'h': //help
                                printf("Usage: ./bmp FILE [-o OUTPUT_FILE] [-i] [-d] [-R] [-r N] [-g N] [-b N]\n");
                                printf("-R only available for grayscale 8bpp, -r -g -b only available for color 24bpp\n");
                                printf("-o must be set for -R, -r, -g, and/or -b options\n");
                                exit(EXIT_SUCCESS);
                                break;
                        default: //for invalid options
                                printf("Usage: ./bmp FILE [-o OUTPUT_FILE] [-i] [-d] [-R] [-r N] [-g N] [-b N]\n");
                                printf("-R only available for grayscale 8bpp, -r -g -b only available for color 24bpp\n");
                                printf("-o must be set for -R, -r, -g, and/or -b options\n");
                                exit(EXIT_SUCCESS);
                                break;
                }
        }
        if (argc == 1 || argc == 2) { //no options set / no file and no options set
                if (argc == 1)
                        printf("No file or options set\n");
                else
                        printf("No options set\n");
                printf("Usage: ./bmp FILE [-o OUTPUT_FILE] [-i] [-d] [-R] [-r N] [-g N] [-b N]\n");
                printf("-R only available for grayscale 8bpp, -r -g -b only available for color 24bpp\n");
                printf("-o must be set for -R, -r, -g, and/or -b options\n");
                exit(EXIT_SUCCESS);
        }
        file = strcpy(file, argv[optind]);
        if (reverseGrayscale == 1 && outputSet == 0)
                err_sys("Cannot reverse image with no output file set\n");
        if (red != -1 && outputSet == 0)
                err_sys("Cannot change red color without output file set\n");
        if (green != -1 && outputSet == 0)
                err_sys("Cannot change green color witout output file set\n");
        if (blue != -1 && outputSet == 0)
                err_sys("Cannot change blue color witout output file set\n");
        if (outputSet == 0)
                free(outputFile);

        void *fileHeaderArr = malloc(14);
        assert(fileHeaderArr);
        fileHeaderArr = processFileHeader(file, fileHeaderArr);
        unsigned char *unsCharArrayFH = malloc(14);
        assert(unsCharArrayFH);
        memcpy(unsCharArrayFH, fileHeaderArr, 14);
        struct fileHeader_t *fileHeader = malloc(10);
        assert(fileHeader);
        fileHeader = getFileHeaderInfo(fileHeader, unsCharArrayFH);

        if (fileHeader -> offset == 54) {
                if (reverseGrayscale == 1)
                        err_sys("Cannot reverse grayscale colors on color image\n");
        }
        else {
                if (red != -1)
                        err_sys("Cannot change red color on grayscale image\n");
                else if (green != -1)
                        err_sys("Cannot change green color on grayscale image\n");
                else if (blue != -1)
                        err_sys("Cannot change blue color on grayscale image\n");
        }

        void *imageHeaderArr = malloc(40);
        assert(imageHeaderArr);
        imageHeaderArr = processImageHeader(file, imageHeaderArr);
        unsigned char *unsCharArrayIH = malloc(40);
        assert(unsCharArrayIH);
        memcpy(unsCharArrayIH, imageHeaderArr, 40);
        struct imageHeader_t *imageHeader = malloc(44);
        assert(imageHeader);
        imageHeader = getImageHeaderInfo(imageHeader, unsCharArrayIH);

        struct colorTable_t **colorTable;
        void *colorTableArr;
        if (fileHeader -> offset > 54) { //grayscale image color table
                int numberOfColors = imageHeader -> numberOfColors;
                colorTableArr = malloc(4 * numberOfColors);
                assert(colorTableArr);
                colorTableArr = processColorTable(file, colorTableArr, numberOfColors);
                colorTable = malloc(16 * numberOfColors); //4 * 4 * NOC
                assert(colorTable);
                for (int i = 0; i < numberOfColors; i++)
                        colorTable[i] = malloc(4 * sizeof(int)); //4 elems of int size
                colorTable = getColorTableInfo(colorTable, colorTableArr, numberOfColors);
        }

        if (fileHeader -> offset > 54) { //grayscale image pixel data
                unsigned char **grayscalePixelData;
                int imageSize = imageHeader -> imageSize;
                void *grayscalePixelDataArr = malloc(imageSize);
                assert(grayscalePixelDataArr);
                grayscalePixelDataArr = processGrayscalePixelData(file, grayscalePixelDataArr, imageSize, fileHeader -> offset);
                grayscalePixelData = initMatrix(grayscalePixelData, imageHeader -> height, imageHeader -> width);
                grayscalePixelData = getGrayscalePixelInfo(grayscalePixelData, grayscalePixelDataArr, imageSize, imageHeader -> height, imageHeader -> width);
                if (headerInfoFull == 1 || headerInfoSimple == 1)
                        printGrayscaleHeader(fileHeader, imageHeader, colorTable, grayscalePixelData, headerInfoFull);
                if (outputSet == 1) {
                        writeGrayscaleOut(fileHeaderArr, imageHeaderArr, colorTableArr, grayscalePixelDataArr, outputFile, fileHeader, imageHeader, colorTable, grayscalePixelData); //assume reversing grayscale
                }
        }
        else { //color image pixel data
                struct colorPixelData_t **colorPixelData;
                int imageSize = imageHeader -> imageSize;
                void *colorPixelDataArr = malloc(imageSize);
                assert(colorPixelDataArr);
                colorPixelDataArr = processColorPixelData(file, colorPixelDataArr, imageSize);
                colorPixelData = initColorMatrix(colorPixelData, imageHeader -> height, imageHeader -> width);
                colorPixelData = getColorPixelInfo(colorPixelData, colorPixelDataArr, imageSize, imageHeader -> height, imageHeader -> width);
                if (headerInfoFull == 1 || headerInfoSimple == 1)
                        printColorHeader(fileHeader, imageHeader, colorPixelData, headerInfoFull);
                if (outputSet == 1)
                        writeColorOut(fileHeaderArr, imageHeaderArr, outputFile, fileHeader, imageHeader, colorPixelData, colorPixelDataArr);
        }
        /* don't forget to free main stack memory
        i.e file, outputFile, fileHeader, imageHeader, colorTable, pixelData */
}

int checkColor(int color)
{
        if (color >= 0 && color <= 255)
                return color;
        else
                err_sys("Color must be between 0-255\n");
}

void *processFileHeader(char *file, void *fileHeaderArr)
{
        int fd, rd;
        fd = open(file, O_RDONLY);
        if (fd == -1)
                err_sys("Error opening file %s, check spelling?\n", file); //file not found
        rd = read(fd, fileHeaderArr, 14); //copy 14 bytes to file header arr
        if (rd == -1)
                err_sys("Error reading file %s\n");
        close(fd);
        return fileHeaderArr;
}

struct fileHeader_t *getFileHeaderInfo(struct fileHeader_t *fileHeader, unsigned char *unsCharArray)
{
        memcpy(fileHeader -> header, unsCharArray, 2); //copies first two bytes 'BM' into struct

        unsigned char *unsCharSizeArr = malloc(4);
        assert(unsCharSizeArr);
        int i;
        int counter = 0;
        for (i = 5; i >= 2; i--) {
                memcpy(unsCharSizeArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp1[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp1[i * 2],"%02x", unsCharSizeArr[i]); //converts to char
        int s = 0;
        int indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp1[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        s = s + (((int) charHexTmp1[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        s = s + (((int) charHexTmp1[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        fileHeader -> size = s;

        unsigned char *unsCharOffsetArr = malloc(4);
        assert(unsCharOffsetArr);
        counter = 0;
        for (i = 13; i >= 10; i--) {
                memcpy(unsCharOffsetArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp2[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp2[i * 2],"%02x", unsCharOffsetArr[i]); //converts to char
        int o = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp2[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        o = o + (((int) charHexTmp2[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        o = o + (((int) charHexTmp2[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        fileHeader -> offset = o;
        free(unsCharArray);
        free(unsCharSizeArr);
        free(unsCharOffsetArr);
        return fileHeader;
}

void *processImageHeader(char *file, void *imageHeaderArr)
{
        int fd, rd;
        fd = open(file, O_RDONLY);
        if (fd == -1)
                err_sys("Error opening file %s, check spelling?\n", file); //file not found
        lseek(fd, 14, SEEK_SET); //go to 14th byte
        if (fd == -1)
                err_sys("lseek failed\n");
        rd = read(fd, imageHeaderArr, 40); //copy 40 bytes to image header arr
        if (rd == -1)
                err_sys("Error reading file %s\n");
        close(fd);
        return imageHeaderArr;
}

struct imageHeader_t *getImageHeaderInfo(struct imageHeader_t *imageHeader, unsigned char *unsCharArray)
{
        unsigned char *unsCharDibSizeArr = malloc(4);
        assert(unsCharDibSizeArr);
        int i;
        int counter = 0;
        for (i = 3; i >= 0; i--) {
                memcpy(unsCharDibSizeArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp1[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp1[i * 2],"%02x", unsCharDibSizeArr[i]); //converts to char
        int ds = 0;
        int indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp1[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        ds = ds + (((int) charHexTmp1[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        ds = ds + (((int) charHexTmp1[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> dibSize = ds;

        unsigned char *unsCharWidthArr = malloc(4);
        assert(unsCharWidthArr);
        counter = 0;
        for (i = 7; i >= 4; i--) {
                memcpy(unsCharWidthArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp2[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp2[i * 2],"%02x", unsCharWidthArr[i]); //converts to char
        int w = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp2[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        w = w + (((int) charHexTmp2[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        w = w + (((int) charHexTmp2[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> width = w;

        unsigned char *unsCharHeightArr = malloc(4);
        assert(unsCharHeightArr);
        counter = 0;
        for (i = 11; i >= 8; i--) {
                memcpy(unsCharHeightArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp3[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp3[i * 2],"%02x", unsCharHeightArr[i]); //converts to char
        int h = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp3[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        h = h + (((int) charHexTmp3[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        h = h + (((int) charHexTmp3[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> height = h;

        unsigned char *unsCharPlaneArr = malloc(4);
        assert(unsCharPlaneArr);
        counter = 0;
        for (i = 13; i >= 12; i--) {
                memcpy(unsCharPlaneArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp4[4]; //char array to hold 4 bytes of hex vars
        for (i = 0; i < 2; i++)
                sprintf(&charHexTmp4[i * 2],"%02x", unsCharPlaneArr[i]); //converts to char
        int p = 0;
        indexBackwards = 3;
        for (i = 0; i < 4; i++) {
                if ((int) charHexTmp4[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        p = p + (((int) charHexTmp4[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        p = p + (((int) charHexTmp4[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> planes = p;

        unsigned char *unsCharBPPArr = malloc(4);
        assert(unsCharBPPArr);
        counter = 0;
        for (i = 15; i >= 14; i--) {
                memcpy(unsCharBPPArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp5[4]; //char array to hold 4 bytes of hex vars
        for (i = 0; i < 2; i++)
                sprintf(&charHexTmp5[i * 2],"%02x", unsCharBPPArr[i]); //converts to char
        int bpp = 0;
        indexBackwards = 3;
        for (i = 0; i < 4; i++) {
                if ((int) charHexTmp5[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        bpp = bpp + (((int) charHexTmp5[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        bpp = bpp + (((int) charHexTmp5[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> bitsPerPixel = bpp;

        unsigned char *unsCharCompressionArr = malloc(4);
        assert(unsCharCompressionArr);
        counter = 0;
        for (i = 19; i >= 16; i--) {
                memcpy(unsCharHeightArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp6[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp6[i * 2],"%02x", unsCharCompressionArr[i]); //converts to char
        int c = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp6[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        c = c + (((int) charHexTmp6[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        c = c + (((int) charHexTmp6[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> compressionType = c;

        unsigned char *unsCharImgSizeArr = malloc(4);
        assert(unsCharImgSizeArr);
        counter = 0;
        for (i = 23; i >= 20; i--) {
                memcpy(unsCharImgSizeArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp7[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp7[i * 2],"%02x", unsCharImgSizeArr[i]); //converts to char
        int is = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp7[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        is = is + (((int) charHexTmp7[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        is = is + (((int) charHexTmp7[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> imageSize = is;

        unsigned char *unsCharPPXArr = malloc(4);
        assert(unsCharPPXArr);
        counter = 0;
        for (i = 27; i >= 24; i--) {
                memcpy(unsCharPPXArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp8[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp8[i * 2],"%02x", unsCharPPXArr[i]); //converts to char
        int ppx = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp8[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        ppx = ppx + (((int) charHexTmp8[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        ppx = ppx + (((int) charHexTmp8[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> pixelsPerXMeter = ppx;

        unsigned char *unsCharPPYArr = malloc(4);
        assert(unsCharPPYArr);
        counter = 0;
        for (i = 31; i >= 28; i--) {
                memcpy(unsCharPPYArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp9[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp9[i * 2],"%02x", unsCharPPYArr[i]); //converts to char
        int ppy = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp9[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        ppy = ppy + (((int) charHexTmp9[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        ppy = ppy + (((int) charHexTmp9[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> pixelsPerYMeter = ppy;

        unsigned char *unsCharColorsArr = malloc(4);
        assert(unsCharColorsArr);
        counter = 0;
        for (i = 35; i >= 32; i--) {
                memcpy(unsCharColorsArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp10[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp10[i * 2],"%02x", unsCharColorsArr[i]); //converts to char
        int colors = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp10[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        colors = colors + (((int) charHexTmp10[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        colors = colors + (((int) charHexTmp10[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> numberOfColors = colors;

        unsigned char *unsCharImpColorsArr = malloc(4);
        assert(unsCharImpColorsArr);
        counter = 0;
        for (i = 39; i >= 36; i--) {
                memcpy(unsCharImpColorsArr + counter, unsCharArray + i, 1);
                counter++;
        } //store hex numbers backwards (backwards of backwards = forwards)
        char charHexTmp11[8]; //char array to hold 8 bytes of hex vars
        for (i = 0; i < 4; i++)
                sprintf(&charHexTmp11[i * 2],"%02x", unsCharImpColorsArr[i]); //converts to char
        int impColors = 0;
        indexBackwards = 7;
        for (i = 0; i < 8; i++) {
                if ((int) charHexTmp11[i] - 48 > 9) { //if (int) cast is more than a 0-9 num, it's a char (abcdef)
                        impColors = impColors + (((int) charHexTmp11[i] - 87) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
                else {
                        impColors = impColors + (((int) charHexTmp11[i] - 48) * (pow(16, indexBackwards)));
                        indexBackwards--;
                }
        }
        imageHeader -> numberOfImportantColors = impColors;

        free(unsCharArray);
        free(unsCharDibSizeArr);
        free(unsCharWidthArr);
        free(unsCharHeightArr);
        free(unsCharPlaneArr);
        free(unsCharBPPArr);
        free(unsCharCompressionArr);
        free(unsCharImgSizeArr);
        free(unsCharPPXArr);
        free(unsCharPPYArr);
        free(unsCharColorsArr);
        free(unsCharImpColorsArr);
        return imageHeader;
}

void *processColorTable(char *file, void *colorTableArr, int numberOfColors)
{
        int fd, rd;
        fd = open(file, O_RDONLY);
        if (fd == -1)
                err_sys("Error opening file %s, check spelling?\n", file); //file not found
        lseek(fd, 54, SEEK_SET); //go to 54th byte
        if (fd == -1)
                err_sys("lseek failed\n");
        rd = read(fd, colorTableArr, 4 * numberOfColors); //(usually) will be 1024 bytes -> 4 * 256
        if (rd == -1)
                err_sys("Error reading file %s\n");
        close(fd);
        return colorTableArr;
}

struct colorTable_t **getColorTableInfo(struct colorTable_t **colorTable, void *colorTableArr, int numberOfColors)
{
        unsigned char *unsCharArr = malloc(4 * numberOfColors);
        assert(unsCharArr);
        memcpy(unsCharArr, colorTableArr, 4 * numberOfColors);
        for (int i = 0; i < numberOfColors; i++) {
                for (int j = 0; j < 4; j++) {
                        if (j == 0)
                                colorTable[i] -> red = (int) unsCharArr[(4 * i) + j];
                        if (j == 1)
                                colorTable[i] -> green = (int) unsCharArr[(4 * i) + j];
                        if (j == 2)
                                colorTable[i] -> blue = (int) unsCharArr[(4 * i) + j];
                        else
                                colorTable[i] -> alpha = (int) unsCharArr[(4 * i) + j];
                }
        }
        return colorTable;
}

void *processGrayscalePixelData(char *file, void *grayscalePixelDataArr, int imageSize, int offset)
{
        int fd, rd;
        fd = open(file, O_RDONLY);
        if (fd == -1)
                err_sys("Error opening file %s, check spelling?\n", file); //file not found
        lseek(fd, offset, SEEK_SET); //go to the offset byte
        if (fd == -1)
                err_sys("lseek failed\n");
        rd = read(fd, grayscalePixelDataArr, imageSize);
        if (rd == -1)
                err_sys("Error reading file %s\n");
        close(fd);
        return grayscalePixelDataArr;
}

unsigned char **initMatrix(unsigned char **a, int height, int width)
{
        int i, j;
        /* allocate rows */
        a = malloc(height * sizeof(int *));
        if(!a)
                return NULL;
        for(i = 0; i < width; i++) {
                /* allocate cols for each row */
                a[i] = malloc(width * sizeof(int));
                if (!a[i]) {
                        for (j = 0; j < i; j++)
                                free(a[j]);
                        free(a);
                        return NULL;
                }
        }
        return a;
}

unsigned char **getGrayscalePixelInfo(unsigned char **grayscalePixelData, void *grayscalePixelDataArr, int imageSize, int height, int width)
{
        unsigned char *unsCharArr = malloc(imageSize);
        assert(unsCharArr);
        memcpy(unsCharArr, grayscalePixelDataArr, imageSize);
        int counter = 0;
        for (int i = height - 1; i >= 0; i--) { //start at last row, which is how data is stored
                for (int j = 0; j <= width - 1; j++) {
                        grayscalePixelData[i][j] = (int)unsCharArr[counter];
                        counter++;
                }
        }
        free(unsCharArr);
        return grayscalePixelData;
}

void *processColorPixelData(char *file, void *colorPixelDataArr, int imageSize)
{
        int fd, rd;
        fd = open(file, O_RDONLY);
        if (fd == -1)
                err_sys("Error opening file %s, check spelling?\n", file); //file not found
        lseek(fd, 54, SEEK_SET); //go to the 54th byte
        if (fd == -1)
                err_sys("lseek failed\n");
        rd = read(fd, colorPixelDataArr, imageSize);
        if (rd == -1)
                err_sys("Error reading file %s\n");
        close(fd);
        return colorPixelDataArr;
}

struct colorPixelData_t **initColorMatrix(struct colorPixelData_t **b, int height, int width)
{
        int i;
        int j;
        b = malloc(height * sizeof(struct colorPixelData_t));
        if(!b)
                return NULL;
        for(i = 0; i < width; i++) {
                /* allocate cols for each row */
                b[i] = malloc(width * sizeof(struct colorPixelData_t));
                if (!b[i]) {
                        for (j = 0; j < i; j++)
                                free(b[j]);
                free(b);
                return NULL;
                }
        }
        return b;
}

struct colorPixelData_t **getColorPixelInfo(struct colorPixelData_t **colorPixelData, void *colorPixelDataArr, int imageSize, int height, int width)
{
        unsigned char *unsCharArr = malloc(imageSize);
        assert(unsCharArr);
        memcpy(unsCharArr, colorPixelDataArr, imageSize);

        int counter = 0;
        for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                        for (int k = 0; k < 3; k++) { //used to iterate over every three pixels
                                if (k == 0)
                                        colorPixelData[i][j].blue = (int)unsCharArr[counter]; //stored backwards, so blue comes first
                                else if (k == 1)
                                        colorPixelData[i][j].green = (int)unsCharArr[counter];
                                else {
                                        colorPixelData[i][j].red = (int)unsCharArr[counter];
                                }
                                counter++;
                        }
                }
        }
        free(colorPixelDataArr);
        free(unsCharArr);
        return colorPixelData;
}

void printGrayscaleHeader(struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorTable_t **colorTable, unsigned char **grayscalePixelData, int headerInfoFull)
{
        printf("Header: %s\nSize of bitmap (bytes): %d\nOffset (start of image data): %d\n\n", fileHeader -> header, fileHeader -> size, fileHeader -> offset);
        printf("Size of DIB: %d\nBitmap width in pixels: %d\nBitmap height in pixels: %d\n", imageHeader -> dibSize, imageHeader -> width, imageHeader -> height);
        printf("Number of color planes: %d\nNumber of bits per pixel: %d\nCompression Method: %d\n", imageHeader -> planes, imageHeader -> bitsPerPixel, imageHeader -> compressionType);
        printf("Image size: %d\nHorizontal resolution: %d\nVertical Resolution: %d\n", imageHeader -> imageSize, imageHeader -> pixelsPerXMeter, imageHeader -> pixelsPerYMeter);
        printf("Number of colors: %d\nNumber of important colors: %d\n\n", imageHeader -> numberOfColors, imageHeader -> numberOfImportantColors);

        if (headerInfoFull == 1) {
                printf("Color Table\nindex\tred\tgreen\tblue\talpha\t\n-------------------------------------\n");
                for (int i = 0; i < imageHeader -> numberOfColors; i++)
                        printf("%d\t%d\t%d\t%d\t%d\n", i, colorTable[i] -> red, colorTable[i] -> green, colorTable[i] -> blue, colorTable[i] -> alpha);
                printf("Pixel Data\n(row\tcol)\tindex\n---------------------\n");
                for (int i = 0; i < imageHeader -> height; i++) {
                        for (int j = 0; j < imageHeader -> width; j++)
                                printf("(%d\t%d)\t%d\n", i, j, grayscalePixelData[i][j]);
                }
        }
}

void printColorHeader(struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorPixelData_t **colorPixelData, int headerInfoFull)
{
        printf("Header: %s\nSize of bitmap (bytes): %d\nOffset (start of image data): %d\n\n", fileHeader -> header, fileHeader -> size, fileHeader -> offset);
        printf("Size of DIB: %d\nBitmap width in pixels: %d\nBitmap height in pixels: %d\n", imageHeader -> dibSize, imageHeader -> width, imageHeader -> height);
        printf("Number of color planes: %d\nNumber of bits per pixel: %d\nCompression Method: %d\n", imageHeader -> planes, imageHeader -> bitsPerPixel, imageHeader -> compressionType);
        printf("Image size: %d\nHorizontal resolution: %d\nVertical Resolution: %d\n", imageHeader -> imageSize, imageHeader -> pixelsPerXMeter, imageHeader -> pixelsPerYMeter);
        printf("Number of colors: %d\nNumber of important colors: %d\n\n", imageHeader -> numberOfColors, imageHeader -> numberOfImportantColors);

        if (headerInfoFull == 1) {
                printf("No color table present\n\n");
                printf("Pixel Data\n(row, col)  red  green  blue\n-------------------------------------\n");
                for (int i = 0; i < imageHeader -> height; i++) {
                        for (int j = 0; j < imageHeader -> width; j++)
                                printf("(%d, %d)  %d  %d  %d\n", i, j, colorPixelData[i][j].red, colorPixelData[i][j].green, colorPixelData[i][j].blue);
                }
        }
}

void writeGrayscaleOut(void *fileHeaderArr, void *imageHeaderArr, void *colorTableArr, void *grayscalePixelDataArr, char *outputFile, struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorTable_t **colorTable, unsigned char **grayscalePixelData)
{
        int fd, rd;
        fd = open(outputFile, O_WRONLY | O_CREAT | O_EXCL);
        if (fd == -1)
                err_sys("Error creating / opening file\n");
        rd = write(fd, fileHeaderArr, 14);
        if (rd < 0)
                err_sys("Error writing to file\n");
        rd = write(fd, imageHeaderArr, 40);
        if (rd < 0)
                err_sys("Error writing to file\n");
        rd = write(fd, colorTableArr, fileHeader -> offset - 54);
        if (rd <  0)
                err_sys("Error writing to file\n");
        rd = write(fd, grayscalePixelDataArr, imageHeader -> imageSize);
        if (rd < 0)
                err_sys("Error writing to file\n");
        close(fd);
}

void writeColorOut(void *fileHeaderArr, void *imageHeaderArr, char *outputFile, struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorPixelData_t **colorPixelData, void *colorPixelDataArr)
{
        int fd, rd;
        fd = open(outputFile, O_WRONLY | O_CREAT | O_EXCL);
        if (fd == -1)
                err_sys("Error creating / opening file\n");
        rd = write(fd, fileHeaderArr, 14);
        if (rd < 0)
                err_sys("Error writing to file\n");
        rd = write(fd, imageHeaderArr, 40);
        if (rd < 0)
                err_sys("Error writing to file\n");
        rd = write(fd, colorPixelDataArr, imageHeader -> imageSize);
        if (rd < 0)
                err_sys("Error writing to file\n");
        close(fd);
}

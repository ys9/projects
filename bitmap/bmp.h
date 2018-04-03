/**
 * @file bmp.h
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

struct fileHeader_t {
        unsigned char header[2];
        int size;
        int offset;
};

struct imageHeader_t {
        int dibSize;
        int width;
        int height;
        int planes;
        int bitsPerPixel;
        int compressionType;
        int imageSize;
        int pixelsPerXMeter;
        int pixelsPerYMeter;
        int numberOfColors;
        int numberOfImportantColors;
};

struct colorTable_t {
        int red;
        int green;
        int blue;
        int alpha;
};

struct colorPixelData_t {
        int red;
        int green;
        int blue;
};

int checkColor(int color);
void *processFileHeader(char *file, void *fileHeaderArr);
struct fileHeader_t *getFileHeaderInfo(struct fileHeader_t *fileHeader, unsigned char *unsCharArray);
void *processImageHeader(char *file, void *imageHeaderArr);
struct imageHeader_t *getImageHeaderInfo(struct imageHeader_t *imageHeader, unsigned char *unsCharArray);
void *processColorTable(char *file, void *colorTableArr, int numberOfColors);
struct colorTable_t **getColorTableInfo(struct colorTable_t **colorTable, void *colorTableArr, int numberOfColors);
void *processGrayscalePixelData(char *file, void *grayscalePixelData, int imageSize, int offset);
unsigned char **initMatrix(unsigned char **a, int height, int width);
unsigned char **getGrayscalePixelInfo(unsigned char **grayscalePixelData, void *grayscalePixelDataArr, int imageSize, int height, int width);
void *processColorPixelData(char *file, void *colorPixelDataArr, int imageSize);
struct colorPixelData_t **initColorMatrix(struct colorPixelData_t **b, int height, int width);
struct colorPixelData_t **getColorPixelInfo(struct colorPixelData_t **colorPixelData, void *colorPixelDataArr, int imageSize, int height, int width);
void printGrayscaleHeader(struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorTable_t **colorTable, unsigned char **grayscalePixelData, int headerInfoFull);
void printColorHeader(struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorPixelData_t **colorPixelData, int headerInfoFull);
void writeGrayscaleOut(void *fileHeaderArr, void *imageHeaderArr, void *colorTableArr, void *grayscalePixelDataArr, char *outputFile, struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorTable_t **colorTable, unsigned char **grayscalePixelData);
void writeColorOut(void *fileHeaderArr, void *imageHeaderArr, char *outputFile, struct fileHeader_t *fileHeader, struct imageHeader_t *imageHeader, struct colorPixelData_t **colorPixelData, void *colorPixelDataArr);

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

// BMP header structure.

// Reference: http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
#pragma pack(push, 1)
typedef struct {
    // Bitmap Header - 14 bytes
    uint16_t Signature;
    uint32_t FileSize;
    uint32_t Reserved;
    uint32_t DataOffset;
 
    // Bitmap Info Header - 40 bytes
    uint32_t Size; // =40
    uint32_t Width; // Horizontal width of bitmap in pixels
    uint32_t Height; // Vertical height
    uint16_t Planes; // number of planes (=1)

    uint16_t BitsPerPx; /*
        Bits per Pixel used to store palette entry information. This also identifies in an indirect way the number of possible colors. Possible values are:
            1 = monochrome palette. NumColors = 1  
            4 = 4bit palletized. NumColors = 16  
            8 = 8bit palletized. NumColors = 256 
            16 = 16bit RGB. NumColors = 65536
            24 = 24bit RGB. NumColors = 16M
    */

   uint32_t Compression; // Level of compression - 0 = none, 1 = 8bit RLE, 2 = 4bit RLE
   uint32_t ImageSize; // Compressed size of image. Set to 0 if compression is set to 0
   uint32_t XpxPerM;
   uint32_t YpxPerM;
   uint32_t ColorsUsed; 
   uint32_t ImportantColors;

} BMPHeader;
#pragma pack(pop)

// color start and end. start is at bottom of image.
typedef struct {
    // bottom of image
    uint8_t redStart;
    uint8_t greenStart;
    uint8_t blueStart;

    // top of image
    uint8_t redTarget;
    uint8_t greenTarget;
    uint8_t blueTarget;
} ColorParams;

// Image File parameters
typedef struct {
    int bpp;
    int height;
    int width;
    char *fileName;
} ImageFile;


// for passing gradient parameters
typedef struct {
    int redDelta;
    int greenDelta;
    int blueDelta;
    uint8_t redStart;
    uint8_t greenStart;
    uint8_t blueStart;
    int height;
    int width;
    int rowSize;
} GradientParams;

// configure bmp generator from file
typedef struct {
    char gradient_direction[11];
    int height;
    int width;
    char fileName[32];
    uint8_t redStart;
    uint8_t greenStart;
    uint8_t blueStart;
    uint8_t redTarget;
    uint8_t greenTarget;
    uint8_t blueTarget;
} Configuration;

// Functions

int configure(FILE *file, Configuration *config);

#endif // MAIN_H
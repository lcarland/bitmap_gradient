#include <stdio.h>
#include <stdint.h>

// improvements:
// read from parameter file
// generate default parameter file if not exists

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

// gradient direction options
typedef enum {
    horizontal,
    diagonal,
    vertical,
} gradient;


void horizontalGradient(GradientParams p, FILE *file) {
    // rowSize reinitiated outside of object for equation usage
    int rowSize = p.rowSize;

    for (int y = 0; y < p.height; y++) {
        float yprog = (float)y / p.height; // percentage

        uint8_t red = p.redStart - (yprog *  p.redDelta);
        uint8_t green = p.greenStart - (yprog * p.greenDelta);
        uint8_t blue = p.blueStart - (yprog * p.blueDelta); 

        for (int x = 0; x < p.width; x ++) {

            fwrite(&blue, 1, 1, file);
            fwrite(&green, 1, 1, file);
            fwrite(&red, 1, 1, file);

        }

        // padding for 24bit color
        for (int pad = 0; pad < (4 - (rowSize % 4)) % 4; pad++) {
            fputc(0, file);
        }
    }
}


void verticalGradient(GradientParams p, FILE *file) {
    // rowSize reinitiated outside of object for equation usage
    int rowSize = p.rowSize;

    for (int y = 0; y < p.height; y++) {
        for (int x = 0; x < p.width; x++) {
            float xprog = (float)x / p.width;

            uint8_t red = p.redStart - (xprog * p.redDelta);
            uint8_t green = p.greenStart - (xprog * p.greenDelta);
            uint8_t blue = p.blueStart - (xprog * p.blueDelta);

            fwrite(&blue, 1, 1, file);
            fwrite(&green, 1, 1, file);
            fwrite(&red, 1, 1, file);
        }

        for (int pad = 0; pad < (4 - (rowSize % 4)) % 4; pad++) {
            fputc(0, file);
        }
    }
}


void diagonalGradient(GradientParams p, FILE *file) {
    // rowSize reinitiated outside of object for equation usage
    int rowSize = p.rowSize;

    // deltas halved
    int _redDelta = p.redDelta / 2;
    int _greenDelta = p.greenDelta / 2;
    int _blueDelta = p.blueDelta / 2;

    for (int y = 0; y < p.height; y++) {
        float yprog = (float)y / p.height; // percentage 

        for (int x = 0; x < p.width; x ++) {
            float xprog = (float)x / p.width; // percentage

            uint8_t red = (p.redStart - (yprog *  _redDelta)) - (xprog * _redDelta);
            uint8_t green = (p.greenStart - (yprog * _greenDelta)) - (xprog * _greenDelta);
            uint8_t blue = (p.blueStart - (yprog * _blueDelta)) - (xprog * _blueDelta);

            fwrite(&blue, 1, 1, file);
            fwrite(&green, 1, 1, file);
            fwrite(&red, 1, 1, file);

        }

        // padding for 24bit color
        for (int pad = 0; pad < (4 - (rowSize % 4)) % 4; pad++) {
            fputc(0, file);
        }
    }
}


int makeBMP(ImageFile p, ColorParams c, gradient direction) {
    int row_size = ((p.bpp * p.width + 31) / 32) * 4;

    BMPHeader header = {
        .Signature = 0x4D42,
        .FileSize = sizeof(BMPHeader) + row_size * p.height,
        .DataOffset = sizeof(BMPHeader),
        .Reserved = 0,
        .Size = 40,
        .Width = p.width,
        .Height = p.height,
        .Planes = 1,
        .BitsPerPx = p.bpp,
        .Compression = 0,
        .ImageSize = 0,
        .XpxPerM = 0,
        .YpxPerM = 0,
        .ColorsUsed = 0,
        .ImportantColors = 0
    };

    FILE *file = fopen(p.fileName, "wb");
    if (!file) {
        perror("Unable to create bmp image");
        return 1;
    }

    fwrite(&header, sizeof(BMPHeader), 1, file);

    int red_delta = c.redStart - c.redTarget;
    int green_delta = c.greenStart - c.greenTarget;
    int blue_delta = c.blueStart - c.blueTarget;

    GradientParams gradParams = {
        .redDelta = c.redStart - c.redTarget,
        .greenDelta = c.greenStart - c.greenTarget,
        .blueDelta = c.blueStart - c.blueTarget,
        .redStart = c.redStart,
        .greenStart = c.greenStart,
        .blueStart = c.blueStart,
        .height = p.height,
        .width = p.width,
        .rowSize = row_size,
    };

    switch (direction) {
    case horizontal:
        horizontalGradient(gradParams, file);
        break;
    
    case vertical:
        verticalGradient(gradParams, file);
        break;

    case diagonal:
        diagonalGradient(gradParams, file);
        break;
    
    default:
        printf("Invalid direction selected\n");
        return 1;
    }

    fclose(file);
}


int main(void) {
    ImageFile params = {
        .bpp = 24,
        .height = 1024,
        .width = 512,
        .fileName = "output.bmp",
    };

    ColorParams colors = {
        .redStart = 255,
        .greenStart = 0,
        .blueStart = 0,

        .redTarget = 120,
        .greenTarget = 120,
        .blueTarget = 160,
    };

    gradient direction = diagonal;

    makeBMP(params, colors, direction);
}
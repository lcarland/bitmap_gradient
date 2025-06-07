#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "main.h"


void horizontalGradient(GradientParams p, FILE *file) {
    // rowSize reinitialized outside of object for equation usage
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
    // rowSize reinitialized outside of object for equation usage
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
    // rowSize reinitialized outside of object for equation usage
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

    return 0;
}


void setDefaultConfig(FILE *file) {
    char config[15][32] = {
        "# image settings",
        "gradient_direction=vertical",
        "height=512",
        "width=512",
        "fileName=output.bmp",
        "",
        "# Starting Color",
        "redStart=255",
        "greenStart=200",
        "blueStart=0",
        "",
        "# Ending Color",
        "redTarget=100",
        "greenTarget=0",
        "blueTarget=255"
    };

    for (int i = 0; i <= 14; i++) {
        fprintf(file, "%s\n", config[i]);
    }
}


int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "config") == 0) {
            FILE *file = fopen("image.conf", "w");
            // insert file defaults, then close and exit.
            setDefaultConfig(file);
            fclose(file);
            puts("image.conf set to default values");
            return 0;
        }
    }

    FILE *file = fopen("image.conf", "r");
    if (!file) {
        puts("image.conf not detected, run config as an argument to create one");
        return 0;
    };

    Configuration config;
    configure(file, &config, argc, argv);

    fclose(file);


    ImageFile params = {
        .bpp = 24,
        .height = config.height,
        .width = config.width,
        .fileName = config.fileName,
    };

    ColorParams colors = {
        .redStart = config.redStart,
        .greenStart = config.greenStart,
        .blueStart = config.blueStart,

        .redTarget = config.redTarget,
        .greenTarget = config.greenTarget,
        .blueTarget = config.blueTarget,
    };

    gradient direction = config.gradient_direction;

    makeBMP(params, colors, direction);
}
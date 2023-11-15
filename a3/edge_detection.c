#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel.
 *
 * Note that this function should allocate space only for a single Pixel;
 * do *not* store more than one Pixel at a time, it isn't necessary here!
 */
// typedef struct {
//     int headerSize;          // The size of the header.
//     unsigned char *header;   // The contents of the image header.
//     int width;               // The width of the image, in pixels.
//     int height;              // The height of the image, in pixels.
// } Bitmap;
// #define BMP_FILE_SIZE_OFFSET 2
// #define BMP_HEADER_SIZE_OFFSET 10
// #define BMP_WIDTH_OFFSET 18
// #define BMP_HEIGHT_OFFSET 22
void edge_detection_filter(Bitmap *bmp) {
Pixel *row0 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *row1 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *row2 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *pixel = malloc(sizeof(Pixel*));


    if (pixel == NULL || row0 == NULL || row1 == NULL || row2 == NULL){
        fprintf(stderr, "Memory not allocated in edge detection filter.\n");
        exit(1);
    }

    if (fread(row0, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row0 data in edge detection filter.\n");
        free(row0);
        exit(1);

    } if (fread(row1, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row1 data in edge detection filter.\n");
        free(row1);
        exit(1);

    } if (fread(row2, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row2 data in edge detection filter.\n");
        free(row2);
        exit(1);

    }

    for (int i = 0; i < bmp -> height; i++){
        if (i >= 2 && i < bmp -> height - 1){
            row0 = row1;
            row1 = row2;
            if(fread(row2, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){
                fprintf(stderr, "Cannot read pixel data in edge detection filter.\n");
                free(row2);
                exit(1);
            }
        }
        for (int j = 0; j < bmp -> width; j++){
            int col = j;
            col = max(1, min(col, bmp -> width - 2));

            *pixel = apply_edge_detection_kernel(row0+col-1, row1+col-1, row2+col-1);

            if(fwrite(pixel, sizeof(Pixel), 1, stdout) != 1){
                fprintf(stderr, "Cannot write pixel data in edge detection filter.\n");
                free(pixel);
                exit(1);
            };
        }
    }
    free(pixel);

}

int main() {
    run_filter(edge_detection_filter, 1);
    return 0;
}
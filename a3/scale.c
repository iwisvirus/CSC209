#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

int GLOBAL_SCALE_FACTOR;

/*
 * Main filter loop.
 * This function is responsible for doing the following:
 *   1. Read in pixels one at a time (because copy is a pixel-by-pixel transformation).
 *   2. Immediately write out each pixel with scaled factor where
 * The pixel at position (i, j) in the scaled image should equal the pixel at position 
 * (i / scale_factor, j / scale_factor) in the original image.
 *
 *
 * TODO: complete this function when working on the "scale" filter.
 *

 */

void scale_filter(Bitmap *bmp) {
    Pixel *data = malloc(sizeof(Pixel*) * bmp -> width * bmp -> height);
    if (data == NULL){
        fprintf(stderr, "Memory for pixel data not allocated in scale filter.\n");
        exit(1);
    }

    for (int i = 0; i < (bmp -> height)/ GLOBAL_SCALE_FACTOR; i++){
        for (int j = 0; j < (bmp -> width)/GLOBAL_SCALE_FACTOR; j++){
            if (fread(data + (i * (bmp -> width) + j) * 3, sizeof(Pixel), 1, stdin) != 1){
                fprintf(stderr, "Cannot read pixel datas in scale filter.\n");
                free(data);
                exit(1);
            };
        }
    }
    for (int i = 0; i < bmp -> height; i++){
        for (int j = 0; j < bmp -> width; j++){
            if (fwrite(data + (i/GLOBAL_SCALE_FACTOR * (bmp -> width) + j/GLOBAL_SCALE_FACTOR)*3, sizeof(Pixel), 1, stdout) != 1){
                fprintf(stderr, "Cannot write pixel datas in scale filter.\n");
                free(data);
                exit(1);
            };
        }
    }
    
}

int main(int argc, char **argv) {
    if (argc != 2){
        fprintf(stderr, "Usage: %s <scale_factor>\n", argv[0]);
    }
    GLOBAL_SCALE_FACTOR = atoi(argv[1]);
    run_filter(scale_filter, GLOBAL_SCALE_FACTOR);
    return 0;
}
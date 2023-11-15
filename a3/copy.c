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

void copy_filter(Bitmap *bmp) {
    Pixel *pixel = malloc(sizeof(Pixel*));
    if (pixel == NULL){
        fprintf(stderr, "Memory not allocated in copy_filter.\n");
        exit(1);
    }

    for (int i = 0; i < bmp -> height; i++){
        for (int j = 0; j < bmp -> width; j++){
            if(fread(pixel, sizeof(Pixel), 1, stdin) != 1){
                fprintf(stderr, "Cannot read pixel data from copy_filter.\n");
                free(pixel);
                exit(1);
            };
            if(fwrite(pixel, sizeof(Pixel), 1, stdout) != 1){
                fprintf(stderr, "Cannot write pixel data from copy_filter.\n");
                free(pixel);
                exit(1);
            };
        }
    }

    free(pixel);

}

int main() {
    run_filter(copy_filter, 1);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

void greyscale_filter(Bitmap *bmp) {
    Pixel *pixel = malloc(sizeof(Pixel*));
    if (pixel == NULL){
        fprintf(stderr, "Memory not allocated in greyscale filter.\n");
        exit(1);
    }

    Pixel *grey_pixel = malloc(sizeof(Pixel*));
    if (grey_pixel == NULL){
        fprintf(stderr, "Memory not allocated in greyscale filter.\n");
        exit(1);
    }

    for (int i = 0; i < bmp -> height; i++){
        for (int j = 0; j < bmp -> width; j++){
            if(fread(pixel, sizeof(Pixel), 1, stdin) != 1){
                fprintf(stderr, "Cannot read pixel data in greyscale filter.\n");
                free(pixel);
                exit(1);
            };
            grey_pixel -> blue = (pixel -> blue + pixel -> green + pixel -> red)/3;
            grey_pixel -> green = (pixel -> blue + pixel -> green + pixel -> red)/3;
            grey_pixel -> red = (pixel -> blue + pixel -> green + pixel -> red)/3;
            
            if(fwrite(grey_pixel, sizeof(Pixel), 1, stdout) != 1){
                fprintf(stderr, "Cannot write pixel data in greyscale filter.\n.\n");
                free(grey_pixel);
                exit(1);
            };
        }
    }

    free(pixel);
    free(grey_pixel);

}

int main() {
    run_filter(greyscale_filter, 1);
    return 0;
}
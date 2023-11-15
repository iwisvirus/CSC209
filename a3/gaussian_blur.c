#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"


/*

Note about boundaries: because each of these filters compute 
transformations based on each pixel’s neighbours, the pixels 
at the very edge of the image pose a problem (they don’t have 
neighbours on one or more sides). For simplicity on this assignment, 
resolve this simply by making every “edge” pixel have the exact same 
transformed value as its “inner” neighbour. For example, the pixel 
at position (0, 5) should have the same transformed value as the 
pixel (1, 5), and the pixel at position (0, 0) should have the same 
transformed value as the pixel (1, 1). Consider this as, using the 
pixel in the ring of values immediately inside the edge, so, if the 
image width is 200, the pixel at position (199, 3) should have the same 
transformed value as the pixel (198, 3). You may find the provided 
min and max macros helpful for handling these boundary pixels elegantly.

 */

void gaussian_blur_filter(Bitmap *bmp) {
Pixel *row0 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *row1 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *row2 = malloc(sizeof(Pixel*) * bmp -> width);
Pixel *pixel = malloc(sizeof(Pixel*));


    if (pixel == NULL || row0 == NULL || row1 == NULL || row2 == NULL){
        fprintf(stderr, "Memory not allocated in gaussian blur filter.\n");
        exit(1);
    }

    if (fread(row0, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row0 data in gaussian blur filter.\n");
        free(row0);
        exit(1);

    } if (fread(row1, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row1 data in gaussian blur filter.\n");
        free(row1);
        exit(1);

    } if (fread(row2, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){            
        fprintf(stderr, "Cannot read row2 data in gaussian blur filter.\n");
        free(row2);
        exit(1);

    }

    for (int i = 0; i < bmp -> height; i++){
        if (i >= 2 && i < bmp -> height - 1){
            row0 = row1;
            row1 = row2;
            if(fread(row2, sizeof(Pixel), bmp -> width, stdin) != bmp -> width){
                fprintf(stderr, "Cannot read pixel data in gaussian blur filter.\n");
                free(row2);
                exit(1);
            }
        }
        for (int j = 0; j < bmp -> width; j++){
            int col = j;
            col = max(1, min(col, bmp -> width - 2));

            *pixel = apply_gaussian_kernel(row0+col-1, row1+col-1, row2+col-1);

            if(fwrite(pixel, sizeof(Pixel), 1, stdout) != 1){
                fprintf(stderr, "Cannot write pixel data in gaussian blur filter.\n");
                free(pixel);
                exit(1);
            };
        }
    }

    free(pixel);

}

int main() {
    run_filter(gaussian_blur_filter, 1);
    return 0;
}
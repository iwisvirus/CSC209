#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"


/*
 * Read in the location of the pixel array, the image width, and the image 
 * height in the given bitmap file.
 */
void read_bitmap_metadata(FILE *image, int *pixel_array_offset, int *width, int *height) {
    fseek(image, 10, SEEK_SET);
    fread(pixel_array_offset, 4, 1,image);
    fseek(image, 4, SEEK_CUR);
    fread(width, 4, 1, image);
    fseek(image, 0, SEEK_CUR);
    fread(height, 4, 1, image);
}

/*
 * Read in pixel array by following these instructions:
 *
 * 1. First, allocate space for m `struct pixel *` values, where m is the 
 *    height of the image.  Each pointer will eventually point to one row of 
 *    pixel data.
 * 2. For each pointer you just allocated, initialize it to point to 
 *    heap-allocated space for an entire row of pixel data.
 * 3. Use the given file and pixel_array_offset to initialize the actual 
 *    struct pixel values. Assume that `sizeof(struct pixel) == 3`, which is 
 *    consistent with the bitmap file format.
 *    NOTE: We've tested this assumption on the Teaching Lab machines, but 
 *    if you're trying to work on your own computer, we strongly recommend 
 *    checking this assumption!
 * 4. Return the address of the first `struct pixel *` you initialized.
 */
struct pixel **read_pixel_array(FILE *image, int pixel_array_offset, int width, int height) {
    // struct pixel **rows = malloc(sizeof(struct pixel *) * height);
    struct pixel *rows = malloc(sizeof(struct pixel) * width);
    struct pixel **columns = malloc(sizeof(rows) * height);
    int i;
    for(i = 0; i < height; i++){
        columns[i] = &rows[i]; 
    }
    fseek(image, pixel_array_offset, SEEK_SET);
    for (i = 0; i < height; i++){
            fread(columns[i], 3 * width, 1, image);
            // fseek(image, 0, SEEK_CUR);
    }
    return columns;




}


/*
 * Print the blue, green, and red colour values of a pixel.
 * You don't need to change this function.
 */
void print_pixel(struct pixel p) {
    printf("(%u, %u, %u)\n", p.blue, p.green, p.red);
}

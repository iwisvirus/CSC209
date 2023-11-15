#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

#define MAXHEADER 1024

Bitmap *read_header() {
    //allocating space for bitmap pointer
    Bitmap *bmp = malloc(sizeof(Bitmap*));
    if(bmp == NULL){
        fprintf(stderr, "Failed to allocate memory for Bitmap in read_header.\n");
        exit(1);
    }

    //read in first 10 bytes
    unsigned char bytes_0_9[BMP_HEADER_SIZE_OFFSET];
    if(fread(bytes_0_9, 1, BMP_HEADER_SIZE_OFFSET, stdin) != BMP_HEADER_SIZE_OFFSET){
        fprintf(stderr,"Failed to read bmp header.\n");
        exit(1);
    };
    
    //read in header size
    unsigned char bytes_10_13[4];
    if(fread(bytes_10_13, sizeof(int), 1, stdin) != 1){
        fprintf(stderr, "Failed to read bmp header size.\n");
        exit(1);
    };
    memcpy(&bmp -> headerSize, bytes_10_13, 4);

    // allocate space for actual header
    bmp -> header = malloc(bmp -> headerSize);
    if (bmp -> header == NULL){
        fprintf(stderr, "Failed to allocate memory for bmp headersize.\n");
        exit(1);
    }
    //read in rest of header data
    unsigned char bytes_rest[bmp -> headerSize - BMP_HEADER_SIZE_OFFSET - 4];
    if(fread(bytes_rest, 1, bmp -> headerSize - BMP_HEADER_SIZE_OFFSET - 4, stdin) != bmp -> headerSize - BMP_HEADER_SIZE_OFFSET - 4){
        fprintf(stderr, "Failed to read remaining bmp header data.\n");
    };

    memcpy(bmp -> header, bytes_0_9, BMP_HEADER_SIZE_OFFSET);
    memcpy(bmp -> header + BMP_HEADER_SIZE_OFFSET, bytes_10_13, sizeof(int));
    memcpy(bmp -> header + BMP_HEADER_SIZE_OFFSET + sizeof(int), bytes_rest, bmp -> headerSize - BMP_HEADER_SIZE_OFFSET - sizeof(bytes_10_13));

    // read in height from header data
    int height;
    memcpy(&height, bmp -> header + BMP_HEIGHT_OFFSET, sizeof(int));

    // read in width 
    int width;
    memcpy(&width, bmp -> header + BMP_WIDTH_OFFSET, sizeof(int));

    bmp -> height = height;
    bmp -> width = width;

    return bmp;

}

/*
 * Write out bitmap metadata to stdout.
 * You may add extra fprintf calls to *stderr* here for debugging purposes.
 */
void write_header(const Bitmap *bmp) {
    fwrite(bmp->header, bmp->headerSize, 1, stdout);
}

/*
 * Free the given Bitmap struct.
 */
void free_bitmap(Bitmap *bmp) {
    free(bmp->header);
    free(bmp);
}

void scale(Bitmap *bmp, int GLOBAL_SCALE_FACTOR) {
    int new_width = (bmp -> width) * GLOBAL_SCALE_FACTOR;
    int new_height = (bmp -> height) * GLOBAL_SCALE_FACTOR;

    bmp -> width = new_width;
    bmp -> height = new_height;

    int new_file_size = (bmp -> headerSize) + (new_width * new_height * sizeof(Pixel));
    memcpy(bmp -> header + BMP_FILE_SIZE_OFFSET, &new_file_size, sizeof(int));
    memcpy(bmp -> header + BMP_HEIGHT_OFFSET, &new_height, sizeof(int));
    memcpy(bmp -> header + BMP_WIDTH_OFFSET, &new_width, sizeof(int));

}


/*
 * The "main" function.
 *
 * Run a given filter function, and apply a scale factor if necessary.
 * You don't need to modify this function to make it work with any of
 * the filters for this assignment.
 */
void run_filter(void (*filter)(Bitmap *), int GLOBAL_SCALE_FACTOR) {
    Bitmap *bmp = read_header();

    if (GLOBAL_SCALE_FACTOR > 1) {
        scale(bmp, GLOBAL_SCALE_FACTOR);
    }

    write_header(bmp);

    // Note: here is where we call the filter function.
    filter(bmp);

    free_bitmap(bmp);
}


/******************************************************************************
 * The gaussian blur and edge detection filters.
 * You should NOT modify any of the code below.
 *****************************************************************************/
const int gaussian_kernel[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
};

const int kernel_dx[3][3] = {
    {1, 0, -1},
    {2, 0, -2},
    {1, 0, -1}
};

const int kernel_dy[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
};

const int gaussian_normalizing_factor = 16;


Pixel apply_gaussian_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b = 0, g = 0, r = 0;
    Pixel *rows[3] = {row0, row1, row2};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b += rows[i][j].blue * gaussian_kernel[i][j];
            g += rows[i][j].green * gaussian_kernel[i][j];
            r += rows[i][j].red * gaussian_kernel[i][j];
        }
    }

    b /= gaussian_normalizing_factor;
    g /= gaussian_normalizing_factor;
    r /= gaussian_normalizing_factor;

    Pixel new = {
        .blue = b,
        .green = g,
        .red = r
    };

    return new;
}


Pixel apply_edge_detection_kernel(Pixel *row0, Pixel *row1, Pixel *row2) {
    int b_dx = 0, b_dy = 0;
    int g_dx = 0, g_dy = 0;
    int r_dx = 0, r_dy = 0;
    Pixel *rows[3] = {row0, row1, row2};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            b_dx += rows[i][j].blue * kernel_dx[i][j];
            b_dy += rows[i][j].blue * kernel_dy[i][j];
            g_dx += rows[i][j].green * kernel_dx[i][j];
            g_dy += rows[i][j].green * kernel_dy[i][j];
            r_dx += rows[i][j].red * kernel_dx[i][j];
            r_dy += rows[i][j].red * kernel_dy[i][j];
        }
    }
    int b = floor(sqrt(square(b_dx) + square(b_dy)));
    int g = floor(sqrt(square(g_dx) + square(g_dy)));
    int r = floor(sqrt(square(r_dx) + square(r_dy)));

    int edge_val = max(r, max(g, b));
    Pixel new = {
        .blue = edge_val,
        .green = edge_val,
        .red = edge_val
    };

    return new;
}

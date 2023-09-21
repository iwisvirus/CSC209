#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_image(int num_rows, int num_cols, 
                int arr[num_rows][num_cols], FILE *fp);
                
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

// TODO add the function prototype for count_cells


int main(int argc, char **argv) {
    // Print a message to stderr and exit with an argument of 1
    // if there are not the right number of parameters, or if
    // there is a second argument that is not -p

    // The following snippet helps you check your arguments.
    // strcmp returns 0 if the strings are equal and
    // you haven't learned about this yet.
    // Notice that this is NOT COMPLETE, but we have given you this
    // much to show you how to check '-p' and print to stderr.
    // Be careful with the array indexing!
    if (strcmp(argv[2], "-p") != 0) {
        fprintf(stderr, "Usage: count_cells <imagefile.txt> [-p]\n");
        return 1;
    }

    // TODO read and print images, count cells

    return 0;
}

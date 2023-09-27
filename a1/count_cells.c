#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_image(int num_rows, int num_cols, 
                int arr[num_rows][num_cols], FILE *fp);
                
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

void group_cells(int num_rows, int num_cols, int arr[num_rows][num_cols], int rows, int cols, int colour);

void count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int main(int argc, char **argv) {

    /* Checking if it has the right number of parameters
     * If not, returning 1 with error message
     */
    if ((argc != 2) && (argc != 3)){
        fprintf(stderr, "Not the right number of parameters. \nEnter: count_cells <imagefile.txt> [-p]\n");
        return 1;

    }
    /* If there are 3 arguments, checking if the third argument is -p
     * If not -p, returning 1 with error message 
     */
    if (argc == 3){
        if (strcmp(argv[2], "-p") != 0) {
            fprintf(stderr, "Usage: count_cells <imagefile.txt> [-p]\n");
            return 1;
        }
    }

    // If the argument conditions are met 
    if ((argc == 3 && strcmp(argv[2], "-p") == 0) | (argc == 2)){

        // reading files from the first argument implemented 
        FILE *fp = fopen(argv[1], "r");

        // initializing number of rows and columns
        int num_rows;
        int num_cols;
 
        // scanning number of rows and columns of the image
        fscanf (fp, "%d %d", &num_rows, &num_cols);
    
        // creating array to print and read file
        int image[num_rows][num_cols];

        // reading images 
        read_image(num_rows, num_cols, image, fp);

        if (argc == 2){
            // counting cells 
            count_cells(num_rows, num_cols, image); 


        } else{
            // printing images and counting cells 
            print_image(num_rows, num_cols, image);
            count_cells(num_rows, num_cols, image);

    }
    // closing file 
    fclose(fp);


    }
    
    return 0;
}

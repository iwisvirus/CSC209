#include <stdio.h>
    
/* Reads the image from the open file fp into the two-dimensional array arr 
 * num_rows and num_cols specify the dimensions of arr
 */
void read_image(int num_rows, int num_cols, 
                int arr[num_rows][num_cols], FILE *fp) {

    int rows;
    int cols;

    for (rows = 0; rows < num_rows; rows++){
        for (cols = 0; cols < num_cols; cols++){
            fscanf(fp, "%d", &arr[rows][cols]);
        }
    }
}

/* Print to standard output the contents of the array arr */
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]) {

    int rows;
    int cols;

    for (rows = 0; rows < num_rows; rows++){
        for (cols = 0; cols < num_cols; cols++){
            printf("%d ", arr[rows][cols]);
        }
        printf("\n");
    }
}

/* Helper function for count_cells.
 * Grouping cells that are adjacent to each other 
 * paramater row and columns are the rows and columns
 * that it's checking to see if there are cells for count_cells.
 * Colour is identified by integers. 
 */
void group_cells(int num_rows, int num_cols, int arr[num_rows][num_cols], int rows, int cols, int colour){
   
    if(arr[rows][cols] == 255){
        arr[rows][cols] = colour;
            if (rows >= 1){
                group_cells(num_rows, num_cols, arr, rows-1, cols, colour);
                    }
            if (cols >= 1){
                group_cells(num_rows, num_cols, arr, rows, cols-1, colour);               
                    }
            if (rows + 1 < num_rows){
                group_cells(num_rows, num_cols, arr, rows+1, cols, colour);
                    }
            if (cols + 1 < num_cols){
                group_cells(num_rows, num_cols, arr, rows, cols+1, colour);
                    }
            }
    }
 
void count_cells(int num_rows, int num_cols, int arr[num_rows][num_cols]){

    int colour = 0;
    int rows;
    int cols;

    for (rows = 0; rows < num_rows; rows++){
        for (cols = 0; cols < num_cols; cols++){
            // checking if the cell has been coloured or if there are no cells
            if(arr[rows][cols] == 255){
                colour ++;           
                group_cells(num_rows, num_cols, arr, rows, cols, colour);
            }
        }
    }
        printf("Number of Cells is %d\n", colour);

    }


    



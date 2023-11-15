#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "bitmap.h"


#define ERROR_MESSAGE "Warning: one or more filter had an error, so the output image may not be correct.\n"
#define SUCCESS_MESSAGE "Image transformed successfully!\n"


/*
 * Check whether the given command is a valid image filter, and if so,
 * run the process.
 *
 * We've given you this function to illustrate the expected command-line
 * arguments for image_filter. No further error-checking is required for
 * the child processes.
 */
void run_command(const char *cmd) {
    if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "./copy") == 0 ||
        strcmp(cmd, "greyscale") == 0 || strcmp(cmd, "./greyscale") == 0 ||
        strcmp(cmd, "gaussian_blur") == 0 || strcmp(cmd, "./gaussian_blur") == 0 ||
        strcmp(cmd, "edge_detection") == 0 || strcmp(cmd, "./edge_detection") == 0) {
        execl(cmd, cmd, NULL);
    } else if (strncmp(cmd, "scale", 5) == 0) {
        // Note: the numeric argument starts at cmd[6]
        execl("scale", "scale", cmd + 6, NULL);
    } else if (strncmp(cmd, "./scale", 7) == 0) {
        // Note: the numeric argument starts at cmd[8]
        execl("./scale", "./scale", cmd + 8, NULL);
    } else {
        fprintf(stderr, "Invalid command '%s'\n", cmd);
        exit(1);
    }
}


int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: image_filter input output [filter ...]\n");
        exit(1);
    }

    // Creating pipes
    int filter[argc-3][2];
    for (int i = 0; i < argc-4; i++) {
        if (pipe(filter[i]) == -1) {
            perror("Piping failed.\n");
            exit(1);
        }
    }

    for (int i = 0; i < argc - 3; i++) {
        // creating forks
        int pid = fork();

        // error checking for forking
        if (pid == 0) { 
            // reading in from input file to first pipe
            if (i == 0){
                FILE *input_file = fopen(argv[1], "rb");
                if(dup2(fileno(input_file), fileno(stdin)) == -1){
                    perror("Failed to dup2 from input file to stdin.\n");
                    exit(1);
                };
                if(fclose(input_file) == EOF){
                    perror("Failed to close input file.\n");
                };

            // other child pipes     
            }else{
                if(dup2(filter[i-1][0], fileno(stdin)) == -1){
                    perror("Failed to dup2 from pipe to stdin.\n");
                    exit(1);
                };
                if(close(filter[i-1][0]) == -1){
                    perror("Failed to close writing end of pipe.\n");
                    exit(1);
                };
                if(close(filter[i-1][1]) == -1){
                    perror("Failed to close reading end of pipe.\n");
                    exit(1);
                };
            }
            // last child process writing to output file and to stdout
            if (i == argc - 4){
                FILE *output_file = fopen(argv[2], "wb");
                if(dup2(fileno(output_file), fileno(stdout)) == -1){
                    perror("Failed to dup2 from pipe to stdout.\n");
                    exit(1);
                };
                if(fclose(output_file) == -1){
                    perror("Failed to close output file.\n");
                    exit(1);
                };
            } else{
                if(dup2(filter[i][1], fileno(stdout)) == -1){
                    perror("Failed to dup2 from writing end to stdout.\n");
                    exit(1);
                };
                if(close(filter[i][0]) == -1){
                    perror("Failed to close reading end of pipe.\n");
                    exit(1);
                };
                if(close(filter[i][1]) == -1){
                    perror("Failed to close writing end of pipe.\n");
                    exit(1);
                };
            }
            run_command(argv[i+3]);
        } else if (pid < 0) {
            perror("Fork failed.\n");
            exit(1);
        }
        // child processes
    } 

    // Close all pipe ends in the parent process
    for (int i = 0; i < argc - 4; i++) {
        if(close(filter[i][0]) == -1){
            perror("Failed to close reading end of pipe.\n");
            exit(1);
        };
        if(close(filter[i][1]) == -1){
            perror("Failed to close writing end of pipe.\n");
            exit(1);
        };
    }

    for (int i = 0; i < argc - 3; ++i) {
        int status;
        if (wait(&status) == -1){
            perror("Failed to execute wait.\n");
            exit(1);
        } 
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            fprintf(stderr, ERROR_MESSAGE);
            exit(EXIT_FAILURE);
        }
    }

    printf(SUCCESS_MESSAGE);
    return 0;
}

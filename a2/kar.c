#include "kar_tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_help() {
    printf("Usage: kar [COMMAND] [ARCHIVE]\n");
    printf("Creates or extracts files from the kar-formatted ARCHIVE.\n");
    printf("\n");
    printf("COMMAND may be one of the following:\n");
    printf("    create [ARCHIVE] [FILES/DIRS]\n");
    printf("    extract [ARCHIVE]\n");
    printf("    --help\n");
    printf("\n");
    printf("create:\n");
    printf("    Creates an archive with the name specified in [ARCHIVE] consisting of the listed files and dirs. Paths to the files are not preserved, but the structure inside the archive will be.\n");
    printf("\n");
    printf("extract:\n");
    printf("    Extracts the files from the [ARCHIVE] file into the current directory.\n");
    printf("\n");
    printf("--help:\n");
    printf("    Prints this message and exits.\n");
}

int main(int argc, char *argv[]) {
    
    // error checking
    if (argc < 2) {
        print_help();
        exit(1);
    }

    char *call = argv[1];

    if (strcmp(call, "--help") == 0){
        print_help();
        exit(0);
    }
    char *archive_name = argv[2];
    int num_files = argc - 3;
    char **files_to_add = &argv[3];


    if (strcmp(call, "create") == 0){
        if (argc < 4){
            print_help();
            exit (0);
        }

        create_archive(archive_name, num_files, files_to_add);
    } else if (strcmp(call, "extract") == 0){
        if (argc != 3){
            print_help();
            exit(0);
        }
        extract_archive(archive_name);
    } else {
        print_help();
        exit(0);
    }

    return 0;
}

#include "kar_tree.h"
#include <stdio.h>
#include <string.h>

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

    // We've given you the usage string
    // main should only need to check arguments and
    // call the appropriate functions

    print_help();

    return 0;
}

/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_tree(arch_tree_node *root) {
    // TODO: This needs to be implemented
    printf("free_tree not implemented\n");
}


/*
** Some helper functions so that you don't get
** bogged down by formatting paths correctly.
*/
void only_filename(char* filepath){
    int total_chars = strlen(filepath);

    if(filepath[total_chars - 1] == '/'){
        filepath[total_chars - 1] = '\0';
    }

    char* filename = strrchr(filepath, '/');
    for(size_t i = 0; i <= strlen(filename); i++){
        filepath[i] = filename[i + 1];
    }
}

// WARNING: This function assumes that path_buffer is large enough
void valid_path(char path_buffer[], char *directory, char *filename){
    strcpy(path_buffer, directory);

    // If the directory doesn't end with a slash, add one
    if (strlen(path_buffer) > 0){
        if (path_buffer[strlen(path_buffer) - 1] != '/'){
            strcat(path_buffer, "/");
        }
    }
    strcat(path_buffer, filename);
}
// WARNING

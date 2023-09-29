/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>


arch_tree_node* create_tree_node(char *filepath){
    // Get the file's metadata
    struct stat st;
    if (stat(filepath, &st) == -1) {
        perror("Failed to get file metadata");
        return NULL;
    }

    // TODO create the node

    // return node;
    return NULL;
}


int create_archive(char *archive_name, int num_files, char *files_to_add[num_files]){

    // Create the archive file
    FILE *archive = fopen(archive_name, "w");
    if (!archive) {
        perror("Failed to open archive file");
        return 1;
    }

    kar_tree root;
    arch_tree_node **current = &root.root;
    current = &root.root;

    // TODO Build the tree and write the archive
    // you may implement this as you see fit, but
    // you must use the structures given to you
    // and build the datastructure that we have asked

    free_tree(root.root);
    return 0;
}

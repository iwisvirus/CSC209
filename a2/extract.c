/*
** This file is part of the CSC209 -- Fall 2023 Assignment 2
**
** All of the files in this directory and all subdirectories are:
** Copyright (c) 2023 Demetres Kostas
*/

#include "kar_tree.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


// This function is provided to help you get started
// You are not required to use it.
arch_tree_node *read_node(FILE *archive){
    arch_tree_node *node = malloc(sizeof(arch_tree_node));
    if (!node) {
        perror("Failed to allocate memory for node");
        return NULL;
    }

    fread(node, sizeof(arch_tree_node), 1, archive);
    return node;
}


int extract_archive(char *archive_name){
    FILE* archive = fopen(archive_name, "r");
    if (!archive) {
        perror("Failed to open archive.\n");
        return 1;
    }

    kar_tree root;
    arch_tree_node **current = &root.root;
    current = &root.root;

    // Read the data structures from the archive
    // such that you can re-create the archived files

    free_tree(root.root);
    return 0;
}

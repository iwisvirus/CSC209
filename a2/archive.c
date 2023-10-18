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
#include <string.h>

arch_tree_node* create_tree_node(char *filepath){
    // Get the file's metadata
    struct stat st;
    if (stat(filepath, &st) == -1) {
        perror("Failed to get file metadata");
        return NULL;
    }

    arch_tree_node *node = malloc(sizeof(arch_tree_node));
    if (node == NULL){
        perror("Memory not allocated");
        return NULL;
    }
    only_filename(filepath);
    strncpy(node -> name, filepath, FILENAME_MAX);
    node -> size = st.st_size;
    node -> is_directory = S_ISDIR(st.st_mode);
    node -> next_file = NULL;
    node -> dir_contents = NULL;

    return node;
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
    
    for (int i = 0; i < num_files; i++){
        int a = recursively_add(files_to_add[i], current, archive_name);
        if (a != 0){
            exit(1);
        }
    }
    free_tree(root.root);
    fclose(archive);
    return 0;
}
int recursively_add(char *path, arch_tree_node **current, char *archive){
    // only_filename(path);
    arch_tree_node *node = create_tree_node(path);

    if(node -> is_directory){
        DIR* dir = opendir(path);
        struct dirent* stuff = readdir(dir);
        while (stuff != 0x0){
            if(strcmp(stuff -> d_name, ".") == 0 || strcmp(stuff -> d_name, "..")== 0){
                continue;
            }
            char full_path[FILENAME_MAX];
            snprintf(full_path, FILENAME_MAX, "%s/%s", path, stuff->d_name);
            recursively_add(full_path, &(node -> dir_contents), archive);
        }
        closedir(dir);
    }else{
        FILE *text = fopen(path, "r");

        fseek(text, 0, SEEK_END);
        long size = ftell(text);
        fseek(text, 0, SEEK_SET);

        char *stuff = (char*) malloc(size);
            if (stuff == NULL){
                perror("Memory not allocated");
                exit(0);
            }
        fread(stuff, 1, size, text);
        fclose(text);

        FILE *file = fopen(archive, "ab");
        fwrite(stuff, 1, size, file);
        

        if (*current == NULL){
            *current = node;
        }else {
            (*current) -> next_file = node;
        }
        free(stuff);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matlab_file_list.h"

#define INITIAL_FILE_CAPACITY 10

void init_file_list(File_list *list) {
    list->files = malloc(INITIAL_FILE_CAPACITY * sizeof(Matlab_file *));
    if (!list->files) {
        fprintf(stderr, "Initial file memory allocation failed.\n");
        return;
    }
    list->count = 0;
    list->capacity = INITIAL_FILE_CAPACITY;
}

void free_matlab_file(Matlab_file *file) {
    free(file->content);
    free(file->file_name);
    free(file);
}

void free_file_list(File_list *list) {
    for (size_t i = 0; i < list->count; ++i) {
        free_matlab_file(list->files[i]);
    }
    free(list->files);
}

static int grow_file_list(File_list *list) {
    Matlab_file **larger_list = realloc(list->files, list->capacity * 2 * sizeof(Matlab_file *));
    if (!larger_list) {
        return -1;
    } 
    list->files = larger_list;
    list->capacity *= 2;
    return 0;
}

int add_matlab_file(Matlab_file *file, File_list *list) {
    if (list->count >= list->capacity) {
        if (grow_file_list(list) != 0) {
            return -1;
        }
    }
    list->files[list->count] = file;
    list->count++;
    return 0;
}
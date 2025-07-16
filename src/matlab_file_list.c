#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matlab_file_list.h"

#define INITIAL_FILE_CAPACITY 10

void init_file_list(File_list *list) {
    list->files = malloc(INITIAL_FILE_CAPACITY * sizeof(Matlab_file));
    if (!list->files) {
        fprintf(stderr, "Initial file memory allocation failed.\n");
        return;
    }
    list->count = 0;
    list->capacity = INITIAL_FILE_CAPACITY;
}

void free_file_list(File_list *list) {
    for (size_t i = 0; i < list->count; ++i) {
        free(list->files[i].file_name);
        free(list->files[i].content);
    }
    free(list->files);
}

static void grow_file_list(File_list *list) {
    list->capacity *= 2;
    list->files = realloc(list->files, list->capacity * sizeof(Matlab_file));
}

// because strcpy() is not available on windows
static char* string_duplicate(const char *source) {
    size_t length = strlen(source);
    char *copy = malloc(length + 1);
    if (copy) {
        memcpy(copy, source, length + 1);
    }
    return copy;
}

int add_matlab_file(File_list *list, char *file_name, char *content) {
    if (list->count >= list->capacity) {
        grow_file_list(list);
    }

    Matlab_file *file = &list->files[list->count];
    file->file_name = string_duplicate(file_name);
    file->content = content;

    if (!file->content) {
        free(file->file_name);
        return -1;
    }

    list->count++;
    return 0;
}
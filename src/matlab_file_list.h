#ifndef MATLAB_FILE_LIST_H
#define MATLAB_FILE_LIST_H

#include <stddef.h>

/*
struct to store Matlab files and
dynamic list that store the Matlab files
*/

typedef struct {
    char *file_name;
    char *content;
} Matlab_file;

typedef struct {
    Matlab_file **files;
    size_t count;
    size_t capacity;
} File_list;

void init_file_list(File_list *list);
void free_file_list(File_list *list);
int add_matlab_file(Matlab_file *file, File_list *list);

#endif
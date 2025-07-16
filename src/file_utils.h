#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>

#include "matlab_file_list.h"
#include "smells.h"

/*
loads all .m files from a given path into a dynamic
file list (matlab_file_list.h)
*/
int load_files(const char *path, File_list *list);

/*
exports the Smell List to output.csv
*/
void smell_list_to_CSV(Smell_list *list);

#endif

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>

#include "matlab_file_list.h"
#include "smell_list.h"
#include "detector_registry.h"

/*
loads all .m files from a given path into a dynamic
file list (matlab_file_list.h)
*/
int load_files(const char *path, File_list *list);

int load_config(const char *file_name, Smell_detector **detectors, size_t detector_count);

void smell_lists_to_CSV();

#endif

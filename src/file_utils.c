#include "tinydir.h"
#include "file_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #define PATH_SEPARATOR '\\'
#else
    #define PATH_SEPARATOR '/'
#endif

// is this the right way of defining constants?
#define INITIAL_FILE_CAPACITY 10
#define MAX_PATH_LENGTH 4096

static int has_m_extension(const char* file_name) {
    const char* file_extension = strrchr(file_name, '.');
    return file_extension && strcmp(file_extension, ".m") == 0;
}

Matlab_file *read_file(const char *file_path) {
    FILE *file = fopen(file_path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);

    if (length <= 0) {
        fclose(file);
        return NULL;
    }

    // reset file position
    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    if (fread(buffer, 1, length, file) != (size_t)length) {
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);

    Matlab_file *matlab_file = malloc(sizeof(Matlab_file));
    if (!matlab_file) {
        free(buffer);
        return NULL;
    } 

    size_t path_length = strlen(file_path);
    char *path = malloc(path_length+1);
    if (!path) {
        free(buffer);
        free(matlab_file);
        return NULL;
    }
    memcpy(path, file_path, path_length + 1);

    matlab_file->content = buffer;
    matlab_file->file_name = path;

    return matlab_file;
}

// TODO: - add some kind of limit to search depth,
// - add error check for path construction
int load_files(const char *path, File_list *list) {
    tinydir_dir dir;
    if (tinydir_open(&dir, path) == -1) {
        perror("tinydir_open");
        return -1;
    }

    while (dir.has_next) {
        tinydir_file file;
        if (tinydir_readfile(&dir, &file) == -1) {
            tinydir_next(&dir);
            continue;
        }
        if (file.is_dir) {
            // if directory isn't current or parent directory, then recursive call
            if (strcmp(file.name, ".") != 0 && strcmp(file.name, "..") != 0) {
                char subpath[MAX_PATH_LENGTH];
                snprintf(subpath, sizeof(subpath), "%s%c%s", path, PATH_SEPARATOR, file.name);
                load_files(subpath, list);
            }
        } else if (has_m_extension(file.name)) {
            char file_path[MAX_PATH_LENGTH];
            snprintf(file_path, sizeof(file_path), "%s%c%s", path, PATH_SEPARATOR, file.name);

            Matlab_file *matlab_file = read_file(file_path);
            if (!matlab_file) {
                fprintf(stderr, "Failed to read file %s.\n", file_path);
                tinydir_next(&dir);
                continue;
            }
            
            if (add_matlab_file(matlab_file, list) != 0) {
                fprintf(stderr, "Failed to allocate additional memory for file list.\n");
            }
            
        }
        tinydir_next(&dir);
    }

    tinydir_close(&dir);
    return 0;
}

void write_smell_csv_row(FILE *file, const Smell *smell) {
    fprintf(file, "%s,\"%s\",%d",
            smell_type_to_str(smell->type),
            smell->location.file_name,
            smell->location.line);

    for (size_t violation_i = 0; violation_i < MAX_VIOLATIONS; ++violation_i) {
        if (violation_i < smell->violation_count) {
            const Violation *violation = &smell->violations[violation_i];
            fprintf(file, ",%s,%d,%d",
                    violation_type_to_str(violation->type),
                    violation->actual_value,
                    violation->threshold_value);
        } else {
            fprintf(file, ",,,");
        }
    }
    fprintf(file, "\n");
}

void smell_list_to_CSV(Smell_list *list) {
    if (!list || list->count == 0) return;

    FILE *file = fopen("output.csv", "w");
    if (!file) {
        perror("output.csv");
        return;
    }

    fprintf(file, "smell_type,file_name,line");
    for (int violation_i = 0; violation_i < MAX_VIOLATIONS; ++violation_i) {
        fprintf(file, ",violation%d_type,violation%d_actual,violation%d_threshold",
                violation_i + 1, violation_i + 1, violation_i + 1);
    }
    fprintf(file, "\n");

    for (size_t smell_i = 0; smell_i < list->count; ++smell_i) {
        write_smell_csv_row(file, &list->smells[smell_i]);
    }

    fclose(file);
}
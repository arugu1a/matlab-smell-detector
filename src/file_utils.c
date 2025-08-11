#include "tinydir.h"
#include "file_utils.h"
#include "detector_registry.h"

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
#define NUM_SMELLS 3

static int has_m_extension(const char* file_name) {
    const char* file_extension = strrchr(file_name, '.');
    return file_extension && strcmp(file_extension, ".m") == 0;
}

int load_config(const char* file_name, Smell_detector **detectors, size_t detector_count) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open config file.\n");
        return -1;
    }
    
    char line[256];
    char current_section[128] = "";
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        if (line[0] == '[' && line[strlen(line)-1] == ']') {
            strncpy(current_section, line + 1, sizeof(current_section) - 1);
            current_section[strlen(current_section) - 1] = '\0';
            continue;
        }
        
        // split the line into key and value by replacing '=' with '\0'
        char* equals = strchr(line, '=');
        if (!equals) continue;
        *equals = '\0';
        char* key = line;
        char* value = equals + 1;

        for (size_t i = 0; i < detector_count; i++) {
            if (strcmp(detectors[i]->name, current_section) == 0) {
                for (size_t j = 0; j < detectors[i]->config_count; j++) {
                    Configuration *config = &detectors[i]->configs[j];
                    
                    if (strcmp(config->key_absolute, key) == 0) {
                        if (config->absolute_is_float) {
                            config->absolute_value.float_absolute = strtof(value, NULL);
                        } else {
                            config->absolute_value.int_absolute = strtol(value, NULL, 10);
                        }
                        break;
                    }
                    else if (strcmp(config->key_percentage, key) == 0) {
                        config->percentage_value = strtof(value, NULL);
                        break;
                    }
                    else if (strcmp(config->key_use_percentage, key) == 0) {
                        config->use_percentage = strtol(value, NULL, 10);
                        break;
                    }
                }
                break;
            }
        }
    }
    fclose(file);
    return 0;
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
    tinydir_file file;
    if (tinydir_file_open(&file, path) == -1) {
        perror("tinydir_file_open");
        return -1;
    }

    // --- base case: path is file ---
    if (!file.is_dir) {
        if (has_m_extension(file.name)) {
            Matlab_file *matlab_file = read_file(path);
            if (!matlab_file) {
                fprintf(stderr, "Failed to read file %s.\n", path);
                return -1;
            }

            if (add_matlab_file(matlab_file, list) != 0) {
                fprintf(stderr, "Failed to allocate additional memory for file list.\n");
            }
        }
        return 0;
    }

    // --- recursive case: path is directory ---
    tinydir_dir dir;
    if (tinydir_open(&dir, path) == -1) {
        perror("tinydir_open");
        return -1;
    }

    while (dir.has_next) {
        tinydir_file child;
        if (tinydir_readfile(&dir, &child) == -1) {
            tinydir_next(&dir);
            continue;
        }

        if (strcmp(child.name, ".") != 0 && strcmp(child.name, "..") != 0) {
            char child_path[MAX_PATH_LENGTH];
            snprintf(child_path, sizeof(child_path), "%s%c%s", path, PATH_SEPARATOR, child.name);
            load_files(child_path, list); // recursive call
        }

        tinydir_next(&dir);
    }

    tinydir_close(&dir);
    return 0;
}

static void write_smell_csv_row(FILE *file, const Smell *smell, const char *detector_name) {
    fprintf(file, "%s,\"%s\",%d",
            detector_name,
            smell->location.file_name,
            smell->location.line);
    for (size_t metric_i = 0; metric_i < MAX_METRICS; ++metric_i) {
        if (metric_i < smell->metric_count) {
            const Metric *metric = &smell->metrics[metric_i];
            if (metric->is_float) {
                fprintf(file, ",%s,%.2f",
                        metric->name,
                        metric->measured_value.float_value);
            } else {
                fprintf(file, ",%s,%d",
                        metric->name,
                        metric->measured_value.int_value);
            }
        } else {
            fprintf(file, ",,");
        }
    }
    fprintf(file, "\n");
}

static void single_list_to_CSV(FILE *file, Smell_list *list, const char *detector_name) {
    for (size_t smell_i = 0; smell_i < list->count; ++smell_i) {
        write_smell_csv_row(file, &list->smells[smell_i], detector_name);
    }
}

void smell_lists_to_CSV() {
    FILE *file = fopen("output.csv", "w");
    if (!file) {
        perror("output.csv");
        return;
    }
    fprintf(file, "smell_type,file_name,line");
    for (int metric_i = 0; metric_i < MAX_METRICS; ++metric_i) {
        fprintf(file, ",metric%d_name,metric%d_measured_value",
                metric_i + 1, metric_i + 1);
    }
    fprintf(file, "\n");
    
    for (size_t list_i = 0; list_i < detector_count; ++list_i) {
        single_list_to_CSV(file, detectors[list_i]->smell_list, detectors[list_i]->name);
    }
    fclose(file);
}
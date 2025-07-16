#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


Config global_config = {0};

int load_config(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open config file.\n");
        return -1;
    }
    
    char line[256];
    char current_section[128] = "";
    
    while (fgets(line, sizeof(line), file)) {

        // replace newline with 0
        line[strcspn(line, "\n")] = '\0';
        // skip comments and empty lines
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        
        // identify and store current section, then go to next line
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

        // depending on the current section, check if key matches, then define
        if (strcmp(current_section, "long_function") == 0) {
            if (strcmp(key, "min_LOC") == 0) {
                global_config.long_function.min_LOC = strtol(value,NULL,10);
            }
        }
        else if (strcmp(current_section, "long_parameter_list") == 0) {
            if (strcmp(key, "min_n_params") == 0) {
                global_config.long_parameter_list.min_n_params = strtol(value,NULL,10);
            }
        }
    }
    fclose(file);
    return 0;
}

void print_config() {
    printf("------\n");
    printf("current configuration :\n");
    printf("Long Function min_LOC: %d\n", global_config.long_function.min_LOC);
    printf("Long Parameter List min_n_params: %d\n", global_config.long_parameter_list.min_n_params);
    printf("------\n");
}
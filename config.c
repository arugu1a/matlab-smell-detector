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
    
    // arbitrary value
    // TODO: define constant
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

        if (strcmp(current_section, "long_function") == 0) {
            if (strcmp(key, "absolute_LOC") == 0) {
                global_config.long_function.absolute_LOC = strtol(value, NULL, 10);
            } else if (strcmp(key, "top_percentage_LOC") == 0) {
                global_config.long_function.top_percentage_LOC = strtof(value, NULL);
            } else if (strcmp(key, "use_percentage") == 0) {
                global_config.long_function.use_percentage = strtol(value, NULL, 10);
            }
        }
        else if (strcmp(current_section, "long_parameter_list") == 0) {
            if (strcmp(key, "absolute_param_count") == 0) {
                global_config.long_parameter_list.absolute_param_count = strtol(value, NULL, 10);
            } else if (strcmp(key, "top_percentage_param_count") == 0) {
                global_config.long_parameter_list.top_percentage_param_count = strtof(value, NULL);
            } else if (strcmp(key, "use_percentage") == 0) {
                global_config.long_parameter_list.use_percentage = strtol(value, NULL, 10);
            }
        }
        else if (strcmp(current_section, "god_class") == 0) {
            if (strcmp(key, "absolute_wmc") == 0) {
                global_config.god_class.absolute_wmc = strtol(value, NULL, 10);
            } else if (strcmp(key, "top_percentage_wmc") == 0) {
                global_config.god_class.top_percentage_wmc = strtof(value, NULL);
            } else if (strcmp(key, "use_percentage_wmc") == 0) {
                global_config.god_class.use_percentage_wmc = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "absolute_tcc") == 0) {
                global_config.god_class.absolute_tcc = strtof(value, NULL);
            } else if (strcmp(key, "bottom_percentage_tcc") == 0) {
                global_config.god_class.bottom_percentage_tcc = strtof(value, NULL);
            } else if (strcmp(key, "use_percentage_tcc") == 0) {
                global_config.god_class.use_percentage_tcc = strtol(value, NULL, 10);
            }
            else if (strcmp(key, "absolute_atfd") == 0) {
                global_config.god_class.absolute_atfd = strtol(value, NULL, 10);
            } else if (strcmp(key, "top_percentage_atfd") == 0) {
                global_config.god_class.top_percentage_atfd = strtof(value, NULL);
            } else if (strcmp(key, "use_percentage_atfd") == 0) {
                global_config.god_class.use_percentage_atfd = strtol(value, NULL, 10);
            }
        }
    }
    fclose(file);
    return 0;
}

void print_config() {
    printf("------\n");
    printf("current configuration:\n");
    printf("\n");
    printf("long_function:\n");
    printf("use_percentage: %d\n", global_config.long_function.use_percentage);
    printf("absolute_LOC: %d\n", global_config.long_function.absolute_LOC);
    printf("top_percentage_LOC: %.2f\n", global_config.long_function.top_percentage_LOC);

    printf("long_parameter_list:\n");
    printf("use_percentage: %d\n", global_config.long_parameter_list.use_percentage);
    printf("absolute_param_count: %d\n", global_config.long_parameter_list.absolute_param_count);
    printf("top_percentage_param_count: %.2f\n", global_config.long_parameter_list.top_percentage_param_count);

    printf("god_class:\n");
    printf("use_percentage_wmc: %d\n", global_config.god_class.use_percentage_wmc);
    printf("absolute_wmc: %d\n", global_config.god_class.absolute_wmc);
    printf("top_percentage_wmc: %.2f\n", global_config.god_class.top_percentage_wmc);

    printf("use_percentage_tcc: %d\n", global_config.god_class.use_percentage_tcc);
    printf("absolute_tcc: %.2f\n", global_config.god_class.absolute_tcc);
    printf("bottom_percentage_tcc: %.2f\n", global_config.god_class.bottom_percentage_tcc);

    printf("use_percentage_atfd: %d\n", global_config.god_class.use_percentage_atfd);
    printf("absolute_atfd: %d\n", global_config.god_class.absolute_atfd);
    printf("top_percentage_atfd: %.2f\n", global_config.god_class.top_percentage_atfd);

    printf("------\n");
}

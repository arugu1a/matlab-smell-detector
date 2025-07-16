#ifndef CONFIG_H
#define CONFIG_H

/*
data structures to store the smell thresholds 
for different smells
*/

typedef struct {
    int min_LOC;
} Long_function;

typedef struct {
    int min_n_params;
} Long_parameter_list;

typedef struct {
    Long_function long_function;
    Long_parameter_list long_parameter_list;
} Config;

extern Config global_config;

// loads thresholds into global Config struct
// from specified file (in our case only config.ini)
int load_config(const char* file_name);

void print_config();

#endif
#ifndef CONFIG_H
#define CONFIG_H

/*
data structures to store the smell thresholds 
for different smells
*/

typedef struct {
    int absolute_LOC;
    float top_percentage_LOC;
    int use_percentage;
} Long_function;

typedef struct {
    int absolute_param_count;
    float top_percentage_param_count;
    int use_percentage;
} Long_parameter_list;

typedef struct {
    int absolute_wmc;
    float top_percentage_wmc;
    int use_percentage_wmc;

    float absolute_tcc;
    float bottom_percentage_tcc;
    int use_percentage_tcc;

    int absolute_atfd;
    float top_percentage_atfd;
    int use_percentage_atfd;
} God_class;

typedef struct {
    Long_function long_function;
    Long_parameter_list long_parameter_list;
    God_class god_class;
} Config;

extern Config global_config;

// loads thresholds into global Config struct
// from specified file (in our case only config.ini)
int load_config(const char* file_name);

void print_config();

#endif

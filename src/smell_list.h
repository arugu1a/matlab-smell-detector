#ifndef SMELL_LIST_H
#define SMELL_LIST_H

#include "tree_sitter/api.h"
#include <stddef.h>

#include "matlab_file_list.h"

//maximum amount of thresholds any kind of smell has
// this should be dynamic based on the detector
#define MAX_METRICS 3

/*
data structures to store found smells and store them
in a dynamic list 
*/

typedef struct {
    char *file_name;
    uint32_t line;
} Smell_location;

/*
TODO: actual value currently unused, designed to
evaluate severity of a smell (difference between
values)

TODO: threshold value is always taken from global_config
-> maybe there is some better way to refrence this
information / storage might be unneccessary
    -> metric type could be enough
*/
typedef struct {
    const char *name;
    union {
        float float_value;
        int int_value;
    } measured_value;
    int is_float;
} Metric;

typedef struct {
    Smell_location location;
    Metric metrics[MAX_METRICS];
    size_t metric_count;
} Smell;

typedef struct {
    Smell *smells;
    size_t capacity;
    size_t count;
} Smell_list;

// language function declaration
// maybe put it somewhere else
TSLanguage *tree_sitter_matlab(void);

Smell_location create_location(char *file_name, uint32_t line);

Metric create_int_metric(const char *name, uint32_t measured_value);
Metric create_float_metric(const char *name, float measured_value);
void add_metric(Smell *candidate, Metric metric);

Smell *create_smell(Smell_location location);
void add_smell_to_list(Smell_list *list, Smell smell);

void init_smell_list(Smell_list *list);
void free_smell_list(Smell_list *list);

void print_smell_list(Smell_list *list);

#endif

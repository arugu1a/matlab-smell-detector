#ifndef SMELLS_H
#define SMELLS_H

#include "tree_sitter/api.h"
#include <stddef.h>

#include "matlab_file_list.h"

//maximum amount of thresholds any kind of smell has
#define MAX_VIOLATIONS 5 

/*
data structures to store found smells and store them
in a dynamic list 
*/
typedef enum {
    VIOLATION_LOC,
    VIOLATION_N_PARAMS,
} Violation_type;

typedef enum {
    LONG_FUNCTION,
    LONG_PARAMETER_LIST,
    GOD_CLASS
} Smell_type;

typedef struct {
    char *file_name;
    uint32_t line;
} Smell_location;

/*
violation of a smell threshold, stores the
threshold value and the actual value 

TODO: actual value currently unused, designed to
evaluate severity of a smell (difference between
values)

TODO: actual value is always taken from global_config
-> maybe there is some better way to refrence this
information / storage might be unneccessary
    -> violation type could be enough
*/
typedef struct {
    Violation_type type;
    int actual_value;
    int threshold_value;
} Violation;

typedef struct {
    Smell_type type;
    Smell_location location;
    Violation violations[MAX_VIOLATIONS];
    size_t violation_count;
} Smell;

typedef struct {
    Smell *smells;
    size_t capacity;
    size_t count;
} Smell_list;

// language function declaration
TSLanguage *tree_sitter_matlab(void);

// allocates memory
void init_smell_list(Smell_list *list);

/*
builds AST and performs smell search for every file in
the file list, every found smell is stored in the
smell list
*/
void smell_search(File_list *file_list, Smell_list *smell_list);

// to print enum values into csv
const char* smell_type_to_str(Smell_type t);
const char* violation_type_to_str(Violation_type t);

void print_smell_list(Smell_list *list);
void free_smell_list(Smell_list *list);

#endif

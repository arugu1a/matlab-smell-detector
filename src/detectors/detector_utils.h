#ifndef DETECTOR_UTILS_H
#define DETECTOR_UTILS_H

#include "tree_sitter/api.h"

extern TSLanguage *tree_sitter_matlab(void);

typedef struct {
    char **items;
    int count;
    int capacity;
} StringList;

StringList* create_string_list(void);
void add_to_string_list(StringList *list, const char *str);
int string_list_contains(StringList *list, const char *str);
int string_list_index_of(StringList *list, const char *str);
void free_string_list(StringList *list);

char* extract_first_parameter(TSNode method_node, const char* source_code);
char* extract_class_name(TSNode class_node, const char* source_code);
int is_static_methods_block(TSNode methods_node, const char* source_code);
char* get_node_text(TSNode node, const char *source_code);
int count_methods(TSNode node);

#endif
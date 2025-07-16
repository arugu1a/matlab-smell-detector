#include "file_utils.h"
#include "matlab_file_list.h"
#include "smells.h"
#include "config.h"

#include "tree_sitter/api.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SMELL_CAPACITY 32

//TODO: define static functions

/*
TODO: refactor smell data strucutres and their
functions into a seperate file
-> this file is only getting larger!
*/  

/*
TODO: !duplicate code in the smell search functions:
maybe it's possible to move this to separate functions
*/

static Smell_location create_location(char *file_name, uint32_t line) {
    Smell_location location;
    location.file_name = file_name; //copy pointer, points to file_name in the file_list
    location.line = line;
    return location;
}

static Violation create_violation(Violation_type type, uint32_t actual_value, int threshold_value) {
    Violation violation;
    violation.type = type;
    violation.actual_value = actual_value;
    violation.threshold_value = threshold_value;
    return violation;
}

static Smell *create_smell(Smell_type type, Smell_location location) {
    Smell *smell = malloc(sizeof(Smell));
    if (!smell) return NULL;
    smell->type = type;
    smell->location = location;
    smell->violation_count = 0;
    return smell;
}

void init_smell_list(Smell_list *list) {
    list->smells = malloc(INITIAL_SMELL_CAPACITY * sizeof(Smell));
    if (!list->smells) {
        fprintf(stderr, "Initial smell memory allocation failed.\n");
        return;
    }
    list->capacity = INITIAL_SMELL_CAPACITY;
    list->count = 0;
}

void add_smell_to_list(Smell_list *list, Smell smell) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity*2;
        list->smells = realloc(list->smells, new_capacity*sizeof(Smell));
        if (!list->smells) {
            fprintf(stderr, "Failed to allocate memory for new smells.\n");
            return;
        }
        list->capacity = new_capacity;
    }
    list->smells[list->count] = smell;
    list->count = list->count + 1; 
}


void add_violation(Smell *smell, Violation violation) {
    smell->violations[smell->violation_count] = violation;
    smell->violation_count = smell->violation_count + 1;
}


void find_long_functions(TSNode root_node, char *file_name, Smell_list *list) {
    const char* query_string = "(function_definition) @function";
    
    uint32_t error_offset;
    TSQueryError error_type;
    
    TSQuery* query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "find_long_functions: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return;
    }

    TSQueryCursor* cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, root_node);

    TSQueryMatch match;

    while (ts_query_cursor_next_match(cursor, &match)) {
        TSNode function_node = match.captures[0].node;

        TSPoint start = ts_node_start_point(function_node);
        TSPoint end = ts_node_end_point(function_node);
        uint32_t line_count = end.row - start.row + 1;

        if (line_count >= global_config.long_function.min_LOC) { 
            Smell_location location = create_location(file_name, start.row + 1);
            Smell *smell = create_smell(LONG_FUNCTION, location);
            if (!smell) continue;

            Violation violation = create_violation(VIOLATION_LOC, line_count, 
                                                    global_config.long_function.min_LOC);
            smell->violations[0] = violation;
            smell->violation_count = 1;

            add_smell_to_list(list, *smell);
            free(smell);
        }
    }

    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
}

void find_long_parameter_lists(TSNode root_node, char *file_name, Smell_list *list) {
    const char *query_string = "(function_arguments) @params";

    TSQueryError error_type;
    uint32_t error_offset;

    TSQuery *query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "find_long_parameter_list: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return;
    }

    TSQueryCursor *query_cursor = ts_query_cursor_new();
    ts_query_cursor_exec(query_cursor, query, root_node);

    TSQueryMatch match;

    while (ts_query_cursor_next_match(query_cursor, &match)) {
        TSNode params = match.captures[0].node;

        uint32_t parameter_count = ts_node_named_child_count(params);
        if (parameter_count > global_config.long_parameter_list.min_n_params) {
            Smell_location location = create_location(file_name,
                                                        ts_node_start_point(params).row + 1);
            Smell *smell = create_smell(LONG_PARAMETER_LIST, location);
            if (!smell) continue;

            Violation violation = create_violation(VIOLATION_N_PARAMS, parameter_count,
                                                    global_config.long_parameter_list.min_n_params);
            smell->violations[0] = violation;
            smell->violation_count = 1;
            add_smell_to_list(list, *smell);
            free(smell);
        }
    }

    ts_query_cursor_delete(query_cursor);
    ts_query_delete(query);
}

static void file_smell_search(TSParser *parser, Matlab_file file, Smell_list *list) {
    char *source = file.content;
    TSTree *tree = ts_parser_parse_string(parser, NULL, source, strlen(source));
    TSNode root_node = ts_tree_root_node(tree);

    // printf("%s", ts_node_string(root_node)); // only for debugging
    // printf("when doing the search, file_name: %s\n", file.file_name);
    find_long_functions(root_node, file.file_name, list);
    find_long_parameter_lists(root_node, file.file_name, list);

    ts_tree_delete(tree);
}

void smell_search(File_list *file_list, Smell_list *smell_list) {
    // create parser and set language
    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_matlab());
    for (size_t file_i = 0; file_i < file_list->count; ++file_i) {
        file_smell_search(parser, file_list->files[file_i], smell_list);
    }
    ts_parser_delete(parser);
}

const char* smell_type_to_str(Smell_type type) {
    switch (type) {
        case LONG_FUNCTION: return "LONG_FUNCTION";
        case LONG_PARAMETER_LIST: return "LONG_PARAMETER_LIST";
        case GOD_CLASS: return "GOD_CLASS";
    }
}

const char* violation_type_to_str(Violation_type type) {
    switch (type) {
        case VIOLATION_LOC: return "VIOLATION_LOC";
        case VIOLATION_N_PARAMS: return "VIOLATION_N_PARAMS";
    }
}

void print_smell_list(Smell_list *list) {
    printf("\n");
    printf("Smell List:\n");
    for (size_t smell_i = 0; smell_i < list->count; ++smell_i) {
        Smell current_smell = list->smells[smell_i];
        printf("-------------\n");
        printf("Type: %s\n",smell_type_to_str(current_smell.type));
        printf("File: %s Line: %d\n",current_smell.location.file_name, current_smell.location.line);
        for (size_t violation_i = 0; violation_i < current_smell.violation_count; ++violation_i) {
            Violation current_violation = current_smell.violations[violation_i];
            printf("Violation %zu: Type: %s, Threshold: %d, Actual_value: %d\n",
                    violation_i, violation_type_to_str(current_violation.type),
                    current_violation.threshold_value, current_violation.actual_value);
        }
    }
}

void free_smell_list(Smell_list *list) {
    // list->smells[i].location.file_name belongs to file list,
    // gets freed in free_file_list(File_list *list) in file_utils.c
    free(list->smells);
    list->smells = NULL;
    list->capacity = 0;
    list->count = 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_sitter/api.h"
#include "detector_utils.h"
#include "tcc.h"

static void free_access_matrix(int **matrix, int max_methods) {
    if (!matrix) return;
    
    for (int method_i = 0; method_i < max_methods; ++method_i) {
        free(matrix[method_i]);
    }
    free(matrix);
}

static StringList* collect_class_properties(TSNode class_node, const char *source_code) {
    uint32_t error_offset;
    TSQueryError error_type;

    const char *properties_query_src = "(properties (property name: (identifier) @property))";
    TSQuery *properties_query = ts_query_new(tree_sitter_matlab(), properties_query_src,
                                            strlen(properties_query_src), &error_offset, &error_type);
    if (!properties_query) {
        fprintf(stderr, "TCC: Properties query error at %u: %d\n", error_offset, error_type);
        return NULL;
    }

    StringList *properties = create_string_list();
    if (!properties) {
        fprintf(stderr, "TCC: Failed to create properties list.\n");
        ts_query_delete(properties_query);
        return NULL;
    }

    TSQueryCursor *property_cursor = ts_query_cursor_new();
    ts_query_cursor_exec(property_cursor, properties_query, class_node);

    TSQueryMatch match;
    while (ts_query_cursor_next_match(property_cursor, &match)) {
        if (match.capture_count > 0) {
            char *property = get_node_text(match.captures[0].node, source_code);
            if (property) {
                add_to_string_list(properties, property);
                free(property);
            }
        }
    }
    ts_query_cursor_delete(property_cursor);
    ts_query_delete(properties_query);

    return properties;
}

static int** build_method_property_matrix(TSNode class_node, StringList *properties, const char *source_code, int *actual_method_count, int *max_methods) {
    // counts all methods (upper boundary for memory allocation)
    *max_methods = count_methods(class_node);
    if (*max_methods <= 0) {
        return NULL;
    }

    int **access_matrix = malloc(sizeof(int*) * (*max_methods));
    if (!access_matrix) {
        fprintf(stderr, "TCC: Memory allocation failed for access matrix.\n");
        return NULL;
    }

    for (int row_i = 0; row_i < *max_methods; ++row_i) {
        access_matrix[row_i] = calloc(properties->count, sizeof(int));
        if (!access_matrix[row_i]) {
            fprintf(stderr, "TCC: Memory allocation failed for access matrix row %d\n", row_i);
            // free rows that have been already allocated
            for (int cleanup_row_i = 0; cleanup_row_i < row_i; ++cleanup_row_i) {
                free(access_matrix[cleanup_row_i]);
            }
            free(access_matrix);
            return NULL;
        }
    }

    // ------ create queries ------
    uint32_t error_offset;
    TSQueryError error_type;

    const char *all_methods_query_src = "(function_definition) @method";
    TSQuery *all_methods_query = ts_query_new(tree_sitter_matlab(), all_methods_query_src,
                                             strlen(all_methods_query_src), &error_offset, &error_type);
    if (!all_methods_query) {
        fprintf(stderr, "TCC: Methods query error at %u: %d\n", error_offset, error_type);
        free_access_matrix(access_matrix, *max_methods);
        return NULL;
    }

    const char *property_access_query_src = "(field_expression object: (identifier) @object field: (identifier) @property)";
    TSQuery *property_access_query = ts_query_new(tree_sitter_matlab(), property_access_query_src,
                                                 strlen(property_access_query_src), &error_offset, &error_type);
    if (!property_access_query) {
        fprintf(stderr, "TCC: Property access query error at %u: %d\n", error_offset, error_type);
        free_access_matrix(access_matrix, *max_methods);
        ts_query_delete(all_methods_query);
        return NULL;
    }

    const char *method_block_query_src = "(methods) @methods_block";
    TSQuery *method_block_query = ts_query_new(tree_sitter_matlab(), method_block_query_src,
                                               strlen(method_block_query_src), &error_offset, &error_type);
    if (!method_block_query) {
        fprintf(stderr, "TCC: Method block query error at %u: %d\n", error_offset, error_type);
        free_access_matrix(access_matrix, *max_methods);
        ts_query_delete(all_methods_query);
        ts_query_delete(property_access_query);
        return NULL;
    }

    char *class_name = extract_class_name(class_node, source_code);
    if (!class_name) {
        fprintf(stderr, "TCC: Could not extract class name\n");
        free_access_matrix(access_matrix, *max_methods);
        ts_query_delete(all_methods_query);
        ts_query_delete(property_access_query);
        ts_query_delete(method_block_query);
        return NULL;
    }

    *actual_method_count = 0;

    // iterate method blocks, if not static, get all methods
    TSQueryCursor *methods_block_cursor = ts_query_cursor_new();
    ts_query_cursor_exec(methods_block_cursor, method_block_query, class_node);
    
    TSQueryMatch match;
    while (ts_query_cursor_next_match(methods_block_cursor, &match)) {
        TSNode methods_block = match.captures[0].node;

        // skip static method block
        if (is_static_methods_block(methods_block, source_code)) {
            continue;
        }

        // find all methods in method block
        TSQueryCursor *method_cursor = ts_query_cursor_new();
        ts_query_cursor_exec(method_cursor, all_methods_query, methods_block);
        TSQueryMatch method_match;
        while (ts_query_cursor_next_match(method_cursor, &method_match)) {
            TSNode method_node = method_match.captures[0].node;

            // skip constructor
            TSNode name_node = ts_node_child_by_field_name(method_node, "name", strlen("name"));
            if (ts_node_is_null(name_node)) {
                continue;
            }
            char *method_name = get_node_text(name_node, source_code);
            if (method_name && strcmp(method_name, class_name) == 0) {
                free(method_name);
                continue;
            }

            // extract self parameter
            char *self_parameter = extract_first_parameter(method_node, source_code);
            if (!self_parameter) {
                free(method_name);
                continue;
            }

            if (*actual_method_count >= *max_methods) {
                fprintf(stderr, "TCC: Method count exceeded max_methods.\n");
                free(method_name);
                free(self_parameter);
                break;
            }

            // check which properties are accessed
            TSQueryCursor *access_cursor = ts_query_cursor_new();
            ts_query_cursor_exec(access_cursor, property_access_query, method_node);
            TSQueryMatch access_match;

            while (ts_query_cursor_next_match(access_cursor, &access_match)) {
                char *object = get_node_text(access_match.captures[0].node, source_code);
                char *property = get_node_text(access_match.captures[1].node, source_code);

                if (object && property && strcmp(object, self_parameter) == 0) {
                    int property_index = string_list_index_of(properties, property);
                    if (property_index >= 0) {
                        access_matrix[*actual_method_count][property_index] = 1;
                    }
                }

                free(object);
                free(property);
            }
            ts_query_cursor_delete(access_cursor);

            free(self_parameter);
            free(method_name);
            (*actual_method_count)++;
        }

        ts_query_cursor_delete(method_cursor);
    }
    
    ts_query_cursor_delete(methods_block_cursor);
    ts_query_delete(method_block_query);
    ts_query_delete(all_methods_query);
    ts_query_delete(property_access_query);
    free(class_name);

    return access_matrix;
}

static float calculate_tcc_from_matrix(int **matrix, int method_count, int property_count) {
    if (method_count < 2 || property_count == 0) {
        printf("TCC undefined for less than 2 methods or no properties, TCC = 0.0\n");
        return 0.0f;
    }

    int connected_pairs = 0;
    int total_pairs = 0;
    
    for (int method_i = 0; method_i < method_count; ++method_i) {
        for (int method_j = method_i + 1; method_j < method_count; ++method_j) {
            total_pairs++;
            for (int property_i = 0; property_i < property_count; ++property_i) {
                // if both methods access the same property -> conntected
                if (matrix[method_i][property_i] && matrix[method_j][property_i]) {
                    connected_pairs++;
                    break;
                }
            }
        }
    }
    
    float tcc = (float)connected_pairs / total_pairs;
    printf("TCC = %d/%d = %.2f\n", connected_pairs, total_pairs, tcc);
    
    return tcc;
}

float compute_tcc(TSNode class_node, const char *source_code) {
    StringList *properties = collect_class_properties(class_node, source_code);
    if (!properties || properties->count == 0) {
        if (properties) free_string_list(properties);
        return 0.0f;
    }

    int actual_method_count = 0;
    int max_methods = 0;
    int **access_matrix = build_method_property_matrix(class_node, properties, source_code, &actual_method_count, &max_methods);
    if (!access_matrix) {
        free_string_list(properties);
        return 0.0f;
    }

    float tcc = calculate_tcc_from_matrix(access_matrix, actual_method_count, properties->count);

    free_access_matrix(access_matrix, max_methods);
    free_string_list(properties);

    return tcc;
}
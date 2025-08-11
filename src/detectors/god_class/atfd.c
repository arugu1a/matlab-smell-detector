#include "tree_sitter/api.h"
#include "detector_utils.h"
#include "atfd.h"

#include <string.h>
#include <stdio.h>

int compute_atfd(TSNode class_node, const char *source_code) {
    // get class name to skip constructor
    char *class_name = extract_class_name(class_node, source_code);
    if (!class_name) {
        fprintf(stderr, "Could not extract class name\n");
        return 0;
    }
    
    uint32_t error_offset;
    TSQueryError error_type;
    
    const char *method_query_src = "(function_definition) @method";
    TSQuery *method_query = ts_query_new(tree_sitter_matlab(), method_query_src,
                                        strlen(method_query_src), &error_offset, &error_type);
    if (!method_query) {
        fprintf(stderr, "Method query error at %u: %d\n", error_offset, error_type);
        return 0;
    }
    
    const char *access_query_src = "(field_expression object: (identifier) @obj field: (identifier) @field)";
    TSQuery *access_query = ts_query_new(tree_sitter_matlab(), access_query_src,
                                        strlen(access_query_src), &error_offset, &error_type);
    if (!access_query) {
        fprintf(stderr, "Access query error at %u: %d\n", error_offset, error_type);
        ts_query_delete(method_query);
        return 0;
    }
    
    int foreign_access_count = 0;
    
    TSQueryCursor *cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, method_query, class_node);
    
    TSQueryMatch match;
    while (ts_query_cursor_next_match(cursor, &match)) {
        TSNode method_node = match.captures[0].node;

        // skip constructor
        TSNode name_node = ts_node_child_by_field_name(method_node, "name", strlen("name"));
        if (ts_node_is_null(name_node)) {
            continue;
        }
        char *method_name = get_node_text(name_node, source_code);
        if (strcmp(method_name, class_name) == 0) {
            continue;
        }

        char *self_param = extract_first_parameter(method_node, source_code);
        if (!self_param) {
            continue;
        }
        
        TSQueryCursor *access_cursor = ts_query_cursor_new();
        ts_query_cursor_exec(access_cursor, access_query, method_node);
        
        TSQueryMatch access_match;
        while (ts_query_cursor_next_match(access_cursor, &access_match)) {
            char *obj = get_node_text(access_match.captures[0].node, source_code);
            
            // count as foreign if: obj != self AND obj != class_name
            if (obj && strcmp(obj, self_param) != 0 && strcmp(obj, class_name) != 0) {
                foreign_access_count++;
            }
            
            free(obj);
        }
        
        ts_query_cursor_delete(access_cursor);
        free(self_param);
    }
    
    ts_query_cursor_delete(cursor);
    ts_query_delete(method_query);
    ts_query_delete(access_query);
    free(class_name);
    
    return foreign_access_count;
}
#include "detector_utils.h"
#include "tree_sitter/api.h"

#include <stdio.h>
#include <string.h>

#define INITIAL_CAPACITY 10

char* extract_first_parameter(TSNode method_node, const char *source_code) {
    for (uint32_t child_i = 0; child_i < ts_node_child_count(method_node); ++child_i) {
        TSNode child = ts_node_child(method_node, child_i);
        if (strcmp(ts_node_type(child), "function_arguments") == 0) {
            if (ts_node_named_child_count(child) > 0) {
                return get_node_text(ts_node_named_child(child, 0), source_code);
            }
            break;
        }
    }
    return NULL;
}

char* extract_class_name(TSNode class_node, const char *source_code) {
    TSNode name_node = ts_node_child_by_field_name(class_node, "name", strlen("name"));
    if (ts_node_is_null(name_node)) return NULL;
    return get_node_text(name_node, source_code);
}

int is_static_methods_block(TSNode methods_node, const char *source_code) {
    // (methods (attributes (attribute (identifier)))
    // compare attribute identifier to "Static"
    uint32_t total_children = ts_node_child_count(methods_node);
    TSNode attributes_node = {0};
    int found_attributes = 0;
    
    for (uint32_t child_i = 0; child_i < total_children; ++child_i) {
        TSNode child = ts_node_child(methods_node, child_i);
        if (strcmp(ts_node_type(child), "attributes") == 0) {
            attributes_node = child;
            found_attributes = 1;
            break;
        }
    }
    if (!found_attributes) {
        return 0;
    }
    uint32_t child_count = ts_node_child_count(attributes_node);
    for (uint32_t child_j = 0; child_j < child_count; ++child_j) {
        TSNode current_child = ts_node_child(attributes_node, child_j);
        
        if (strcmp(ts_node_type(current_child), "attribute") != 0) {
            continue;
        } 
        // get identifier and compare to Static
        if (ts_node_named_child_count(current_child) >= 1) {
            TSNode identifier_node = ts_node_named_child(current_child, 0);
            char* attribute_name = get_node_text(identifier_node, source_code);
            if (attribute_name) {
                if (strcmp(attribute_name, "Static") == 0) {
                    free(attribute_name);
                    return 1;
                }
                free(attribute_name);
            }
        }
    }
    return 0;
}

int count_methods(TSNode node) {
    const char *query_string = "(function_definition) @function";
    
    uint32_t error_offset;
    TSQueryError error_type;
    
    TSQuery *query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "count_methods: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return -1;
    }
    
    TSQueryCursor *cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, node);
    
    int count = 0;
    TSQueryMatch match;
    
    while (ts_query_cursor_next_match(cursor, &match)) {
        count++;
    }
    
    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
    
    return count;
}

StringList *create_string_list() {
    StringList *list = malloc(sizeof(StringList));
    list->items = malloc(sizeof(char*) * INITIAL_CAPACITY);
    list->count = 0;
    list->capacity = INITIAL_CAPACITY;
    return list;
}

void add_to_string_list(StringList *list, const char *str) {
    for (int string_i = 0; string_i < list->count; ++string_i) {
        if (strcmp(list->items[string_i], str) == 0) return;
    }
    
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->items = realloc(list->items, sizeof(char*) * list->capacity);
    }
    
    size_t length = strlen(str);
    list->items[list->count] = malloc(length + 1);
    memcpy(list->items[list->count], str, length);
    list->items[list->count][length] = '\0';
    list->count++;
}

int string_list_contains(StringList *list, const char *str) {
    for (int string_i = 0; string_i < list->count; ++string_i) {
        if (strcmp(list->items[string_i], str) == 0) return 1;
    }
    return 0;
}

int string_list_index_of(StringList *list, const char *str) {
    for (int string_i = 0; string_i < list->count; ++string_i) {
        if (strcmp(list->items[string_i], str) == 0) return string_i;
    }
    return -1;
}

void free_string_list(StringList *list) {
    if (!list) return;
    for (int i = 0; i < list->count; i++) {
        free(list->items[i]);
    }
    free(list->items);
    free(list);
}

char *get_node_text(TSNode node, const char *source_code) {
    if (ts_node_is_null(node)) return NULL;
    
    uint32_t start = ts_node_start_byte(node);
    uint32_t end = ts_node_end_byte(node);
    size_t length = end - start;
    
    char *text = malloc(length + 1);
    memcpy(text, source_code + start, length);
    text[length] = '\0';
    return text;
}
#include "detector.h"
#include "smell_list.h"
#include "detector_utils.h"
#include "filter_utils.h"
#include "cc.h"

#include <string.h>
#include <stdio.h>

uint32_t count_LOC(TSNode node) {
    if (ts_node_is_null(node)) return 0;
    TSPoint start = ts_node_start_point(node);
    TSPoint end = ts_node_end_point(node);
    uint32_t LOC = end.row - start.row + 1;
    return LOC;
}

void detect_long_function_candidates(TSNode root_node, Matlab_file *file, Smell_list *list) {
    char *file_name = file->file_name;

    const char* query_string = "(function_definition) @function";
    
    uint32_t error_offset;
    TSQueryError error_type;
    
    TSQuery* query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "find_long_function_candidates: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return;
    }

    TSQueryCursor* cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, root_node);

    TSQueryMatch match;

    while (ts_query_cursor_next_match(cursor, &match)) {
        TSNode function_node = match.captures[0].node;

        
        TSPoint start = ts_node_start_point(function_node);

        Smell_location location = create_location(file_name, start.row + 1);
        Smell *candidate = create_smell(location);
        if (!candidate) continue;

        uint32_t LOC = count_LOC(function_node);
        Metric LOC_metric = create_int_metric("LOC", LOC);
        add_metric(candidate, LOC_metric);

        int CC = count_binary_splits(function_node) + 1;
        Metric CC_metric = create_int_metric("CC", CC);
        add_metric(candidate, CC_metric);

        add_smell_to_list(list, *candidate);
        free(candidate);
    }

    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
}

// don't know where this function belongs best
// exists here and in god_class.c
// TODO: find common place
static Configuration* get_config_by_name(Smell_detector *detector, const char *name) {
    for (size_t config_i = 0; config_i < detector->config_count; ++config_i) {
        if (strcmp(detector->configs[config_i].name, name) == 0) {
            return &detector->configs[config_i];
        }
    }
    return NULL;
}

void filter_long_function_candidates(Smell_detector *detector) {
    size_t total_count = detector->smell_list->count;
    
    Configuration *LOC_config = get_config_by_name(detector, "LOC");
    sort_smell_list_by_metric(detector->smell_list, "LOC", 0);
    if (LOC_config->use_percentage) { 
        cut_smell_list_relative(detector->smell_list, total_count, LOC_config->percentage_value);
    } else {
        threshold_value threshold = {.int_value = LOC_config->absolute_value.int_absolute};
        cut_smell_list_absolute(detector->smell_list, "LOC", threshold, 0);
    }
    
    Configuration *CC_config = get_config_by_name(detector, "CC");
    sort_smell_list_by_metric(detector->smell_list, "CC", 0);
    if (CC_config->use_percentage) { 
        cut_smell_list_relative(detector->smell_list, total_count, CC_config->percentage_value);
    } else {
        threshold_value threshold = {.int_value = CC_config->absolute_value.int_absolute};
        cut_smell_list_absolute(detector->smell_list, "CC", threshold, 0);
    }
}

Smell_detector long_function_detector = {
    .name = "long_function",
    .detect_candidates = detect_long_function_candidates,
    .filter = filter_long_function_candidates,
    .configs = {
        {
        .name = "LOC",
        .key_absolute = "absolute_LOC",
        .key_percentage = "top_percentage_LOC",
        .key_use_percentage = "use_percentage_LOC",
        .absolute_is_float = 0
        },
        {
        .name = "CC",
        .key_absolute = "absolute_CC",
        .key_percentage = "top_percentage_CC",
        .key_use_percentage = "use_percentage_CC",
        .absolute_is_float = 0
        }
    },
    .config_count = 2
};
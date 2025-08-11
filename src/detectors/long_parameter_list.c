#include "tree_sitter/api.h"
#include "smell_list.h"
#include "detector.h"
#include "filter_utils.h"

#include <string.h>
#include <stdio.h>


void find_long_parameter_list_candidates(TSNode root_node, Matlab_file *file, Smell_list *list) {

    char *file_name = file->file_name;

    const char *query_string = "(function_arguments) @params";

    TSQueryError error_type;
    uint32_t error_offset;

    TSQuery *query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "find_long_parameter_list_candidates: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return;
    }

    TSQueryCursor *query_cursor = ts_query_cursor_new();
    ts_query_cursor_exec(query_cursor, query, root_node);

    TSQueryMatch match;

    while (ts_query_cursor_next_match(query_cursor, &match)) {
        TSNode params = match.captures[0].node;

        uint32_t parameter_count = ts_node_named_child_count(params);
        Smell_location location = create_location(file_name,
                                                    ts_node_start_point(params).row + 1);
        Smell *candidate = create_smell(location);
        if (!candidate) continue;

        Metric metric = create_int_metric("NUMBER_PARAMETER", parameter_count);
        add_metric(candidate, metric);
        add_smell_to_list(list, *candidate);
        free(candidate);
    }

    ts_query_cursor_delete(query_cursor);
    ts_query_delete(query);
}

void filter_long_parameter_list_candidates(Smell_detector *detector) {
    size_t total_count = detector->smell_list->count;
    Configuration config = detector->configs[0];
    
    sort_smell_list_by_metric(detector->smell_list, "NUMBER_PARAMETER", 0);
    if (config.use_percentage) { 
        cut_smell_list_relative(detector->smell_list, total_count, config.percentage_value);
    } else {
        threshold_value threshold = {.int_value = config.absolute_value.int_absolute};
        cut_smell_list_absolute(detector->smell_list, "NUMBER_PARAMETER", threshold, 0);
    }
}

Smell_detector long_parameter_list_detector = {
    .name = "long_parameter_list",
    .detect_candidates = find_long_parameter_list_candidates,
    .filter = filter_long_parameter_list_candidates,
    .configs[0] = {
        .key_absolute = "absolute_param_count",
        .key_percentage = "top_percentage_param_count",
        .key_use_percentage = "use_percentage",
        .absolute_is_float = 0
    },
    .config_count = 1
};
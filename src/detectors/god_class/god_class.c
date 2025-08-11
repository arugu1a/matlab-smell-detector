#include "tree_sitter/api.h"
#include "filter_utils.h"
#include "detector_utils.h"
#include "detector.h"

#include "cc.h"
#include "atfd.h"
#include "tcc.h"

#include <string.h>
#include <stdio.h>


static void detect_god_class_candidates(TSNode root_node, Matlab_file *file, Smell_list *list) {

    const char *query_string = "(class_definition) @class";

    TSQueryError error_type;
    uint32_t error_offset;

    TSQuery *query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                  &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "find_god_class_candidates: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return;
    }

    TSQueryCursor *query_cursor = ts_query_cursor_new();
    ts_query_cursor_exec(query_cursor, query, root_node);

    TSQueryMatch match;

    while (ts_query_cursor_next_match(query_cursor, &match)) {
        TSNode class_node = match.captures[0].node;

        char *class_name = extract_class_name(class_node, file->content);
        if (!class_name) {
            fprintf(stderr, "Could not extract class name.\n");
            continue;
        }

        Smell_location location = create_location(file->file_name,
                                                    ts_node_start_point(class_node).row + 1);
        Smell *candidate = create_smell(location);
        if (!candidate) continue;

        int wmc = count_binary_splits(class_node) + count_methods(class_node);
        Metric wmc_metric = create_int_metric("WMC", wmc);
        add_metric(candidate, wmc_metric);

        int atfd = compute_atfd(class_node, file->content);
        Metric atfd_metric = create_int_metric("ATFD", atfd);
        add_metric(candidate, atfd_metric);

        float tcc = compute_tcc(class_node, file->content);
        Metric tcc_metric = create_float_metric("TCC", tcc);
        add_metric(candidate, tcc_metric);

        add_smell_to_list(list, *candidate);

        printf("Class Summary - %s\n", class_name);
        printf("File: %s\n", file->file_name);
        printf("  WMC: %d\n", wmc);
        printf("  ATFD: %d\n", atfd);
        printf("  TCC: %f\n", tcc);
        printf("\n");
        free(candidate);

    }

    ts_query_cursor_delete(query_cursor);
    ts_query_delete(query);
}

// don't know where this function belongs best
// exists here and in long_function.c
// TODO: find common place
static Configuration* get_config_by_name(Smell_detector *detector, const char *name) {
    for (size_t config_i = 0; config_i < detector->config_count; ++config_i) {
        if (strcmp(detector->configs[config_i].name, name) == 0) {
            return &detector->configs[config_i];
        }
    }
    return NULL;
}

void filter_god_class_candidates(Smell_detector *detector) {
    size_t total_count = detector->smell_list->count;
    sort_smell_list_by_metric(detector->smell_list, "WMC", 0);
    Configuration *wmc_config = get_config_by_name(detector, "WMC");
    if (!wmc_config) {
        fprintf(stderr, "filter_god_class: Config with given name WMC doesn't exist.\n");
        return;
    }
    if (wmc_config->use_percentage) {
        cut_smell_list_relative(detector->smell_list, total_count, wmc_config->percentage_value);
    } else {
        threshold_value wmc_threshold = {.int_value = wmc_config->absolute_value.int_absolute};
        cut_smell_list_absolute(detector->smell_list, "WMC", wmc_threshold, 0);
    }

    sort_smell_list_by_metric(detector->smell_list, "TCC", 1);
    Configuration *tcc_config = get_config_by_name(detector, "TCC");
    if (!tcc_config) {
        fprintf(stderr, "filter_god_class: Config with given name TCC doesn't exist.\n");
        return;
    }
    if (tcc_config->use_percentage) {
        cut_smell_list_relative(detector->smell_list, total_count, tcc_config->percentage_value);
    } else {
        threshold_value tcc_threshold = {.float_value = tcc_config->absolute_value.float_absolute};
        cut_smell_list_absolute(detector->smell_list, "TCC", tcc_threshold, 1);
    }

    sort_smell_list_by_metric(detector->smell_list, "ATFD", 0);
    Configuration *atfd_config = get_config_by_name(detector, "ATFD");
    if (!atfd_config) {
        fprintf(stderr, "filter_god_class: Config with given name ATFD doesn't exist.\n");
        return;
    }
    if (atfd_config->use_percentage) {
        cut_smell_list_relative(detector->smell_list, total_count, atfd_config->percentage_value);
    } else {
        threshold_value atfd_threshold = {.int_value = atfd_config->absolute_value.int_absolute};
        cut_smell_list_absolute(detector->smell_list, "ATFD", atfd_threshold, 0);
    }
    
}

Smell_detector god_class_detector = {
    .name = "god_class",
    .detect_candidates = detect_god_class_candidates,
    .filter = filter_god_class_candidates,
    .configs = {
        {
        .name = "TCC",
        .key_absolute = "absolute_tcc",
        .key_percentage = "bottom_percentage_tcc",
        .key_use_percentage = "use_percentage_tcc",
        .absolute_is_float = 1
        },
        {
        .name = "WMC",
        .key_absolute = "absolute_wmc",
        .key_percentage = "top_percentage_wmc",
        .key_use_percentage = "use_percentage_wmc",
        .absolute_is_float = 0
        },
        {
        .name = "ATFD",
        .key_absolute = "absolute_atfd",
        .key_percentage = "top_percentage_atfd",
        .key_use_percentage = "use_percentage_atfd",
        .absolute_is_float = 0
        }
    },
    .config_count = 3
};


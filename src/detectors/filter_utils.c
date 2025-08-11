#include "filter_utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// global variable to pass context to compare function
static size_t current_metric_index = 0;

// finds index and stores in *index
static int find_metric_index(const Smell *smell, const char *metric_name, size_t *index) {
    for (size_t metric_i = 0; metric_i < smell->metric_count; metric_i++) {
        if (strcmp(smell->metrics[metric_i].name, metric_name) == 0) {
            *index = metric_i;
            return 1;
        }
    }
    return 0;
}

static int compare_ascending(const void *a, const void *b) {
    const Smell *smell_a = (const Smell *)a;
    const Smell *smell_b = (const Smell *)b;
    
    const Metric *metric_a = &smell_a->metrics[current_metric_index];
    const Metric *metric_b = &smell_b->metrics[current_metric_index];
    
    if (metric_a->is_float) {
        float value_a = metric_a->measured_value.float_value;
        float value_b = metric_b->measured_value.float_value;
        
        if (value_a < value_b) return -1;
        if (value_a > value_b) return 1;
        return 0;
    } else {
        int value_a = metric_a->measured_value.int_value;
        int value_b = metric_b->measured_value.int_value;
        
        if (value_a < value_b) return -1;
        if (value_a > value_b) return 1;
        return 0;
    }
}

static int compare_descending(const void *a, const void *b) {
    return -compare_ascending(a, b);
}

void sort_smell_list_by_metric(Smell_list *list, const char *metric_name, int ascending) {
    if (!metric_name || !list || list->count == 0) return;
    
    size_t metric_index;
    if (!find_metric_index(&list->smells[0], metric_name, &metric_index)) {
        fprintf(stderr, "Metric doesn't exist in smell list.\n");
        return;
    }
    
    // set global variable, not thread safe
    current_metric_index = metric_index;
    
    if (ascending) {
        qsort(list->smells, list->count, sizeof(Smell), compare_ascending);
    } else {
        qsort(list->smells, list->count, sizeof(Smell), compare_descending);
    }
}

void cut_smell_list_relative(Smell_list *list, size_t total_count, float percentage) {
    if (!list || percentage < 0.0f || percentage > 1.0f) return;
    size_t new_count = (size_t)(total_count * percentage);
    list->count = new_count;
}

// sorting asc. vs desc. is directly related to is_upper_bound
// -> would be good to wrap into one function call
void cut_smell_list_absolute(Smell_list *list, const char *metric_name,
                            threshold_value threshold, int is_upper_bound) {
    if (!list || !metric_name || list->count == 0) return;
    
    size_t metric_index;
    if (!find_metric_index(&list->smells[0], metric_name, &metric_index)) {
        fprintf(stderr, "Metric doesn't exist in smell list.\n");
        return;
    }
    
    int is_float = list->smells[0].metrics[metric_index].is_float;

    for (size_t smell_i = 0; smell_i < list->count; ++smell_i) {
        const Metric *metric = &list->smells[smell_i].metrics[metric_index];
        
        if (is_float) {
            if (is_upper_bound) {
                if (metric->measured_value.float_value > threshold.float_value) {
                    list->count = smell_i;
                    break;
                }
            } else {
                if (metric->measured_value.float_value < threshold.float_value) {
                    list->count = smell_i;
                    break;
                }
            }
        } else {
            if (is_upper_bound) {
                if (metric->measured_value.int_value > threshold.int_value) {
                    list->count = smell_i;
                    break;
                }
            } else {
                if (metric->measured_value.int_value < threshold.int_value) {
                    list->count = smell_i;
                    break;
                }
            }
        }
    }
}
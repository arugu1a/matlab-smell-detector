#ifndef FILTER_UTILS_H
#define FILTER_UTILS_H

#include "smell_list.h"

typedef union {
    float float_value;
    int int_value;
} threshold_value;

// ! ASSUMPTION: all smells have the same metric at the same index in a smell list
// ! this function is not thread safe
void sort_smell_list_by_metric(Smell_list *list, const char *metric_name, int ascending);

// ASSUMPTION for both cut functions: list is sorted
// always keeps the left side of the list, behavior changes depending on sort type (asc./desc.)

void cut_smell_list_relative(Smell_list *list, size_t total_count, float percentage);

// threshold value is chosen based on metric.is_float
// ? -> this could be changed so that not the metric type decides but rather the
// detector.config
void cut_smell_list_absolute(Smell_list *list, const char *metric_name, threshold_value threshold, int is_upper_bound);

#endif
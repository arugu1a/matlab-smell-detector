#include "smell_list.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SMELL_CAPACITY 32

Smell_location create_location(char *file_name, uint32_t line) {
    Smell_location location;
    location.file_name = file_name; //copy pointer, points to file_name in the file_list
    location.line = line;
    return location;
}

Metric create_float_metric(const char *name, float measured_value) {
    Metric metric;
    metric.name = name;
    metric.measured_value.float_value = measured_value;
    metric.is_float = 1;
    return metric;
}

Metric create_int_metric(const char *name, uint32_t measured_value) {
    Metric metric;
    metric.name = name;
    metric.measured_value.int_value = measured_value;
    metric.is_float = 0;
    return metric;
}

Smell *create_smell(Smell_location location) {
    Smell *smell = malloc(sizeof(Smell));
    if (!smell) return NULL;
    smell->location = location;
    smell->metric_count = 0;
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


void add_metric(Smell *smell, Metric metric) {
    if (smell->metric_count < MAX_METRICS) {
        smell->metrics[smell->metric_count] = metric;
        smell->metric_count = smell->metric_count + 1;
    } else {
        fprintf(stderr, "Maximum amount of metrics reached.\n");
    }
    
}

void print_smell_list(Smell_list *list) {
    for (size_t smell_i = 0; smell_i < list->count; ++smell_i) {
        Smell current_smell = list->smells[smell_i];
        printf("-------------\n");
        printf("File: %s Line: %d\n", current_smell.location.file_name, current_smell.location.line);
        for (size_t metric_i = 0; metric_i < current_smell.metric_count; ++metric_i) {
            Metric current_metric = current_smell.metrics[metric_i];
            printf("Metric: %s, Measured Value: ", current_metric.name);
            if (current_metric.is_float) {
                printf("%.2f\n", current_metric.measured_value.float_value);
            } else {
                printf("%d\n", current_metric.measured_value.int_value);
            }
        }
    }
    printf("\n");
}

void free_smell_list(Smell_list *list) {
    // list->smells[i].location.file_name belongs to file list,
    // gets freed in free_file_list(File_list *list) in file_utils.c
    free(list->smells);
}
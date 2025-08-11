#ifndef DETECTOR_H
#define DETECTOR_H

#include "smell_list.h"
#include "matlab_file_list.h"

#include <stddef.h>

#define MAX_CONFIGS 3

typedef struct TSNode TSNode;


typedef struct {
    // these are defined when implementing a detector
    const char *name;
    const char *key_use_percentage;
    const char *key_absolute;
    const char *key_percentage;
    int absolute_is_float;

    // these are loaded from config.ini
    union {
        int int_absolute;
        float float_absolute;
    } absolute_value;
    float percentage_value;
    int use_percentage;
} Configuration;

typedef struct Smell_detector Smell_detector;

struct Smell_detector {
    const char *name;
    Smell_list *smell_list;
    void (*detect_candidates)(TSNode, Matlab_file*, Smell_list*);
    void (*filter)(Smell_detector*);
    Configuration configs[MAX_CONFIGS];
    size_t config_count;
};

#endif
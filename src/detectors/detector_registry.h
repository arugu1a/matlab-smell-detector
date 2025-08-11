#ifndef DETECTOR_REGISTRY_H
#define DETECTOR_REGISTRY_H

#include "detector.h"

#include <stddef.h>

extern Smell_detector long_function_detector;
extern Smell_detector long_parameter_list_detector;
extern Smell_detector god_class_detector;

extern Smell_detector* detectors[];
extern const size_t detector_count;

void print_detector_configs(void);

#endif
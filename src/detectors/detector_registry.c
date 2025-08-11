#include "detector_registry.h"

#include <stdio.h>

Smell_detector* detectors[] = {
    &long_function_detector,
    &long_parameter_list_detector,
    &god_class_detector
};

const size_t detector_count = sizeof(detectors) / sizeof(detectors[0]);

void print_detector_configs(void) {
    printf("------\n");
    printf("Loaded Detector Configurations:\n");
    printf("\n");
    
    for (size_t detector_i = 0; detector_i < detector_count; ++detector_i) {
        Smell_detector *detector = detectors[detector_i];
        printf("[%s]:\n", detector->name);
        
        for (size_t config_i = 0; config_i < detector->config_count; ++config_i) {
            Configuration *config = &detector->configs[config_i];
            
            if (config->absolute_is_float) {
                printf("  %s: %.2f\n", config->key_absolute, config->absolute_value.float_absolute);
            } else {
                printf("  %s: %d\n", config->key_absolute, config->absolute_value.int_absolute);
            }

            printf("  %s: %.2f\n", config->key_percentage, config->percentage_value);
            printf("  %s: %d\n", config->key_use_percentage, config->use_percentage);
            printf("\n");
        }
        printf("------\n");
    }
}
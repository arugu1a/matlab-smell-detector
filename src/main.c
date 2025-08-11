#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "file_utils.h"
#include "smell_list.h"
#include "detector_registry.h"

extern uint32_t count_LOC(TSNode node);


char* parse_argument(int argc, char* argv[], const char* program_name) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", program_name); // TODO: add usage based on windows/linux
        fprintf(stderr, "Error: Expected exactly 1 argument (path), got %d.\n", argc - 1);
        return NULL;
    }
    return argv[1];
}

int main(int argc, char *argv[]) {
    clock_t begin = clock();

    char* path = parse_argument(argc, argv, argv[0]);
    if (!path) return EXIT_FAILURE;
    
    load_config("config.ini", detectors, detector_count);

    File_list file_list;
    init_file_list(&file_list);

    load_files(path, &file_list);

    for (size_t i = 0; i < detector_count; ++i) {
        detectors[i]->smell_list = malloc(sizeof(Smell_list));
        init_smell_list(detectors[i]->smell_list);
    }

    TSParser *parser = ts_parser_new();
    ts_parser_set_language(parser, tree_sitter_matlab());

    uint32_t total_LOC = 0;

    for (size_t file_i = 0; file_i < file_list.count; ++file_i) {
        Matlab_file *current_file = file_list.files[file_i];
        char *source = current_file->content;
        TSTree *tree = ts_parser_parse_string(parser, NULL, source, strlen(source));
        TSNode root_node = ts_tree_root_node(tree);

        total_LOC = total_LOC + count_LOC(root_node);

        for (size_t detector_i = 0; detector_i < detector_count; ++detector_i) {
            Smell_detector *current_detector = detectors[detector_i];
            current_detector->detect_candidates(root_node, current_file, current_detector->smell_list);
        }
        ts_tree_delete(tree);
    }
    ts_parser_delete(parser);

    for (size_t detector_i = 0; detector_i < detector_count; ++detector_i) {
            Smell_detector *current_detector = detectors[detector_i];
            current_detector->filter(current_detector);
            printf("Smell List: %s\n", current_detector->name);
            print_smell_list(current_detector->smell_list);
    }
    smell_lists_to_CSV();

    for (size_t detector_i = 0; detector_i < detector_count; ++detector_i) {
            Smell_detector *current_detector = detectors[detector_i];
            printf("Total number of %s smells: %zu\n", current_detector->name, 
                    current_detector->smell_list->count);
    }
    printf("\n");

    for (size_t i = 0; i < detector_count; ++i) {
        free_smell_list(detectors[i]->smell_list);
        free(detectors[i]->smell_list);
    }
    printf("Files analyzed: %zu\n", file_list.count);
    free_file_list(&file_list);
    printf("Total LOC analyzed: %d\n", total_LOC);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("CPU time used: %lf seconds\n", time_spent);

    return EXIT_SUCCESS;
}
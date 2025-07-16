#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "file_utils.h"
#include "smells.h"


char* parse_argument(int argc, char* argv[], const char* program_name) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <path>\n", program_name); // TODO: add usage based on windows/linux
        fprintf(stderr, "Error: Expected exactly 1 argument (path), got %d.\n", argc - 1);
        return NULL;
    }
    return argv[1];
}

int main(int argc, char *argv[]) {
    char* path = parse_argument(argc, argv, argv[0]);
    if (!path) return EXIT_FAILURE;
    
    load_config("config.ini");
    print_config();

    File_list file_list;
    init_file_list(&file_list);

    Smell_list smell_list;
    init_smell_list(&smell_list);
    load_files(path, &file_list);

    smell_search(&file_list, &smell_list);

    print_smell_list(&smell_list);
    smell_list_to_CSV(&smell_list);

    free_smell_list(&smell_list);
    free_file_list(&file_list);
}
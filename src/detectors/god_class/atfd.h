#ifndef ATFD_H
#define ATFD_H

#include "tree_sitter/api.h"

int compute_atfd(TSNode class_node, const char* source_code);

#endif
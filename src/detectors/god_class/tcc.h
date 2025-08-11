#ifndef TCC_H
#define TCC_H

#include "tree_sitter/api.h"

float compute_tcc(TSNode class_node, const char *source_code);

#endif
#include <string.h>
#include <stdio.h>

#include "cc.h"
#include "detector_utils.h"
#include "tree_sitter/api.h"

int count_binary_splits(TSNode node) {
    const char* query_string = 
        "["
        "  (if_statement)"
        "  (elseif_clause)"
        "  (while_statement)"
        "  (for_statement)"
        "  (switch_statement)"
        "  (case_clause)"
        "  (try_statement)"
        "] @split";
    
    uint32_t error_offset;
    TSQueryError error_type;
    TSQuery* query = ts_query_new(tree_sitter_matlab(), query_string, strlen(query_string),
                                    &error_offset, &error_type);
    if (!query) {
        fprintf(stderr, "calculate_wmc: TSQuery error: %d at offset %u\n",
                error_type, error_offset);
        return 0;
    }
    TSQueryCursor* cursor = ts_query_cursor_new();
    ts_query_cursor_exec(cursor, query, node);
    
    int count = 0;
    TSQueryMatch match;
    while (ts_query_cursor_next_match(cursor, &match)) {
        count++;
    }
    ts_query_cursor_delete(cursor);
    ts_query_delete(query);
    
    return count;
}
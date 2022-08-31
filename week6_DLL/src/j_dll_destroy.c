#include <stddef.h>
#include <stdlib.h>
#include "../include/j_dll.h"

/* j_dll_destory: destroy the whole dll */
void j_dll_destroy(j_dll_t *dll)
{
    struct j_dllnode *curr_node_ptr;
    size_t node_cnt, i;

    node_cnt = j_dll_cnt(dll);
    for (i = 0; i != node_cnt; i++) {
	curr_node_ptr = j_dll_head(dll);
	dll->delete(dll, curr_node_ptr);
    }
    free(dll);
}

#include "../include/j_dll.h"

int j_dll_update(j_dll_t *dll, void *origin, void *new)
{
    struct j_dllnode *curr_node_ptr;

    curr_node_ptr = j_dll_head(dll);
    while (curr_node_ptr != NULL) {
	

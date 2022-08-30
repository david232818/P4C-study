#include "../include/j_dll.h"

/* j_dll_search: search node by given data */
struct j_dllnode *j_dll_search(j_dll_t *dll, void *data)
{
    struct j_dllnode *curr_node_ptr;

    curr_node_ptr = j_dll_head(dll);
    while (curr_node_ptr != NULL) {
	if (j_dll_mtable(dll, COMPARE)->method(j_dllnode_data(curr_node_ptr),
					       data) == 0)
	    return curr_node_ptr;
	curr_node_ptr = j_next_node_of(curr_node_ptr);
    }
    return NULL;
}

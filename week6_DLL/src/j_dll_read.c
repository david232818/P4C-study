#include <stdio.h>
#include "../include/j_dll.h"

/* j_dll_read: print data in dll */
int j_dll_read(j_dll_t *dll)
{
    struct j_dllnode *curr_node_ptr;
    void *data;

    curr_node_ptr = j_dll_head(dll);
    while (curr_node_ptr != NULL) {
	j_dllnode_read(curr_node_ptr, j_dll_mtable(dll, READ)->method);
	curr_node_ptr = j_next_node_of(curr_node_ptr);
    }
    return 0;
}

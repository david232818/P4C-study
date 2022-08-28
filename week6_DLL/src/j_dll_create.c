#include <stdio.h>
#include "../include/j_assert.h"
#include "../include/j_dll.h"

/* j_dll_create: create node then add to dll */
int j_dll_create(j_dll_t *dll, void *data)
{
    struct j_dllnode *node, *curr_node_ptr;
    int cres;

    if (j_dll_is_full(dll)) {
	j_dll_errno = J_DLL_ERR_FULL_DLL;
	return -1;
    }

    J_CALL_AND_ASSIGNMENT_CHECK(j_dllnode_init, node, NULL, -1, data);

    if (j_dll_is_empty(dll)) {	/* dll is empty */
	j_dll_head(dll) = node;
	j_dll_tail(dll) = node;
	j_dll_cnt(dll)++;
	return 0;
    }

    /* since dll is not empty, head node exists */
    curr_node_ptr = j_dll_head(dll);
    while ((cres = j_dll_mtable(dll, COMPARE)->method(
		j_dllnode_data(curr_node_ptr),
		j_dllnode_data(node))) <= 0) {
	if (j_next_node_of(curr_node_ptr) == NULL)
	    break;
	curr_node_ptr = j_next_node_of(curr_node_ptr);
    }
    if (curr_node_ptr == j_dll_head(dll) && cres > 0) {
	j_dll_head(dll) = node;
	j_previous_node_of(node) = NULL;
	j_next_node_of(node) = curr_node_ptr;
	j_previous_node_of(curr_node_ptr) = node;
	j_dll_tail(dll) = (j_dll_cnt(dll) == 1) ? curr_node_ptr :
	    j_dll_tail(dll);
    } else if (cres > 0) {
	j_previous_node_of(node) = j_previous_node_of(curr_node_ptr);
	j_next_node_of(node) = curr_node_ptr;
	j_next_node_of(j_previous_node_of(curr_node_ptr)) = node;
	j_previous_node_of(curr_node_ptr) = node;
    } else {
	j_dll_tail(dll) = node;
	j_previous_node_of(node) = curr_node_ptr;
	j_next_node_of(curr_node_ptr) = node;
	j_next_node_of(node) = NULL;
    }
    j_dll_cnt(dll)++;
    return 0;
}

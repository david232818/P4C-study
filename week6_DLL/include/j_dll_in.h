#ifndef __J_DLL_IN_H__
#define __J_DLL_IN_H__

#include <stdio.h>
#include "../include/j_dll.h"

/*
 * prv_j_dll_add_node: add node to dll
 *
 * this function assumes at least one node is in the list
 */
static void prv_j_dll_add_node(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *curr_node_ptr;
    int cres;

    curr_node_ptr = j_dll_head(dll);

    /*
     * There are two ways: first is seperating the loop and conditional
     * part, and second is locating conditional part in the loop.
     * The first method have to deal with the code location of comparing
     * node pointer (here, curr_node_ptr) and NULL. And I cannot find
     * the good solution of this problem. Whatever I try, additional codes
     * are made. So, I use the second method
     */
    while (curr_node_ptr != NULL) {
	cres = j_dll_mtable(dll, COMPARE)->method(
	    j_dllnode_data(curr_node_ptr),
	    j_dllnode_data(node));
	if (curr_node_ptr == j_dll_head(dll) && cres > 0) {
	    j_dll_head(dll) = node;
	    j_previous_node_of(node) = NULL;
	    j_next_node_of(node) = curr_node_ptr;
	    j_previous_node_of(curr_node_ptr) = node;
	    j_dll_tail(dll) = (j_dll_cnt(dll) == 1) ? curr_node_ptr :
		j_dll_tail(dll);
	    break;
	} else if (cres > 0) {
	    j_previous_node_of(node) = j_previous_node_of(curr_node_ptr);
	    j_next_node_of(node) = curr_node_ptr;
	    j_next_node_of(j_previous_node_of(curr_node_ptr)) = node;
	    j_previous_node_of(curr_node_ptr) = node;
	    break;
	} else if (j_next_node_of(curr_node_ptr) == NULL) {
	    j_dll_tail(dll) = node;
	    j_previous_node_of(node) = curr_node_ptr;
	    j_next_node_of(curr_node_ptr) = node;
	    j_next_node_of(node) = NULL;
	    break;
	}
	curr_node_ptr = j_next_node_of(curr_node_ptr);
    }
}

/* prv_j_dll_unlink_node: unlink node from list */
static void prv_j_dll_unlink_node(j_dll_t *dll, struct j_dllnode *node)
{
    if (node == j_dll_head(dll)) {
	j_dll_head(dll) = j_next_node_of(node);
	if (j_dll_head(dll) != NULL)
	    j_previous_node_of(j_next_node_of(node)) = NULL;
	j_next_node_of(node) = NULL;
    } else {
	j_next_node_of(j_previous_node_of(node)) = j_next_node_of(node);
	if (j_next_node_of(node) != NULL) {
	    j_previous_node_of(j_next_node_of(node)) =
		j_previous_node_of(node);
	    j_previous_node_of(node) = NULL;
	    j_next_node_of(node) = NULL;
	} else {
	    j_dll_tail(dll) = j_previous_node_of(node);
	    j_next_node_of(j_previous_node_of(node)) = NULL;
	    j_previous_node_of(node) = NULL;
	}
    }
}

#endif

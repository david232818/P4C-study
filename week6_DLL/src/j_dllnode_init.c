#include <stdio.h>
#include <stdlib.h>
#include "../include/j_assert.h"
#include "../include/j_dllnode.h"

/* J_CALL_AND_ASSIGNMENT_CHECK is too long for 80 cols */
#define J_CALL_ASSIGN_CHECK J_CALL_AND_ASSIGNMENT_CHECK

/* j_dllnode_init: initialize node of dll (double linked list) */
struct j_dllnode *j_dllnode_init(void *data)
{
    struct j_dllnode *node;

    J_CALL_ASSIGN_CHECK(malloc, node, NULL, NULL, sizeof(struct j_dllnode));
    
    if (data == NULL) {
	j_dllnode_errno = J_DLLNODE_ERR_INVALID_DATA;
	free(node);
	return NULL;
    } else
	j_dllnode_data(node) = data;

    j_previous_node_of(node) = NULL;
    j_next_node_of(node) = NULL;
    return node;
}

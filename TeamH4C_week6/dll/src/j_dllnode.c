#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "j_dllnode.h"
#include "j_error.h"

/* j_dllnode_init: initialize node structure */
struct j_dllnode *j_dllnode_init(void *data)
{
    struct j_dllnode *node;

    if ((node = malloc(sizeof(*node))) == NULL) {
	j_dllnode_errno = J_DLLNODE_ERR_NODE_ALLOC;
	return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* j_dllnode_destroy: destroy node structure */
void j_dllnode_destroy(struct j_dllnode *node)
{
    if (node->data != NULL) {
	memset(node->data,
	       0x00,
	       sizeof(*(node->data)));
	free(node->data); /* we assume that the data is malloced */
    }

    node->prev = NULL;
    node->next = NULL;

    /*
     * We think free() will handle invalid pointer problem. So
     * this function always works and does not need to check
     * invalid pointer.
     */
    free(node);
}

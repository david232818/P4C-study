#include <stdio.h>
#include <stdlib.h>
#include "../include/j_dllnode.h"

/* j_dllnode_delete: delete node */
int j_dllnode_delete(struct j_dllnode *node)
{
    j_previous_node_of(node) = NULL;
    j_next_node_of(node) = NULL;
    free(j_dllnode_data(node));
    free(node);
    return !(-1);
}
    

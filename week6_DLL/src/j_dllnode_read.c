#include <stdio.h>
#include "../include/j_dllnode.h"

/* j_dllnode_read: read node data */
void *j_dllnode_read(struct j_dllnode *node)
{
    return j_dllnode_data(node);
}

#include <stdio.h>
#include "../include/j_dllnode.h"

/* j_dllnode_read: read node data */
int j_dllnode_read(struct j_dllnode *node,
		     int (*rm)(void *data, void *nad))
{
    void *data;

    data = j_dllnode_data(node);
    return rm(data, NULL);
}

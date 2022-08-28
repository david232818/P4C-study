#include <stdio.h>
#include "../include/j_dllnode.h"

/* j_dllnode_update: update node data, origin to new */
int j_dllnode_update(struct j_dllnode *node,
		     void *new,
		     int (*um)(void *origin, void *new))
{
    if (new == NULL) {
	j_dllnode_errno = J_DLLNODE_ERR_INVALID_DATA;
	return -1;
    } else if (um == NULL) {
	j_dllnode_errno = J_DLLNODE_ERR_INVALID_METHOD;
	return -1;
    } else
	return um(j_dllnode_data(node), new);
}

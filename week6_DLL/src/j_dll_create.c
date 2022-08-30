#include <stdio.h>
#include "../include/j_assert.h"
#include "../include/j_dll.h"
#include "../include/j_dll_in.h"

/* j_dll_create: create node then add to dll */
int j_dll_create(j_dll_t *dll, void *data)
{
    struct j_dllnode *node;

    if (j_dll_is_full(dll)) {
	j_dll_errno = J_DLL_ERR_FULL_DLL;
	return -1;
    }

    J_CALL_AND_ASSIGNMENT_CHECK(j_dllnode_init, node, NULL, -1, data);

    if (j_dll_is_empty(dll)) {	/* dll is empty */
	j_dll_head(dll) = node;
	j_dll_tail(dll) = node;
    } else
	prv_j_dll_add_node(dll, node);
    j_dll_cnt(dll)++;
    return 0;
}

#include <stdio.h>
#include "../include/j_assert.h"
#include "../include/j_dll.h"
#include "../include/j_dll_in.h"

/* j_dll_update: update node data, origin to new */
int j_dll_update(j_dll_t *dll, void *origin, void *new)
{
    struct j_dllnode *target;

    J_CALL_AND_ASSIGNMENT_CHECK(dll->search, target, NULL, -1, dll, origin);
    prv_j_dll_unlink_node(dll, target);
    j_dllnode_update(target, new, j_dll_mtable(dll, UPDATE)->method);
    prv_j_dll_add_node(dll, target);
    return 0;
}
	

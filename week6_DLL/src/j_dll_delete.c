#include "../include/j_assert.h"
#include "../include/j_dll.h"
#include "../include/j_dll_in.h"

/* j_dll_delete: delete node from dll */
int j_dll_delete(j_dll_t *dll, void *data)
{
    struct j_dllnode *target;

    if (j_dll_is_empty(dll)) {
	j_dll_errno = J_DLL_ERR_EMPTY_DLL;
	return -1;
    }
    J_CALL_AND_ASSIGNMENT_CHECK(dll->search, target, NULL, -1, dll, data);
    prv_j_dll_unlink_node(dll, target);
    j_dllnode_delete(target);
    j_dll_cnt(dll)--;
    return 0;
}

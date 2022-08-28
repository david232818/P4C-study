#include <stdio.h>
#include <stdlib.h>
#include "../include/j_assert.h"
#include "../include/j_dll.h"

/* j_dll_init: initialize dll */
j_dll_t *j_dll_init(j_dll_method_t rm,
		    j_dll_method_t cm,
		    j_dll_method_t um)
{
    j_dll_t *dll;

    J_CALL_AND_ASSIGNMENT_CHECK(malloc, dll, NULL, NULL, sizeof(j_dll_t));
    j_dll_head(dll) = NULL;
    j_dll_tail(dll) = NULL;
    j_dll_cnt(dll) = 0;
    J_IF_GOTO_THEN_ELS(rm,NULL,ERROR,j_dll_mtable(dll, READ)->method,rm);
    J_IF_GOTO_THEN_ELS(cm,NULL,ERROR,j_dll_mtable(dll, COMPARE)->method,cm);
    J_IF_GOTO_THEN_ELS(um,NULL,ERROR,j_dll_mtable(dll, UPDATE)->method,um);
    dll->create = j_dll_create;
    dll->read = j_dll_read;
    return dll;

ERROR:
    free(dll);
    j_dll_errno = J_DLL_ERR_INVALID_METHOD;
    return NULL;
}

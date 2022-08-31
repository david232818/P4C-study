#ifndef __J_DLL_H__
#define __J_DLL_H__

#include <stddef.h>
#include <stdint.h>

#include "j_dllnode.h"

/* method function type */
typedef int (*j_dll_method_t)(void *, void *);

/* user defined dll method table */
struct j_dll_mtable {
    j_dll_method_t method;
};

#define J_DLL_MTABLE_CNT 3

/* user defined method indexes */
enum method_indexes {
    READ = 0,
    COMPARE = 1,
    UPDATE = 2
};

/* j_dll_t: double linked list (dll) */
typedef struct j_dll {
    struct j_dllnode *head;	/* head node of dll */
    struct j_dllnode *tail;	/* tail node of dll */

    size_t cnt;			/* node count of dll */

    /* user defined method table */
    struct j_dll_mtable mtable[J_DLL_MTABLE_CNT];

    int (*create)(struct j_dll *, void *);
    int (*read)(struct j_dll *);
    int (*update)(struct j_dll *, void *, void *);
    int (*delete)(struct j_dll *, void *);
    struct j_dllnode *(*search)(struct j_dll *, void *);
} j_dll_t;

enum j_dll_err_flags {
    J_DLL_ERR_FULL_DLL = 01,
    J_DLL_ERR_EMPTY_DLL = 02,
    J_DLL_ERR_INVALID_METHOD = 04
};

extern int j_dll_errno;

/*
 * user shall use these access interfaces rather than directly
 * accessing to the member variables of j_dll_t
 */
#define j_dll_head(dll) ((dll)->head)
#define j_dll_tail(dll) ((dll)->tail)
#define j_dll_cnt(dll) ((dll)->cnt)
#define j_dll_mtable(dll, op) ((dll)->mtable + (op))

#define j_dll_is_empty(dll) (j_dll_cnt((dll)) == 0 ? !0 : 0)
#define j_dll_is_full(dll) (j_dll_cnt((dll)) == SIZE_MAX ? !0 : 0)

/* j_dll_init: initialize dll */
extern j_dll_t *j_dll_init(j_dll_method_t read_method,
			   j_dll_method_t compare_method,
			   j_dll_method_t update_method);

/* j_dll_destroy: destroy the whole dll */
extern void j_dll_destroy(j_dll_t *dll);

/* j_dll_create: create node then add to dll */
extern int j_dll_create(j_dll_t *dll, void *data);

/* j_dll_read: print data in dll */
extern int j_dll_read(j_dll_t *dll);

/* j_dll_update: update node data, origin to new */
extern int j_dll_update(j_dll_t *dll, void *origin, void *new);

/* j_dll_delete: delete node from dll */
extern int j_dll_delete(j_dll_t *dll, void *delete_data);

/* j_dll_search: search node by given data */
extern struct j_dllnode *j_dll_search(j_dll_t *dll, void *search_data);

#endif

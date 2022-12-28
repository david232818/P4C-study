#ifndef __J_DLL_H__
#define __J_DLL_H__

#include <stdint.h>

#include "j_dllnode.h"

#define J_DLLNODE_MTABLE_LEN 3

enum j_dllnode_mtable_idx {
    J_DLLNODE_READ = 00,
    J_DLLNODE_UPDATE = 01,
    J_DLLNODE_COMPARE = 02
};

typedef struct _j_dll {
    struct j_dllnode *head, *tail;
    size_t cnt, data_offset;

    /*
     * User shall provide read, update, and compare methods. Since these will
     * applied to all nodes in the list, list structure should have the method
     * table.
     */
    struct j_dllnode_mtable {
	int (*method)(struct _j_dll *, struct j_dllnode *, void *);
    } mt[J_DLLNODE_MTABLE_LEN];

    int (*is_empty)(struct _j_dll *);
    int (*is_full)(struct _j_dll *);
    void *(*get_data)(struct _j_dll *, struct j_dllnode *);
    struct j_dllnode *(*get_node)(struct _j_dll *, void *);

    struct j_dllnode *(*search)(struct _j_dll *,
				struct j_dllnode *,
				void *,
				int);
    int (*read)(struct _j_dll *, struct j_dllnode *, void *);
    int (*update)(struct _j_dll *, void *, void *);
    int (*create)(struct _j_dll *, void *);
    int (*delete)(struct _j_dll *, void *);
} j_dll_t;

enum j_dll_errflags {
    J_DLL_ERR_ALLOC = 1,
    J_DLL_ERR_INVALID_METHOD = 2,
    J_DLL_ERR_INVALID_DLL = 4,
    J_DLL_ERR_INVALID_DATA = 8,
    J_DLL_ERR_WHILE_METHOD = 16,
    J_DLL_ERR_FULL = 32,
    J_DLL_ERR_EMPTY = 64
};

typedef int (*j_dllnode_method_t)(j_dll_t *, struct j_dllnode *, void *);

extern j_dll_t *j_dll_init(j_dllnode_method_t read,
			   j_dllnode_method_t update,
			   j_dllnode_method_t compare,
			   size_t data_offset);
extern void j_dll_destroy(j_dll_t *dll);

#endif

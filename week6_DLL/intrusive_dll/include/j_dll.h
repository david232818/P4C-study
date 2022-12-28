#ifndef __J_DLL_H__
#define __J_DLL_H__

#include <stdint.h>

#include "j_dllnode.h"

/* user defined method length and indexes */
#define J_DLLNODE_MTABLE_LEN 3

enum j_dllnode_mtable_idx {
    J_DLLNODE_READ = 00,
    J_DLLNODE_UPDATE = 01,
    J_DLLNODE_COMPARE = 02
};

/* list structure for intrusive double linked list */
typedef struct _j_dll {
    struct j_dllnode *head, *tail;
    size_t cnt;

    /*
     * In intrusive double linked list, we need offset of the node
     * structure to access the data from the node and the node from
     * the data. And user shall provide this.
     */
    size_t data_offset;

    /*
     * User shall provide read, update, and compare methods. Since these will
     * be applied to the all nodes in the dll, the dll structure should have
     * the method table.
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

enum j_dll_flags {
    J_DLL_ERR_ALLOC = 1,	/* malloc error */
    J_DLL_ERR_INVALID_METHOD = 2, /* NULL method */
    J_DLL_ERR_INVALID_DLL = 4,	  /* NULL dll */
    J_DLL_ERR_INVALID_DATA = 8,	  /* NULL data */
    J_DLL_ERR_WHILE_METHOD = 16,  /* error while doing the method */
    J_DLL_FULL = 32,	  /* dll is full */
    J_DLL_EMPTY = 64	  /* dll is empty */
};

/* type define for user defined method */
typedef int (*j_dllnode_method_t)(j_dll_t *, struct j_dllnode *, void *);

extern j_dll_t *j_dll_init(j_dllnode_method_t read,
			   j_dllnode_method_t update,
			   j_dllnode_method_t compare,
			   size_t data_offset);
extern void j_dll_destroy(j_dll_t *dll);

#endif

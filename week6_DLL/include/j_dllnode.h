#ifndef __J_DLLNODE_H__
#define __J_DLLNODE_H__

/* user shall include j_dll.h instead of this header */

struct j_dllnode {
    void *data;			/* user defined data */

    struct j_dllnode *prev;	/* previous node */
    struct j_dllnode *next;	/* next node */
};

#define j_dllnode_data(node) ((node)->data)
#define j_previous_node_of(node) ((node)->prev)
#define j_next_node_of(node) ((node)->next)

enum j_dllnode_err_flags {
    J_DLLNODE_ERR_INVALID_METHOD = 01,
    J_DLLNODE_ERR_INVALID_DATA = 02
};

extern int j_dllnode_errno;		/* store error flag */

/*
 * below functions conform following rules:
 *     - if return type is pointer, returning NULL pointer means error
 *     - if return type is int, returning -1 means error and
 *       j_dllnode_errno is setted to corresponding error code
 */

/* j_dllnode_init: initialize node of dll (double linked list) */
extern struct j_dllnode *j_dllnode_init(void *data);

/*
 * j_dllnode_update: update node data, orign to new
 *
 * returning -1 when error occurred at update_method is recommended for
 * consistency
 */
extern int j_dllnode_update(struct j_dllnode *node,
			    void *new,
			    int (*update_method)(void *origin, void *new));

/*
 * j_dllnode_delete: delete node
 *
 * j_dllnode_delete assumes that node data is allocated by malloc family
 */
extern int j_dllnode_delete(struct j_dllnode *node);

#endif

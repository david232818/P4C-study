#ifndef __J_DLLNODE_H__
#define __J_DLLNODE_H__

/* generic node structure for double linked list (dll) */
struct j_dllnode {
    void *data;

    struct j_dllnode *prev;
    struct j_dllnode *next;

    /*
     * We do not need read and update method here because all of the
     * nodes in a list will have the same method.
     */
};

enum j_dllnode_errflags {
    J_DLLNODE_ERR_NODE_ALLOC = 01
};

extern struct j_dllnode *j_dllnode_init(void *data);
extern void j_dllnode_destroy(struct j_dllnode *node);

#endif

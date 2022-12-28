#ifndef __J_DLLNODE_H__
#define __J_DLLNODE_H__

/* generic node structure for double linked list (dll) */
struct j_dllnode {
    struct j_dllnode *prev;
    struct j_dllnode *next;
};

#endif

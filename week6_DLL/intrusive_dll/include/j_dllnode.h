#ifndef __J_DLLNODE_H__
#define __J_DLLNODE_H__

/*
 * node structure for intrusive double linked list
 *
 * => user shall embed this structure in the last field of user defined data
 */
struct j_dllnode {
    struct j_dllnode *prev;
    struct j_dllnode *next;
};

#endif

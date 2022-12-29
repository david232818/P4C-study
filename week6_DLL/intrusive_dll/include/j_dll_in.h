#ifndef __J_DLL_IN_H__
#define __J_DLL_IN_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "j_dll.h"

/* User shall not include this header directly. Use j_dll.h header. */

/*
 * prv_ functions are not complete. They are called by the caller and
 * and have some assumptions that shall be satisfied.
 */

/* prv_j_dll_insert_node_at_the_end: insert the target at the end of the dll */
static void prv_j_dll_insert_node_at_the_end(j_dll_t *dll,
					  struct j_dllnode *curr,
					  struct j_dllnode *target)
{
    dll->tail = target;
    target->prev = curr;
    curr->next = target;
}

/* prv_j_dll_insert_node: insert the target in front of the curr */
static void prv_j_dll_insert_node(j_dll_t *dll,
			       struct j_dllnode *curr,
			       struct j_dllnode *target)
{
    if (curr == dll->head) {
	dll->head = target;
	target->next = curr;
	curr->prev = target;
	dll->tail = (dll->cnt == 1) ? curr : dll->tail;
    } else {
	target->prev = curr->prev;
	target->next = curr;
	curr->prev->next = target;
	curr->prev = target;
    }
}

/* prv_j_dll_insert: insert the node to the dll */
static void prv_j_dll_insert(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *target;

    if ((target = dll->search(dll,
			      dll->get_data(dll, node),
			      1)) == NULL)
	prv_j_dll_insert_node_at_the_end(dll, dll->tail, node);
    else
	prv_j_dll_insert_node(dll, target, node);
}

/* prv_j_dll_unlink_node: unlink the node from the dll */
static void prv_j_dll_unlink_node(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *head, *tail;

    head = dll->head;
    tail = dll->tail;
    if (head == tail) {		/* there is only one node in the dll */
	dll->head = NULL;
	dll->tail = NULL;
    } else if (node == head) {
	dll->head = node->next;
	node->next->prev = NULL;
	node->next = NULL;
    } else if (node == tail) {
	dll->tail = node->prev;
	node->prev->next = NULL;
	node->prev = NULL;
    } else {
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node->next = NULL;
    }
}

/* prv_j_dll_delete_node: delete the node from the dll */
static void prv_j_dll_delete_node(j_dll_t *dll, struct j_dllnode *node)
{
    prv_j_dll_unlink_node(dll, node);
    dll->cnt--;
    free(dll->get_data(dll, node));
}

/* prv_j_dll_search_node: search the start that makes cmpres with data */
static struct j_dllnode *prv_j_dll_search_node(j_dll_t *dll,
					       struct j_dllnode *start,
					       void *data,
					       int cmpres)
{
    if (start == NULL ||
	dll->mt[J_DLLNODE_COMPARE].method(dll, start, data) == cmpres)
	return start;
    prv_j_dll_search_node(dll, start->next, data, cmpres);
}

/* prv_j_dll_read_nodes: read the data and print it to the stdin or buf */
static int prv_j_dll_read_nodes(j_dll_t *dll,
			       struct j_dllnode *start,
			       void *buf)
{
    if (start == NULL)
	return 0;
    if (dll->mt[J_DLLNODE_READ].method(dll, start, buf) == -1)
	return -1;
    prv_j_dll_read_nodes(dll, start->next, buf);
}

#endif

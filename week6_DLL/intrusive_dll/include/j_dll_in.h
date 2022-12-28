#ifndef __J_DLL_IN_H__
#define __J_DLL_IN_H__

#include <stdio.h>
#include <stddef.h>
#include "j_dll.h"

/*
 * prv_ functions are basically not a complete function. prv_ functions
 * are called by a caller and have some assumptions. User shall not use
 * these functions directly.
 */

static void prv_j_dll_add_node_at_the_end(j_dll_t *dll,
					  struct j_dllnode *curr,
					  struct j_dllnode *target)
{
    dll->tail = target;
    target->prev = curr;
    curr->next = target;
}

static void prv_j_dll_add_node(j_dll_t *dll,
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

static void prv_j_dll_add(j_dll_t *dll, void *data)
{
    struct j_dllnode *target, *node;

    node = dll->get_node(dll, data);
    if ((target = dll->search(dll, dll->head, data, 1)) == NULL)
	prv_j_dll_add_node_at_the_end(dll, dll->tail, node);
    else
	prv_j_dll_add_node(dll, target, node);
}

/* prv_j_dll_unlink_node: unlink node from list */
static void prv_j_dll_unlink_node(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *head, *tail;

    head = dll->head;
    tail = dll->tail;
    if (head == tail) {
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

#endif

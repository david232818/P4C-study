#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "j_dll.h"
#include "j_error.h"

/* j_dll_is_empty: check the self is empty */
static int j_dll_is_empty(j_dll_t *self)
{
    return (self->cnt == 0);
}

/* j_dll_is_full: check the self is full */
static int j_dll_is_full(j_dll_t *self)
{
    return (self->cnt == SIZE_MAX);
}

/* j_dll_get_data: get the data from the node */
static void *j_dll_get_data(j_dll_t *self, struct j_dllnode *node)
{
    if (node == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_NODE;
	return NULL;
    }
    return ((char *) node - self->data_offset);
}

/* j_dll_get_node: get the node from the data */
static struct j_dllnode *j_dll_get_node(j_dll_t *self, void *data)
{
    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return NULL;
    }
    return (struct j_dllnode *) ((char *) data + self->data_offset);
}

/*
 * prv_ functions are not complete. They are called by the caller and
 * and have some assumptions that shall be satisfied.
 */

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

/* j_dll_search: search the start that makes cmpres with data */
static struct j_dllnode *j_dll_search(j_dll_t *self,
				      void *data,
				      int cmpres)
{
    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return NULL;
    }

    return prv_j_dll_search_node(self, self->head, data, cmpres);
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

/*
 * j_dll_read: read the data that the node is embedded and print it
 * to the stdin or buf
 */
static int j_dll_read(j_dll_t *self, struct j_dllnode *node, void *buf)
{
    if (node == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_NODE;
	return -1;
    }

    return prv_j_dll_read_nodes(self, self->head, buf);
}

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

/* j_dll_create: create the node in the self */
static int j_dll_create(j_dll_t *self, void *data)
{
    struct j_dllnode *node;

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    node = self->get_node(self, data);
    node->prev = NULL;
    node->next = NULL;
    if (self->is_empty(self)) {
	self->head = node;
	self->tail = node;
    } else
	prv_j_dll_insert(self, self->get_node(self, data));
    self->cnt++;
    return 0;
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

/* j_dll_delete: delete the node that has corresponding data from the self */
static int j_dll_delete(j_dll_t *self, void *data)
{
    struct j_dllnode *target;

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if ((target = self->search(self, data, 0)) == NULL)
	return -1;

    prv_j_dll_delete_node(self, target);
    return 0;
}

/* j_dll_update: update the data that has corresponding origin to new */
static int j_dll_update(j_dll_t *self, void *origin, void *new)
{
    struct j_dllnode *target;
    void *data;
    
    if (origin == NULL || new == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if ((target = self->search(self, origin, 0)) == NULL)
	return -1;

    prv_j_dll_unlink_node(self, target);
    if (self->mt[J_DLLNODE_UPDATE].method(self, target, new) == -1) {
	prv_j_dll_insert(self, target);
	return -1;
    }
    prv_j_dll_insert(self, target);
    return 0;
}

/* j_dll_init: initialize dll */
j_dll_t *j_dll_init(j_dllnode_method_t read,
		    j_dllnode_method_t update,
		    j_dllnode_method_t compare,
		    size_t data_offset)
{
    j_dll_t *dll;

    if (read == NULL || update == NULL || compare == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_METHOD;
	return NULL;
    }

    if ((dll = malloc(sizeof(*dll))) == NULL) {
	j_dll_errno = J_DLL_ERR_ALLOC;
	return NULL;
    }

    dll->head = NULL;
    dll->tail = NULL;
    dll->cnt = 0;
    dll->data_offset = data_offset;
    dll->mt[J_DLLNODE_READ].method = read;
    dll->mt[J_DLLNODE_UPDATE].method = update;
    dll->mt[J_DLLNODE_COMPARE].method = compare;
    dll->is_empty = j_dll_is_empty;
    dll->is_full = j_dll_is_full;
    dll->get_data = j_dll_get_data;
    dll->get_node = j_dll_get_node;
    dll->search = j_dll_search;
    dll->read = j_dll_read;
    dll->update = j_dll_update;
    dll->create = j_dll_create;
    dll->delete = j_dll_delete;
    return dll;
}

/* j_dll_destroy: destroy dll */
void j_dll_destroy(j_dll_t *dll)
{
    struct j_dllnode *target;

    if (dll == NULL)
	return ;
    
    if (dll->is_empty(dll)) {
	free(dll);
	return ;
    }

    target = dll->head;
    while (target->next != NULL) {
	target = target->next;
	prv_j_dll_delete_node(dll, target->prev);
    }
    prv_j_dll_delete_node(dll, target);
    free(dll);
}

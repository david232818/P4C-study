#include <stdio.h>
#include <stdlib.h>
#include "j_dll.h"
#include "j_dll_in.h"
#include "j_error.h"

/* j_dll_is_empty: check the list is empty */
static int j_dll_is_empty(j_dll_t *self)
{
    return (self->cnt == 0);
}

/* j_dll_is_full: check the list is full */
static int j_dll_is_full(j_dll_t *self)
{
    return (self->cnt == SIZE_MAX);
}

/* j_dll_create: make node and add to the list */
static int j_dll_create(j_dll_t *self, void *data)
{
    struct j_dllnode *node;
    
    if (self == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return -1;
    }

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if (self->is_full(self)) {
	j_dll_errno = J_DLL_ERR_FULL;
	return -1;
    }

    if ((node = j_dllnode_init(data)) == NULL) {
	j_dll_errno = J_DLL_ERR_ALLOC;
	return -1;
    }

    if (self->cnt == 0) {
	self->head = node;
	self->tail = node;
    } else
	prv_j_dll_add(self, node);
    self->cnt++;
    return 0;
}

/*
 * j_dll_delete: remove node that has corresponding data from the list and
 *               free it
 */
static int j_dll_delete(j_dll_t *self, void *data)
{
    struct j_dllnode *target;

    if (self == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return -1;
    }

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if (self->is_empty(self)) {
	j_dll_errno = J_DLL_ERR_EMPTY;
	return -1;
    }

    if ((target = self->search(self, data)) == NULL) {
	j_dll_errno = J_DLL_ERR_WHILE_METHOD;
	return -1;
    }
    prv_j_dll_unlink_node(self, target);
    j_dllnode_destroy(target);
    self->cnt--;
    return 0;
}

/* j_dll_read: read node data in self by user defined read method */
static int j_dll_read(j_dll_t *self, void *buf)
{
    struct j_dllnode *curr_node;

    if (self == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return -1;
    }

    /*
     * NULL buf is not an error here. But the application should handle
     * the NULL buf case.
     */

    curr_node = self->head;
    while (curr_node != NULL) {
	if (self->mt[J_DLLNODE_READ].method(curr_node, buf) == -1)
	    return -1;
	curr_node = curr_node->next;
    }
    return 0;
}

/* j_dll_search: search node that has corresponding data in self */
static struct j_dllnode *j_dll_search(j_dll_t *self, void *data)
{
    struct j_dllnode *curr_node;

    if (self == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return NULL;
    }

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return NULL;
    }
    
    curr_node = self->head;
    while (curr_node != NULL) {
	if (self->mt[J_DLLNODE_COMPARE].method(curr_node, data) == 0)
	    break;
	curr_node = curr_node->next;
    }
    return curr_node;
}

/* j_dll_update: update node that has corresponding data (=origin) to new */
static int j_dll_update(j_dll_t *self, void *origin, void *new)
{
    struct j_dllnode *curr_node;

    if (self == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return -1;
    }

    if (origin == NULL || new == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }
    
    if ((curr_node = self->search(self, origin)) == NULL)
	return -1;
    prv_j_dll_unlink_node(self, curr_node);
    if (self->mt[J_DLLNODE_UPDATE].method(curr_node, new) == -1) {
	j_dll_errno = J_DLL_ERR_WHILE_METHOD;
	return -1;
    }
    self->mt[J_DLLNODE_READ].method(curr_node, NULL);
    prv_j_dll_add(self, curr_node);
}

/* j_dll_init: initialize dll (double linked list) structure */
j_dll_t *j_dll_init(j_dllnode_method_t read,
		    j_dllnode_method_t update,
		    j_dllnode_method_t compare)
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
    dll->mt[J_DLLNODE_READ].method = read;
    dll->mt[J_DLLNODE_UPDATE].method = update;
    dll->mt[J_DLLNODE_COMPARE].method = compare;
    dll->is_empty = j_dll_is_empty;
    dll->is_full = j_dll_is_full;
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
    struct j_dllnode *curr_node, *head, *tail;
    size_t cnt;
    
    if (dll == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DLL;
	return;
    }

    head = dll->head;
    tail = dll->tail;
    if ((cnt = dll->cnt) == 0)
	free(dll);
    else if (cnt == 1) {
	dll->delete(dll, head->data);
	free(dll);
    } else {
	curr_node = head;
	while (curr_node->next != NULL) {
	    curr_node = curr_node->next;
	    dll->delete(dll, curr_node->prev->data);
	}
	dll->delete(dll, curr_node->data);
	free(dll);
    }
}

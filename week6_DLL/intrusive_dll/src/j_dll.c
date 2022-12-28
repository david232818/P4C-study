#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "j_dll.h"
#include "j_dll_in.h"
#include "j_error.h"

static int j_dll_is_empty(j_dll_t *self)
{
    return (self->cnt == 0);
}

static int j_dll_is_full(j_dll_t *self)
{
    return (self->cnt == SIZE_MAX);
}

static void *j_dll_get_data(j_dll_t *self, struct j_dllnode *node)
{
    return ((char *) node - self->data_offset);
}

static struct j_dllnode *j_dll_get_node(j_dll_t *self, void *data)
{
    return (struct j_dllnode *) ((char *) data + self->data_offset);
}

static struct j_dllnode *j_dll_search(j_dll_t *self,
				      struct j_dllnode *start,
				      void *data,
				      int cmpres)
{
    if (start == NULL)
	return NULL;
    if (self->mt[J_DLLNODE_COMPARE].method(self, start, data) == cmpres)
	return start;
    return j_dll_search(self, start->next, data, cmpres);
}

static int j_dll_read(j_dll_t *self, struct j_dllnode *node, void *buf)
{
    if (node == NULL)
	return -1;
    if (self->mt[J_DLLNODE_READ].method(self, node, buf) == -1)
	return -1;
    return j_dll_read(self, node->next, buf);
}

static int j_dll_update(j_dll_t *self, void *origin, void *new)
{
    struct j_dllnode *target;
    void *data;
    
    if (origin == NULL || new == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if ((target = self->search(self, self->head, origin, 0)) == NULL)
	return -1;

    prv_j_dll_unlink_node(self, target);
    if (self->mt[J_DLLNODE_UPDATE].method(self, target, new) == -1) {
	prv_j_dll_add(self, self->get_data(self, target));
	return -1;
    }
    prv_j_dll_add(self, self->get_data(self, target));
    return 0;
}

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
	prv_j_dll_add(self, data);
    self->cnt++;
    return 0;
}

static int j_dll_delete(j_dll_t *self, void *data)
{
    struct j_dllnode *target;

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if ((target = self->search(self, self->head, data, 0)) == NULL)
	return -1;

    prv_j_dll_unlink_node(self, target);
    self->cnt--;
    free(self->get_data(self, target));
    return 0;
}

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

void j_dll_destroy(j_dll_t *dll, struct j_dllnode *start)
{
    if (start == NULL)
	return ;
    j_dll_destroy(dll, start->next);
    dll->delete(dll, dll->get_data(dll, start));
}

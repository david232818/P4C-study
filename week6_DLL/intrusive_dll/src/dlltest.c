#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include "j_dll.h"

typedef struct _kv {
    int key;
    char value[BUFSIZ];

    struct j_dllnode node;
} kv_t;

int kv_read(j_dll_t *dll, struct j_dllnode *node, void *buf);
int kv_update(j_dll_t *dll, struct j_dllnode *node, void *new);
int kv_compare(j_dll_t *dll, struct j_dllnode *node, void *cmp);

int main()
{
    j_dll_t *dll;

    dll = j_dll_init(kv_read, kv_update, kv_compare, offsetof(kv_t, node));

    kv_t *data = malloc(sizeof(*data));
    data->key = 201821255;
    strncpy(data->value, "Jung Kown Young", strlen("Jung Kown Young"));
    dll->create(dll, data);
    printf("---[ Stage 1: create\n");
    dll->read(dll, dll->head, NULL);

    data = malloc(sizeof(*data));
    data->key = 201821257;
    strncpy(data->value, "Kown Young Jung", strlen("Kown Young Jung"));
    dll->create(dll, data);
    printf("\n---[ Stage 2: create\n");
    dll->read(dll, dll->head, NULL);

    data = malloc(sizeof(*data));
    data->key = 201821244;
    strncpy(data->value, "Kown Young Jung", strlen("Kown Young Jung"));
    dll->create(dll, data);
    printf("\n---[ Stage 3: create\n");
    dll->read(dll, dll->head, NULL);

    kv_t ud1, ud2;

    ud1.key = 201821255;
    ud2.key = 201821278;
    strncpy(ud2.value, "haha", strlen("haha"));
    dll->update(dll, &ud1, &ud2);
    printf("\n---[ Stage 4: update\n");
    dll->read(dll, dll->head, NULL);

    ud1.key = 201821257;
    dll->delete(dll, &ud1);
    printf("\n---[ Stage 5: delete\n");
    dll->read(dll, dll->head, NULL);

    return 0;
}

int kv_read(j_dll_t *dll, struct j_dllnode *node, void *buf)
{
    kv_t *data;

    if (dll == NULL || node == NULL)
	return -1;
    data = dll->get_data(dll, node);
    printf("[ %d => %s ]\n", data->key, data->value);
    return 0;
}

int kv_update(j_dll_t *dll, struct j_dllnode *node, void *data)
{
    kv_t *target, *new;

    if (dll == NULL || node == NULL || data == NULL)
	return -1;
    target = dll->get_data(dll, node);
    new = data;
    target->key = new->key;
    strncpy(target->value, new->value, strlen(target->value));
    return 0;
}

int kv_compare(j_dll_t *dll, struct j_dllnode *node, void *data)
{
    kv_t *target, *cmp;
    int cmpres, res;

    target = dll->get_data(dll, node);
    cmp = data;
    cmpres = target->key - cmp->key;
    if (cmpres > 0)
	res = 1;
    else if (cmpres < 0)
	res = -1;
    else
	res = 0;
    return res;
}

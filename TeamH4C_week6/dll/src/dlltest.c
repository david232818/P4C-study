#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "j_dll.h"

typedef struct _kv {
    int key;
    char value[BUFSIZ];
} kv_t;

int cmd_analyzer(j_dll_t *dll);

int kv_read(struct j_dllnode *node, void *data);
int kv_update(struct j_dllnode *node, void *new);
int kv_compare(struct j_dllnode *node, void *cmp);

int main()
{
    j_dll_t *dll;

    dll = j_dll_init(kv_read, kv_update, kv_compare);
    
    printf("$ ");
    while (cmd_analyzer(dll) != 1)
	printf("$ ");
    j_dll_destroy(dll);
    return 0;
}

#define CMD_CNT 5
#define CMD_DELIM " \n"

int cmd_analyzer(j_dll_t *dll)
{
    char *cmdline, *token;
    enum {
	CMD_SEARCH = 0, CMD_READ = 1, CMD_UPDATE, CMD_CREATE,
	CMD_DELETE
    };
    char *cmdtab[CMD_CNT] = {
	"search", "read", "update", "create",
	"delete"
    };
    size_t len;
    ssize_t nread;
    kv_t kv[2], *data;
    int cmdidx, i, res;

    cmdline = NULL;
    if ((nread = getline(&cmdline, &len, stdin)) == -1) {
	free(cmdline);
	return -1;
    }

    if (nread < 4) {
	free(cmdline);
	return -1;
    }

    if (strcmp(cmdline, "quit\n") == 0) {
	free(cmdline);
	return 1;
    }

    token = strtok(cmdline, CMD_DELIM);
    cmdidx = -1;
    for (i = 0; i < CMD_CNT; i++)
	if (strcmp(token, cmdtab[i]) == 0)
	    cmdidx = i;
    if (cmdidx == -1)
	return -1;
    switch (cmdidx) {
    case CMD_SEARCH:
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    break;
	}
	kv[0].key = atoi(token);
	res = dll->mt[J_DLLNODE_READ].method(dll->search(dll, &kv[0]), NULL);
	break;
    case CMD_READ:
	res = dll->read(dll, NULL);
	printf("count: %ld\n", dll->cnt);
	break;
    case CMD_UPDATE:
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    break;
	}
	kv[0].key = atoi(token);
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    break;
	}
	kv[1].key = atoi(token);
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    break;
	}
	strncpy(kv[1].value, token, strlen(token));
	res = dll->update(dll, &kv[0], &kv[1]);
	break;
    case CMD_CREATE:
	data = malloc(sizeof(*data));

	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    free(data);
	    break;
	}
	data->key = atoi(token);
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    free(data);
	    break;
	}
	strncpy(data->value, token, strlen(token));
	res = dll->create(dll, data);
	break;
    case CMD_DELETE:
	if ((token = strtok(NULL, CMD_DELIM)) == NULL) {
	    res = -1;
	    break;
	}
	kv[0].key = atoi(token);
	res = dll->delete(dll, &kv[0]);
	break;
    default:
	break;
    }
    free(cmdline);
    return res;
}

int kv_read(struct j_dllnode *node, void *data)
{
    kv_t *kv;

    if (node == NULL)
	return -1;
    kv = node->data;
    printf("[ %d => %s ]\n", kv->key, kv->value);
    return 0;
}
    
int kv_update(struct j_dllnode *node, void *new)
{
    kv_t *kv, *newkv;

    kv = node->data;
    newkv = new;
    kv->key = newkv->key;
    strncpy(kv->value, newkv->value, BUFSIZ);
    return 0;
}

int kv_compare(struct j_dllnode *node, void *cmp)
{
    kv_t *kv, *cmpkv;

    kv = node->data;
    cmpkv = cmp;
    if (kv->key > cmpkv->key)
	return 1;
    else if (kv->key < cmpkv->key)
	return -1;
    else
	return 0;
}

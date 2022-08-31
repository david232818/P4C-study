#include <stdio.h>
#include <stdlib.h>
#include "../include/j_dll.h"

#define deref_data(data, type) (*((type *) (data)))

int read_method(void *, void *);
int compare_method(void *, void *);
int update_method(void *, void *);

/* dll library test program */
int main()
{
    j_dll_t *list;
    struct j_dllnode *searched;
    int *tmpdata, searchdata, origin;

    if ((list = j_dll_init(read_method,
			  compare_method,
			  update_method)) == NULL)
	return -1;
    printf("list init successful..\n");

    tmpdata = malloc(sizeof(int));
    *tmpdata = 12;
    if (list->create(list, tmpdata) == -1)
	return -1;

    tmpdata = malloc(sizeof(int));
    *tmpdata = 10;
    if (list->create(list, tmpdata) == -1)
	return -1;
 
    tmpdata = malloc(sizeof(int));
    *tmpdata = 24;
    if (list->create(list, tmpdata) == -1)
	return -1;

    tmpdata = malloc(sizeof(int));
    *tmpdata = 23;
    if (list->create(list, tmpdata) == -1)
	return -1;

    tmpdata = malloc(sizeof(int));
    *tmpdata = 100;
    if (list->create(list, tmpdata) == -1)
	return -1;

    printf("node count: %zu\n", j_dll_cnt(list));
    list->read(list);

    searchdata = 12;
    if ((searched = list->search(list, &searchdata)) == NULL)
	printf("Not found..\n");
    printf("found: %d\n", deref_data(j_dllnode_data(searched), int));

    if (list->delete(list, &searchdata) == -1)
	printf("Not deleted..\n");
    printf("deleted..\n");

    printf("node count: %zu\n", j_dll_cnt(list));
    list->read(list);

    origin = 24;
    searchdata = 9;
    if (list->update(list, &origin, &searchdata) == -1)
	printf("Not updated..\n");
    printf("update successful..\n");

    printf("node count: %zu\n", j_dll_cnt(list));
    list->read(list);

    j_dll_destroy(list);
    printf("list is destroyed..\n");
    return 0;
}

/* read_method: dereference method for data */
int read_method(void *data, void *nad)
{
    (void) nad;			/* second argument is not used here */
    
    printf("[\tdata: %04d\t]\n", deref_data(data, int));
    return 0;
}

/* compare_method: compare d1 and d2 */
int compare_method(void *d1, void *d2)
{
    int data1, data2;

    data1 = deref_data(d1, int);
    data2 = deref_data(d2, int);
    return (data1 - data2);
}

int update_method(void *d1, void *d2)
{
    return (deref_data(d1, int) = deref_data(d2, int));
}
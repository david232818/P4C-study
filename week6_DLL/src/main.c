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
    int *tmpdata;

    if ((list = j_dll_init(read_method,
			  compare_method,
			  update_method)) == NULL)
	return -1;
    printf("list init successful..\n");

    tmpdata = malloc(sizeof(int));
    *tmpdata = 2;
    list->create(list, tmpdata);

    tmpdata = malloc(sizeof(int));
    *tmpdata = 10;
    list->create(list, tmpdata);
 
    tmpdata = malloc(sizeof(int));
    *tmpdata = 6;
    list->create(list, tmpdata);

    list->read(list);
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

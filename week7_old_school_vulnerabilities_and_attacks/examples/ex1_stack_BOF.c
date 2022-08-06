#include <stdio.h>
#include <string.h>

/* gcc -fno-stack-protector ex1_stack_BOF.c -o ex1_stack_BOF */
int main()
{
    char A[12];
    int B;

    B = 0;
    gets(A);
    if (strcmp(A, "Password") == 0)
	B = 1;

    if (B)
	printf("Access Granted..\n");
    else
	printf("Access Denied..\n");
    return 0;
}

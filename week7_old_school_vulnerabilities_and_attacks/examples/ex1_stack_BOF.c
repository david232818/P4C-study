#include <stdio.h>
#include <string.h>

int main()
{
    char A[12];
    int B;

    gets(A);
    B = 0;
    if (strcmp(A, "Password") == 0)
	B = 1;

    if (B)
	printf("Access Granted..\n");
    else
	printf("Access Denied..\n");
    return 0;
}

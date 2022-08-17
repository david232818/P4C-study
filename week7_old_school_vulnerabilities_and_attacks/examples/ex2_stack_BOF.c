#include <stdio.h>

/* gcc
 *     -fno-stack-protector -no-pie -z execstack
 *     ex2_stack_BOF.c -o ex2_stack_BOF
 */
int main()
{
    char buf[12];

    gets(buf);
    return 0;
}

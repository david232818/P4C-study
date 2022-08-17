/* gcc ex2_find_machine_code.c -o ex2_find_machine_code */
int main()
{
    __asm__ __volatile__ (
	/* for 32-bit system: "jmp *%esp\n\t" */
	"jmp *%rsp\n\t"
	);
    return 0;
}

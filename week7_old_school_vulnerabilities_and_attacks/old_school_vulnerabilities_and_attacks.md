Old School Vulnerabilities And Attacks
======================================


Contents
--------
1. 버퍼 오버플로우
   1. 변수가 스택에 배치되는 순서
   2. 쉘코드로 점프하기
2. 형식 문자열 버그
3. 메모리 쓰기 프리미티브
4. References



# 버퍼 오버플로우
 버퍼 (buffer)는 [3, Ch. 8.5.]가 설명하는 전형적인 표준 입출력의 구현에서 볼 수 있듯이 데이터를 이동시킬 때 일시적으로 보관하기 위한 메모리이다. 이러한 버퍼의 필요성은 [4]가 입증하고, 그것은 상대적으로 매우 큰 양의 출력 데이터를 효율적으로 표기하는 문제가 자기 기록 장치라는 버퍼를 출력 장치로 제공하였을 때 해결됨을 보인 것이다. 이는 버퍼의 초기 구현으로 볼 수 있고, 큰 데이터의 이동을 효율적으로 수행한다는 목적은 오늘날까지 이어지고 있다.
 
# References
[1] ISO/IEC JTC 1/SC 22/WG 14, "ISO/IEC 9899:1999, Programming languages -- C", ISO/IEC, 1999

[2] James van Artsdalen, et al., "GCC, the GNU Compiler Collection", Free Software Foundation, Inc., 2022. [Online]. Available: https://gcc.gnu.org/, [Accessed Jun. 04, 2022]

[3] Brian W. Kernighan, Dennis M. Ritchie, "The C Programming Language Second Edition", AT&T Bell Laboratories, 1988

[4] Russel A. Kirsch, "SEAC MAINTENANCE MANUAL THE OUTSCRIBER", NATIONAL BUREAU OF STANDARDS, 1953

[5] Aleph One, "Smashing The Stack For Fun And Profit", Phrack Volume 7, Issue 49, File 14, 1996

[6] Turing, A. M., "Proposals for Development in Mathematics Division of an Automatic Computing Engine (ACE), Report E882", Executive Committee, NPL, 1945

[7] B. E. Carpenter and R. W. Doran, "The other Turing machine", Department of Computer Science, Massey University, 1975

[8] Etoh, Hiroaki, "Gcc extension for protecting applications from stack-smashing attacks", IBM, 2004

[9] "SYSTEM V APPLICAITON BINARY INTERFACE Intel386 Architecture Processor Supplement", The Santa Cruz Operation, Inc.

[10] Michael Kerrisk, "printf(3) -- Linux manual page", man7.org, 2021. [Online]. Available: https://man7.org/linux/man-pages/man3/printf.3.html

[11] Barton P. Miller, et al., "An empirical study of the reliability of UNIX utilities", Communications of the ACM, Volume 33, Issue 12, 1990

[12] Michael Matz, et al., "System V Application Binary Interface AMD64 Arhitecture Processor Supplement Draft"

[13] gera, "Advances in format string exploitation", Phrack Issue 0x3b, Phile 0x07, 2002

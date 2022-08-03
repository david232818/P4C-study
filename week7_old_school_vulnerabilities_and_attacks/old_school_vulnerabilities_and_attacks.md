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
 버퍼 (buffer)는 [3, Ch. 8.5.]가 설명하는 전형적인 표준 입출력의 구현에서 볼 수 있듯이 데이터를 이동시킬 때 일시적으로 보관하기 위한 메모리이다. 이러한 버퍼의 필요성은 [4]가 입증하고, 그것은 상대적으로 매우 큰 양의 출력 데이터를 효율적으로 표기하는 문제가 자기 기록 장치라는 버퍼를 출력 장치로 제공하였을 때 해결됨을 보인 것이다. 이는 버퍼의 초기 구현으로 볼 수 있고, 큰 데이터의 이동을 효율적으로 수행한다는 목적은 오늘날에도 그대로 적용된다.
 
 버퍼 오버플로우 취약점은 프로그램이 버퍼에 저장될 것이라고 기대하는 크기를 초과하는 양의 데이터를 입력할 수 있을 때 발생하는 취약점이다. 이 취약점을 공격하는 방법은 버퍼가 어떤 메모리 영역에 위치하는지에 따라 다르다. 예를 들어, [5]는 스택 버퍼 오버플로우 취약점을 공격하는 방법을 설명한다.
 
## 변수가 스택에 배치되는 순서
 [6]은 하위 연산 (subsidiary operation)을 시작하고자 할 때 상위 연산 (major operation)으로부터 벗어나고, 하위 연산이 종료되었을 때 상위 연산을 계속 진행하는 방법을 설명하기 위해 상위 연산 탈출 지점 등이 적힌 노트인 딜레이 라인 (delay lines)을 제시한다. 그리고 가장 최근의 노트를 가리키는 것으로 TS를 두며 이는 하위 연산이 실행되고 종료됨에 따라 수정된다고 설명한다. 이때 수정의 방법은 하위 연산이 실행될 때 노트에 탈출 지점을 적고 (burying), 하위 연산이 종료될 때 노트에서 탈출 지점을 없애는 (disinterring) 것으로, 상기의 두 동작을 수행하는 명령어를 각각 BURY, UNBURY로 정의한다. [6]이 설명하는 개념과 동작은 현대의 스택과 크게 다르지 않고, 이들을 현대 스택에 비추어보면 딜레이 라인이 스택 메모리에 대응되고, TS가 스택 포인터에 대응되며, BURY, UNBURY는 각각 PUSH, POP에 대응됨을 알 수 있다.

```
Delay Line: [ Note1 ][ Note2 ][ Note3 ] ...
            ^
			|
            +--TS
 ```
 
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

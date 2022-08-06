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
			
BURY:
    Delay Line: [ New Note ][ Note1 ][ Note2 ][ Note3 ] ...
                ^
                |
                +--TS

UNBURY:
    Delay Line: [ Note1 ][ Note2 ][ Note3 ] ...
                ^
                |
                +--TS
    Disintered: [ New Note ]
```
 
이렇게 마지막에 들어간 것이 첫 번째로 나오는 형태를 후입선출 (Last In First Out, LIFO)이라고 부르고, 스택은 대표적인 후입선출 자료구조이다.

 이러한 스택은 주로 함수의 호출과 리턴을 구현할 때 사용된다. 물론, [1]은 함수의 호출과 리턴을 스택으로 구현해야 함을 명시하지는 않는다. 그러나 대부분의 C 컴파일러 (또는 C 구현체, C implementation)들은 함수의 호출과 리턴을 구현할 때 스택을 사용한다는 것이다. 따라서 스택의 요소들은 프로그램이 구동되는 기계와 언어의 구현체에 의존하게 된다. 이러한 스택의 요소 중에서 대표적인 것이 바로 지역 변수이다. 지역 변수는 대부분 스택에 배치되지만 [1]은 이를 명시하고 있지 않다. [1]이 명시하는 것은 지역 변수의 선언 시 키워드, 선언 시 코드상에서의 위치 등에 따른 기억수명 (Storage duration) 뿐이다. 즉, [1]은 스택의 요소에 대해 구현체가 (표준에 순응한다면) 마땅히 해야만 하는 동작들을 기술하고, 구현체는 [1]에 명시된 동작을 나름의 방법 (스택 등)으로 구현한다는 것이다.
 
 상기에 서술하였듯이 스택에는 프로그램의 실행 흐름에 관여하는 데이터 (하위 연산이 실행될 때의 탈출 지점, 리턴 주소 등)가 존재하고 이 데이터를 어떻게 스택에 배치할지 결정하는 것은 컴파일러 (구현체)의 몫이다. 예를 들어, 프로그램에서 사용자 입력을 받아들이는 배열 A (12 바이트)가 있고, A의 값에 따라 다른 값을 가지는 변수 B (4 바이트)가 있으며, A와 B가 다음과 같이 스택에 배치되었다고 가정하자.
 
```
(LOW) ... [AAAAAAAAAAAA][BBBB] ... (HIGH)
```

그리고 다음과 같은 코드가 동작하는 중이라고 가정하자.

```C
#include <stdio.h>
#include <string.h>

/* gcc -fno-stack-protector ex1_stack_BOF.c -o ex1_stack_BOF */
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
```

위 코드에서 사용자는 12 바이트를 초과하는 데이터를 입력할 수 있다 (오버플로우). 그리고 초과된 데이터는 기존에 존재하던 데이터를 덮어쓰게 된다. 이를 위의 스택 배치에 비추어서 생각해보면 사용자가 입력한, 12 바이트를 초과한 데이터는 변수 B의 값을 조작할 수 있다는 것을 알 수 있다. 여기서 B는 if (B) { ... } else { ... } 블록에 따른, 프로그램의 실행 흐름에 관여하는 변수이다. 즉, 사용자는 오버플로우를 발생시켜 프로그램의 실행 흐름을 조작할 수 있다는 것이다.

 앞의 예시는 스택 배치가 가정한대로 구성되어야만 성립한다. 하지만 이전에 설명했듯이 스택 배치의 구성은 컴파일러가 관심가지는 사항이고, 컴파일러마다 다르기 때문에 위에서 가정한대로 스택 배치를 구성하는 컴파일러로 컴파일된 프로그램에서만 성립하게 된다. 이렇게 (특정 컴파일러상에서만 성립함에도) 사용자 입력을 받는 변수가 실행 흐름에 관여하는 변수의 앞에 배치되어 발생하는 문제는 [8]이 해결책을 제시하여 일부 해결되었다. 여기서 [8]이 제시한 해결책은 지역변수의 배치 순서를 조정하는, 즉 사용자 입력을 받아들이는 변수가 다른 변수의 뒤에 위치하도록 만드는 SSP (Stack Smashing Protector)이다. 그리고 현대의 컴파일러 대부분은 이 기능을 제공한다. 그래서 위 예시 코드를 컴파일하고 실행시켜서 오버플로우 공격을 수행해도 원하는 결과가 나오지 않을 수 있다.
 
## 쉘코드로 점프하기

### 스택 프레임
 [9, p. 36]은 다음과 같은 표준 스택 프레임을 제시한다.
 
```
Position         Contents          Frame
--------------+-----------------+---------- High address
4n + 8 (%ebp) | argument word n | Previous
 ...          | ...             |
8 (%ebp)      | argument word 0 |
--------------+-----------------+----------
4 (%ebp)      | return address  | Current
              +-----------------+
0 (%ebp)      | previous %ebp   |
              | (optional)      |
              +-----------------+
-4 (%ebp)     | unspecified     |
 ...          | ...             |
0 (%esp)      | variable size   |
--------------+-----------------+---------- Low address
```

그리고 [9, pp. 39 - 40]는 함수 프롤로그와 에필로그의 예시를 제시한다.

```
prologue:
	push %ebp		/ save frame pointer
	movl %esp, %ebp	/save new frame pointer
	subl $80, %esp	/ allocate stack space
	pushl %edi		/ save local register
	pushl %esi		/ save local register
	pushl %ebx		/ save local register
	movl %edi, %eax	/ set up return value
```

```
epilogue:
	popl %ebx	/ restore local register
	popl %esi	/ restore local register
	popl %edi	/ restore local register
	leave		/ restore frame pointer
	ret 		/ pop return address
```	

위 어셈블리어 코드에서 확인할 수 있듯이 함수 프롤로그와 에필로그는 함수의 정상적인 호출과 리턴을 위한 작업들을 포함한다. 그리고 스택 프레임에는 함수의 정상적인 호출과 리턴을 위한 정보들이 저장된다. 즉, 프로그램의 실행 흐름에 보다 직접적으로 관여하는 정보가 스택 프레임에 저장된다는 것이다.

### 쉘코드로 점프하기
 앞에서 스택 프레임에 저장되는 정보가 프로그램의 실행 흐름에 깊게 관여한다고 설명하였다. 이 중에서 대표적인 것이 바로 리턴 주소 (Return Address)이다. 리턴 주소는 함수 에필로그에서 ret 명령어를 통해 pop되고 이는 명령 포인터 (Instruction Pointer)에 로드된다. 그리고 이렇게 로드된 리턴 주소에 위치한 명령어가 실행된다. 이때 리턴 주소를 조작하여 공격 코드의 주소 또는 공격 코드의 주소를 최종적으로 명령 포인터에 로드하는 코드의 주소로 변경할 수 있다면, 프로그램의 실행 흐름을 바꿀 수 있다.
 
 예를 들어, 다음과 같이 구성된 스택이 존재한다고 가정하자.
 
```
(LOW) ... [BBBBBBBBBBBB][FFFF][RRRR][SSSSSSSS...] ... (HIGH)

@ B: Input Buffer
@ F: Saved Frame Pointer
@ R: Return Address
@ S: Shell Code

@ 위 그림에서 대괄호 사이에 위치한 알파벳 문자 하나는 1 바이트를 의미함
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

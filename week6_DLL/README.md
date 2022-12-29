이중 연결 리스트
=============


Contents
--------
1. 개요
2. 이중 연결 리스트
3. 구현 방식
   1. 노드 구성: non-intrusive vs intrusive
   2. 노드를 연결하는 방법
   3. CRUD 구현
   4. 사용자 인터페이스
4. 사용자 메뉴얼
   1. j_dllnode_init 함수
   2. j_dllnode_destroy 함수
   3. j_dllnode_errno
   4. j_dll_init 함수
   5. j_dll_destroy 함수
   6. j_dll_errno
References



# 개요
 빡공팟 6주차 과제는 C 언어를 사용하여 일반적인 (generic) 데이터를 저장할 수 있는 이중 연결 리스트를 구현하는 것이다.
 
# 이중 연결 리스트
 이중 연결 리스트 (Double Linked List)는 [1, Sec. 3.1]이 설명하는 연결된 자료 구조 (Linked Data Structures)에 속하는 단일 연결 리스트 (Single Linked List)에서 이전 노드를 가리키는 것이 추가된 자료구조이다. 이를 그림으로 표현하면 다음과 같다.
 
```
    +----+    +----+    +----+
<---|node|<---|node|<---|node|--->
    |    |--->|    |--->|    |
    +----+    +----+    +----+
```

위와 같은 구조에서 첫 번째 노드와 마지막 노드는 각각 head, tail이라고 불리며 접근하기 위한 변수가 별도로 존재하여 접근하는데 O(1)이 소요된다. 그리고 이전 노드와 다음 노드를 가리키는 변수는 prev (previous)와 next 또는 bk (back)와 fd (forward)로 표현된다. 이렇게 이전 노드와 다음 노드를 가리키는 부분이 존재하기 때문에 단일 연결 리스트와 다르게 순회하는 방향을 조절할 수 있게 된다. 즉, 앞에서부터 순회할 수도 있고, 뒤에서부터 순회할 수도 있다. 이는 단일 연결 리스트에서의 오름차순과 내림차순 문제를 정렬 문제에서 읽는 순서의 문제로 단순화 시킨다.

# 구현 방식
 여기서는 C 언어를 사용하여 이중 연결 리스트를 구현한다. 이때 노드를 어떻게 구성할 것인지, 노드를 어떻게 연결할지, CRUD를 어떻게 구현할지, 그리고 사용자가 이 라이브러리를 쓴다고 할 때 어떻게 인터페이스를 만들 것인지 고민해야 한다.
 
## 노드 구성: non-intrusive vs intrusive
 먼저 노드를 어떻게 구성할지 고민해보자. 여기에는 [2]가 설명하듯이, 두 가지 방식이 있다. 하나는 노드에 이전과 다음 노드를 가리키는 변수를 포함시키는 것 (non-intrusive)이고, 다른 하나는 노드와 이전과 다음 노드를 가리키는 변수를 분리시키는 것 (intrusive)이다. 전자는 일반적으로 다음과 같은 형태를 가지고,
 
```C
struct node {
    /* some data */
	
    struct node *prev;
    struct node *next;
};
```

후자는 일반적으로 다음과 같은 형태를 가진다.

```C
struct node {
    struct node *prev;
    struct node *next;
};

struct data {
    /* some data */
	
	struct node link;
};
```

전자는 일반적으로 흔히 사용되는 이중 연결 리스트 노드의 형태이고, 사용자가 전달한 인자를 할당된 노드에 복사한다. 따라서 메모리 할당이 2회 (사용자 데이터, 이중 연결 리스트의 노드) 발생하게 된다. 반면 후자는 노드에 사용자 데이터가 포함된 것이 아니라 사용자 데이터에 이전과 다음 노드를 가리키기 위한 변수가 포함되어 (embedded) 있다. 따라서 사용자 데이터를 위한 메모리를 할당하는 것만으로 노드가 만들어진다. 메모리 할당 횟수가 적다는 것은 그만큼 메모리 할당 에러 확인 횟수가 적어진다는 것을 의미한다.

 여기서는 non-intrusive한 방식으로 노드를 구성할 것이다. 이때 노드의 구조는 다음과 같고,
 
```C
struct j_dllnode {
    struct j_dllnode *prev;
    struct j_dllnode *next;
};
```

그리고 사용자는 이를 데이터에 포함시켜야 한다. 즉, 리스트 라이브러리는 사용자가 위 노드 구조체를 데이터에 포함시켰다고 가정한다.

## CRUD 구현
 [1, Sec. 3.1]은 노드를 리스트에 추가할 때 정렬할 필요가 없다면, 리스트를 순회할 필요가 없는 방법을 선택하는 것이 가장 간단하다고 설명한다. 그리고 노드를 삭제할 때는 삭제할 노드를 찾기 위해서 함수가 필요하며, 리스트의 끝 (단일 연결 리스트에서는 맨 앞, 이중 연결 리스트에서는 맨 앞과 맨 끝) 노드를 삭제할 때의 경우를 특별히 다루어야 함을 설명한다. 여기서 노드를 찾는 함수는 재귀적으로 문제를 줄여 나가면서 (검색해야 하는 리스트의 크기를 줄이면서) 해결하는 방법을 선택한다. 마지막으로 이렇게 재귀적인 방법을 선택하는 것이 보다 간단한 리스트 연산을 만들고, 효율적인 분할 정복 (divide-and-conquer) 알고리즘으로 안내한다고 설명한다.
 
 여기서는 이중 연결 리스트의 노드를 다루며 노드를 추가할 때는 오름차순으로 정렬한다. 그래서 리스트의 양 끝에 노드를 삽입하는 방법을 쓸 수 없고, 노드를 추가할 위치를 찾기 위해 리스트를 순회해야만 한다. 이는 삭제, 갱신도 마찬가지이다. 이때 노드를 찾는 함수는 재귀적으로 구현하되 [3, Sec. 1.2.1]이 설명하는 선형 재귀 프로세스 (linear iterative process)의 형태로 구현한다. 이 선형 재귀 프로세스는 흔히 꼬리 재귀 (tail-recursive) 최적화라고 불리는 기법을 적용할 수 있는 형태이다.
 
## 사용자 인터페이스
 여기서 설명하는 사용자는 리스트 라이브러리를 사용하여 애플리케이션을 개발하는 개발자이다. 즉, 개발자가 이 라이브러리를 어떻게 사용할지에 대한 문제인 것이다. 여기서 구현하는 라이브러리는 객체지향의 스타일이 어느정도 가미되어 있다.

# References
[1] Steven S. Skiena, "The Algorithm Design Manual", Springer, 2008

[2] "Intrusive and non-intrusive containers", boost C++ LIBRARIES. [Online]. Available: https://www.boost.org/doc/libs/1_35_0/doc/html/intrusive/intrusive_vs_nontrusive.html, [Accessed Dec. 27, 2022]

[3] Harold Abelson, Gerald Jay Sussman, Julie Sussman, "Structure and Interpretation of Computer Programs", MIT Press, 1984

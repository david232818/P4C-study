이중 연결 리스트
=============


Contents
--------
1. 개요
2. 이중 연결 리스트
3. 구현
   1. 노드 구성: non-intrusive vs intrusive
   2. 노드를 연결하는 방법
   3. 사용자 인터페이스: 객체지향 스타일
   4. CRUD (Create, Read, Update, Delete)
4. References



# 개요
 빡공팟 6주차 과제는 C 언어를 사용하여 일반적인 (generic) 데이터를 저장할 수 있는 이중 연결 리스트를 구현하는 것이다.


# 이중 연결 리스트
 연결 리스트 (Linked List)라는 개념은 [5]가 제시한 개념으로, 문제의 증명을 기호 논리를 통해 찾는 프로그램인 Logical Theorist (LT)을 개발하는 과정에서 제안되었다. 해당 프로그램에 적용되는 여러 요구사항 (표현의 리스트를 다룰 수 있어야 하고, 논리 문제를 해결하기 위한 프로세스의 리스트를 다룰 수 있어야 한다는 것 등)을 해결하기 위해서는 한 언어가 필요했고 이것이 바로 IPL (Information Processing Language)이다. 즉, LT가 다룰 수 있어야 하는 리스트를 표현하기 위해 IPL이 사용된 것이다. 그리고 IPL은 리스트를 표현할 때 location words를 사용했는데, 이는 현대의 포인터와 같은 개념이라고 볼 수 있다. IPL은 리스트에 두 가지 location words를 두었는데, 하나는 element를 가리키고, 다른 하나는 다음 location words를 가리키도록 구성했다. 이를 그림으로 표현하면 다음과 같다.
 
```
    +--------+    +--------+    +--------+
--->|Location|--->|Location|--->|Location|---> (-1)
    +--------+    +--------+    +--------+
        |              |             |
        V              V             V
      element       element       element
```

 이중 연결 리스트 (Double Linked List)는 [1, Sec. 3.1]이 설명하는 연결된 자료 구조 (Linked Data Structures)에 속하는 단일 연결 리스트 (Single Linked List)에서 이전 노드를 가리키는 것이 추가된 자료구조이다. 이를 그림으로 표현하면 다음과 같다.
 
```
    +----+    +----+    +----+
<---|node|<---|node|<---|node|--->
    |    |--->|    |--->|    |
    +----+    +----+    +----+
```

위와 같은 구조에서 첫 번째 노드와 마지막 노드는 각각 head, tail이라고 불리며 접근하기 위한 변수가 별도로 존재하여 접근하는데 O(1)이 소요된다. 그리고 이전 노드와 다음 노드를 가리키는 변수는 prev (previous)와 next 또는 bk (back)와 fd (forward)로 표현된다. 이렇게 이전 노드와 다음 노드를 가리키는 부분이 존재하기 때문에 단일 연결 리스트와 다르게 순회하는 방향을 조절할 수 있게 된다. 즉, 앞에서부터 순회할 수도 있고, 뒤에서부터 순회할 수도 있다. 이는 단일 연결 리스트에서의 오름차순과 내림차순 문제를 정렬 문제에서 읽는 순서의 문제로 단순화 시킨다. 하지만 임의의 노드에 접근하는데는 여전히 단일 연결 리스트와 같이 O(n)이다.


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

 여기서는 non-intrusive한 방식으로 노드를 구성한다. 이때 노드의 구조는 다음과 같고,
 
```C
struct j_dllnode {
	struct j_dllnode *prev;
	struct j_dllnode *next;
};
```

사용자는 이를 데이터에 포함시켜야 한다. 즉, 라이브러리는 사용자가 위 노드 구조체를 데이터에 포함시켰다고 가정한다. 여기서 사용자는 이 라이브러리를 사용하여 애플리케이션을 작성하는 개발자를 말한다.

 이러한 노드 구성은 노드에서 데이터를, 데이터에서 노드를 얻는 방법이 non-intrusive한 노드 구성보다 복잡해지도록 만든다. 일반적인 (generic) 데이터를 이중 연결 리스트에 저장하고자 할 때 이를 구현하는 방법은 주어진 데이터 타입에서 노드의 오프셋을 구하는 것이다. 다만, 여기서 오프셋은 사용자가 적절한 값으로 제공해야만 한다. 여기서 [4, Sec. 7.17]은 구조체 멤버의 (구조체의 시작 주소로부터의) 오프셋을 구하기 위한 offsetof 매크로를 정의한다. 따라서 사용자는 이 매크로를 사용하여 데이터에 포함된 노드 구조체의 오프셋을 제공할 수 있고, 제공해야만 한다. 이 값이 data_offset이라는 attribute로 저장된다고 가정하면, 이로부터 다음과 같이 노드에서 데이터를, 데이터에서 노드를 구하는 함수를 다음과 같이 작성할 수 있다.
 
```C
/* j_dll_get_data: get the data from the node */
static void *j_dll_get_data(j_dll_t *self, struct j_dllnode *node)
{
    if (node == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_NODE;
	return NULL;
    }
    return ((char *) node - self->data_offset);
}

/* j_dll_get_node: get the node from the data */
static struct j_dllnode *j_dll_get_node(j_dll_t *self, void *data)
{
    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return NULL;
    }
    return (struct j_dllnode *) ((char *) data + self->data_offset);
}
```


## 노드를 연결하는 방법


## 사용자 인터페이스: 객체지향 스타일


## CRUD (Create, Read, Update, Delete)
 [1, Sec. 3.1]은 노드를 리스트에 추가할 때 정렬할 필요가 없다면, 리스트를 순회할 필요가 없는 방법을 선택하는 것이 가장 간단하다고 설명한다. 그리고 노드를 삭제할 때는 삭제할 노드를 찾기 위해서 함수가 필요하며, 리스트의 끝 (단일 연결 리스트에서는 맨 앞, 이중 연결 리스트에서는 맨 앞과 맨 끝) 노드를 삭제할 때의 경우를 특별히 다루어야 함을 설명한다. 여기서 노드를 찾는 함수는 재귀적으로 문제를 줄여 나가면서 (검색해야 하는 리스트의 크기를 줄이면서) 해결하는 방법을 선택하는데, 이 방법이 보다 간단한 리스트 연산을 만들고, 효율적인 분할 정복 (divide-and-conquer) 알고리즘으로 안내한다고 설명한다.
 
 여기서는 이중 연결 리스트의 노드를 다루며 노드를 추가할 때는 오름차순으로 정렬한다. 그래서 리스트의 양 끝에 노드를 삽입하는 방법을 쓸 수 없고, 노드를 추가할 위치를 찾기 위해 리스트를 순회해야만 한다. 이는 삭제, 갱신도 마찬가지이다. 이때 노드를 찾는 함수는 재귀적으로 구현하되 [3, Sec. 1.2.1]이 설명하는 선형 반복 프로세스 (linear iterative process)의 형태로 구현한다. 이러한 선형 반복 프로세스는 흔히 꼬리 재귀 (tail-recursive) 최적화라고 불리는 기법을 적용할 수 있는 형태이다.
 
 선형 반복 프로세스로 구현된 검색 함수는 다음과 같다.
 
```C
/* prv_j_dll_search_node: search the start that makes cmpres with data */
static struct j_dllnode *prv_j_dll_search_node(j_dll_t *dll,
					       struct j_dllnode *start,
					       void *data,
					       int cmpres)
{
    if (start == NULL ||
		dll->mt[J_DLLNODE_COMPARE].method(dll, start, data) == cmpres)
	return start;
    prv_j_dll_search_node(dll, start->next, data, cmpres);
}
```

```C
/* j_dll_search: search the start that makes cmpres with data */
static struct j_dllnode *j_dll_search(j_dll_t *self,
				      void *data,
				      int cmpres)
{
    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return NULL;
    }

    return prv_j_dll_search_node(self, self->head, data, cmpres);
}
```

 위와 같은 검색 함수는 특정 노드를 찾는 것뿐만 아니라 노드 생성 시, 노드를 삽입할 위치도 찾을 수 있다는 것을 기억하자. 이는 검색 함수가 인자로 전달된 데이터와 노드를 비교할 때 같은 경우 (반환값이 0인 경우)만을 검사하는 것이 아니라 전달된 cmpres와 같은지 검사하기 때문이다. 이때 주의해야할 것은 사용자는 비교 함수를 정의할 때 반환값을 -1 (해당 노드의 데이터가 전달된 데이터보다 작은 경우), 0 (해당 노드의 데이터가 전달된 데이터와 같은 경우), 1 (해당 노드의 데이터가 전달된 데이터보다 큰 경우)로 정의해야 한다는 것이다. 그럼 이제 검색 함수가 노드 생성, 삭제, 갱신에서 어떻게 사용되는지 알아보자.
 
 노드 생성을 수행하는 함수는 다음과 같다.
 
```C
/* prv_j_dll_insert_node_at_the_end: insert the target at the end of the dll */
static void prv_j_dll_insert_node_at_the_end(j_dll_t *dll,
					  struct j_dllnode *curr,
					  struct j_dllnode *target)
{
    dll->tail = target;
    target->prev = curr;
    curr->next = target;
}

/* prv_j_dll_insert_node: insert the target in front of the curr */
static void prv_j_dll_insert_node(j_dll_t *dll,
			       struct j_dllnode *curr,
			       struct j_dllnode *target)
{
    if (curr == dll->head) {
	dll->head = target;
	target->next = curr;
	curr->prev = target;
	dll->tail = (dll->cnt == 1) ? curr : dll->tail;
    } else {
	target->prev = curr->prev;
	target->next = curr;
	curr->prev->next = target;
	curr->prev = target;
    }
}

/* prv_j_dll_insert: insert the node to the dll */
static void prv_j_dll_insert(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *target;

    if ((target = dll->search(dll,
			      dll->get_data(dll, node),
			      1)) == NULL)
	prv_j_dll_insert_node_at_the_end(dll, dll->tail, node);
    else
	prv_j_dll_insert_node(dll, target, node);
}
```
 
```C
/* j_dll_create: create the node in the self */
static int j_dll_create(j_dll_t *self, void *data)
{
    struct j_dllnode *node;

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    node = self->get_node(self, data);
    node->prev = NULL;
    node->next = NULL;
    if (self->is_empty(self)) {
	self->head = node;
	self->tail = node;
    } else
	prv_j_dll_insert(self, self->get_node(self, data));
    self->cnt++;
    return 0;
}
```

위 코드의 prv_j_dll_insert()가 검색 메서드를 호출할 때 네 번째 인자로 1을 전달한다는 것에 주목하자. 이는 전달된 데이터보다 큰 노드를 검색하라는 의미이며, 곧 오름차순으로 노드를 삽입할 때 삽입할 위치를 검색하는 것과 같다. 이러한 노드 생성 함수에서 재귀적인 부분은 prv_j_dll_insert()가 검색 메서드를 호출할 때 뿐이다.

 이제 노드 삭제를 수행하는 함수를 살펴보자. 노드 삭제를 수행하는 함수의 코드는 다음과 같다.
 
```C
/* prv_j_dll_unlink_node: unlink the node from the dll */
static void prv_j_dll_unlink_node(j_dll_t *dll, struct j_dllnode *node)
{
    struct j_dllnode *head, *tail;

    head = dll->head;
    tail = dll->tail;
    if (head == tail) {		/* there is only one node in the dll */
	dll->head = NULL;
	dll->tail = NULL;
    } else if (node == head) {
	dll->head = node->next;
	node->next->prev = NULL;
	node->next = NULL;
    } else if (node == tail) {
	dll->tail = node->prev;
	node->prev->next = NULL;
	node->prev = NULL;
    } else {
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node->next = NULL;
    }
}

/* prv_j_dll_delete_node: delete the node from the dll */
static void prv_j_dll_delete_node(j_dll_t *dll, struct j_dllnode *node)
{
    prv_j_dll_unlink_node(dll, node);
    dll->cnt--;
    free(dll->get_data(dll, node));
}
```

```C
/* j_dll_delete: delete the node that has corresponding data from the self */
static int j_dll_delete(j_dll_t *self, void *data)
{
    struct j_dllnode *target;

    if (data == NULL) {
	j_dll_errno = J_DLL_ERR_INVALID_DATA;
	return -1;
    }

    if ((target = self->search(self, data, 0)) == NULL)
	return -1;

    prv_j_dll_delete_node(self, target);
    return 0;
}
```

노드를 삭제하는 함수는 삭제할 노드를 찾기 위해 검색 메서드를 사용한다. 그리고 노드 생성 함수에서와 같이 검색 메서드가 노드를 삭제하는 과정에서 유일하게 재귀적인 부분이다.

 마지막으로 노드의 데이터를 갱신하는 함수를 살펴보자.
 
```C
```

# References
[1] Steven S. Skiena, "The Algorithm Design Manual", Springer, 2008

[2] "Intrusive and non-intrusive containers", boost C++ LIBRARIES. [Online]. Available: https://www.boost.org/doc/libs/1_35_0/doc/html/intrusive/intrusive_vs_nontrusive.html, [Accessed Dec. 27, 2022]

[3] Harold Abelson, Gerald Jay Sussman, Julie Sussman, "Structure and Interpretation of Computer Programs", MIT Press, 1984

[4] ISO/IEC JTC 1/SC 22/WG 14, "ISO/IEC 9899:1999, Programming Languages -- C", ISO/IEC, 1999

[5] Allen Newell, J. C. Shaw, "Programming the Logic Theory Machin", Proceedings of the Western Joint Computer Conference, pp. 230-240, 1957

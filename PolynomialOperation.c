#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> 



// 연결 리스트의 노드의 구조
typedef struct ListNode {
	int coef;
	int exp;
	struct ListNode *link;
} ListNode;


// 연결 리스트 헤더
typedef struct ListHeader {
	int length;
	ListNode *head;
	ListNode *tail;
} ListHeader;


// 초기화 함수
void init(ListHeader *plist) {
	plist->length = 0;
	plist->head = plist->tail = NULL;
}


// plist는 연결 리스트의 헤더를 가리키는 포인터, coef는 계수, exp는 지수
void insert_node_last(ListHeader *plist, int coef, int exp) {
	ListNode *temp = (ListNode *)malloc(sizeof(ListNode));
	if (temp == NULL) exit;

	temp->coef = coef;
	temp->exp = exp;
	temp->link = NULL;

	if (plist->tail == NULL) {
		plist->head = plist->tail = temp;
	}else{
		plist->tail->link = temp;
		plist->tail = temp;
	}plist->length++;
}

ListHeader poly_read(ListHeader *plist, FILE*fp) {
	
	int n;
	int co;
	int ex;
	fscanf(fp, "%d", &n);
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d", &co);
		fscanf(fp, "%d", &ex);
		insert_node_last(plist, co, ex);	
	}

	return *plist;
}

// list3 = list1 + list2
ListHeader poly_add(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3) {
	ListNode *a = plist1->head;
	ListNode *b = plist2->head;
	int sum;
	while (a && b) {
		if (a->exp == b->exp) { // a의 차수 > b의 차수
			sum = a->coef + b->coef;
			if (sum != 0) insert_node_last(plist3, sum, a->exp);
			a = a->link; b = b->link;
		}
		else if (a->exp > b->exp) { // a의 차수 == b의 차수
			insert_node_last(plist3, a->coef, a->exp);
			a = a->link;
		}
		else { // a의 차수 < b의 차수
			insert_node_last(plist3, b->coef, b->exp);
			b = b->link;
		}
	}
	// a나 b 중의 하나가 먼저 끝나게 되면 남아있는 항들을 모두
	// 결과 다항식으로 복사
	for (; a != NULL; a = a->link)
		insert_node_last(plist3, a->coef, a->exp);
	for (; b != NULL; b = b->link)
		insert_node_last(plist3, b->coef, b->exp);

	return *plist3;

}

int poly_eval(ListHeader* plist, int x)
{	
	ListNode *p = plist->head;
	int xpower;
	int value = 0;
	for (; p; p = p->link) {
		xpower = 1;
		for (int i = 0; i < p->exp; i++) {
			xpower = xpower * x;
		}
		value += p->coef *xpower;
	}
	return value;
}


void poly_print(ListHeader *plist)
{
	ListNode *p = plist->head;
	for (; p; p = p->link) {
		printf("%dx^%d", p->coef, p->exp);//ax^n 형태
		if (p->link != NULL)
			printf("+");
	}
	printf("\n");
}


int main() {

	FILE*fp;
	fp = fopen("poly5.txt", "r");
	ListHeader list1, list2, list3;
	int x, y, z;

	// 연결 리스트의 초기화
	init(&list1);
	init(&list2);
	init(&list3);

	// 다항식 p(x)을 생성
	list1 = poly_read(&list1, fp);

	// 다항식 q(x)를 생성
	list2 = poly_read(&list2, fp);

	// r(x) = p(x) + q(x)
	list3 = poly_add(&list1, &list2, &list3);

	printf("p(x) : ");
	poly_print(&list1);
	printf("q(x) : ");
	poly_print(&list2);
	printf("r(x) : ");
	poly_print(&list3);

	fscanf(fp, "%d", &x);
	fscanf(fp, "%d", &y);
	fscanf(fp, "%d", &z);

	printf("p(%d) = %d\n", x, poly_eval(&list1, x));
	printf("q(%d) = %d\n", y, poly_eval(&list2, y));
	printf("r(%d) = %d\n", z, poly_eval(&list3, z));
}
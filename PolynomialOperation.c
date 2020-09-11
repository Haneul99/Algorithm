#include <stdio.h>
#include <stdlib.h>
#include <malloc.h> 



// ���� ����Ʈ�� ����� ����
typedef struct ListNode {
	int coef;
	int exp;
	struct ListNode *link;
} ListNode;


// ���� ����Ʈ ���
typedef struct ListHeader {
	int length;
	ListNode *head;
	ListNode *tail;
} ListHeader;


// �ʱ�ȭ �Լ�
void init(ListHeader *plist) {
	plist->length = 0;
	plist->head = plist->tail = NULL;
}


// plist�� ���� ����Ʈ�� ����� ����Ű�� ������, coef�� ���, exp�� ����
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
		if (a->exp == b->exp) { // a�� ���� > b�� ����
			sum = a->coef + b->coef;
			if (sum != 0) insert_node_last(plist3, sum, a->exp);
			a = a->link; b = b->link;
		}
		else if (a->exp > b->exp) { // a�� ���� == b�� ����
			insert_node_last(plist3, a->coef, a->exp);
			a = a->link;
		}
		else { // a�� ���� < b�� ����
			insert_node_last(plist3, b->coef, b->exp);
			b = b->link;
		}
	}
	// a�� b ���� �ϳ��� ���� ������ �Ǹ� �����ִ� �׵��� ���
	// ��� ���׽����� ����
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
		printf("%dx^%d", p->coef, p->exp);//ax^n ����
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

	// ���� ����Ʈ�� �ʱ�ȭ
	init(&list1);
	init(&list2);
	init(&list3);

	// ���׽� p(x)�� ����
	list1 = poly_read(&list1, fp);

	// ���׽� q(x)�� ����
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
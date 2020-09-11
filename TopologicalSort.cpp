#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define MAX_VERTICES 100
#define MAX_STACK_SIZE 100

typedef int element;

typedef struct GraphNode {
	int vertex;//����
	struct GraphNode *link;
}GraphNode;

typedef struct GraphType {
	int n;
	GraphNode *adj_list[MAX_VERTICES];
	GraphNode *head[MAX_VERTICES];
} GraphType;//Graph ����ü

void init_Graph(GraphType *g) {
	g->n = 0;
	for (int i = 0; i < MAX_VERTICES; i++) {
		g->head[i] = g->adj_list[i]=NULL;
	}
}//GraphType �ʱ�ȭ �Լ�

typedef struct {
	element stack[MAX_STACK_SIZE];
	int top;
}StackType;//Stack ����ü

void init(StackType *s) {
	s->top = -1;
}//Stack �ʱ�ȭ �Լ�

int is_empty(StackType *s) {
	return(s->top == -1);
}//Stack�� ����ִ��� üũ

int is_full(StackType *s) {
	return(s->top == (MAX_STACK_SIZE - 1));
}//Stack�� �� ���ִ��� üũ

void push(StackType *s, element item) {
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->stack[++(s->top)] = item;//Stack�� top�� element�� �����ϰ� top�� ������Ŵ
}//Stack�� element�� �����ϴ� �Լ�

element pop(StackType *s) {
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
		exit;
	}
	else return s->stack[(s->top)--];//Stack�� top�� return�ϰ� top�� ���ҽ�Ŵ
}//Stack���� element�� �����ϴ� �Լ�

void insert(GraphType *g, int ver, int eg) {
	GraphNode *temp = (GraphNode*)malloc(sizeof(GraphNode));//�޸� �����Ҵ�
	if (temp == NULL) exit;//�������� �ʾ��� ��� ���α׷� ����

	temp->vertex = eg;
	temp->link = NULL;

	if (g->head[ver] == NULL) {
		g->adj_list[ver] = temp;
		g->head[ver] = temp;
	}//adj_list[ver]�� ������� ���
	else {
		g->head[ver]->link = temp;
		g->head[ver] = temp;
	}//adj_list[ver]�� ������� ���� ���
}//Graph�� ���Ḯ��Ʈ�� �����ϴ� �Լ�

void f_read(FILE*fp, GraphType *g) {
	int v, e;//�׷����� �� ������ ������ ����
	int vertex1;//���Ͽ��� �о�� vertex
	int edge1;//���Ͽ��� �о�� edge
	fscanf(fp, "%d", &v);
	fscanf(fp, "%d", &e);
	g->n = v;//��ü ������ ���� 
	for (int i = 0; i < e; i++) {
		fscanf(fp, "%d", &vertex1);
		fscanf(fp, "%d", &edge1);
		insert(g, vertex1, edge1);//�׷��� g�� vertex�� edge�� �Է��Ͽ� GraphNode �߰�
	}
}

void top_sort(GraphType *g) {
	int i;
	int count=0;//���ĵ� ������ ���� üũ
	StackType s;
	GraphNode *node;
	// ��� ������ ���� ������ ���
	int *in_degree = (int *)malloc(g ->n * sizeof(int));
	for (i = 0; i < g->n; i++) // �ʱ�ȭ
		in_degree[i] = 0;
	for (i = 0; i < g->n; i++) {
		GraphNode *node = g-> adj_list[i]; // ���� i���� ������ ������
		while (node != NULL) {
			in_degree[node->vertex]++;
			node = node-> link;
		}
	}
	// ���� ������ 0�� ������ ���ÿ� ����
	init(&s);
	for (i = 0; i < g->n; i++) {
		if (in_degree[i] == 0) {
			push(&s, i);
			count++;
		}
	}
	// ���� ������ ����
	while (!is_empty(&s)) {
		int w;
		w = pop(&s);
		printf("%d\t", w);
		node = g-> adj_list[w]; // �� ������ ���� ������ ����
		while (node != NULL) {
			int u = node-> vertex;
			in_degree[u]--; // ���� ������ ����
			if (in_degree[u] == 0) {
				push(&s, u);
				count++;
			}
			node = node-> link; // ���� ����
		}
	}
	if (count != g->n) {
		printf("\nImpossible\n");
		exit;
	}//���ĵ� ������ ������ �׷����� ������ ������ ���� ���� ��� ���� ����
	free(in_degree);
	return; // ��ȯ���� 1�̸� ����, 0�̸� ����
}

int main() {
	FILE *fp;
	fp = fopen("dag1.txt","r");
	GraphType graph;
	init_Graph(&graph);//�׷��� �ʱ�ȭ
	f_read(fp, &graph);//�׷��� �����
	top_sort(&graph);//���� ����
}
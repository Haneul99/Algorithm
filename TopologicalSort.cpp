#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#define MAX_VERTICES 100
#define MAX_STACK_SIZE 100

typedef int element;

typedef struct GraphNode {
	int vertex;//정점
	struct GraphNode *link;
}GraphNode;

typedef struct GraphType {
	int n;
	GraphNode *adj_list[MAX_VERTICES];
	GraphNode *head[MAX_VERTICES];
} GraphType;//Graph 구조체

void init_Graph(GraphType *g) {
	g->n = 0;
	for (int i = 0; i < MAX_VERTICES; i++) {
		g->head[i] = g->adj_list[i]=NULL;
	}
}//GraphType 초기화 함수

typedef struct {
	element stack[MAX_STACK_SIZE];
	int top;
}StackType;//Stack 구조체

void init(StackType *s) {
	s->top = -1;
}//Stack 초기화 함수

int is_empty(StackType *s) {
	return(s->top == -1);
}//Stack이 비어있는지 체크

int is_full(StackType *s) {
	return(s->top == (MAX_STACK_SIZE - 1));
}//Stack이 꽉 차있는지 체크

void push(StackType *s, element item) {
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->stack[++(s->top)] = item;//Stack의 top에 element를 삽입하고 top을 증가시킴
}//Stack에 element를 삽입하는 함수

element pop(StackType *s) {
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit;
	}
	else return s->stack[(s->top)--];//Stack의 top을 return하고 top을 감소시킴
}//Stack에서 element를 삭제하는 함수

void insert(GraphType *g, int ver, int eg) {
	GraphNode *temp = (GraphNode*)malloc(sizeof(GraphNode));//메모리 동적할당
	if (temp == NULL) exit;//생성되지 않았을 경우 프로그램 종료

	temp->vertex = eg;
	temp->link = NULL;

	if (g->head[ver] == NULL) {
		g->adj_list[ver] = temp;
		g->head[ver] = temp;
	}//adj_list[ver]가 비어있을 경우
	else {
		g->head[ver]->link = temp;
		g->head[ver] = temp;
	}//adj_list[ver]가 비어있지 않을 경우
}//Graph의 연결리스트에 삽입하는 함수

void f_read(FILE*fp, GraphType *g) {
	int v, e;//그래프의 총 정점과 에지의 개수
	int vertex1;//파일에서 읽어올 vertex
	int edge1;//파일에서 읽어올 edge
	fscanf(fp, "%d", &v);
	fscanf(fp, "%d", &e);
	g->n = v;//전체 에지의 개수 
	for (int i = 0; i < e; i++) {
		fscanf(fp, "%d", &vertex1);
		fscanf(fp, "%d", &edge1);
		insert(g, vertex1, edge1);//그래프 g에 vertex와 edge를 입력하여 GraphNode 추가
	}
}

void top_sort(GraphType *g) {
	int i;
	int count=0;//정렬된 정점의 개수 체크
	StackType s;
	GraphNode *node;
	// 모든 정점의 진입 차수를 계산
	int *in_degree = (int *)malloc(g ->n * sizeof(int));
	for (i = 0; i < g->n; i++) // 초기화
		in_degree[i] = 0;
	for (i = 0; i < g->n; i++) {
		GraphNode *node = g-> adj_list[i]; // 정점 i에서 나오는 에지들
		while (node != NULL) {
			in_degree[node->vertex]++;
			node = node-> link;
		}
	}
	// 진입 차수가 0인 정점을 스택에 삽입
	init(&s);
	for (i = 0; i < g->n; i++) {
		if (in_degree[i] == 0) {
			push(&s, i);
			count++;
		}
	}
	// 위상 순서를 생성
	while (!is_empty(&s)) {
		int w;
		w = pop(&s);
		printf("%d\t", w);
		node = g-> adj_list[w]; // 각 정점의 진입 차수를 변경
		while (node != NULL) {
			int u = node-> vertex;
			in_degree[u]--; // 진입 차수를 감소
			if (in_degree[u] == 0) {
				push(&s, u);
				count++;
			}
			node = node-> link; // 다음 정점
		}
	}
	if (count != g->n) {
		printf("\nImpossible\n");
		exit;
	}//정렬된 정점의 개수와 그래프의 정점의 개수가 같지 않을 경우 정렬 실패
	free(in_degree);
	return; // 반환값이 1이면 성공, 0이면 실패
}

int main() {
	FILE *fp;
	fp = fopen("dag1.txt","r");
	GraphType graph;
	init_Graph(&graph);//그래프 초기화
	f_read(fp, &graph);//그래프 만들기
	top_sort(&graph);//위상 정렬
}
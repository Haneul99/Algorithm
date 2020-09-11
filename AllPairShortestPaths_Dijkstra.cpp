#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100 //�ִ� ������ ��
#define INF 9999 // ���Ѵ� (������ ���� ���)

int adjacency_matrix[MAX_VERTICES][MAX_VERTICES] = {};


void init() {
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < MAX_VERTICES; j++) {
			adjacency_matrix[i][j] = INF;
			adjacency_matrix[i][i] = 0;//�ڱ� �ڽ��� distance 0
		}

	}
}

int* read(FILE*fp,int *n) {
	int n1;//������ ��
	int e;//������ ��
	int v1;//���� ��� ���� 
	int v2;//���� ���� ����
	int w;//����ġ
	fscanf(fp, "%d", &n1);//������ �� file���� �о��
	fscanf(fp, "%d", &e);//������ �� file���� �о��
	*n = n1;//������ ���� ���� �����س���
	for (int i = 0; i < e ; i++) {
		fscanf(fp, "%d", &v1);
		fscanf(fp, "%d", &v2);
		fscanf(fp, "%d", &w);//��������, ���� ����, ����ġ�� ���� ����
		adjacency_matrix[v1 - 1][v2 - 1] = w;//������Ŀ� �� ��ġ�� �Է�
	}
	return *adjacency_matrix;
}

int distance[MAX_VERTICES]; // �����������κ����� �ִܰ�� �Ÿ�
int found[MAX_VERTICES]; // �湮�� ���� ǥ��
int choose(int distance[], int n, int found[]) { // �������� �� distance �ּ��� ���� ã��
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for (i = 0; i < n; i++)
		if (distance[i] < min && !found[i]) {
			min = distance[i];
			minpos = i;
		}
	return minpos;
}

void shortest_path(int start, int n) {
	int i, u, w;
	for (i = 0; i < n; i++) { // �ʱ�ȭ
		distance[i] = adjacency_matrix[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE; // ���� ���� �湮 ǥ��
	distance[start] = 0;
	for (i = 0; i < n-2; i++) {
		u = choose(distance, n, found); // Set S�� ���� ���� ����� u ã��
		found[u] = TRUE;
		for (w = 0; w < n; w++) // u�� ������ w ���� distance ����
			if (!found[w])//�湮���� ���� ������ ���
				if (distance[u] + adjacency_matrix[u][w] < distance[w])
					distance[w] = distance[u] + adjacency_matrix[u][w];//�ּҰ� ������Ʈ
	}
}

void matrix_print(int n) {
	printf("Adjacency matrix\n\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t", adjacency_matrix[i][j]);
		}
		printf("\n");
	}//������� ���
}


void main()
{
	FILE*fp;
	fp = fopen("graph6.txt","r");//���� open

	int n;//������ ����

	init();
	read(fp, &n);

	matrix_print(n);
	printf("\n\n");
	for (int i = 0; i < n; i++) {
		shortest_path(i, n);//��� ������ resetting
		printf("Shortest paths from v%d = (",i+1);
		for (int j = 0; j < n-1; j++) {
			printf("%d, ", distance[j]);//���� i���� ���� j������ �ּ� �Ÿ� ���
		}
		printf("%d)\n", distance[n-1]);
	}

}
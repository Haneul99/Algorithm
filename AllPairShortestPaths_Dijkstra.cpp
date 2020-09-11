#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100 //최대 정점의 수
#define INF 9999 // 무한대 (연결이 없는 경우)

int adjacency_matrix[MAX_VERTICES][MAX_VERTICES] = {};


void init() {
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < MAX_VERTICES; j++) {
			adjacency_matrix[i][j] = INF;
			adjacency_matrix[i][i] = 0;//자기 자신은 distance 0
		}

	}
}

int* read(FILE*fp,int *n) {
	int n1;//정점의 수
	int e;//에지의 수
	int v1;//에지 출발 정점 
	int v2;//에지 도착 도착
	int w;//가중치
	fscanf(fp, "%d", &n1);//정점의 수 file에서 읽어옴
	fscanf(fp, "%d", &e);//에지의 수 file에서 읽어옴
	*n = n1;//정점의 수를 따로 저장해놓음
	for (int i = 0; i < e ; i++) {
		fscanf(fp, "%d", &v1);
		fscanf(fp, "%d", &v2);
		fscanf(fp, "%d", &w);//시작정점, 도착 정점, 가중치를 각각 저장
		adjacency_matrix[v1 - 1][v2 - 1] = w;//인접행렬에 각 수치를 입력
	}
	return *adjacency_matrix;
}

int distance[MAX_VERTICES]; // 시작정점으로부터의 최단경로 거리
int found[MAX_VERTICES]; // 방문한 정점 표시
int choose(int distance[], int n, int found[]) { // 인접정점 중 distance 최소인 정점 찾기
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
	for (i = 0; i < n; i++) { // 초기화
		distance[i] = adjacency_matrix[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE; // 시작 정점 방문 표시
	distance[start] = 0;
	for (i = 0; i < n-2; i++) {
		u = choose(distance, n, found); // Set S로 부터 가장 가까운 u 찾기
		found[u] = TRUE;
		for (w = 0; w < n; w++) // u에 인접한 w 들의 distance 재계산
			if (!found[w])//방문하지 않은 정점일 경우
				if (distance[u] + adjacency_matrix[u][w] < distance[w])
					distance[w] = distance[u] + adjacency_matrix[u][w];//최소값 업데이트
	}
}

void matrix_print(int n) {
	printf("Adjacency matrix\n\n");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d\t", adjacency_matrix[i][j]);
		}
		printf("\n");
	}//인접행렬 출력
}


void main()
{
	FILE*fp;
	fp = fopen("graph6.txt","r");//파일 open

	int n;//정점의 개수

	init();
	read(fp, &n);

	matrix_print(n);
	printf("\n\n");
	for (int i = 0; i < n; i++) {
		shortest_path(i, n);//출발 정점을 resetting
		printf("Shortest paths from v%d = (",i+1);
		for (int j = 0; j < n-1; j++) {
			printf("%d, ", distance[j]);//정점 i부터 정점 j까지의 최소 거리 출력
		}
		printf("%d)\n", distance[n-1]);
	}

}
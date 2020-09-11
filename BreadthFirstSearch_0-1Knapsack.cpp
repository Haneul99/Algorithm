#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX_SIZE 10000//queue의 최대 size

int front = 0;//queue의 맨 앞
int rear = 0;//queue의 맨 뒤
int M;//Maximum capacity
int best;//optimal
int best_index;//optimal node index

typedef struct ITEM { //가방에 담을 물건
	int index; //item의 index
	int p;//profit
	int w;//weight
	float ppw; //p/w;
}item;

typedef struct Node {
	int weight;//포함 무게
	int profit;//현재까지 이익
	int index;//노드번호
	float bound;//bound
}node;

item* insertion_sort(int n, item* list) {
	int i = 0, j = 0;
	item* sorted_list = (item*)malloc(sizeof(item)*n);//list를 따로 저장할 item type의 sorted_list
	for (int i = 0; i < n; i++) {
		sorted_list[i].index = list[i].index;
		sorted_list[i].p = list[i].p;
		sorted_list[i].w = list[i].w;
		sorted_list[i].ppw = list[i].ppw;
	}//sorted list에 list값 저장
	item tmp;
	for (j = 1; j < n; j++) {
		tmp = sorted_list[j];
		i = j - 1;
		while (i >= 0 && sorted_list[i].ppw < tmp.ppw) {//ppw 값에 따라 정렬
			sorted_list[i + 1] = sorted_list[i];
			i--;
		}
		sorted_list[i + 1] = tmp;
	}
	return sorted_list;
}

int level(int n) {
	if (n == 0) return 0;//root 인 경우
	n++;//index가 0부터 시작하기 때문에 +1
	int level = (int)(log(n) / log(2))-1;
	return level;
}

int value(node temp) {
	if (best < temp.profit) {
		best = temp.profit;//best와 node의 profit을 비교
		best_index = temp.index;//index update
	}
	return best;
}

float bound(int n, item* sorted, node parent_node, int size) {
	float totprofit = 0;
	float totweight = 0;
	int i = 0;
	int lv=level(n);//sorted의 몇번째 item인지 체크하기 위한 level 값
	if (n == 0) {//root인 경우
		while (totweight <= M && lv <= size) {//total weight가 M이하이고, lv이 item 개수 이하일 동안
			if (totweight + sorted[i].w > M)//total weight가 M보다 커지면 break
				break;
			totprofit += sorted[i].p;//total profit update
			totweight += sorted[i].w;//total weight update
			i++;
		}
		totprofit += (sorted[i].p)*(M - totweight)/sorted[i].w;//남은 weight만큼 fractional 값 더함
	}
	else {
		//왼쪽자식노드, 1일 경우
		if (n % 2 == 1) {
			totprofit = parent_node.bound;//부모노드의 bound 값을 가져옴
		}
		//오른쪽자식노드, 0일 경우
		else {
			totweight = parent_node.weight;//부모노드의 weight값을 가져옴
			totprofit = parent_node.profit;//부모노드의 profit값을 가져옴
			lv++;//해당 level의 item을 건너뛰고 다음 item으로 넘어감
			while (totweight <= M && lv<= size) {//total weight가 M이하이고, lv이 item 개수 이하일 동안
				if (totweight + sorted[lv].w > M) break;//total weight가 M보다 커지면 break
				totprofit += sorted[lv].p;//total profit update
				totweight += sorted[lv].w;//total weight update
				lv++;
			}
			if(lv<=size) totprofit += (sorted[lv].p)*(M - totweight) / sorted[lv].w;//남은 weight만큼 fractional 값 더함
		}
	}
	return totprofit;
}

int IsEmpty(void) {
	if (front == rear)//front와 rear가 같으면 큐는 비어있는 상태 
		return true;
	else return false;
}
bool promising(node temp) {
	//promising 조건 : weight가 넘지 않을 것, value보다 bound가 클 것
	if (temp.weight<=M && temp.bound > best) return true;
	else return false;
}
void enqueue(node array[], node temp) {
	array[rear] = temp;//queue에 삽입
	rear = (rear + 1) % MAX_SIZE;
}
node dequeue(node array[]) {
	int i = front;
	front = (front + 1) % MAX_SIZE;//queue에서 제거
	return array[i];
}

void bfs_bb(node array[], item* sorted,int n, int size) {
	node temp;//생성한 node를 저장할 변수
	node parent_node;
	//root일 경우
	if (n == 0){
		temp.weight = 0;
		temp.profit = 0;
		temp.index = 0;
		temp.bound = 0;
		temp.bound = bound(n, sorted, temp, size);
		enqueue(array, temp);
	}//root 초기화
	while (!IsEmpty()) {//큐가 empty할때까지 반복
		parent_node = dequeue(array);//부모노드 큐에서 제거
		best = value(parent_node);//best 값을 초기화
		for (int i = 2 * parent_node.index + 1; i < 2 * parent_node.index + 3 ; i++) {//dequeue된 노드의 자식노드
			//왼쪽자식노드(1,3,5,7,9…)면 1
			if (i % 2 == 1) {
				temp.weight = parent_node.weight + sorted[level(i)].w;//부모노드의 weight에 item[lv]의 weight 더함
				temp.profit = parent_node.profit + sorted[level(i)].p;//부모노드의 profit에 item[lv]의 profit 더함
				temp.index = i;
				temp.bound = bound(i, sorted, parent_node, size);
				if (temp.bound < temp.profit) temp.bound = 0;
			}
			//오른쪽자식노드(2,4,6,8,10…)면 0
			else {
				temp.weight = parent_node.weight;//부모노드의 weight를 가져옴
				temp.profit = parent_node.profit;//부모노드의 profit을 가져옴
				temp.index = i;
				temp.bound = bound(i, sorted, parent_node, size);
				if (temp.bound < temp.profit) temp.bound = 0;
			}
			if (promising(temp)) {//temp가 promising하다면
				best = value(temp);//best값 계산
				enqueue(array, temp);//자식노드를 enqueue
			}
		}
	}
}

int* get_solution_vector(int n, item* sorted) {
	int* vectorX = (int*)malloc(sizeof(int)*n);//n만큼 동적 할당
	int lv = level(best_index);//best_index의 level을 구함
	int index = best_index;
	for (int i = 0; i <= lv; i++) {
		if (index % 2 == 1)//index가 왼쪽 노드인 경우 1
			vectorX[(sorted[i].index)] = 1;
		else//index가 오른쪽 노드인 경우 0
			vectorX[sorted[i].index] = 0;
		index = (index - 1) / 2;//best의 부모노드
	}
	for (int i = lv+1; i < n; i++) {
		vectorX[sorted[i].index] = 0;//best보다 아래 level의 item은 0
	}
	return vectorX;
}

int main() {
	//컴퓨터공학과 187375 정하늘
	FILE*fp;//file pointer 선언
	char file_name[30] = "p2data1.txt";//파일 이름을 저장할 문자배열 선언
	fp = fopen(file_name, "r");//파일 open
	item *list;//item을 저장할 list 변수 선언
	int n;//data의 개수를 저장
	int n2;//파일에서 읽은 data를 저장

	node array[MAX_SIZE];
	
	fscanf(fp, "%d", &n);//파일에서 data 개수를 먼저 읽어온 후 n에 저장
	list = (item*)malloc(sizeof(item)*n);//n만큼 동적 할당
	for (int i = 0; i < n; i++) {
		list[i].index = i;
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d", &n2);//n2에 data 저장
		list[i].p = n2;//list에 data 저장
	}//pi
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d", &n2);//n2에 data 저장
		list[i].w = n2;//list에 data 저장
	}//wi
	for (int i = 0; i < n; i++) {
		list[i].ppw = (float)list[i].p / list[i].w;
	}
	item* sorted = insertion_sort(n, list);
	fscanf(fp, "%d", &M);//파일에서 data 개수를 먼저 읽어온 후 n에 저장

	bfs_bb(array, sorted, 0, n);

	printf("%s 결과 출력\n", file_name);
	printf("n = %d\n", n);
	printf("pi :");
	for (int i = 0; i < n; i++) {
		printf(" %d", list[i].p);
	}
	printf("\n");
	printf("wi :");
	for (int i = 0; i < n; i++) {
		printf(" %d", list[i].w);
	}
	printf("\n");
	printf("pi/wi :");
	for (int i = 0; i < n; i++) {
		printf(" %.1f", list[i].ppw);
	}
	printf("\n");
	printf("M = %d\n", M);
	printf("The maximum profit is $%d\n", best);
	printf("The solution vector X = (");
	int *solution_Vector = get_solution_vector(n,sorted);
	for (int i = 0; i < n; i++) {
		if (i != n-1)
			printf("%d\t", solution_Vector[i]);
		else
			printf("%d", solution_Vector[i]);
	}
	printf(")\n");
}

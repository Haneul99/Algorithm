#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX_SIZE 10000//queue�� �ִ� size

int front = 0;//queue�� �� ��
int rear = 0;//queue�� �� ��
int M;//Maximum capacity
int best;//optimal
int best_index;//optimal node index

typedef struct ITEM { //���濡 ���� ����
	int index; //item�� index
	int p;//profit
	int w;//weight
	float ppw; //p/w;
}item;

typedef struct Node {
	int weight;//���� ����
	int profit;//������� ����
	int index;//����ȣ
	float bound;//bound
}node;

item* insertion_sort(int n, item* list) {
	int i = 0, j = 0;
	item* sorted_list = (item*)malloc(sizeof(item)*n);//list�� ���� ������ item type�� sorted_list
	for (int i = 0; i < n; i++) {
		sorted_list[i].index = list[i].index;
		sorted_list[i].p = list[i].p;
		sorted_list[i].w = list[i].w;
		sorted_list[i].ppw = list[i].ppw;
	}//sorted list�� list�� ����
	item tmp;
	for (j = 1; j < n; j++) {
		tmp = sorted_list[j];
		i = j - 1;
		while (i >= 0 && sorted_list[i].ppw < tmp.ppw) {//ppw ���� ���� ����
			sorted_list[i + 1] = sorted_list[i];
			i--;
		}
		sorted_list[i + 1] = tmp;
	}
	return sorted_list;
}

int level(int n) {
	if (n == 0) return 0;//root �� ���
	n++;//index�� 0���� �����ϱ� ������ +1
	int level = (int)(log(n) / log(2))-1;
	return level;
}

int value(node temp) {
	if (best < temp.profit) {
		best = temp.profit;//best�� node�� profit�� ��
		best_index = temp.index;//index update
	}
	return best;
}

float bound(int n, item* sorted, node parent_node, int size) {
	float totprofit = 0;
	float totweight = 0;
	int i = 0;
	int lv=level(n);//sorted�� ���° item���� üũ�ϱ� ���� level ��
	if (n == 0) {//root�� ���
		while (totweight <= M && lv <= size) {//total weight�� M�����̰�, lv�� item ���� ������ ����
			if (totweight + sorted[i].w > M)//total weight�� M���� Ŀ���� break
				break;
			totprofit += sorted[i].p;//total profit update
			totweight += sorted[i].w;//total weight update
			i++;
		}
		totprofit += (sorted[i].p)*(M - totweight)/sorted[i].w;//���� weight��ŭ fractional �� ����
	}
	else {
		//�����ڽĳ��, 1�� ���
		if (n % 2 == 1) {
			totprofit = parent_node.bound;//�θ����� bound ���� ������
		}
		//�������ڽĳ��, 0�� ���
		else {
			totweight = parent_node.weight;//�θ����� weight���� ������
			totprofit = parent_node.profit;//�θ����� profit���� ������
			lv++;//�ش� level�� item�� �ǳʶٰ� ���� item���� �Ѿ
			while (totweight <= M && lv<= size) {//total weight�� M�����̰�, lv�� item ���� ������ ����
				if (totweight + sorted[lv].w > M) break;//total weight�� M���� Ŀ���� break
				totprofit += sorted[lv].p;//total profit update
				totweight += sorted[lv].w;//total weight update
				lv++;
			}
			if(lv<=size) totprofit += (sorted[lv].p)*(M - totweight) / sorted[lv].w;//���� weight��ŭ fractional �� ����
		}
	}
	return totprofit;
}

int IsEmpty(void) {
	if (front == rear)//front�� rear�� ������ ť�� ����ִ� ���� 
		return true;
	else return false;
}
bool promising(node temp) {
	//promising ���� : weight�� ���� ���� ��, value���� bound�� Ŭ ��
	if (temp.weight<=M && temp.bound > best) return true;
	else return false;
}
void enqueue(node array[], node temp) {
	array[rear] = temp;//queue�� ����
	rear = (rear + 1) % MAX_SIZE;
}
node dequeue(node array[]) {
	int i = front;
	front = (front + 1) % MAX_SIZE;//queue���� ����
	return array[i];
}

void bfs_bb(node array[], item* sorted,int n, int size) {
	node temp;//������ node�� ������ ����
	node parent_node;
	//root�� ���
	if (n == 0){
		temp.weight = 0;
		temp.profit = 0;
		temp.index = 0;
		temp.bound = 0;
		temp.bound = bound(n, sorted, temp, size);
		enqueue(array, temp);
	}//root �ʱ�ȭ
	while (!IsEmpty()) {//ť�� empty�Ҷ����� �ݺ�
		parent_node = dequeue(array);//�θ��� ť���� ����
		best = value(parent_node);//best ���� �ʱ�ȭ
		for (int i = 2 * parent_node.index + 1; i < 2 * parent_node.index + 3 ; i++) {//dequeue�� ����� �ڽĳ��
			//�����ڽĳ��(1,3,5,7,9��)�� 1
			if (i % 2 == 1) {
				temp.weight = parent_node.weight + sorted[level(i)].w;//�θ����� weight�� item[lv]�� weight ����
				temp.profit = parent_node.profit + sorted[level(i)].p;//�θ����� profit�� item[lv]�� profit ����
				temp.index = i;
				temp.bound = bound(i, sorted, parent_node, size);
				if (temp.bound < temp.profit) temp.bound = 0;
			}
			//�������ڽĳ��(2,4,6,8,10��)�� 0
			else {
				temp.weight = parent_node.weight;//�θ����� weight�� ������
				temp.profit = parent_node.profit;//�θ����� profit�� ������
				temp.index = i;
				temp.bound = bound(i, sorted, parent_node, size);
				if (temp.bound < temp.profit) temp.bound = 0;
			}
			if (promising(temp)) {//temp�� promising�ϴٸ�
				best = value(temp);//best�� ���
				enqueue(array, temp);//�ڽĳ�带 enqueue
			}
		}
	}
}

int* get_solution_vector(int n, item* sorted) {
	int* vectorX = (int*)malloc(sizeof(int)*n);//n��ŭ ���� �Ҵ�
	int lv = level(best_index);//best_index�� level�� ����
	int index = best_index;
	for (int i = 0; i <= lv; i++) {
		if (index % 2 == 1)//index�� ���� ����� ��� 1
			vectorX[(sorted[i].index)] = 1;
		else//index�� ������ ����� ��� 0
			vectorX[sorted[i].index] = 0;
		index = (index - 1) / 2;//best�� �θ���
	}
	for (int i = lv+1; i < n; i++) {
		vectorX[sorted[i].index] = 0;//best���� �Ʒ� level�� item�� 0
	}
	return vectorX;
}

int main() {
	//��ǻ�Ͱ��а� 187375 ���ϴ�
	FILE*fp;//file pointer ����
	char file_name[30] = "p2data1.txt";//���� �̸��� ������ ���ڹ迭 ����
	fp = fopen(file_name, "r");//���� open
	item *list;//item�� ������ list ���� ����
	int n;//data�� ������ ����
	int n2;//���Ͽ��� ���� data�� ����

	node array[MAX_SIZE];
	
	fscanf(fp, "%d", &n);//���Ͽ��� data ������ ���� �о�� �� n�� ����
	list = (item*)malloc(sizeof(item)*n);//n��ŭ ���� �Ҵ�
	for (int i = 0; i < n; i++) {
		list[i].index = i;
	}
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d", &n2);//n2�� data ����
		list[i].p = n2;//list�� data ����
	}//pi
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%d", &n2);//n2�� data ����
		list[i].w = n2;//list�� data ����
	}//wi
	for (int i = 0; i < n; i++) {
		list[i].ppw = (float)list[i].p / list[i].w;
	}
	item* sorted = insertion_sort(n, list);
	fscanf(fp, "%d", &M);//���Ͽ��� data ������ ���� �о�� �� n�� ����

	bfs_bb(array, sorted, 0, n);

	printf("%s ��� ���\n", file_name);
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

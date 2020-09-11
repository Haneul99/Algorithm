#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_SIZE 100000

double sorted[MAX_SIZE];
// ���ĵ� ����Ʈ�� ������ �߰� ����


void merge(double list[], int left, int mid, int right, int*merge_cmp)
{
	int i, j, k, l;
	i = left; j = mid + 1; k = left;
	// i�� ���ĵ� ���ʸ���Ʈ�� ���� �ε���
	// j�� ���ĵ� �����ʸ���Ʈ�� ���� �ε���
	// k�� ���ĵ� ����Ʈ�� ���� �ε���

	// ���� ���ĵ� list�� �պ�
	while (i <= mid && j <= right) {
		(*merge_cmp)++;//�պ� �������� �� Ƚ�� ����
		if (list[i] <= list[j]) {
			sorted[k++] = list[i++];
		}
		else sorted[k++] = list[j++];
	}

	if (i > mid) // ���� �ִ� ���ڵ��� �ϰ� ����
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	else // ���� �ִ� ���ڵ��� �ϰ� ����
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];

	// �迭 sorted[]�� ����Ʈ�� �迭 list[]�� ����
	for (l = left; l <= right; l++)
		list[l] = sorted[l];
}

void merge_sort(double list[], int left, int right, int*merge_cmp) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2; // ����Ʈ�� �յ����
		merge_sort(list, left, mid, merge_cmp); // �κи���Ʈ ����
		merge_sort(list, mid + 1, right, merge_cmp);//�κи���Ʈ ����
		merge(list, left, mid, right, merge_cmp); // ����
	}
}

void f_print(double *array, int cmp, char file_name[], int n, double time) {
	printf("�Է� ���� = %s\n", file_name);
	printf("���� ��� =\n");
	if (n >= 20) {//�Է� �����Ͱ� 20�� �̻��� ���
		for (int i = 0; i < 10; i++) {//���� 10�� ���
			printf("%.4lf\t", array[i]);
		}
		printf("\n");
		for (int i = 10; i > 0; i--) {//���� 10�� ���
			printf("%.4lf\t", array[n - i]);
		}
		printf("\n");
	}
	else {//�Է� �����Ͱ� 20�� �̸��� ���
		for (int i = 0; i < n; i++) {//���� ���
			printf("%.4lf\t", array[i]);
		}
		printf("\n");
	}
	printf("�� Ƚ�� = %d\n", cmp);
	printf("���� �ð� = %.4f\n", time);
}


int main() {

	FILE*fp;//file pointer ����
	char file_name[30] = "d_data1.txt";//���� �̸��� ������ ���ڹ迭 ����
	fp = fopen(file_name, "r");//���� open

	double *list1;//double type��  list1 ���� ����
	int n;//data�� ������ ����
	double n2;//���Ͽ��� ���� data�� ����

	clock_t start, end;//�ð��� ������ ����
	double result;//���� �ð��� ������ ����

	start = clock(); // ���� �ð� ���� ���� 

	fscanf(fp, "%d", &n);//���Ͽ��� data ������ ���� �о�� �� n�� ����
	list1 = (double*)malloc(sizeof(double)*n);//n��ŭ double type���� ���� �Ҵ�
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf", &n2);//n2�� data ����
		list1[i] = n2;//list1�� data ����
	}

	int merge_cmp = 0;//merge sort�� ��Ƚ���� ����
	int left = 0;//merge sort�� ȣ���� ���� ����
	int right = n - 1;//merge sort�� ȣ���� ���� ����

	printf("merge sort: \n");
	merge_sort(list1, left, right, &merge_cmp);

	end = clock(); //�ð� ���� ��
	result = (double)(end - start) / CLOCKS_PER_SEC;
	f_print(list1, merge_cmp, file_name, n, result);//��� ���
}
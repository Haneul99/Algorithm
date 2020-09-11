#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<malloc.h>

#define SWAP(x,y,t) ( (t)=(x), (x)=(y), (y)=(t) )

int partition(double* array, int left, int right, int* quick_cmp) {
	double pivot, temp;
	int low, high;

	low = left;
	high = right + 1;
	pivot = array[left]; //������ ����Ʈ�� ���� ���� �����͸� pivot���� ����
	if (low < high)(*quick_cmp)++;
	do {
		do {//array[low]�� pivot���� ������ low ����
			low++;
			if (low <= right && array[low] < pivot) (*quick_cmp)++;
		} while (low <= right && array[low] < pivot);

		do {//array[high]�� pivot���� ũ�� ��� high ����
			high--;
			if (high >= left && array[high] > pivot) (*quick_cmp)++;
		} while (high >= left && array[high] > pivot);

		if (low < high) {//low�� high�� �������� �ʾ��� ��� list[low]�� list[high] ��ȯ
			(*quick_cmp)++;
			SWAP(array[low], array[high], temp);
		}
	} while (low < high);

	SWAP(array[left], array[high], temp);//low�� high�� �������� ��� list[left]�� list[high] ��ȯ
	return high;//pivot�� ��ġ high return

}

double* quick_sort(double* array, int left, int right, int*quick_cmp) {
	if (left < right) {//������ ������ 2�� �̻��� �������� ���
		int q = partition(array, left, right, quick_cmp);//q=pivot�� ��ġ
		quick_sort(array, left, q - 1, quick_cmp);//pivot ���� ���� ����Ʈ ����
		quick_sort(array, q + 1, right, quick_cmp);//pivot ���� ������ ����Ʈ ����
	}
	return array;
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
	char file_name[30] = "r_data1.txt";//���� �̸��� ������ ���ڹ迭 ����
	fp = fopen(file_name, "r");//���� open
	double *list1;//double type��  list1 ���� ����
	int n;//data�� ������ ����
	double n2;//���Ͽ��� ���� data�� ����
	clock_t start, end;
	double result;
	start = clock(); // ���� �ð� ���� ���� 

	fscanf(fp, "%d", &n);//���Ͽ��� data ������ ���� �о�� �� n�� ����
	list1 = (double*)malloc(sizeof(double)*n);//n��ŭ double type���� ���� �Ҵ�
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf", &n2);//n2�� data ����
		list1[i] = n2;//list1�� data ����
	}

	int quick_cmp = 0;//quick sort�� ��Ƚ���� ����
	int left = 0;//quick sort�� ȣ���� ���� ����
	int right = n - 1;//quick sort�� ȣ���� ���� ����

	printf("quick sort: \n");
	quick_sort(list1, left, right, &quick_cmp);


	end = clock(); //�ð� ���� ��
	result = (double)(end - start)/CLOCKS_PER_SEC;
	f_print(list1, quick_cmp, file_name, n, result);

}

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<windows.h>

#define SWAP(x,y,t) ( (t)=(x), (x)=(y), (y)=(t) )


double* insertion_sort(double *array,int*ins_cmp,int n) {
	int i, j;
	double  key;
	for (i = 1; i < n; i++) {
		key = array[i];//���� ���ε� i��° �Ǽ��� key�� ����
		if(array[0]<key) (*ins_cmp)++;//�̹� ���ĵ� ��� ��Ƚ�� count
		for (j = i - 1; j >= 0 && array[j] > key; j--) {
			(*ins_cmp)++;//���ĵ��� ���� ������ ��Ƚ�� count
			array[j + 1] = array[j];//���ڵ��� ���������� �̵�
		}
		array[j + 1] = key;
	}
	return array;
}

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
			if(low <= right && array[low] < pivot) (*quick_cmp)++;
		} while (low <= right && array[low]< pivot);

		do {//array[high]�� pivot���� ũ�� ��� high ����
			high--;
			if(high >= left && array[high] > pivot) (*quick_cmp)++;
		} while(high >= left && array[high] > pivot);
		
		if (low < high) {//low�� high�� �������� �ʾ��� ��� list[low]�� list[high] ��ȯ
			(*quick_cmp)++;
			SWAP(array[low], array[high], temp);
		}
	} while (low < high);
	
	SWAP(array[left], array[high], temp);//low�� high�� �������� ��� list[left]�� list[high] ��ȯ
	return high;//pivot�� ��ġ high return

}

double* quick_sort(double* array, int left, int right,int*quick_cmp) {
	if (left < right) {//������ ������ 2�� �̻��� �������� ���
		int q = partition(array, left, right, quick_cmp);//q=pivot�� ��ġ
		quick_sort(array, left, q - 1,quick_cmp);//pivot ���� ���� ����Ʈ ����
		quick_sort(array, q + 1, right,quick_cmp);//pivot ���� ������ ����Ʈ ����
	}
	return array;
}

void adjust(double *array, int root, int n, int*heap_cmp) {
	int child;
	double rootkey;
	rootkey = array[root];
	child = 2 * root + 1;
	while (child <= n - 1) {//�ڽķ�Ʈ�� ���� ����
		(*heap_cmp)++;
		if ((child < n - 1) && (array[child] < array[child + 1])) child++;//
		
		(*heap_cmp)++;
		if (rootkey > array[child]) break;//rootkey�� ���� �� Ŀ���� while�� Ż��
		else {
			array[(child - 1) / 2] = array[child];
			child = 2 * child + 1;
		}
	}
	array[(child - 1) / 2] = rootkey;
}

double* heap_sort(double *array,int*heap_cmp, int n) {
	int i;
	double temp;
	for (i = (n - 1) / 2; i >= 0; i--) {
		adjust(array, i, n, heap_cmp);
	}//�ִ������� ����

	for (i = n - 2; i >= 0; i--) {
		SWAP(array[0], array[i + 1], temp);//���� ū ��Ʈ���� ����
		adjust(array, 0, i, heap_cmp);//�ٽ� �ִ��� �籸��
	}
	if (array[0] > array[1]) SWAP(array[0], array[1], temp);
	(*heap_cmp)++;
	return array;
}

void f_print(double *array,int cmp, char file_name[],int n) {
	printf("�Է� ���� = %s\n",file_name);
	printf("���� ��� =\n");
	if (n >= 20) {//�Է� �����Ͱ� 20�� �̻��� ���
		for (int i = 0; i < 10; i++) {//���� 10�� ���
			printf("%.4lf\t", array[i]);
		}
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
	printf("�� Ƚ�� = %d\n",cmp);
}


int main() {
	//��ǻ�Ͱ��а� 187375 ���ϴ�
	FILE*fp;//file pointer ����
	char file_name[30] = "a_data1.txt";//���� �̸��� ������ ���ڹ迭 ����
	fp = fopen(file_name, "r");//���� open
	double *list1;//double type��  list1 ���� ����
	int n;//data�� ������ ����
	double n2;//���Ͽ��� ���� data�� ����

	fscanf(fp, "%d", &n);//���Ͽ��� data ������ ���� �о�� �� n�� ����
	list1 = (double*)malloc(sizeof(double)*n);//n��ŭ double type���� ���� �Ҵ�
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf", &n2);//n2�� data ����
		list1[i] = n2;//list1�� data ����
	}
	int ins_cmp=0;//insersion sort�� ��Ƚ���� ����
	int quick_cmp=0;//quick sort�� ��Ƚ���� ����
	int heap_cmp=0;//heap sort�� ��Ƚ���� ����


	int left = 0;//quick sort�� ȣ���� ���� ����
	int right = n-1;//quick sort�� ȣ���� ���� ����

	//printf("Insertion sort: \n");
	//insertion_sort(list1,&ins_cmp,n);
	//f_print(list1,ins_cmp,file_name,n);

	//printf("quick sort: \n");
	//quick_sort(list1,left,right,&quick_cmp);
	//f_print(list1,quick_cmp,file_name,n);

	printf("heap sort: \n");
	heap_sort(list1,&heap_cmp,n);
	f_print(list1,heap_cmp, file_name,n);

}

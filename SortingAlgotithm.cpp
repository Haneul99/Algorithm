#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<windows.h>

#define SWAP(x,y,t) ( (t)=(x), (x)=(y), (y)=(t) )


double* insertion_sort(double *array,int*ins_cmp,int n) {
	int i, j;
	double  key;
	for (i = 1; i < n; i++) {
		key = array[i];//현재 삽인된 i번째 실수를 key에 저장
		if(array[0]<key) (*ins_cmp)++;//이미 정렬된 경우 비교횟수 count
		for (j = i - 1; j >= 0 && array[j] > key; j--) {
			(*ins_cmp)++;//정렬되지 않은 상태의 비교횟수 count
			array[j + 1] = array[j];//레코드의 오른쪽으로 이동
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
	pivot = array[left]; //정렬할 리스트의 가장 왼쪽 데이터를 pivot으로 선택
	if (low < high)(*quick_cmp)++;
	do {
		do {//array[low]가 pivot보다 작으면 low 증가
			low++;
			if(low <= right && array[low] < pivot) (*quick_cmp)++;
		} while (low <= right && array[low]< pivot);

		do {//array[high]가 pivot보다 크면 계속 high 감소
			high--;
			if(high >= left && array[high] > pivot) (*quick_cmp)++;
		} while(high >= left && array[high] > pivot);
		
		if (low < high) {//low와 high가 교차하지 않았을 경우 list[low]와 list[high] 교환
			(*quick_cmp)++;
			SWAP(array[low], array[high], temp);
		}
	} while (low < high);
	
	SWAP(array[left], array[high], temp);//low와 high가 교차했을 경우 list[left]와 list[high] 교환
	return high;//pivot의 위치 high return

}

double* quick_sort(double* array, int left, int right,int*quick_cmp) {
	if (left < right) {//정렬할 범위가 2개 이상의 데이터일 경우
		int q = partition(array, left, right, quick_cmp);//q=pivot의 위치
		quick_sort(array, left, q - 1,quick_cmp);//pivot 기준 왼쪽 리스트 정렬
		quick_sort(array, q + 1, right,quick_cmp);//pivot 기준 오른쪽 리스트 정렬
	}
	return array;
}

void adjust(double *array, int root, int n, int*heap_cmp) {
	int child;
	double rootkey;
	rootkey = array[root];
	child = 2 * root + 1;
	while (child <= n - 1) {//자식루트가 있을 동안
		(*heap_cmp)++;
		if ((child < n - 1) && (array[child] < array[child + 1])) child++;//
		
		(*heap_cmp)++;
		if (rootkey > array[child]) break;//rootkey의 값이 더 커지면 while문 탈출
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
	}//최대힙으로 구성

	for (i = n - 2; i >= 0; i--) {
		SWAP(array[0], array[i + 1], temp);//가장 큰 루트값을 내림
		adjust(array, 0, i, heap_cmp);//다시 최대힙 재구성
	}
	if (array[0] > array[1]) SWAP(array[0], array[1], temp);
	(*heap_cmp)++;
	return array;
}

void f_print(double *array,int cmp, char file_name[],int n) {
	printf("입력 파일 = %s\n",file_name);
	printf("정렬 결과 =\n");
	if (n >= 20) {//입력 데이터가 20개 이상일 경우
		for (int i = 0; i < 10; i++) {//앞의 10개 출력
			printf("%.4lf\t", array[i]);
		}
		for (int i = 10; i > 0; i--) {//뒤의 10개 출력
			printf("%.4lf\t", array[n - i]);
		}
		printf("\n");
	}
	else {//입력 데이터가 20개 미만일 경우
		for (int i = 0; i < n; i++) {//전부 출력
			printf("%.4lf\t", array[i]);
		}
		printf("\n");
	}
	printf("비교 횟수 = %d\n",cmp);
}


int main() {
	//컴퓨터공학과 187375 정하늘
	FILE*fp;//file pointer 선언
	char file_name[30] = "a_data1.txt";//파일 이름을 저장할 문자배열 선언
	fp = fopen(file_name, "r");//파일 open
	double *list1;//double type의  list1 변수 선언
	int n;//data의 개수를 저장
	double n2;//파일에서 읽은 data를 저장

	fscanf(fp, "%d", &n);//파일에서 data 개수를 먼저 읽어온 후 n에 저장
	list1 = (double*)malloc(sizeof(double)*n);//n만큼 double type으로 동적 할당
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf", &n2);//n2에 data 저장
		list1[i] = n2;//list1에 data 저장
	}
	int ins_cmp=0;//insersion sort의 비교횟수를 저장
	int quick_cmp=0;//quick sort의 비교횟수를 저장
	int heap_cmp=0;//heap sort의 비교횟수를 저장


	int left = 0;//quick sort의 호출을 위한 변수
	int right = n-1;//quick sort의 호출을 위한 변수

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

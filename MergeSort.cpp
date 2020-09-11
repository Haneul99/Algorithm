#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MAX_SIZE 100000

double sorted[MAX_SIZE];
// 정렬된 리스트를 저장할 추가 공간


void merge(double list[], int left, int mid, int right, int*merge_cmp)
{
	int i, j, k, l;
	i = left; j = mid + 1; k = left;
	// i는 정렬된 왼쪽리스트에 대한 인덱스
	// j는 정렬된 오른쪽리스트에 대한 인덱스
	// k는 정렬될 리스트에 대한 인덱스

	// 분할 정렬된 list의 합병
	while (i <= mid && j <= right) {
		(*merge_cmp)++;//합병 과정에서 비교 횟수 증가
		if (list[i] <= list[j]) {
			sorted[k++] = list[i++];
		}
		else sorted[k++] = list[j++];
	}

	if (i > mid) // 남아 있는 레코드의 일괄 복사
		for (l = j; l <= right; l++)
			sorted[k++] = list[l];
	else // 남아 있는 레코드의 일괄 복사
		for (l = i; l <= mid; l++)
			sorted[k++] = list[l];

	// 배열 sorted[]의 리스트를 배열 list[]로 복사
	for (l = left; l <= right; l++)
		list[l] = sorted[l];
}

void merge_sort(double list[], int left, int right, int*merge_cmp) {
	int mid;
	if (left < right) {
		mid = (left + right) / 2; // 리스트의 균등분할
		merge_sort(list, left, mid, merge_cmp); // 부분리스트 정렬
		merge_sort(list, mid + 1, right, merge_cmp);//부분리스트 정렬
		merge(list, left, mid, right, merge_cmp); // 병합
	}
}

void f_print(double *array, int cmp, char file_name[], int n, double time) {
	printf("입력 파일 = %s\n", file_name);
	printf("정렬 결과 =\n");
	if (n >= 20) {//입력 데이터가 20개 이상일 경우
		for (int i = 0; i < 10; i++) {//앞의 10개 출력
			printf("%.4lf\t", array[i]);
		}
		printf("\n");
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
	printf("비교 횟수 = %d\n", cmp);
	printf("수행 시간 = %.4f\n", time);
}


int main() {

	FILE*fp;//file pointer 선언
	char file_name[30] = "d_data1.txt";//파일 이름을 저장할 문자배열 선언
	fp = fopen(file_name, "r");//파일 open

	double *list1;//double type의  list1 변수 선언
	int n;//data의 개수를 저장
	double n2;//파일에서 읽은 data를 저장

	clock_t start, end;//시간을 측정할 변수
	double result;//측정 시간을 저장할 변수

	start = clock(); // 수행 시간 측정 시작 

	fscanf(fp, "%d", &n);//파일에서 data 개수를 먼저 읽어온 후 n에 저장
	list1 = (double*)malloc(sizeof(double)*n);//n만큼 double type으로 동적 할당
	for (int i = 0; i < n; i++) {
		fscanf(fp, "%lf", &n2);//n2에 data 저장
		list1[i] = n2;//list1에 data 저장
	}

	int merge_cmp = 0;//merge sort의 비교횟수를 저장
	int left = 0;//merge sort의 호출을 위한 변수
	int right = n - 1;//merge sort의 호출을 위한 변수

	printf("merge sort: \n");
	merge_sort(list1, left, right, &merge_cmp);

	end = clock(); //시간 측정 끝
	result = (double)(end - start) / CLOCKS_PER_SEC;
	f_print(list1, merge_cmp, file_name, n, result);//결과 출력
}
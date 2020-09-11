#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

struct MEMORY {
	int page_num;
	int reference_bit;
	int dirty_bit;
	bool flag;
};

struct PAGE {
	int memory_location;
	int valid_bit;
};

struct PAGE page[200000];

int main(int argc, char*argv[]) {
	int memory_size = atoi(argv[1]);
	FILE *input_file = fopen("a.txt", "r");
	int page_fault = 0;
	int clock = 0;
	int disk_write = 0;
	int head_move = 0;
	int head_place = 0;
	int pg;
	int op;

	int n = 0;
	struct MEMORY *memory = (struct MEMORY*)malloc(memory_size * sizeof(struct MEMORY));
	for (int i = 0; i < memory_size; i++) {
		memory[i].reference_bit = 0;
		memory[i].dirty_bit = 0;
		memory[i].flag = false;
	}
	for (int i = 0; i < 200000; i++) page[i].valid_bit = 0;
	while (!feof(input_file)) {
		fscanf(input_file, "%d %d", &pg, &op);
		if (page[pg].valid_bit == 1) {
			memory[page[pg].memory_location].reference_bit = 1;
			if (op == 1)
				memory[page[pg].memory_location].dirty_bit = 1;
		}
		else {
			page_fault++;
			if (memory[clock].flag == true) {
				if (memory[clock].reference_bit == 1) {
					while (memory[clock].reference_bit == 1) {
						memory[clock].reference_bit = 0;
						if (clock == memory_size-1) clock = 0;
						else clock++;
					}
				}
				page[memory[clock].page_num].valid_bit = 0;
				if (memory[clock].dirty_bit == 1) {
					disk_write++;
					head_move = head_move + abs(head_place - (memory[clock].page_num / 1000));
					head_place = memory[clock].page_num / 1000;
				}
			}
			head_move += abs(head_place - (pg / 1000));
			head_place = pg / 1000;
			memory[clock].page_num = pg;
			if (op == 1)	memory[clock].dirty_bit = 1;
			else memory[clock].dirty_bit = 0;
			memory[clock].reference_bit = 0;
			page[pg].memory_location = clock;
			page[pg].valid_bit = 1;
			memory[clock].flag = true;
			if (clock == memory_size - 1) clock = 0;
			else clock++;
		}
	}
	printf("Page fault count: %d\nDisk write count:%d\nHead movement: %d\n", page_fault, disk_write, head_move);
}
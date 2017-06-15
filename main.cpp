#include "external_includes.h"
#include "functions.h"
#include "machine.h"

void *print_text_thread(void *arg);
void *keypad_thread(void *arg);
void *dot_matrix_thread(void *arg);

void *change_text_thread(void *arg);
void *formula_thread(void *arg);

void unset_memory(int fd);

int main()
{
	int fd;
	pthread_t print_text_t, keypad_t, dot_matrix_t, formula_t;
	pthread_t change_text_t;

	if ((fd = open(MEMORY_PATH, O_RDWR | O_SYNC)) < 0)
	{
		printf("mem open failed\n");
		return 0;
	}
	addr_fpga = (unsigned short *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BASEADDR);

	// 기계 쓰레드
	pthread_create(&print_text_t, NULL, print_text_thread, fd);
	pthread_create(&keypad_t, NULL, keypad_thread, fd);
	pthread_create(&dot_matrix_t, NULL, dot_matrix_thread, fd);

	// 동작 쓰레드
	pthread_create(&change_text_t, NULL, change_text, NULL);
	pthread_create(&formula_t, NULL, formula_thread, NULL);


	// 기계 쓰레드 조인
	pthread_join(print_text_t, NULL);
	pthread_join(keypad_t, NULL);
	pthread_join(dot_matrix_t, NULL);

	// 동작 쓰레드
	pthread_join(change_text_t, NULL);
	pthread_join(formula_t, NULL);

	unset_memory(fd);
	return 0;
}

void *print_text_thread(void *arg)
{
	print("print_text_thread start!");
	print_text((int)&arg);
}
void *keypad_thread(void *arg)
{
	print("keypad_thread start!");
	keypad((int)&arg);
}
void *dot_matrix_thread(void *arg)
{
	print("dot_matrix_thread start!");
	dot_matrix((int)&arg);
}

void *change_text_thread(void *arg)
{
    while(1) change_text();
}
void *formula_thread(void *arg)
{

}

void unset_memory(int fd)
{
	munmap(addr_fpga, 4096);
	munmap(pTextlcd, 4096);
	munmap(dot_row_addr, 4096);
	munmap(dot_col_addr, 4096);
	munmap(keypad_row_addr, 4096);
	munmap(keypad_col_addr, 4096);
	
	close(fd)
}

#include "external_includes.h"
#include "functions.h"
#include "machine.h"
#include "thread_functions.h"

void unset_memory(int fd);

int main()
{
	int fd;
	pthread_t print_text_t, keypad_t, dot_matrix_t, formula_t;
	pthread_t state_change_t, text_change_t;

	if ((fd = open(MEMORY_PATH, O_RDWR | O_SYNC)) < 0)
	{
		printf("mem open failed\n");
		return 0;
	}
	addr_fpga = (unsigned short *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BASEADDR);

    // 뮤텍스 초기화
    pthread_mutex_init(&PRINT_TEXT_MUTEX, NULL);
    pthread_mutex_init(&KEYPAD_MUTEX, NULL);
    pthread_mutex_init(&DOT_MATRIX_MUTEX, NULL);

    pthread_mutex_init(&CURRENT_STATE_MUTEX, NULL);
    pthread_mutex_init(&BUTTON_MUTEX, NULL);

    // 기계 쓰레드
	pthread_create(&print_text_t, NULL, print_text_thread, NULL);
	pthread_create(&keypad_t, NULL, keypad_thread, NULL);
	pthread_create(&dot_matrix_t, NULL, dot_matrix_thread, NULL);

	// 동작 쓰레드
    pthread_create(&state_change_t, NULL, state_change_thread, NULL);
	pthread_create(&text_change_t, NULL, text_change_thread, NULL);
	//pthread_create(&formula_t, NULL, formula_thread, NULL);


	// 기계 쓰레드 조인
	pthread_join(print_text_t, NULL);
	pthread_join(keypad_t, NULL);
	pthread_join(dot_matrix_t, NULL);

	// 동작 쓰레드
	pthread_join(change_text_t, NULL);
	//pthread_join(formula_t, NULL);

	unset_memory(fd);
	return 0;
}

// 기계 콜백 함수
void *print_text_thread(void *arg)
{
	printf("print_text_thread start!");
	print_text();
}
void *keypad_thread(void *arg)
{
    printf("keypad_thread start!");
	keypad();
}
void *dot_matrix_thread(void *arg)
{
    printf("dot_matrix_thread start!");
	dot_matrix();
}

// 동작 콜백 함수
void *state_change_thread(void *arg)
{
    state_change();
}
void *text_change_thread(void *arg)
{
    text_change();
}
void *formula_thread(void *arg)
{

}

void unset_memory(int fd)
{
	munmap(addr_fpga, 4096);
//	munmap(pTextlcd, 4096);
//	munmap(dot_row_addr, 4096);
//	munmap(dot_col_addr, 4096);
//	munmap(keypad_row_addr, 4096);
//	munmap(keypad_col_addr, 4096);

    pthread_mutex_destroy(&PRINT_TEXT_MUTEX);
    pthread_mutex_destroy(&KEYPAD_MUTEX);
    pthread_mutex_destroy(&DOT_MATRIX_MUTEX);

    pthread_mutex_destroy(&CURRENT_STATE_MUTEX);
    pthread_mutex_destroy(&BUTTON_MUTEX);

    close(fd);
}

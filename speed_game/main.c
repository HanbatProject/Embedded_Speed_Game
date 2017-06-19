#include "external_includes.h"
#include "functions.h"
#include "machine.h"
#include "thread_functions.h"
#include "state.h"

pthread_mutex_t PRINT_TEXT_MUTEX;
pthread_mutex_t KEYPAD_MUTEX;
pthread_mutex_t DOT_MATRIX_MUTEX;

pthread_mutex_t CURRENT_STATE_MUTEX;
pthread_mutex_t BUTTON_MUTEX;

void unset_memory(int fd);

int main()
{
	int fd;
	pthread_t print_text_t, keypad_t, dot_matrix_t;
	pthread_t state_change_t, text_change_t;
	pthread_t go_time_t;

	printf("초기화\n");
	if ((fd = open(MEMORY_PATH, O_RDWR | O_SYNC)) < 0)
	{
		printf("mem open failed\n");
		return 0;
	}
	addr_fpga = (unsigned short *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FPGA_BASEADDR);

	// 뮤텍스 초기화
	//pthread_mutex_init(&PRINT_TEXT_MUTEX, NULL);
	pthread_mutex_init(&KEYPAD_MUTEX, NULL);
	//pthread_mutex_init(&DOT_MATRIX_MUTEX, NULL);

	//pthread_mutex_init(&CURRENT_STATE_MUTEX, NULL);
	//pthread_mutex_init(&BUTTON_MUTEX, NULL);

	//current_state = STATE_GAME;

	// 기계 쓰레드
	pthread_create(&print_text_t, NULL, print_text_thread, NULL);
	pthread_create(&keypad_t, NULL, keypad_thread, NULL);
	pthread_create(&dot_matrix_t, NULL, dot_matrix_thread, NULL);

	// 동작 쓰레드
	pthread_create(&state_change_t, NULL, state_change_thread, NULL);
	pthread_create(&text_change_t, NULL, text_change_thread, NULL);
	pthread_create(&go_time_t, NULL, go_time_thread, NULL);

	// 기계 쓰레드 조인
	//pthread_join(print_text_t, NULL);
	//pthread_join(keypad_t, NULL);
	//pthread_join(dot_matrix_t, NULL);
	// 동작 쓰레드
	//pthread_join(state_change_t, NULL);
	//pthread_join(text_change_t, NULL);
	//pthread_join(key_input_t, NULL);
	while (1)
	{
		if (button > 0x00) break;
	}
	unset_memory(fd);
	return 0;
}

// 기계 콜백 함수
void *print_text_thread(void *arg)
{
	printf("print_text_thread start!\n");
	print_text(PRINT_TEXT_MUTEX);
}
void *keypad_thread(void *arg)
{
    printf("keypad_thread start!\n");
	keypad(KEYPAD_MUTEX);
}
void *dot_matrix_thread(void *arg)
{
    printf("dot_matrix_thread start!\n");
	dot_matrix(DOT_MATRIX_MUTEX);
}

// 동작 콜백 함수
void *state_change_thread(void *arg)
{
	printf("state_chanage_start\n");
    state_change(CURRENT_STATE_MUTEX);
}
void *text_change_thread(void *arg)
{
	printf("text_chanage_start\n");
    text_change(PRINT_TEXT_MUTEX);
}
void *go_time_thread(void *arg)
{
	printf("go_time_start\n");
	go_time();
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

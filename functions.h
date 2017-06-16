#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// 기계 쓰레드 함수들
void print_text(pthread_mutex_t print_text_mutex);
void dot_matrix(pthread_mutex_t dot_matrix_mutex);
void keypad(pthread_mutex_t keypad_mutex);

// 상태에 따른 변수 변경 함수들
void state_change(pthread_mutex_t current_state_mutex);
void text_change(pthread_mutex_t current_state_mutex);
void score_change();
void time_change();
void next_question();
void is_highscore();

#endif // !FUNCTIONS_H

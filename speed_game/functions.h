#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "pthread.h"

// 기계 쓰레드 함수들
void print_text(pthread_mutex_t);
void dot_matrix(pthread_mutex_t);
void keypad(pthread_mutex_t);

// 상태에 따른 변수 변경 함수들
void state_change(pthread_mutex_t);
void text_change(pthread_mutex_t);
void when_score_change(pthread_mutex_t);
void time_change(pthread_mutex_t);
void next_question();

#endif // !FUNCTIONS_H

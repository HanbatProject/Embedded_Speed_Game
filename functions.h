#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// 기계 쓰레드 함수들
void print_text(int fd);
void dot_matrix(int fd);
void keypad(int fd);

// 상태에 따른 변수 변경 함수들
void state_change();
void text_change();
void score_change();
void time_change();
void next_question();
void is_highscore();

#endif // !FUNCTIONS_H

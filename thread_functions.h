#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

extern pthread_mutex_t PRINT_TEXT_MUTEX, KEYPAD_MUTEX, DOT_MATRIX_MUTEX;
extern pthread_mutex_t CURRENT_STATE_MUTEX, BUTTON_MUTEX;

void *print_text_thread(void *arg);
void *keypad_thread(void *arg);
void *dot_matrix_thread(void *arg);

void *state_change_thread(void *arg);
void *text_change_thread(void *arg);
void *formula_thread(void *arg);


#endif //THREAD_FUNCTIONS_H

#ifndef THREAD_FUNCTIONS_H
#define THREAD_FUNCTIONS_H

void *print_text_thread(void *arg);
void *keypad_thread(void *arg);
void *dot_matrix_thread(void *arg);

void *state_change_thread(void *arg);
void *text_change_thread(void *arg);
void *key_input_thread(void *arg);
void *go_time_thread(void *arg);


#endif //THREAD_FUNCTIONS_H

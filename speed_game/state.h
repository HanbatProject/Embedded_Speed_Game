#ifndef STATE_H
#define STATE_H

int current_state;
#define STATE_MAIN 0
#define STATE_READY 1
#define STATE_GAME 2
#define STATE_GAME_OVER 3

int question_count;
int high_score, game_score, combo;
char formula[10];
int answer, answer_input;


#endif // !STATE_H

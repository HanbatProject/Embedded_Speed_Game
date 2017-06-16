#ifndef STATE_H
#define STATE_H

int current_state;
#define STATE_MAIN 0
#define STATE_READY 1
#define STATE_GAME 2
#define STATE_GAME_OVER 3

int play_game;
#define NOT_PLAY_GAME 0
#define PLAY_GAME 1

int question_count;
int high_score, game_score, combo;
char formula[10];
int answer, answer_input;


int is_correct;
#define ANS_NOTHING 0
#define ANS_CORRECT 1
#define ANS_WRONG 2


#endif // !STATE_H

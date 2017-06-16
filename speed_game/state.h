#ifndef STATE_H
#define STATE_H

int current_state;
#define STATE_MAIN 0
#define STATE_READY 1
#define STATE_GAME 2
#define STATE_GAME_OVER 3

int high_score, game_score, combo;

int is_correct;
#define ANS_NOTHING 0
#define ANS_CORRECT 1
#define ANS_WRONG 2


#endif // !STATE_H

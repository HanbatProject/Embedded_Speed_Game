#include "machine.h"
#include "state.h"

unsigned short *addr_fpga = 0;
char buf1[TEXTLCD_LENGTH] = BLANK, buf2[TEXTLCD_LENGTH] = BLANK;
int ten_number = 3, one_number = 0;
unsigned short button = 0;

int current_state = 0;

int question_count = 1;
int high_score=0, game_score=0, combo = 1;
char formula[10] = "";
int answer=0, answer_input=0;

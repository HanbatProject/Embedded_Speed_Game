#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"
extern char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];
extern int ten_number, one_number;
extern unsigned short button;
extern int current_state;
extern int high_score, game_score;
extern int is_correct;

void state_change(pthread_mutex_t current_state_mutex)
{
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
                if (button > 0x00)
					current_state = STATE_READY;
                break;
            // 게임 준비 상태
            case STATE_READY:
                // 지우기 눌렀을 때
                if (button == 0x1000)
					current_state = STATE_MAIN;
				current_state = STATE_GAME;
                break;
            // 게임 진행 상태
            case STATE_GAME:
				if (ten_number == 0 && one_number == 0)
					current_state = STATE_GAME_OVER;
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
                if (button > 0x00)
					current_state = STATE_MAIN;
                break;
            default:
                current_state = 0;
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}

void init_and_change_highscore(char * print_high_score)
{
	if (high_score == NULL)
		high_score = 0;
	else if (high_score >= game_score)
		high_score = game_score;
	combo = 1;
	game_score = 0;
	itoa(high_score, print_high_score, 10);
	return;
}
void text_change(pthread_mutex_t current_state_mutex)
{
	int i;
	char print_high_score[10];
    char temp[TEXTLCD_LENGTH];
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
                strcpy(buf1, GAME_START_LINE1);
				strcpy(buf2, BLANK);
                strcpy(buf2, GAME_START_LINE2);
				init_and_change_highscore(print_high_score);
				strcat(buf2, print_high_score);
                break;
			// 게임 준비 상태
            case STATE_READY:
                strcpy(buf2, BLANK);
                for (i = 3; i > 0; i--)
                {
                    strcpy(temp, HALF_BLANK);
                    if(i==3) strcat(temp, "3");
                    else if(i==2) strcat(temp, "2");
                    else strcat(temp, "1");
                    strcat(temp, HALF_BLANK);

                    strcpy(buf1, temp);
                    usleep(1000 * 1000);
                }
                strcpy(buf1, START_LINE1);
                usleep(1000 * 1000);
                break;
            // 게임 진행 상태
            case STATE_GAME:
                strcpy(buf1, GAME_LINE1);
				strcpy(buf2, GAME_LINE2);
				strcat(buf2, game_score);
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				if (button > 0x00)
					current_state = STATE_MAIN;
                break;
            default:
                strcpy(buf1, BLANK);
                strcpy(buf2, BLANK);
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}

int get_time()
{
	return ten_number * 10 + one_number;
}
void change_time(int time)
{
	ten_number = (int)(time / 10);
	one_number = (int)(time % 10);
	return;
}

void when_score_change(pthread_mutex_t print_text_mutex)
{
    while(1)
    {
        if (current_state == 2)
        {
			if(is_correct != ANS_NOTHING){
				pthread_mutex_lock(&print_text_mutex);

				if (is_correct == ANS_CORRECT)
				{
					game_score += 10 * combo;
				}
				else if (is_correct == ANS_WRONG)
				{
					change_time(get_time() - 3);
					combo = 1;
				}
				is_correct = ANS_NOTHING;

				pthread_mutex_lock(&print_text_mutex);
			}
        }
    }
}
void time_change(pthread_mutex_t dot_matrix_mutex)
{
    while(1)
    {
        if (current_state == STATE_GAME)
        {
            pthread_mutex_lock(&dot_matrix_mutex);

			change_time(get_time() - 1);
			
            pthread_mutex_unlock(&dot_matrix_mutex);
        }
    }
}
void next_question()
{
	if (current_state == STATE_GAME)
	{
		if(button)
			
	}
}

#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"
extern char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];
extern int ten_number, one_number;
extern unsigned short button;
extern int current_state;

void state_change(pthread_mutex_t current_state_mutex)
{
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case 0:
                if (button > 0x00)
                {
                    current_state = 1;
                }
                break;
                // 게임 준비 상태
            case 1:
                // 지우기 눌렀을 때
                if (button == 0000000000001000)
                {
                    current_state = 0;
                }
                break;
                // 게임 진행 상태
            case 2:
                // if(시간이 다됨)
                // 꺼짐
                break;
                // 게임 오버 상태
            case 3:
                if (button > 0x00)
                {
                    current_state = 0;
                }
                break;
            default:
                current_state = 0;
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}
void text_change(pthread_mutex_t current_state_mutex)
{
    int i;
    char temp[TEXTLCD_LENGTH];
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case 0:
                strcpy(buf1, GAME_START_LINE1);
                strcpy(buf2, GAME_START_LINE2);
                break;
            case 1:
                for (i = 3; i > 0; i--)
                {
                    strcpy(temp, "        ");
                    strcat(temp, (char *) &i);
                    strcat(temp, "        ");

                    strcpy(buf1, temp);
                    usleep(1000 * 1000);
                }
                strcpy(buf1, START_LINE1);
                usleep(1000 * 1000);
                current_state = 0;

                break;
                // 게임 진행 상태
            case 2:
                // if(시간이 다됨)
                // 꺼짐
                break;
                // 게임 오버 상태
            case 3:
                if (button > 0x00) {
                    current_state = 0;
                }
                break;
            default:
                strcpy(buf1, "");
                strcpy(buf2, "");
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}
void score_change(pthread_mutex_t print_text_mutex)
{
    int score;
    if (current_state == 2)
    {
        pthread_mutex_lock(&print_text_mutex);
        score = ten_number*10 + one_number;
        score = score - 1;

        pthread_mutex_lock(&print_text_mutex);
    }
}
void time_change();
void next_question();
void is_highscore();

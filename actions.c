#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"

void state_change(pthread_mutex_t current_state_mutex)
{
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (CURRENT_STATE) {
            // 게임 시작 상태
            case 0:
                if (button > 0x00)
                {
                    CURRENT_STATE = 1;
                }
                break;
                // 게임 준비 상태
            case 1:
                // 지우기 눌렀을 때
                if (button == 0000000000001000)
                {
                    CURRENT_STATE = 0;
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
                    CURRENT_STATE = 0;
                }
                break;
            default:
                CURRENT_STATE = 0;
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}
void text_change(pthread_mutex_t current_state_mutex)
{
    int i;
    char blank[TEXTLCD_LENGTH] = "  ";
    while(1) {
        pthread_mutex_lock(&current_state_mutex);
        switch (CURRENT_STATE) {
            // 게임 시작 상태
            case 0:
                strcpy(buf1, GAME_START_LINE1);
                strcpy(buf2, GAME_START_LINE2);
                break;
            case 1:
                for (i = 3; i > 0; i--)
                {
                    strcat(blank, (char *) &i);
                    strcpy(buf1, blank);
                    usleep(500 * 1000);
                    strcpy(blank, "  ");
                }
                strcpy(buf1, START_LINE1);
                usleep(1000 * 1000);
                CURRENT_STATE = 2;

                break;
                // 게임 진행 상태
            case 2:
                // if(시간이 다됨)
                // 꺼짐
                break;
                // 게임 오버 상태
            case 3:
                if (button > 0x00) {
                    CURRENT_STATE = 0;
                }
                break;
            default:
                strcpy(buf1, "");
                strcpy(buf2, "");
        }
        pthread_mutex_unlock(&current_state_mutex);
    }
}
void score_change()
{

}
void time_change();
void next_question();
void is_highscore();

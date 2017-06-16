#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"
#include "thread_functions.h"

void state_change()
{
    while(1) {
        pthread_mutex_lock(&CURRENT_STATE_MUTEX);
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
        pthread_mutex_unlock(&CURRENT_STATE_MUTEX);
    }
}
void text_change()
{
    int i;
    while(1) {
        pthread_mutex_lock(&CURRENT_STATE_MUTEX);
        switch (CURRENT_STATE) {
            // 게임 시작 상태
            case 0:
                buf1 = GAME_START_LINE1;
                buf2 = GAME_START_LINE2;
                break;
            case 1:
                char blank[TEXTLCD_LENGTH] = "  ";
                for (i = 3; i > 0; i--)
                {
                    strcat(blank, (char *) &i);
                    buf1 = blank;
                    usleep(500 * 1000);
                    blank = "  ";
                }
                buf1 = START_LINE1;
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
                buf1 = "";
                buf2 = "";
        }
        pthread_mutex_unlock(&CURRENT_STATE_MUTEX);
    }
}
void score_change()
{

}
void time_change();
void next_question();
void is_highscore();

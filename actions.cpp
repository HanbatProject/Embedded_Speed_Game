#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"

void state_change()
{
    switch(CURRENT_STATE)
    {
        // 게임 시작 상태
        case 0:
            if(button > 0x00)
            {
                CURRENT_STATE = 1;
            }
            break;
        // 게임 준비 상태
        case 1:
            // 지우기 눌렀을 때
            if(button == 0000000000001000)
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
            if(button > 0x00)
            {
                CURRENT_STATE = 0;
            }
            break;
        default:
            CURRENT_STATE = 0;
    }
}
void text_change()
{
    switch(CURRENT_STATE)
    {
        // 게임 시작 상태
        case 0:
            buf1 = GAME_START_LINE1;
            buf2 = GAME_START_LINE2;
        case 1:
            // 지우기 눌렀을 때
            if(button == 0000000000001000)
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
            if(button > 0x00)
            {
                CURRENT_STATE = 0;
            }
            break;
        default:
            CURRENT_STATE = 0;
    }
}
void score_change();
void time_change();
void next_question();
void is_highscore();

void
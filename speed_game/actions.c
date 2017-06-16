#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"
extern char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];
extern int ten_number, one_number;
extern unsigned short button;
extern int current_state;

extern int question_count;
extern int high_score, game_score;
extern int is_correct;
extern char formula[10];
extern int answer, answer_input;

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
                if (button == KEY_BACK)
					current_state = STATE_MAIN;
				if (play_game == PLAY_GAME)
					current_state = STATE_GAME;
                break;
            // 게임 진행 상태
            case STATE_GAME:
				if (ten_number <= 0 && one_number <= 0)
					current_state = STATE_GAME_OVER;
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				play_game = NOT_PLAY_GAME;
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
	char char_anwser[10];
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
				play_game = PLAY_GAME;
                break;
            // 게임 진행 상태
            case STATE_GAME:
				strcpy(buf1, BLANK);
				
				strcpy(buf1, formula);
                strcat(buf1, GAME_LINE1);
				itoa(answer, char_anwser, 10);
				strcat(buf1, char_anwser);

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
        if (current_state == STATE_GAME)
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

void check_answer()
{
	while (1)
	{
		if (current_state == STATE_GAME) // 여기다 언제 답 체크할지 추가해야함
		{
			// 답 체크하는 부분
			if (answer == answer_input)
			{
				is_correct = ANS_CORRECT;
			}
			else
			{
				is_correct = ANS_WRONG;
			}
		}
	}
}

void next_question(pthread_mutex_t print_text_mutex)
{
	question_count = 1;
	int i;
	int difficulty = 10;
	int left_num = 0, right_num = 0;
	char char_left_num[1], char_operation[1], char_right_num[1];
	char* operation[4] = ["+", "-", "/", "*"];
	char result[10];
	while (1)
	{
		if (current_state == STATE_GAME && is_correct > ANS_NOTHING)
		{
			strcpy(formula, "");
			strcpy(char_left_num, "");
			strcpy(char_operation, "");
			strcpy(char_right_num, "");
			strcpy(result, "");

			srand(time(NULL));
			pthread_mutex_lock(&print_text_mutex);
			
			// ************* 다음 문제를 내는 메소드임 *****************
			// 일단 랜덤하게 생성하는 함수로 int를 생성한 뒤
			
			left_num = rand() % difficulty;
			right_num = rand() % difficulty;
			strcpy(char_operation, operation[(int)((rand() % 10) / 5)]);
			
			// itoa는 인트값을 문자열로 바꿔주는 함수
			itoa(left_num, char_left_num, 10);
			// strcpy는 문자열을 첫번째 인자에 넣어주는 함수
			strcpy(char_left_num, left_num);
			strcpy(char_right_num, right_num);
			// strcat은 붙이는 함수
			strcat(result, char_left_num);
			strcat(result, char_operation);
			strcat(result, char_left_num);

			// 위 과정을 거치면 result에 "3+5" 라는 식이 들어감(물론 숫자와 연산자 모두 랜덤)
			// 이제 식 만든 result를 formula 에 식이 들어가게 하면됨 예를 들면
			
			strcpy(formula, result);

			// 이거에 대한 답이 answer에 들어가야함
			// 오퍼레이션에 따라 달라야겠지?
			// answer는 int형임
			if (char_operation == "+")
				answer = left_num + right_num;
			else if (char_operation == "-")
				answer = left_num - right_num;

			// **** 이렇게 formula와 answer에 값이 들어가도록 하면 끝임 ****
			// 갈수록 난이도도 높게 설정해야겠지?
			// 횟수는 question_count임 (인트형)
			if (question_count > 3)
				difficulty = 100; // 두자리숫자까지 나온다는거

			// 난이도를 앞에다 설정해도됨
				

			pthread_mutex_unlock(&print_text_mutex);
		}
	}
}

void key_input(pthread_mutex_t print_text_mutex)
{
	while (1)
	{
		if (current_state == STATE_GAME)
		{
			pthread_mutex_lock(&print_text_mutex);
			// button은 키 입력 받는거(machine.h 참조)
			// state.h를 보면 작동에 관한 키들이 있음
			
			// answer_input이 int형이고, 키를 입력할 때
			// int를 더하는 게 아니라 옆에 붙여줘야함
			// 예: 10에 8 입력 -> 108
			// 아래 방법 중에 하나 선택하던지 다른 방법 선택하던지 해서 구현좀

			// 문자열으로 받아서 인트로 바꾸던
			// 비트 연산을 해서 인트로 바꾸던
			// *10을 하는 연산으로(예-> 자릿수*10 + 내가 입력하려는 수 8) 하던

			if (button == KEY_ONE) // 키가 machine.h를 보면 밑에 디파인 목록이 있음 그거 다해주면됨
				answer_input += 1; // 이렇게 하면 안됨 10에서 1 눌렀는데 11이 아니라 101이 나와야겠지

			// 리턴 안해도되고 answer_input에다가 올바른 값 넣어주게 하기만 하면됨

			pthread_mutex_unlock(&print_text_mutex);
		}
	}
}

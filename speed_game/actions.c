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
					current_state = STATE_GAME;
                break;
            // 게임 준비 상태
            case STATE_READY:
                // 지우기 눌렀을 때
				//if (button == KEY_BACK)
				//{
				//	current_state = STATE_MAIN;
				//	usleep(500 * 1000);
				//}
				//if (play_game == PLAY_GAME)
				usleep(500*1000);
					current_state = STATE_GAME;
                break;
            // 게임 진행 상태
            case STATE_GAME:
				//if (ten_number <= 0 && one_number <= 0)
				//if (button >= 0x00)
				//	current_state = STATE_GAME_OVER;
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
	game_score = 0;
	if (high_score >= game_score)
		high_score = game_score;
	combo = 1;
	answer = 0;
	answer_input = 0;
	itoa(high_score, print_high_score, 10);
	return;
}
void text_change(pthread_mutex_t print_text_mutex)
{
	int i;
	int main_change_ok = 0;
	int game_change_ok = 0;
	char print_high_score[10];
	char char_answer[10], char_game_score[10];
    char temp[TEXTLCD_LENGTH];
	high_score = 0;
    while(1) {
		pthread_mutex_lock(&print_text_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
				if (!main_change_ok)
				{
					strcpy(buf1, GAME_START_LINE1);
					strcpy(buf2, BLANK);
					strcpy(buf2, GAME_START_LINE2);
					init_and_change_highscore(print_high_score);
					strcat(buf2, print_high_score);

					main_change_ok = 1;
				}
                break;
			// 게임 준비 상태
            case STATE_READY:
				//if(!ready_change_ok)
				//{
				//	for (i = 3; i > 0; i--)
				//	{
				//		strcpy(temp, HALF_BLANK);
				//		if(i==3) strcat(temp, "3");
				//		else if(i==2) strcat(temp, "2");
				//		else strcat(temp, "1");
				//		strcat(temp, HALF_BLANK);

				//		strcpy(buf1, temp);
				//		usleep(1000 * 1000);
				//	}

				//	strcpy(buf1, START_LINE1);
				//	play_game = PLAY_GAME;

				//}
                break;
            // 게임 진행 상태
            case STATE_GAME:
				if (!game_change_ok)
				{
					strcpy(buf1, BLANK);
					strcpy(buf2, BLANK);
					game_change_ok = 1;
				}
				strcpy(buf1, formula);
				strcat(buf1, GAME_LINE1);
				strcpy(char_answer, "11");
				//itoa(answer_input, char_answer, 10);
				strcat(buf1, char_answer);

				//game_score = 1;
				//itoa(game_score, char_game_score, 10);
				strcpy(char_game_score, "11111");
				strcpy(buf2, GAME_LINE2);
				strcat(buf2, char_game_score);
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				main_change_ok = 0;
				game_change_ok = 0;
                break;
            default:
                strcpy(buf1, BLANK);
                strcpy(buf2, BLANK);
        }
		pthread_mutex_unlock(&print_text_mutex);
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

void key_input(pthread_mutex_t print_text_mutex)
{
	while (1)
	{
		if (current_state == STATE_GAME)
		{
			pthread_mutex_lock(&print_text_mutex);

			if (button == KEY_ONE)
				answer_input = 10 * answer_input + 1;
			if (button == KEY_TWO)
				answer_input = 10 * answer_input + 2;
			if (button == KEY_THREE)
				answer_input = 10 * answer_input + 3;
			if(button == KEY_FOUR)
				answer_input = 10 * answer_input + 4;
			if (button == KEY_FIVE)
				answer_input = 10 * answer_input + 5;
			if (button == KEY_SIX)
				answer_input = 10 * answer_input + 6;
			if (button == KEY_SEVEN)
				answer_input = 10 * answer_input + 7;
			if (button == KEY_EIGHT)
				answer_input = 10 * answer_input + 8;
			if (button == KEY_NINE)
				answer_input = 10 * answer_input + 9;
			if (button == KEY_ZERO)
				answer_input = 10 * answer_input;
			if (button == KEY_BACK)
				answer_input = 0;
			if (button == KEY_NEXT)
			{
				// 다음 문제로 가는거
			}

			pthread_mutex_unlock(&print_text_mutex);
		}
	}
}

void itoa(int num, char * str, int radix)
{
	int i = 0;
	int deg = 1;
	int cnt = 0;

	char* p = str;
	while (num)
	{
		if (radix <= 10)
			*p++ = (num % radix) + '0';
		else {
			int t = num % radix;
			if (t <= 9)
				*p++ = t + '0';
			else
				*p++ = t - 10 + 'a';
		}
		
		num /= radix;
	}
	
	*p = '\0';
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
			// 이거에 대한 답이 answer에 들어가야함
			// 오퍼레이션에 따라 달라야겠지?
			// answer는 int형임
			if (char_operation == "+")
				answer = left_num + right_num;
			else if (char_operation == "-")
			{
				answer = left_num - right_num;
				while (1)
				{
					if (left_num >= right_num) break;
					left_num = rand() % difficulty;
					right_num = rand() % difficulty;
				}
			}
			else if (char_operation == "*")
				answer = left_num * right_num;
			else
			{
				answer = left_num / right_num;
				while (1)
				{
					if (left_num%right_num == 0) break;
					left_num = rand() % difficulty;
					right_num = rand() % difficulty;
				}
			}

			// itoa는 인트값을 문자열로 바꿔주는 함수

			itoa(left_num, char_left_num, 10);
			itoa(right_num, char_right_num, 10);

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
			// **** 이렇게 formula와 answer에 값이 들어가도록 하면 끝임 ****

			// 갈수록 난이도도 높게 설정해야겠지?

			// 횟수는 question_count임 (인트형)

			if (question_count > 3)
				difficulty = 50; // 두자리숫자까지 나온다는거
			if (question_count > 7)
				difficulty = 100;
			if (question_count > 12)
				difficulty = 250;
			if (question_count > 18)
				difficulty = 500;
			if (question_count > 25)
				difficulty = 750;
			if (question_count > 33)
				difficulty = 1000;
			// 난이도를 앞에다 설정해도됨


			pthread_mutex_unlock(&print_text_mutex);
		}
	}
}

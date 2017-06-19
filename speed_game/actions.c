#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"

void state_change(pthread_mutex_t current_state_mutex)
{
    while(1) {
		//pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
				if (button > 0x00)
					current_state = STATE_GAME;
                break;
            // 게임 준비 상태
            case STATE_READY:
                 //지우기 눌렀을 때
				if (button == KEY_BACK)
					current_state = STATE_MAIN;
                break;
            // 게임 진행 상태
			case STATE_GAME:
				if (ten_number <= 0 && one_number <= 0)
					current_state = STATE_GAME_OVER;
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				if (button > 0x00)
				{
					usleep(1000 * 1000);
					current_state = STATE_MAIN;
				}
                break;
            default:
                current_state = 0;
        }
		//pthread_mutex_unlock(&current_state_mutex);
    }
}

void text_change(pthread_mutex_t print_text_mutex)
{
	int i;
	int main_change_ok = 0;
	int game_change_ok = 0;
	char print_high_score[10];
	char char_answer[10], char_game_score[10];
    char temp[TEXTLCD_LENGTH];
    while(1) {
		//pthread_mutex_lock(&print_text_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
				if (!main_change_ok)
				{
					strcpy(buf1, GAME_START_LINE1);
					strcpy(buf2, BLANK);
					strcpy(buf2, GAME_START_LINE2);
					if (high_score > game_score)
						high_score = game_score;
					itoa(high_score, print_high_score, 10);
					strcat(buf2, print_high_score);

					main_change_ok = 1;
				}
                break;
			// 게임 준비 상태
            case STATE_READY:
				for (i = 3; i > 0; i--)
				{
					strcpy(temp, HALF_BLANK);
					if(i==3) strcat(temp, "3");
					else if(i==2) strcat(temp, "2");
					else strcat(temp, "1");
					strcat(temp, HALF_BLANK);

					strcpy(buf1, temp);
					usleep(1000 * 1000);
					if (current_state == STATE_MAIN) break;
				}
				if (current_state == STATE_MAIN) break;
				strcpy(buf1, START_LINE1);
				usleep(1000 * 1000);
				if (current_state == STATE_MAIN) break;

				current_state = STATE_GAME;
				strcpy(buf1, BLANK);
				strcpy(buf2, BLANK);
                break;
            // 게임 진행 상태
            case STATE_GAME:
				strcpy(buf1, formula);
				strcat(buf1, GAME_LINE1);
				//strcpy(char_answer, "11");
				itoa(answer_input, char_answer, 10);
				strcat(buf1, char_answer);

				itoa(game_score, char_game_score, 10);
				//strcpy(char_game_score, "11111");
				strcpy(buf2, GAME_LINE2);
				strcat(buf2, char_game_score);
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				strcpy(buf1, GAME_OVER_LINE1);
				main_change_ok = 0;
				game_change_ok = 0;
                break;
            default:
                strcpy(buf1, BLANK);
                strcpy(buf2, BLANK);
        }
		//pthread_mutex_unlock(&print_text_mutex);
    }
}

void key_input(pthread_mutex_t print_text_mutex)
{
	while (1)
	{
		if (current_state == STATE_GAME)
		{
			//pthread_mutex_lock(&print_text_mutex);

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
				// 답 체크하는 부분
				if (answer == answer_input)
				{
					game_score += 10 * combo;
				}
				else
				{
					change_time(get_time() - 3);
					combo = 1;
				}
				next_question();
			}

			//pthread_mutex_unlock(&print_text_mutex);
		}
	}
}

void next_question()
{
	int i;
	int difficulty = 10;
	int left_num = 0, right_num = 0;
	char char_left_num[5] = "", char_operation[1] = "", char_right_num[5] = "";
	char* operation[4] = ["+", "-", "/", "*"];
	char result[10] = "";

	srand(time(NULL));
	left_num = rand() % difficulty;
	right_num = rand() % difficulty;
	strcpy(char_operation, operation[(int)((rand() % 10) / 5)]);
	if (char_operation == "+")
		answer = left_num + right_num;
	else if (char_operation == "-")
	{
		while (left_num < right_num)
		{
			left_num = rand() % difficulty;
			right_num = rand() % difficulty;
		}
		answer = left_num - right_num;
	}
	else if (char_operation == "*")
		answer = left_num * right_num;
	else
	{
		while (left_num%right_num)
		{
			left_num = rand() % difficulty;
			right_num = rand() % difficulty;
		}
		answer = left_num / right_num;
	}

	itoa(left_num, char_left_num, 10);
	itoa(right_num, char_right_num, 10);

	strcpy(result, char_left_num);
	strcat(result, char_operation);
	strcat(result, char_right_num);

	strcpy(formula, result);

	if (question_count > 3) difficulty = 50;
	else if (question_count > 7) difficulty = 100;
	else if (question_count > 12) difficulty = 250;
	else if (question_count > 18) difficulty = 500;
	else if (question_count > 25) difficulty = 750;
	else if (question_count > 33) difficulty = 1000;
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

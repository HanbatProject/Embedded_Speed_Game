#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"
#include "text.h"

void state_change(pthread_mutex_t current_state_mutex)
{
	int i;
    while(1) {
		usleep(1);//printf("\n");
		//pthread_mutex_lock(&current_state_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
				if (button > 0x00)
					current_state = STATE_READY;
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

void null_destroy(char* buf)
{
	int i;
	for(i = 0; i < 17 ; i++)
		if(buf[i] == 0) buf[i] = ' ';
}

void text_change(pthread_mutex_t print_text_mutex)
{
	int i;
	int main_change_ok = 0;
	int game_change_ok = 0;
	char print_high_score[10];
	char char_answer[10]="", char_game_score[10]="";
    char temp[1];
    while(1) {
		usleep(1);
		//pthread_mutex_lock(&print_text_mutex);
        switch (current_state) {
            // 게임 시작 상태
            case STATE_MAIN:
				if (!main_change_ok)
				{
					strcpy(buf1, GAME_START_LINE1);
					//strcpy(buf2, BLANK);

					strcpy(buf2, GAME_START_LINE2);
					if (high_score < game_score)
						high_score = game_score;
					game_score = 0;
					itoa(high_score, print_high_score, 10);
					strcat(buf2, print_high_score);
					null_destroy(buf2);

					main_change_ok = 1;
				}
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
					null_destroy(buf1);
					usleep(1000 * 1000);
					if (current_state == STATE_MAIN) {
						main_change_ok = 0;
						break;
					}
				}
				if (current_state == STATE_MAIN) {
					main_change_ok = 0;
					break;
				}
				strcpy(buf1, START_LINE1);
				usleep(1000 * 1000);
				if (current_state == STATE_MAIN) {
					main_change_ok = 0;
					break;
				}

				current_state = STATE_GAME;
				next_question();
				strcpy(buf1, BLANK);
				strcpy(buf2, BLANK);
                break;
            // 게임 진행 상태
            case STATE_GAME:
				strcpy(buf1, BLANK);
				strcpy(buf2, BLANK);

				strcpy(buf1, formula);

				itoa(answer_input, char_answer, 10);
				itoa(game_score, char_game_score, 10);

				strcat(buf1, GAME_LINE1);
				strcat(buf1, char_answer);
				null_destroy(buf1);

				strcpy(buf2, GAME_LINE2);
				strcat(buf2, char_game_score);
				null_destroy(buf2);
                break;
            // 게임 오버 상태
            case STATE_GAME_OVER:
				strcpy(buf1, GAME_OVER_LINE1);

				main_change_ok = 0;
				game_change_ok = 0;
				change_time(30);
                break;
            default:
                strcpy(buf1, BLANK);
                strcpy(buf2, BLANK);
        }
		//pthread_mutex_unlock(&print_text_mutex);
    }
}

void go_time()
{
	while (1)
	{
		usleep(1); //printf("\n");
		if (current_state == STATE_GAME)
		{
			usleep(1000 * 1000);
			change_time(get_time() - 1);
		}
	}
}

void next_question()
{
	int i;
	int difficulty = 10;
	int left_num = 0, right_num = 0;
	char char_left_num[5] = "", char_right_num[5] = "";

	char char_operation = 0;
	char operation[4] = { '+', '-', '/', '*' };

	char result[10] = "";

	srand(time(NULL));

	left_num = rand() % difficulty + 1;
	right_num = rand() % difficulty + 1;
	int k = (int)((rand() % 10) / 3);

	char_operation = operation[k];

	if (char_operation == '+')
		answer = left_num + right_num;
	else if (char_operation == '-')
	{
		while (left_num < right_num)
		{
			left_num = rand() % difficulty + 1;
			right_num = rand() % difficulty + 1;
		}
		answer = left_num - right_num;
	}
	else if (char_operation == '*')
		answer = left_num * right_num;
	else
	{
		while (left_num%right_num)
		{
			left_num = rand() % difficulty + 1;
			right_num = rand() % difficulty + 1;
		}
		answer = left_num / right_num;
	}

	printf("answer :: %d \n", answer);
	printf("left_num :: %d \n", left_num);
	printf("right_num :: %d \n", right_num);

	itoa(left_num, char_left_num, 10);
	itoa(right_num, char_right_num, 10);

	char asdf[2];
	asdf[1] = char_operation;
	asdf[2] = '\0';

	strcpy(result, char_left_num);
	strcat(result, asdf);
	strcat(result, char_right_num);

	strcpy(formula, result);

	answer_input = 0;

	if (question_count > 5) difficulty = 100;
	else if (question_count > 10) difficulty = 1000;
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
void reverse(char* s)
{
	int c, i, j;
	for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

void itoa(int num, char * str, int radix)
{
	int i = 0;
	int deg = 1;
	int cnt = 0;

	if (num == 0)
	{
		strcpy(str, "0");
		return;
	}

	char temp[10] = "";
	int k;

	//strcpy(str, "");
	while (num)
	{
		k = num % radix;
		if (k == 0)
			strcat(temp, "0");
		else if (k == 1)
			strcat(temp, "1");
		else if (k == 2)
			strcat(temp, "2");
		else if (k == 3)
			strcat(temp, "3");
		else if (k == 4)
			strcat(temp, "4");
		else if (k == 5)
			strcat(temp, "5");
		else if (k == 6)
			strcat(temp, "6");
		else if (k == 7)
			strcat(temp, "7");
		else if (k == 8)
			strcat(temp, "8");
		else if (k == 9)
			strcat(temp, "9");
		num = (int)(num / radix);
	}
	reverse(temp);
	strcpy(str, temp);
	//strcat(str, "\0");
}

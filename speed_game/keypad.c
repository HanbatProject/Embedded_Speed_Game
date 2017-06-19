#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"

unsigned short *keypad_row_addr, *keypad_col_addr;
unsigned short keypad_change(unsigned short row);

// #define BINARY_CODE

void keypad(pthread_mutex_t print_text_mutex)
{
	keypad_col_addr = addr_fpga + KEY_COL_OFFSET / sizeof(unsigned short);
	keypad_row_addr = addr_fpga + KEY_ROW_OFFSET / sizeof(unsigned short);

	if (*keypad_col_addr == (unsigned short)-1 || *keypad_row_addr == (unsigned short)-1)
	{
		printf("keypad mmap failed\n");
		return;
	}

	int i;
    *keypad_row_addr = 0x00;
	while (1)
	{
		pthread_mutex_lock(&print_text_mutex);
		button = keypad_change(0x01);
		button |= keypad_change(0x02) << 4;
		button |= keypad_change(0x04) << 8;
		button |= keypad_change(0x08) << 12;
#ifdef BINARY_CODE
		printf("Binary : ");
		printf("%d", button);
		for (i = 15; i >= 0; i--)
			printf("%d", button >> i & 0x1);
		printf("\n");
#endif
		pthread_mutex_unlock(&print_text_mutex);
		usleep(500*1000);
		*keypad_row_addr = 0x00;

		if (current_state == STATE_GAME)
		{
			if (button == KEY_ONE)
				answer_input = 10 * answer_input + 1;
			if (button == KEY_TWO)
				answer_input = 10 * answer_input + 2;
			if (button == KEY_THREE)
				answer_input = 10 * answer_input + 3;
			if (button == KEY_FOUR)
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
		}
    }
}

unsigned short keypad_change(unsigned short row)
{
    *keypad_row_addr = row;
    return (*keypad_col_addr & 0x0f);
}

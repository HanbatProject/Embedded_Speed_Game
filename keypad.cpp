#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"

// #define BINARY_CODE

void keypad(int fd)
{
	keypad_col_addr = addr_fpga + KEY_COL_OFFSET / sizeof(unsigned short);
	keypad_row_addr = addr_fpga + KEY_ROW_OFFSET / sizeof(unsigned short);

	if (*keypad_col_addr == (unsigned short)-1 || *keypad_row_addr == (unsigned short)-1)
	{
		printf("keypad mmap failed\n");
		return;
	}

	short value;
	while (true)
	{
		*keypad_row_addr = 0x01;
		usleep(1000);
		value = (*keypad_col_addr & 0x0f);
		button = value;

		*keypad_row_addr = 0x02;
		usleep(1000);
		value = (*keypad_col_addr & 0x0f);
		button |= value << 4;

		*keypad_row_addr = 0x04;
		usleep(1000);
		value = (*keypad_col_addr & 0x0f);
		button |= value << 8;

		*keypad_row_addr = 0x08;
		usleep(1000);
		value = (*keypad_col_addr & 0x0f);
		button |= value << 12;

		*keypad_row_addr = 0x00;

#ifdef BINARY_CODE
		printf("Binary : ");
		for (i = 15; i >= 0; i--)
		{
			printf("%d", button >> i & 0x1);
		}
		printf("\n");
#endif
		usleep(1000);
	}
}
#include "external_includes.h"
#include "machine.h"
#include "functions.h"
extern unsigned short *addr_fpga;
extern unsigned short button;

unsigned short *keypad_row_addr, *keypad_col_addr;
unsigned short keypad_change(unsigned short row);

//#define BINARY_CODE

void keypad(pthread_mutex_t keypad_mutex)
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
        pthread_mutex_lock(&keypad_mutex);

        button = keypad_change(0x01);
		button |= keypad_change(0x02) << 4;
        button |= keypad_change(0x04) << 8;
        button |= keypad_change(0x08) << 12;
#ifdef BINARY_CODE
		printf("Binary : ");
		printf("%d", button);
		for (i = 15; i >= 0; i--)
		{
			printf("%d", button >> i & 0x1);
			usleep(1000);
		}
		printf("\n");
#endif
		usleep(100*1000);
        pthread_mutex_unlock(&keypad_mutex);

        *keypad_row_addr = 0x00;
    }
}

unsigned short keypad_change(unsigned short row)
{
    *keypad_row_addr = row;
    usleep(1000);
    return (*keypad_col_addr & 0x0f);
}

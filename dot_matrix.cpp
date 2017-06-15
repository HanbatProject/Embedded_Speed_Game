#include "external_includes.h"
#include "machine.h"
#include "functions.h"
#include "state.h"

#define NUMSIZE 4

void get_number(int prev_number)

unsigned short font_num[40] = {
	0x7f, 0x41, 0x41, 0x7f,		// 0
	0x00, 0x42, 0x7f, 0x40,  	// 1
	0x79, 0x49, 0x49, 0x4f,   	// 2
	0x49, 0x49, 0x49, 0x7f,   	// 3
	0x1f, 0x10, 0x7f, 0x10,   	// 4
	0x4f, 0x49, 0x49, 0x79,   	// 5
	0x7f, 0x49, 0x49, 0x79,   	// 6
	0x01, 0x01, 0x01, 0x7f,   	// 7
	0x7f, 0x49, 0x49, 0x7f,   	// 8
	0x4f, 0x49, 0x49, 0x7f,   	// 9		
};

void dot_matrix(int fd)
{
	int dot_dirty = 1;
	int number[2] = { 0, 0 };
	dot_row_addr = addr_fpga + DOT_ROW_OFFSET / sizeof(unsigned short);
	dot_col_addr = addr_fpga + DOT_COL_OFFSET / sizeof(unsigned short);

	if (*dot_row_addr == (unsigned short)-1 || *dot_col_addr == (unsigned short)-1)
	{
		printf("dot_row mmap failed\n");
		return 0;
	}
	while(true)
	{
		if (CURRENT_STATE == 2)
		{
			dot_dirty = 1;
			
			number = get_number()

			for (i = 0; i < 4; i++)
			{
				*dot_row_addr = 1 << i;
				*dot_col_addr = 0x8000 | font_num[ten_number*NUMSIZE + i];
			}
			for (i = 0; i < 4; i++)
			{
				*dot_row_addr = 5 << i;
				*dot_col_addr = 0x8000 | font_num[one_number*NUMSIZE + i];
			}
			usleep(1000 * 1000);
		}
		else
		{
			if (dot_dirty)
			{
				for (i = 0; i < 8; i++)
				{
					*dot_row_addr = 1 << i;
					*dot_col_addr = 0x00;
				}
				dot_dirty = 0;
			}
		}
	}

	return;
}
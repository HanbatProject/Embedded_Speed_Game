#ifndef MACHINE_H
#define MACHINE_H

#define MEMORY_PATH "/dev/mem"

#define FPGA_BASEADDR 0x88000000
unsigned short *addr_fpga;

#define TEXTLCD_OFFSET 0x10
#define TEXTLCD_LENGTH 17
unsigned short *pTextlcd;
char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];

#define DOT_ROW_OFFSET 0x40
#define DOT_COL_OFFSET 0x42
unsigned short *dot_row_addr, *dot_col_addr;

#define KEY_ROW_OFFSET 0x72
#define KEY_COL_OFFSET 0x70
unsigned short *keypad_row_addr, *keypad_col_addr;
unsigned short button = 0;


#endif // !MACHINE_H

#ifndef MACHINE_H
#define MACHINE_H

#define MEMORY_PATH "/dev/mem"

#define FPGA_BASEADDR 0x88000000
extern unsigned short *addr_fpga;

#define TEXTLCD_OFFSET 0x10
#define TEXTLCD_LENGTH 17
extern char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];

#define DOT_ROW_OFFSET 0x40
#define DOT_COL_OFFSET 0x42
extern int ten_number, one_number;

#define KEY_ROW_OFFSET 0x72
#define KEY_COL_OFFSET 0x70
extern unsigned short button;


#endif // !MACHINE_H

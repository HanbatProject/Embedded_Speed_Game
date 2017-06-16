#include "external_includes.h"
#include "functions.h"
#include "machine.h"
extern unsigned short *addr_fpga;
extern char buf1[TEXTLCD_LENGTH], buf2[TEXTLCD_LENGTH];

void make_line(int line_bit, char* buf);
void setcommand(unsigned short command);
void writebyte(char ch);
void initialize_textlcd();
int function_set(int rows, int nfonts);
int display_control(int display_enable, int cursor_enable, int nblink);
int cursor_shift(int set_screen, int set_rightshift);
int entry_mode_set(int increase, int nshift);
int return_home();
int clear_display();
int set_ddram_address(int pos);

unsigned short *pTextlcd;

// #define LINE_TEST

void print_text(pthread_mutex_t print_text_mutex)
{
	pTextlcd = addr_fpga + TEXTLCD_OFFSET / sizeof(unsigned short);

	if (*pTextlcd == (unsigned short)-1)
	{
		printf("text_lcd mmap failed\n");
		return;
	}

	initialize_textlcd();
	while (1){
        pthread_mutex_lock(&print_text_mutex);

		make_line(0, buf1);
		make_line(64, buf2);

        pthread_mutex_unlock(&print_text_mutex);
        usleep(100);
	}
	return;
}

void make_line(int line_bit, char* buf)
{
	int i;
	set_ddram_address(line_bit);

#ifdef LINE_TEST
	if(line_bit == 0) printf("\nLINE 1 : ");
	else printf("\nLINE 2 : ");
#endif
	for (i = 0; i < TEXTLCD_LENGTH; i++)
	{
		printf("%c", buf[i]);
		writebyte(buf[i]);
	}
	printf("\n");
}

void setcommand(unsigned short command)
{
	command &= 0x00FF;

	*pTextlcd = command | 0x0000;
	usleep(1000);
	*pTextlcd = command | 0x0100;
	usleep(1000);
	*pTextlcd = command | 0x0000;
	usleep(1000);
}

void writebyte(char ch)
{
	unsigned short data = ch & 0x00FF;

	*pTextlcd = data & 0x400;
	usleep(1000);
	*pTextlcd = data | 0x500;
	usleep(1000);
	*pTextlcd = data | 0x400;
	usleep(1000);
}

void initialize_textlcd()
{
	function_set(2, 0); //Function Set:8bit, display 2lines, 5x7 mode
	display_control(1, 1, 0); //Display on, Cursor on
	clear_display(); //Display clear
	//entry_mode_set(1, 0); //Entry Mode Set:Shift right cursor
	//return_home(); //go home
}

// send function set command to the text lcd
// rows = 2 : => 2 rows, row = 1 => 1 rows
// nfonts = 1 : => 5x10 dot, nfonts = 0: 5x7 dot
int function_set(int rows, int nfonts)

{
	unsigned short command = 0x30;

	if (rows == 2) command |= 0x08;
	else if (rows == 1) command &= 0xf7;
	else return -1;

	command = nfonts ? (command | 0x04) : command;
	setcommand(command);

	return 1;
}

int display_control(int display_enable, int cursor_enable, int nblink)
{
	unsigned short command = 0x08;
	command = display_enable ? (command | 0x04) : command;
	command = cursor_enable ? (command | 0x02) : command;
	command = nblink ? (command | 0x01) : command;

	setcommand(command);

	return 1;
}

int cursor_shift(int set_screen, int set_rightshift)
{
	unsigned short command = 0x10;
	command = set_screen ? (command | 0x08) : command;
	command = set_rightshift ? (command | 0x04) : command;
	setcommand(command);

	return 1;
}

int entry_mode_set(int increase, int nshift)
{
	unsigned short command = 0x04;
	command = increase ? (command | 0x2) : command;
	command = nshift ? (command | 0x1) : command;

	setcommand(command);

	return 1;
}

int return_home()
{
	unsigned short command = 0x02;
	setcommand(command);

	return 1;
}

int clear_display()
{
	unsigned short command = 0x01;
	setcommand(command);

	return 1;
}

int set_ddram_address(int pos)
{
	unsigned short command = 0x80;
	command += pos;
	setcommand(command);

	return 1;
}


/*----------------------------------------------------------------------/
/ Low level disk I/O module function checker                            /
/-----------------------------------------------------------------------/
/ WARNING: The data on the target drive will be lost!
*/

void *heap_start = (void * )0x190000, *heap_end = (void * )0x193000;

#include <stdio.h>
#include <string.h>
#include "ff.h"         /* Declarations of sector size */
#include "diskio.h"     /* Declarations of disk functions */

//#include <sys/types.h>
#include "../foenixLibrary/vicky.h"
//#include "../foenixLibrary/FMX_printf.h"
#include "../foenixLibrary/timer.h"
#include "../foenixLibrary/interrupt.h"
#define IDE_CMD_STAT  		(*(volatile unsigned char *)0xAFE837)

char spinner[] = {'|', '/', '-', '\\', '|', '/', '-', '\\'};
char spinnerState = 0;
char spinnerStateDisk = 0;

void IRQHandler(void)              
{          
	int reg = 0;

	if (reg = (INT_PENDING_REG0 & FNX0_INT02_TMR0))
	{
		
		textScreen[6] = spinner[spinnerState];
		if (spinnerState < 7)
			spinnerState++;
		else
		{
			spinnerState = 0;
		}

		//disk_timerproc();

		reg = INT_PENDING_REG0 & FNX0_INT02_TMR0;
		INT_PENDING_REG0 = reg;
	}   

	if (reg = (INT_PENDING_REG3 & FNX3_INT02_IDE))
	{
		
		textScreen[10] = spinner[spinnerStateDisk];
		if (spinnerStateDisk < 7)
			spinnerStateDisk++;
		else
		{
			spinnerStateDisk = 0;
		}

		printf("interrupt %d", IDE_CMD_STAT);

		reg = INT_PENDING_REG3 & INT_PENDING_REG3;
		INT_PENDING_REG3 = reg;
	}   
}

void COPHandler(void)              
{             
}

void BRKHandler(void)              
{             
}


DWORD buff[FF_MAX_SS];  /* Working buffer (4 sector in size) */
FATFS fs;           /* Filesystem object */
FIL fil;            /* File object */
FRESULT res;        /* API result code */
UINT bw;            /* Bytes written */

void main (void)
{
    int rc;
	int clock = 14318000;
	//int spd = clock / 100;
	int spd = clock / 1;
    
	LBA_t plist[] = {50, 50, 0};  /* Divide the drive into two partitions */

	

	// Emulator workarround for screen
	//set the display size - 128 x 64
	COLS_PER_LINE = 80;
	LINES_MAX = 60;
	//set the visible display size - 80 x 60
  	COLS_VISIBLE = 80;
	LINES_VISIBLE = 60;

	TIMER0_CHARGE_L = 0x00;
	TIMER0_CHARGE_M = 0x00;
	TIMER0_CHARGE_H = 0x00;
	TIMER0_CMP_L = (spd) & 0xFF;
	TIMER0_CMP_M = (spd >> 8) & 0xFF;
	TIMER0_CMP_H = (spd >> 16) & 0xFF;;
	
	TIMER0_CMP_REG = TMR0_CMP_RECLR;

	TIMER0_CTRL_REG = TMR_EN | TMR_UPDWN | TMR_SCLR;

	INT_MASK_REG0 = 0xFB; // unmask timer 0

	INT_MASK_REG0 = 0xFB; // unmask harddisk;

	// enable interrupts
	enableInterrupts();

	setEGATextPalette();
	clearTextScreen(' ', 0xD, 0xE);

	VKY_TXT_CURSOR_X_REG = 0;
	VKY_TXT_CURSOR_Y_REG = 0;

	printf("\nSD test....");
	
	// printf("\nCreate two partitions...");
    // f_fdisk(0, plist, buff);                    /* Divide physical drive 0 */

	// printf("\nFormat partition 1...");
    // res =  f_mkfs("1:", 0, buff, sizeof buff); /* Create FAT volume on the logical drive 1 */
	// if (res == FR_OK) printf("ok");
	// else printf("Error: %d", res);
	// printf("\nFormat partition 2...");
    // f_mkfs("1:", 0, buff, sizeof buff); /* Create FAT volume on the logical drive 1 */

	/* Gives a work area to the default drive */
	printf("\nMount drive...");
    res = f_mount(&fs, "1:", 1);
	if (res == FR_OK) printf("ok");
	else printf("Error: %d", res);

    /* Create a file as new */
	printf("\nCreate hello.txt...");
    res = f_open(&fil, "1:hello.txt", FA_CREATE_NEW | FA_WRITE);
	if (res == FR_OK) printf("ok");
	//else if (res == FR_NOT_ENABLED) printf("Not enabled");
	else printf("Error: %d", res);

    /* Write a message */
	printf("\nWrite secret message...");
    f_write(&fil, "Hello, Foenix!\r\n", 15, &bw);

    /* Close the file */
	printf("\nClose file...");
    f_close(&fil);

    /* Unregister work area */
	printf("\nClose drive...");
    f_mount(0, "1:", 0);

 	rc = 0;
	while(1)
	{
		if (rc < 4)
		{
			printf("\n");
			rc++;
		}
	}
    //return rc;
}

void _abort(void) {

}

int close(int fd) {
    return 0;
}

int creat(const char *_name, int _mode) {
    return 0;
}


long lseek(int fd, long pos, int rel) {
    return 0;
}

int open(const char * _name, int _mode) {
    return 0;
}

size_t read(int fd, void *buffer, size_t len) {
    return 0;
}

int unlink(const char *filename) {
    return 0;
}

size_t write(int fd, void *buffer, size_t len) {
    size_t count;
	for (count = 0; count < len; count++)
	{
		if (((unsigned char *)buffer)[count] == '\n')
		{
			VKY_TXT_CURSOR_X_REG = 0;
			VKY_TXT_CURSOR_Y_REG++;

			if (VKY_TXT_CURSOR_Y_REG == LINES_VISIBLE)
			{
				VKY_TXT_CURSOR_Y_REG = 0;
			}

			continue;
		}

		textScreen[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = ((unsigned char *)buffer)[count];
		
		textScreenColor[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = 0xE0;
		/*
		textScreenColor[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = color;
		color += 16;
		if (color == 0xF0)
			color = 0x10;*/
		
		VKY_TXT_CURSOR_X_REG++;
		
		if (VKY_TXT_CURSOR_X_REG == COLS_VISIBLE)
		{
			VKY_TXT_CURSOR_X_REG = 0;
			VKY_TXT_CURSOR_Y_REG++;
			
			if (VKY_TXT_CURSOR_Y_REG == LINES_VISIBLE)
			{
				VKY_TXT_CURSOR_Y_REG = 0;
			}
		}
	}
    return len;
}

//
// Missing STDLIB.H function
//
int    isatty(int fd) {
    // descriptors 0, 1 and 2 are STDIN_FILENO, STDOUT_FILENO and STDERR_FILENO
    return fd < 3;
}

;:ts=8
R0	equ	1
R1	equ	5
R2	equ	9
R3	equ	13
;/*----------------------------------------------------------------------/
;/ Low level disk I/O module function checker                            /
;/-----------------------------------------------------------------------/
;/ WARNING: The data on the target drive will be lost!
;*/
;
;void *heap_start = (void * )0x190000, *heap_end = (void * )0x193000;
	data
	xdef	~~heap_start
~~heap_start:
	dl	$190000
	ends
	data
	xdef	~~heap_end
~~heap_end:
	dl	$193000
	ends
;
;#include <stdio.h>
;#include <string.h>
;#include "ff.h"         /* Declarations of sector size */
;#include "diskio.h"     /* Declarations of disk functions */
;
;//#include <sys/types.h>
;#include "../foenixLibrary/vicky.h"
;//#include "../foenixLibrary/FMX_printf.h"
;#include "../foenixLibrary/timer.h"
;#include "../foenixLibrary/interrupt.h"
;#define IDE_CMD_STAT  		(*(volatile unsigned char *)0xAFE837)
;
;char spinner[] = {'|', '/', '-', '\\', '|', '/', '-', '\\'};
	data
	xdef	~~spinner
~~spinner:
	db	$7C,$2F,$2D,$5C,$7C,$2F,$2D,$5C
	ends
;char spinnerState = 0;
	data
	xdef	~~spinnerState
~~spinnerState:
	db	$0
	ends
;char spinnerStateDisk = 0;
	data
	xdef	~~spinnerStateDisk
~~spinnerStateDisk:
	db	$0
	ends
;
;void IRQHandler(void)              
;{          
	code
	xdef	~~IRQHandler
	func
~~IRQHandler:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L2
	tcs
	phd
	tcd
;	int reg = 0;
;
;	if (reg = (INT_PENDING_REG0 & FNX0_INT02_TMR0))
reg_1	set	0
	stz	<L3+reg_1
;	{
	sep	#$20
	longa	off
	lda	>320	; volatile
	rep	#$20
	longa	on
	and	#<$4
	sta	<L3+reg_1
	lda	<L3+reg_1
	bne	L4
	brl	L10001
L4:
;		
;		textScreen[6] = spinner[spinnerState];
	lda	|~~spinnerState
	and	#$ff
	sta	<R0
	sep	#$20
	longa	off
	ldx	<R0
	lda	|~~spinner,X
	sta	>11509766	; volatile
	rep	#$20
	longa	on
;		if (spinnerState < 7)
;			spinnerState++;
	sep	#$20
	longa	off
	lda	|~~spinnerState
	cmp	#<$7
	rep	#$20
	longa	on
	bcc	L5
	brl	L10002
L5:
	sep	#$20
	longa	off
	inc	|~~spinnerState
	rep	#$20
	longa	on
;		else
	brl	L10003
L10002:
;		{
;			spinnerState = 0;
	sep	#$20
	longa	off
	stz	|~~spinnerState
	rep	#$20
	longa	on
;		}
L10003:
;
;		//disk_timerproc();
;
;		reg = INT_PENDING_REG0 & FNX0_INT02_TMR0;
	sep	#$20
	longa	off
	lda	>320	; volatile
	rep	#$20
	longa	on
	and	#<$4
	sta	<L3+reg_1
;		INT_PENDING_REG0 = reg;
	sep	#$20
	longa	off
	lda	<L3+reg_1
	sta	>320	; volatile
	rep	#$20
	longa	on
;	}   
;
;	if (reg = (INT_PENDING_REG3 & FNX3_INT02_IDE))
L10001:
;	{
	sep	#$20
	longa	off
	lda	>323	; volatile
	rep	#$20
	longa	on
	and	#<$4
	sta	<L3+reg_1
	lda	<L3+reg_1
	bne	L6
	brl	L10004
L6:
;		
;		textScreen[10] = spinner[spinnerStateDisk];
	lda	|~~spinnerStateDisk
	and	#$ff
	sta	<R0
	sep	#$20
	longa	off
	ldx	<R0
	lda	|~~spinner,X
	sta	>11509770	; volatile
	rep	#$20
	longa	on
;		if (spinnerStateDisk < 7)
;			spinnerStateDisk++;
	sep	#$20
	longa	off
	lda	|~~spinnerStateDisk
	cmp	#<$7
	rep	#$20
	longa	on
	bcc	L7
	brl	L10005
L7:
	sep	#$20
	longa	off
	inc	|~~spinnerStateDisk
	rep	#$20
	longa	on
;		else
	brl	L10006
L10005:
;		{
;			spinnerStateDisk = 0;
	sep	#$20
	longa	off
	stz	|~~spinnerStateDisk
	rep	#$20
	longa	on
;		}
L10006:
;
;		printf("interrupt %d", IDE_CMD_STAT);
	sep	#$20
	longa	off
	lda	>11528247	; volatile
	rep	#$20
	longa	on
	and	#$ff
	pha
	pea	#^L1
	pea	#<L1
	pea	#8
	jsl	~~printf
;
;		reg = INT_PENDING_REG3 & INT_PENDING_REG3;
	sep	#$20
	longa	off
	lda	>323	; volatile
	rep	#$20
	longa	on
	and	#$ff
	sta	<R0
	sep	#$20
	longa	off
	lda	>323	; volatile
	rep	#$20
	longa	on
	and	#$ff
	sta	<R1
	lda	<R1
	and	<R0
	sta	<L3+reg_1
;		INT_PENDING_REG3 = reg;
	sep	#$20
	longa	off
	lda	<L3+reg_1
	sta	>323	; volatile
	rep	#$20
	longa	on
;	}   
;}
L10004:
L8:
	pld
	tsc
	clc
	adc	#L2
	tcs
	rtl
L2	equ	10
L3	equ	9
	ends
	efunc
	data
L1:
	db	$69,$6E,$74,$65,$72,$72,$75,$70,$74,$20,$25,$64,$00
	ends
;
;void COPHandler(void)              
;{             
	code
	xdef	~~COPHandler
	func
~~COPHandler:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L10
	tcs
	phd
	tcd
;}
L12:
	pld
	tsc
	clc
	adc	#L10
	tcs
	rtl
L10	equ	0
L11	equ	1
	ends
	efunc
;
;void BRKHandler(void)              
;{             
	code
	xdef	~~BRKHandler
	func
~~BRKHandler:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L13
	tcs
	phd
	tcd
;}
L15:
	pld
	tsc
	clc
	adc	#L13
	tcs
	rtl
L13	equ	0
L14	equ	1
	ends
	efunc
;
;
;DWORD buff[FF_MAX_SS];  /* Working buffer (4 sector in size) */
;
;void main (void)
;{
	code
	xdef	~~main
	func
~~main:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L16
	tcs
	phd
	tcd
;    int rc;
;	int clock = 14318000;
;	//int spd = clock / 100;
;	int spd = clock / 1;
;    
;	LBA_t plist[] = {50, 50, 0};  /* Divide the drive into two partitions */
	data
L18:
	dl	$32,$32,$0
	ends
;
;	// Emulator workarround for screen
;	//set the display size - 128 x 64
;	COLS_PER_LINE = 80;
rc_1	set	0
clock_1	set	2
spd_1	set	4
plist_1	set	6
	lda	#$79b0
	sta	<L17+clock_1
	lda	<L17+clock_1
	sta	<L17+spd_1
	pea	#^L18
	pea	#<L18
	clc
	tdc
	adc	#<L17+plist_1
	sta	<R0
	lda	#$0
	sta	<R0+2
	pei	<R0+2
	pei	<R0
	lda	#$c
	xref	~~~fmov
	jsl	~~~fmov
	lda	#$50
	sta	>17	; volatile
;	LINES_MAX = 60;
	lda	#$3c
	sta	>21	; volatile
;	//set the visible display size - 80 x 60
;  	COLS_VISIBLE = 80;
	lda	#$50
	sta	>15	; volatile
;	LINES_VISIBLE = 60;
	lda	#$3c
	sta	>19	; volatile
;
;	TIMER0_CHARGE_L = 0x00;
	sep	#$20
	longa	off
	lda	#$0
	sta	>353	; volatile
	rep	#$20
	longa	on
;	TIMER0_CHARGE_M = 0x00;
	sep	#$20
	longa	off
	lda	#$0
	sta	>354	; volatile
	rep	#$20
	longa	on
;	TIMER0_CHARGE_H = 0x00;
	sep	#$20
	longa	off
	lda	#$0
	sta	>355	; volatile
	rep	#$20
	longa	on
;	TIMER0_CMP_L = (spd) & 0xFF;
	lda	<L17+spd_1
	and	#<$ff
	sta	<R0
	sep	#$20
	longa	off
	lda	<R0
	sta	>357	; volatile
	rep	#$20
	longa	on
;	TIMER0_CMP_M = (spd >> 8) & 0xFF;
	lda	<L17+spd_1
	ldx	#<$8
	xref	~~~asr
	jsl	~~~asr
	and	#<$ff
	sta	<R0
	sep	#$20
	longa	off
	lda	<R0
	sta	>358	; volatile
	rep	#$20
	longa	on
;	TIMER0_CMP_H = (spd >> 16) & 0xFF;;
	lda	<L17+spd_1
	ldx	#<$10
	xref	~~~asr
	jsl	~~~asr
	and	#<$ff
	sta	<R0
	sep	#$20
	longa	off
	lda	<R0
	sta	>359	; volatile
	rep	#$20
	longa	on
;	
;	TIMER0_CMP_REG = TMR0_CMP_RECLR;
	sep	#$20
	longa	off
	lda	#$1
	sta	>356	; volatile
	rep	#$20
	longa	on
;
;	TIMER0_CTRL_REG = TMR_EN | TMR_UPDWN | TMR_SCLR;
	sep	#$20
	longa	off
	lda	#$b
	sta	>352
	rep	#$20
	longa	on
;
;	INT_MASK_REG0 = 0xFB; // unmask timer 0
	sep	#$20
	longa	off
	lda	#$fb
	sta	>332	; volatile
	rep	#$20
	longa	on
;
;	INT_MASK_REG0 = 0xFB; // unmask harddisk;
	sep	#$20
	longa	off
	lda	#$fb
	sta	>332	; volatile
	rep	#$20
	longa	on
;
;	// enable interrupts
;	enableInterrupts();
	asmstart
	cli
	asmend
;
;	setEGATextPalette();
	jsl	~~setEGATextPalette
;	clearTextScreen(' ', 0xD, 0xE);
	pea	#<$e
	pea	#<$d
	pea	#<$20
	jsl	~~clearTextScreen
;
;	VKY_TXT_CURSOR_X_REG = 0;
	lda	#$0
	sta	>11468820	; volatile
;	VKY_TXT_CURSOR_Y_REG = 0;
	lda	#$0
	sta	>11468822	; volatile
;
;	printf("\nATA test....");
	pea	#^L9
	pea	#<L9
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+14
	pea	#<L9+14
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+25
	pea	#<L9+25
	pea	#6
	jsl	~~printf
;	
;	
;    
;    f_fdisk(0, plist, buff);                    /* Divide physical drive 0 */
	lda	#<~~buff
	sta	<R0
	xref	_BEG_DATA
	lda	#_BEG_DATA>>16
	sta	<R0+2
	pei	<R0+2
	pei	<R0
	pea	#0
	clc
	tdc
	adc	#<L17+plist_1
	pha
	pea	#<$0
	jsl	~~f_fdisk
;
;    //f_mkfs("0:", 0, buff, sizeof buff); /* Create FAT volume on the logical drive 0 */
;    //f_mkfs("1:", 0, buff, sizeof buff); /* Create FAT volume on the logical drive 1 */
;
;	printf("\ntest test");
	pea	#^L9+36
	pea	#<L9+36
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+47
	pea	#<L9+47
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+58
	pea	#<L9+58
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+69
	pea	#<L9+69
	pea	#6
	jsl	~~printf
;	printf("\ntest test");
	pea	#^L9+80
	pea	#<L9+80
	pea	#6
	jsl	~~printf
;
; 	rc = 0;
	stz	<L17+rc_1
;	while(1)
L10007:
;	{
;		if (rc < 4)
;		{
	lda	<L17+rc_1
	bmi	L19
	dea
	dea
	dea
	dea
	bmi	L19
	brl	L10009
L19:
;			printf("\ntest test2");
	pea	#^L9+91
	pea	#<L9+91
	pea	#6
	jsl	~~printf
;			rc++;
	inc	<L17+rc_1
;		}
;	}
L10009:
	brl	L10007
;    //return rc;
;}
L16	equ	22
L17	equ	5
	ends
	efunc
	data
L9:
	db	$0A,$41,$54,$41,$20,$74,$65,$73,$74,$2E,$2E,$2E,$2E,$00,$0A
	db	$74,$65,$73,$74,$20,$74,$65,$73,$74,$00,$0A,$74,$65,$73,$74
	db	$20,$74,$65,$73,$74,$00,$0A,$74,$65,$73,$74,$20,$74,$65,$73
	db	$74,$00,$0A,$74,$65,$73,$74,$20,$74,$65,$73,$74,$00,$0A,$74
	db	$65,$73,$74,$20,$74,$65,$73,$74,$00,$0A,$74,$65,$73,$74,$20
	db	$74,$65,$73,$74,$00,$0A,$74,$65,$73,$74,$20,$74,$65,$73,$74
	db	$00,$0A,$74,$65,$73,$74,$20,$74,$65,$73,$74,$32,$00
	ends
;
;void _abort(void) {
	code
	xdef	~~_abort
	func
~~_abort:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L21
	tcs
	phd
	tcd
;
;}
L23:
	pld
	tsc
	clc
	adc	#L21
	tcs
	rtl
L21	equ	0
L22	equ	1
	ends
	efunc
;
;int close(int fd) {
	code
	xdef	~~close
	func
~~close:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L24
	tcs
	phd
	tcd
fd_0	set	4
;    return 0;
	lda	#$0
L26:
	tay
	lda	<L24+2
	sta	<L24+2+2
	lda	<L24+1
	sta	<L24+1+2
	pld
	tsc
	clc
	adc	#L24+2
	tcs
	tya
	rtl
;}
L24	equ	0
L25	equ	1
	ends
	efunc
;
;int creat(const char *_name, int _mode) {
	code
	xdef	~~creat
	func
~~creat:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L27
	tcs
	phd
	tcd
_name_0	set	4
_mode_0	set	8
;    return 0;
	lda	#$0
L29:
	tay
	lda	<L27+2
	sta	<L27+2+6
	lda	<L27+1
	sta	<L27+1+6
	pld
	tsc
	clc
	adc	#L27+6
	tcs
	tya
	rtl
;}
L27	equ	0
L28	equ	1
	ends
	efunc
;
;
;long lseek(int fd, long pos, int rel) {
	code
	xdef	~~lseek
	func
~~lseek:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L30
	tcs
	phd
	tcd
fd_0	set	4
pos_0	set	6
rel_0	set	10
;    return 0;
	lda	#$0
	tax
	lda	#$0
L32:
	tay
	lda	<L30+2
	sta	<L30+2+8
	lda	<L30+1
	sta	<L30+1+8
	pld
	tsc
	clc
	adc	#L30+8
	tcs
	tya
	rtl
;}
L30	equ	0
L31	equ	1
	ends
	efunc
;
;int open(const char * _name, int _mode) {
	code
	xdef	~~open
	func
~~open:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L33
	tcs
	phd
	tcd
_name_0	set	4
_mode_0	set	8
;    return 0;
	lda	#$0
L35:
	tay
	lda	<L33+2
	sta	<L33+2+6
	lda	<L33+1
	sta	<L33+1+6
	pld
	tsc
	clc
	adc	#L33+6
	tcs
	tya
	rtl
;}
L33	equ	0
L34	equ	1
	ends
	efunc
;
;size_t read(int fd, void *buffer, size_t len) {
	code
	xdef	~~read
	func
~~read:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L36
	tcs
	phd
	tcd
fd_0	set	4
buffer_0	set	6
len_0	set	10
;    return 0;
	lda	#$0
L38:
	tay
	lda	<L36+2
	sta	<L36+2+8
	lda	<L36+1
	sta	<L36+1+8
	pld
	tsc
	clc
	adc	#L36+8
	tcs
	tya
	rtl
;}
L36	equ	0
L37	equ	1
	ends
	efunc
;
;int unlink(const char *filename) {
	code
	xdef	~~unlink
	func
~~unlink:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L39
	tcs
	phd
	tcd
filename_0	set	4
;    return 0;
	lda	#$0
L41:
	tay
	lda	<L39+2
	sta	<L39+2+4
	lda	<L39+1
	sta	<L39+1+4
	pld
	tsc
	clc
	adc	#L39+4
	tcs
	tya
	rtl
;}
L39	equ	0
L40	equ	1
	ends
	efunc
;
;size_t write(int fd, void *buffer, size_t len) {
	code
	xdef	~~write
	func
~~write:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L42
	tcs
	phd
	tcd
fd_0	set	4
buffer_0	set	6
len_0	set	10
;    size_t count;
;	for (count = 0; count < len; count++)
count_1	set	0
	stz	<L43+count_1
	brl	L10011
L10010:
	inc	<L43+count_1
L10011:
	lda	<L43+count_1
	cmp	<L42+len_0
	bcc	L44
	brl	L10012
L44:
;	{
;		if (((unsigned char *)buffer)[count] == '\n')
;		{
	sep	#$20
	longa	off
	ldy	<L43+count_1
	lda	[<L42+buffer_0],Y
	cmp	#<$a
	rep	#$20
	longa	on
	beq	L45
	brl	L10013
L45:
;			VKY_TXT_CURSOR_X_REG = 0;
	lda	#$0
	sta	>11468820	; volatile
;			VKY_TXT_CURSOR_Y_REG++;
	lda	>11468822	; volatile
	ina
	sta	>11468822	; volatile
;
;			if (VKY_TXT_CURSOR_Y_REG == LINES_VISIBLE)
;			{
	lda	>11468822	; volatile
	cmp	>19	; volatile
	beq	L46
	brl	L10014
L46:
;				VKY_TXT_CURSOR_Y_REG = 0;
	lda	#$0
	sta	>11468822	; volatile
;			}
;
;			continue;
L10014:
	brl	L10010
;		}
;
;		textScreen[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = ((unsigned char *)buffer)[count];
L10013:
	lda	>11468822	; volatile
	ldx	#<$7
	xref	~~~asl
	jsl	~~~asl
	sta	<R0
	clc
	lda	<R0
	adc	>11468820	; volatile
	sta	<R1
	lda	#$a000
	sta	<R0
	lda	#$af
	sta	<R0+2
	sep	#$20
	longa	off
	ldy	<L43+count_1
	lda	[<L42+buffer_0],Y
	ldy	<R1
	sta	[<R0],Y
	rep	#$20
	longa	on
;		
;		textScreenColor[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = 0xE0;
	lda	>11468822	; volatile
	ldx	#<$7
	xref	~~~asl
	jsl	~~~asl
	sta	<R0
	clc
	lda	<R0
	adc	>11468820	; volatile
	sta	<R1
	lda	#$c000
	sta	<R0
	lda	#$af
	sta	<R0+2
	sep	#$20
	longa	off
	lda	#$e0
	ldy	<R1
	sta	[<R0],Y
	rep	#$20
	longa	on
;		/*
;		textScreenColor[(0x80 * VKY_TXT_CURSOR_Y_REG) + VKY_TXT_CURSOR_X_REG] = color;
;		color += 16;
;		if (color == 0xF0)
;			color = 0x10;*/
;		
;		VKY_TXT_CURSOR_X_REG++;
	lda	>11468820	; volatile
	ina
	sta	>11468820	; volatile
;		
;		if (VKY_TXT_CURSOR_X_REG == COLS_VISIBLE)
;		{
	lda	>11468820	; volatile
	cmp	>15	; volatile
	beq	L47
	brl	L10015
L47:
;			VKY_TXT_CURSOR_X_REG = 0;
	lda	#$0
	sta	>11468820	; volatile
;			VKY_TXT_CURSOR_Y_REG++;
	lda	>11468822	; volatile
	ina
	sta	>11468822	; volatile
;			
;			if (VKY_TXT_CURSOR_Y_REG == LINES_VISIBLE)
;			{
	lda	>11468822	; volatile
	cmp	>19	; volatile
	beq	L48
	brl	L10016
L48:
;				VKY_TXT_CURSOR_Y_REG = 0;
	lda	#$0
	sta	>11468822	; volatile
;			}
;		}
L10016:
;	}
L10015:
	brl	L10010
L10012:
;    return len;
	lda	<L42+len_0
L49:
	tay
	lda	<L42+2
	sta	<L42+2+8
	lda	<L42+1
	sta	<L42+1+8
	pld
	tsc
	clc
	adc	#L42+8
	tcs
	tya
	rtl
;}
L42	equ	10
L43	equ	9
	ends
	efunc
;
;//
;// Missing STDLIB.H function
;//
;int    isatty(int fd) {
	code
	xdef	~~isatty
	func
~~isatty:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L50
	tcs
	phd
	tcd
fd_0	set	4
;    // descriptors 0, 1 and 2 are STDIN_FILENO, STDOUT_FILENO and STDERR_FILENO
;    return fd < 3;
	stz	<R0
	lda	<L50+fd_0
	bmi	L53
	dea
	dea
	dea
	bmi	L53
	brl	L52
L53:
	inc	<R0
L52:
	lda	<R0
L54:
	tay
	lda	<L50+2
	sta	<L50+2+2
	lda	<L50+1
	sta	<L50+1+2
	pld
	tsc
	clc
	adc	#L50+2
	tcs
	tya
	rtl
;}
L50	equ	4
L51	equ	5
	ends
	efunc
;
	xref	~~setEGATextPalette
	xref	~~clearTextScreen
	xref	~~f_fdisk
	xref	~~printf
	udata
	xdef	~~buff
~~buff
	ds	2048
	ends
	end

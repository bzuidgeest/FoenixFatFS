;:ts=8
R0	equ	1
R1	equ	5
R2	equ	9
R3	equ	13
;/*-----------------------------------------------------------------------*/
;/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
;/*-----------------------------------------------------------------------*/
;/* If a working storage control module is available, it should be        */
;/* attached to the FatFs via a glue function rather than modifying it.   */
;/* This is an example of glue functions to attach various exsisting      */
;/* storage control modules to the FatFs module with a defined API.       */
;/*-----------------------------------------------------------------------*/
;
;#include "ff.h"			/* Obtains integer types */
;#include "diskio.h"		/* Declarations of disk functions */
;#include "ata.h"
;#include <stdio.h>
;
;/* Definitions of physical drive number for each drive */
;#define DEV_ATA		0	/* Example: Map ATAdisk to physical drive 0 */
;#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
;
;PARTITION VolToPart[] = {
	data
	xdef	~~VolToPart
~~VolToPart:
;	{0, 1},		/* Logical drive 0 ==> Physical drive 0, 1st partition */
	db	$0,$1
;	{0, 2},		/* Logical drive 1 ==> Physical drive 0, 2nd partition */
	db	$0,$2
;	{0, 3},		/* Logical drive 2 ==> Physical drive 0, 3nd partition */
	db	$0,$3
;	{0, 4}		/* Logical drive 3 ==> Physical drive 0, 4nd partition */
	db	$0,$4
;};
	ends
;
;/*-----------------------------------------------------------------------*/
;/* Get Drive Status                                                      */
;/*-----------------------------------------------------------------------*/
;
;DSTATUS disk_status (
;	BYTE pdrv		/* Physical drive nmuber to identify the drive */
;)
;{
	code
	xdef	~~disk_status
	func
~~disk_status:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L2
	tcs
	phd
	tcd
pdrv_0	set	4
;	DSTATUS stat;
;	int result;
;
;	//printf("a");
;
;	switch (pdrv) {
stat_1	set	0
result_1	set	1
	lda	<L2+pdrv_0
	and	#$ff
	brl	L10001
;	case DEV_ATA :
L10003:
;		return ATA_disk_status(0);
	pea	#<$0
	jsl	~~ATA_disk_status
	sep	#$20
	longa	off
	sta	<R0
	rep	#$20
	longa	on
	lda	<R0
	and	#$ff
L4:
	tay
	lda	<L2+2
	sta	<L2+2+2
	lda	<L2+1
	sta	<L2+1+2
	pld
	tsc
	clc
	adc	#L2+2
	tcs
	tya
	rtl
;
;		// translate the reslut code here
;
;
;	//case DEV_MMC :
;		//result = MMC_disk_status();
;
;		// translate the reslut code here
;
;	//	return stat;
;	}
L10001:
	xref	~~~swt
	jsl	~~~swt
	dw	1
	dw	0
	dw	L10003-1
	dw	L10002-1
L10002:
;	return STA_NOINIT;
	lda	#$1
	brl	L4
;}
L2	equ	7
L3	equ	5
	ends
	efunc
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Inidialize a Drive                                                    */
;/*-----------------------------------------------------------------------*/
;
;DSTATUS disk_initialize (
;	BYTE pdrv				/* Physical drive nmuber to identify the drive */
;)
;{
	code
	xdef	~~disk_initialize
	func
~~disk_initialize:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L5
	tcs
	phd
	tcd
pdrv_0	set	4
;	//printf("b");	
;
;	switch (pdrv) {
	lda	<L5+pdrv_0
	and	#$ff
	brl	L10004
;	 case 0 :
L10006:
;		return ATA_disk_initialize(pdrv);
	pei	<L5+pdrv_0
	jsl	~~ATA_disk_initialize
	sep	#$20
	longa	off
	sta	<R0
	rep	#$20
	longa	on
	lda	<R0
	and	#$ff
L7:
	tay
	lda	<L5+2
	sta	<L5+2+2
	lda	<L5+1
	sta	<L5+1+2
	pld
	tsc
	clc
	adc	#L5+2
	tcs
	tya
	rtl
;
;	// case DEV_MMC :
;	// 	//result = MMC_disk_initialize();
;
;	// 	// translate the reslut code here
;	// 	return stat;
;	}
L10004:
	xref	~~~swt
	jsl	~~~swt
	dw	1
	dw	0
	dw	L10006-1
	dw	L10005-1
L10005:
;	return STA_NOINIT;
	lda	#$1
	brl	L7
;}
L5	equ	4
L6	equ	5
	ends
	efunc
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Read Sector(s)                                                        */
;/*-----------------------------------------------------------------------*/
;
;DRESULT disk_read (
;	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
;	BYTE *buff,		/* Data buffer to store read data */
;	LBA_t sector,	/* Start sector in LBA */
;	UINT count		/* Number of sectors to read */
;)
;{
	code
	xdef	~~disk_read
	func
~~disk_read:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L8
	tcs
	phd
	tcd
pdrv_0	set	4
buff_0	set	6
sector_0	set	10
count_0	set	14
;	//printf("r");
;
;	switch (pdrv) {
	lda	<L8+pdrv_0
	and	#$ff
	brl	L10007
;	case DEV_ATA :
L10009:
;		return ATA_disk_read(0, buff, sector, count);
	pei	<L8+count_0
	pei	<L8+sector_0+2
	pei	<L8+sector_0
	pei	<L8+buff_0+2
	pei	<L8+buff_0
	pea	#<$0
	jsl	~~ATA_disk_read
L10:
	tay
	lda	<L8+2
	sta	<L8+2+12
	lda	<L8+1
	sta	<L8+1+12
	pld
	tsc
	clc
	adc	#L8+12
	tcs
	tya
	rtl
;
;//	case DEV_MMC :
;		// translate the arguments here
;
;//		result = MMC_disk_read(buff, sector, count);
;
;
;
;	}
L10007:
	xref	~~~swt
	jsl	~~~swt
	dw	1
	dw	0
	dw	L10009-1
	dw	L10008-1
L10008:
;
;	return RES_PARERR;
	lda	#$4
	brl	L10
;}
L8	equ	0
L9	equ	1
	ends
	efunc
;
;
;
;/*-----------------------------------------------------------------------*/
;/* Write Sector(s)                                                       */
;/*-----------------------------------------------------------------------*/
;
;#if FF_FS_READONLY == 0
;
;DRESULT disk_write (
;	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
;	const BYTE *buff,	/* Data to be written */
;	LBA_t sector,		/* Start sector in LBA */
;	UINT count			/* Number of sectors to write */
;)
;{
	code
	xdef	~~disk_write
	func
~~disk_write:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L11
	tcs
	phd
	tcd
pdrv_0	set	4
buff_0	set	6
sector_0	set	10
count_0	set	14
;	
;
;	//printf("c");
;
;	switch (pdrv) {
	lda	<L11+pdrv_0
	and	#$ff
	brl	L10010
;	case DEV_ATA :
L10012:
;		return ATA_disk_write(pdrv, buff, sector, count);
	pei	<L11+count_0
	pei	<L11+sector_0+2
	pei	<L11+sector_0
	pei	<L11+buff_0+2
	pei	<L11+buff_0
	pei	<L11+pdrv_0
	jsl	~~ATA_disk_write
L13:
	tay
	lda	<L11+2
	sta	<L11+2+12
	lda	<L11+1
	sta	<L11+1+12
	pld
	tsc
	clc
	adc	#L11+12
	tcs
	tya
	rtl
;
;	//case DEV_MMC :
;		// translate the arguments here
;
;		//result = MMC_disk_write(buff, sector, count);
;
;		// translate the reslut code here
;
;		//return res;
;
;	}
L10010:
	xref	~~~swt
	jsl	~~~swt
	dw	1
	dw	0
	dw	L10012-1
	dw	L10011-1
L10011:
;
;	return RES_PARERR;
	lda	#$4
	brl	L13
;}
L11	equ	0
L12	equ	1
	ends
	efunc
;
;#endif
;
;
;/*-----------------------------------------------------------------------*/
;/* Miscellaneous Functions                                               */
;/*-----------------------------------------------------------------------*/
;
;DRESULT disk_ioctl (
;	BYTE pdrv,		/* Physical drive nmuber (0..) */
;	BYTE cmd,		/* Control code */
;	void *buff		/* Buffer to send/receive control data */
;)
;{
	code
	xdef	~~disk_ioctl
	func
~~disk_ioctl:
	longa	on
	longi	on
	tsc
	sec
	sbc	#L14
	tcs
	phd
	tcd
pdrv_0	set	4
cmd_0	set	6
buff_0	set	8
;	DRESULT res = 0;
;
;	//printf("\nIOCTL %d", cmd);
;
;	switch (pdrv) {
res_1	set	0
	stz	<L15+res_1
	lda	<L14+pdrv_0
	and	#$ff
	brl	L10013
;	case DEV_ATA :
L10015:
;		res = ATA_disk_ioctl(pdrv, cmd, buff);
	pei	<L14+buff_0+2
	pei	<L14+buff_0
	pei	<L14+cmd_0
	pei	<L14+pdrv_0
	jsl	~~ATA_disk_ioctl
	sta	<L15+res_1
;		// Process of the command for the ATA drive
;
;		return res;
	lda	<L15+res_1
L16:
	tay
	lda	<L14+2
	sta	<L14+2+8
	lda	<L14+1
	sta	<L14+1+8
	pld
	tsc
	clc
	adc	#L14+8
	tcs
	tya
	rtl
;
;	//case DEV_MMC :
;
;		// Process of the command for the MMC/SD card
;
;		//return res;
;
;	}
L10013:
	xref	~~~swt
	jsl	~~~swt
	dw	1
	dw	0
	dw	L10015-1
	dw	L10014-1
L10014:
;
;	return RES_PARERR;
	lda	#$4
	brl	L16
;}
L14	equ	2
L15	equ	1
	ends
	efunc
;
;
	xref	~~ATA_disk_ioctl
	xref	~~ATA_disk_write
	xref	~~ATA_disk_read
	xref	~~ATA_disk_status
	xref	~~ATA_disk_initialize
	end

/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "ata.h"
#include "sd.h"
#include <stdio.h>

/* Definitions of physical drive number for each drive */
#define DEV_ATA		0	/* Example: Map ATAdisk to physical drive 0 */
#define DEV_SD		1	/* Example: Map MMC/SD card to physical drive 1 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	//printf("a");

	switch (pdrv) {
		case DEV_ATA :
			return ATA_disk_status(0);
		case DEV_SD :
			return sd_disk_status();
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	//printf("b:%d,", pdrv);	

	switch (pdrv) {
		case DEV_ATA :
			return ATA_disk_initialize(pdrv);

		case DEV_SD :
			return sd_disk_initialize();
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	//printf("r");

	switch (pdrv) {
		case DEV_ATA :
			return ATA_disk_read(0, buff, sector, count);

		case DEV_SD :
			return sd_disk_read(buff, sector, count);
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	

	//printf("w");

	switch (pdrv) {
		case DEV_ATA :
			return ATA_disk_write(pdrv, buff, sector, count);

		case DEV_SD :
			return sd_disk_write(buff, sector, count);
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = 0;

	//printf("\nIOCTL %d", cmd);

	switch (pdrv) {
		case DEV_ATA :
			return ATA_disk_ioctl(pdrv, cmd, buff);

		case DEV_SD :
			return sd_disk_ioctl(cmd, buff);

	}

	return RES_PARERR;
}


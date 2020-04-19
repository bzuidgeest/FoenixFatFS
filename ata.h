#ifndef ata_h
#define ata_h

#include "diskio.h" 

DSTATUS ATA_disk_initialize (BYTE pdrv);
DSTATUS ATA_disk_status (BYTE pdrv);
DRESULT ATA_disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
DRESULT ATA_disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
DRESULT ATA_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#define IDE_DATA8			(*(volatile unsigned char *)0xAFE830)
#define IDE_CMD_STAT		(*(volatile unsigned char *)0xAFE837)
#define IDE_DATA			(*(volatile unsigned short *)0xAFE839)
#define IDE_DATA_LO			(*(volatile unsigned char *)0xAFE838)
#define IDE_DATA_HI			(*(volatile unsigned char *)0xAFE839)
#define IDE_ERROR			(*(volatile unsigned char *)0xAFE831)
#define IDE_FEATURES		(*(volatile unsigned char *)0xAFE831)
#define IDE_SECT_CNT  		(*(volatile unsigned char *)0xAFE832)
#define IDE_SECT_SRT  		(*(volatile unsigned char *)0xAFE833)
#define IDE_CLDR   			(*(volatile unsigned short *)0xAFE834)
#define IDE_CLDR_LO   		(*(volatile unsigned char *)0xAFE834)
#define IDE_CLDR_HI   		(*(volatile unsigned char *)0xAFE835)
#define IDE_HEAD      		(*(volatile unsigned char *)0xAFE836)

// status register
// ;7    6    5   4  3   2   1    0
// ;BSY DRDY DF DSC DRQ CORR IDX ERR

/* ATA commands */
#define CMD_READ		0x20	/* READ SECTOR(S) */
#define CMD_WRITE		0x30	/* WRITE SECTOR(S) */
#define CMD_IDENTIFY	0xEC	/* DEVICE IDENTIFY */
#define CMD_SETFEATURES	0xEF	/* SET FEATURES */

/* ATA register bit definitions */
#define	HEAD_LBA				0x40	/* REG_HEAD */
#define	HEAD_DEV				0x10	/* REG_HEAD */
#define	STATUS_BSY				0x80	/* REG_STATUS */
#define	STATUS_DRDY				0x40	/* REG_STATUS */
#define	STATUS_DRQ				0x08	/* REG_STATUS */
#define	STATUS_ERR				0x01	/* REG_STATUS */



#define _USE_WRITE
#define _USE_IOCTL

#endif /* ata_h */
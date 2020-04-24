/*-----------------------------------------------------------------------*/
/* SDCarc control module                                           */
/*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
//#include "ffconf.h" included in ff.h
#include "ff.h"
#include "diskio.h"

#define _USE_WRITE
#define _USE_IOCTL

#include "../foenixLibrary/interrupt.h"
#include "../foenixLibrary/vicky.h"
#include "sd.h"

/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

/* Disk status */
static 
DSTATUS Stat = STA_NOINIT; 

/*-----------------------------------------------------------------------*/
/* Read an ATA register (Platform dependent)                             */
/*-----------------------------------------------------------------------*/

/* Read 512 bytes from SD RX FIFO */
static
void read_block (
	BYTE *buf
)
{
	short c = 0;

	do {	
		buf[c] = SDC_RX_FIFO_DATA_REG;
		c++;
	} while (c < 512);
}


/* Read 512 bytes from ATA data register but store a part of block */
static
void read_block_part (
	BYTE *buf,
	unsigned short ofs,
	BYTE nw
)
{
	BYTE data = 0;
	short c = 0;

	do {	
		//buf[c] = SDC_RX_FIFO_DATA_REG;
	
		if (nw && (c >= ofs)) {	/* Pick up a part of block */
	 		*buf++ = SDC_RX_FIFO_DATA_REG;
	 		nw--;
		}
		else
		{
			//dummy read to empty FIFO of unneeded bytes
			data = SDC_RX_FIFO_DATA_REG;
		}
		
		c++;
	} while (c < 512);
}

static
void sd_clear_FIFO()
{
	SDC_TX_FIFO_CTRL_REG = SDC_RXTX_FIFO_CTRL_CLEAR;
	SDC_RX_FIFO_CTRL_REG = SDC_RXTX_FIFO_CTRL_CLEAR;
	
	wait_trans(1000, 0);
}

BYTE sd_sendCommand(BYTE cmd, DWORD arg) {
	BYTE status = 0;
	BYTE i = 0;
	BYTE crc = 0;
	BYTE data = 0;
	int s = 0;

	// Clear FIFO
	sd_clear_FIFO();

wait_trans(3000, 0);
printf("go");
	// send command
	SDC_TRANS_TYPE_REG = SDC_TRANS_DIRECT;
	data = cmd | 0x40;
	SDC_DIRECT_ACCESS_REG = data;
	SDC_TRANS_CONTROL_REG = SDC_TRANS_START;
	
	wait_trans(3000, 0);

	//wait_trans(3000, 0);
	// asm nop;
	// asm nop;
	// asm nop;
	// asm nop;
	// asm nop;
	// asm nop;

	//send argument
	s = 24;
	while (s >= 0)
	{
		SDC_DIRECT_ACCESS_REG = arg >> s;
		SDC_TRANS_CONTROL_REG = SDC_TRANS_START;
		//wait_trans(3000, 0);
		printf("\n%d", s);
		s -= 8;
	}
 
	// send CRC
	//SDC_TRANS_TYPE_REG = SDC_TRANS_DIRECT;
	crc = 0xFF;
	if (cmd == CMD0) crc = 0X95;  // correct crc for CMD0 with arg 0
	if (cmd == CMD8) crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
	if (cmd == CMD9) crc = 0X95;  // correct crc for CMD9 with arg 0X00
	SDC_DIRECT_ACCESS_REG = crc;
	//SDC_TRANS_CONTROL_REG = SDC_TRANS_START;
	//data = SDC_DIRECT_ACCESS_REG;
	//wait_trans(3000, 0);

	// wait for response
	// Getting response is unclear....
	//for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++)
	// for (i = 0; ((status = sd_receive_direct()) & 0X80) && i != 0XFF; i++)
	// {
	// 	// if (i < 100)
	// 	// 	printf("\ncmd status %d", status);
	// }
printf("go");
	for (s = 0; ((status = sd_receive_direct()) != 0x00); s++)
	{
		// if (s< 10)
		// printf("go");
	}

	//printf("\nyRX FIFO Size %d %d", SDC_RX_FIFO_DATA_CNT_HI, SDC_RX_FIFO_DATA_CNT_LO);

	return status;
}

BYTE sd_receive_direct()
{
	BYTE response = 0;
	
	SDC_DIRECT_ACCESS_REG = 0xFF;
	SDC_TRANS_CONTROL_REG = SDC_TRANS_START;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;
	asm nop;

	response = SDC_DIRECT_ACCESS_REG;
	//printf("\nresponse: %d", response);
	return response;
}

BYTE sd_read_csd()
{
	BYTE result = 0;
	unsigned short i = 0;

	result = sd_sendCommand(CMD9, 0);

	for (i = 0; i < 16; i++)
	{ 
		//dst[i] = sd_receive_direct();
		printf("\nCSD %d", sd_receive_direct());
	}
	sd_receive_direct();  // get first crc byte
	sd_receive_direct();  // get second crc byte

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Write 512 byte block to SDC TX FIFO register                          */
/*-----------------------------------------------------------------------*/

#ifdef _USE_WRITE
static
void write_block (
	const BYTE *buf
)
{
	short c;

	c = 512;
	do {
		SDC_TX_FIFO_DATA_REG = *buf++;;
	} while (--c);
}
#endif



/*-----------------------------------------------------------------------*/
/* Wait for BSY goes 0 and the specified bit goes 1                                */
/*-----------------------------------------------------------------------*/

static
int wait_trans (	/* 0:Timeout or or ERR goes 1 */
	UINT ms,
	BYTE bit
)
{
	BYTE status = 0, error = 0;

	do {
		status = SDC_TRANS_STATUS_REG;
		//printf("\nstatus! %d", status);
		error = SDC_TRANS_ERROR_REG;
		if (error != 0) 
		{
			
			printf("\nerror! %d", error);
			return 0;	/* Abort when timeout or error occured */
		}
	} while (status & SDC_TRANS_BUSY);		/* Wait for BSY to go 0 */

	error = SDC_TRANS_ERROR_REG;
	return error;
}



/*-----------------------------------------------------------------------*/
/* Issue Read/Write command to the drive                                 */
/*-----------------------------------------------------------------------*/

static
int issue_rwcmd (
	BYTE cmd,
	DWORD sector
)
{
	// Clear the FIFO to clear out old data.
	sd_clear_FIFO();

	// SDC addres is in bytes so multiply sector by 512 as we need to align to 512 byte sectors
	sector *= 512;
	SDC_SD_ADDR_7_0_REG = 0;
	//printf("\nSDC_SD_ADDR_7_0_REG %d",(sector & 0xFF) );
	sector >>= 8;
	SDC_SD_ADDR_15_8_REG = (sector & 0xFF);
	//printf("\nSDC_SD_ADDR_7_0_REG %d",(sector & 0xFF) );
	sector >>= 8;
	//printf("\nSDC_SD_ADDR_7_0_REG %d",(sector & 0xFF) );
	SDC_SD_ADDR_23_16_REG = (sector & 0xFF);
	sector >>= 8;
	//printf("\nSDC_SD_ADDR_7_0_REG %d",(sector & 0xFF) );
	SDC_SD_ADDR_31_24_REG = (sector & 0xFF);

	//printf("\ndo %d", cmd);
	SDC_TRANS_TYPE_REG = cmd;
	SDC_TRANS_CONTROL_REG = SDC_TRANS_START;

	return 1;
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS sd_disk_initialize ()
{
	/* Returns current status if initialization has been done */
	if (Stat != STA_NOINIT) return Stat;	

	// Reset core logic
	//SDC_CONTROL_REG = 1;

	// small delay for core reset to happen
	// asm nop;
	// asm nop;
	// asm nop;
	// asm nop;
	
//printf("x");
	// Init SDCard
	SDC_TRANS_TYPE_REG = SDC_TRANS_INIT_SD;
	SDC_TRANS_CONTROL_REG = SDC_TRANS_START;
	
	if (wait_trans(3000, 0) == 0) {
		Stat = 0;
	}
	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS sd_disk_status (void)
{
	/* Supports only single drive */
	return Stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT sd_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector number (LBA) */
	UINT count		/* Sector count (1..128) */
)
{
	BYTE status, i = 0;

	if (Stat & STA_NOINIT) return RES_NOTRDY;

	// Do count single sector reads.
	for (i = 0; i < count; i++)
	{
	
		/* Issue Read Setor(s) command */
		if (!issue_rwcmd(SDC_TRANS_READ_BLK, sector)) return RES_ERROR;

		//printf("\n count: %d sector: %d", i, sector);
		
		if (wait_trans(2000, 0) != 0) return RES_ERROR; 	/* Wait for a sector to be read into FIFO */
		//printf("\nbuff %d", buff);
		
		//printf("\nBlock read: RX FIFO Size %d", (SDC_RX_FIFO_DATA_CNT_HI << 8) | SDC_RX_FIFO_DATA_CNT_LO);

		read_block(buff);	/* Read a sector */
		buff += 512;
		sector++;
	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#ifdef _USE_WRITE
DRESULT sd_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector number (LBA) */
	UINT count			/* Sector count (1..128) */
)
{
	BYTE stat;
	short dummy = 0;

	if (Stat & STA_NOINIT) return RES_NOTRDY;

	//printf("\nCount: %d Sector: %d", count, sector);
	
	/* Send data blocks */
	do {
		// Write block to FIFO
		write_block(buff);	

		// Issue Write Setor command 
		if (!issue_rwcmd(SDC_TRANS_WRITE_BLK, sector++)) return RES_ERROR;

		// Wait for transaction to end or error
		if (wait_trans(2000, 0) != 0) return RES_ERROR;	/* Wait for request to send data */

		buff += 512;

	} while (--count);		/* Repeat until all sector sent */

	return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#ifdef _USE_IOCTL
DRESULT sd_disk_ioctl (
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
)
{
	BYTE *ptr = (BYTE*)buff;

	if (Stat & STA_NOINIT) return RES_NOTRDY;

	switch (cmd) {
		case CTRL_SYNC :		/* Nothing to do */
			return RES_OK;

		case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
			// unfortunately current SD controller does not seem to support reading CSD data. 
			// So we get sector count from partition table
			//*(DWORD*)buff = 100000;
			if (!issue_rwcmd(SDC_TRANS_READ_BLK, 0)) return RES_ERROR;
			wait_trans(3000, 0);
			read_block_part(ptr, 458, 4);
			break;

		case GET_BLOCK_SIZE :	/* Get erase block size in sectors (DWORD) */
			*(DWORD*)buff = 1;
			return RES_OK;

		case MMC_GET_TYPE :
			
			break;

		case MMC_GET_CSD :
			
			break;

		case MMC_GET_CID :
			
			break;
		
		case MMC_GET_OCR :

			break;

		case MMC_GET_SDSTAT:

			break;

		default:
			return RES_PARERR;
	}

	//read_block_part(ptr, ofs, w);

	return RES_OK;
}
#endif

  
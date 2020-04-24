// GABE Internal SDCard Controller

#ifndef SD_H
#define SD_H

#define SDC_VERSION_REG				(*(volatile unsigned char *)0xAFEA00)	// Ought to read 12
#define SDC_CONTROL_REG				(*(volatile unsigned char *)0xAFEA01)	// Bit0 1 = Reset core logic, and registers. Self clearing
#define SDC_TRANS_TYPE_REG 			(*(volatile unsigned char *)0xAFEA02)	// Bit[1:0]
#define SDC_TRANS_DIRECT 			0x00	// 00 = Direct Access
#define SDC_TRANS_INIT_SD			0x01	// 01 = Init SD
#define SDC_TRANS_READ_BLK 			0x02	// 10 = RW_READ_BLOCK (512 Bytes)
#define SDC_TRANS_WRITE_BLK			0x03	// 11 = RW_WRITE_SD_BLOCK

#define SDC_TRANS_CONTROL_REG		(*(volatile unsigned char *)0xAFEA03)
#define SDC_TRANS_START				0x01

#define SDC_TRANS_STATUS_REG		(*(volatile unsigned char *)0xAFEA04)
#define SDC_TRANS_BUSY				0x01	// 1 = Transaction Busy

#define SDC_TRANS_ERROR_REG			(*(volatile unsigned char *)0xAFEA05)
#define SDC_TRANS_INIT_NO_ERR		0x00	// Init Error Report[1:0]
#define SDC_TRANS_INIT_CMD0_ERR		0x01
#define SDC_TRANS_INIT_CMD1_ERR		0x02

#define SDC_TRANS_RD_NO_ERR			0x00	// Read Error Report[3:2]
#define SDC_TRANS_RD_CMD_ERR		0x04
#define SDC_TRANS_RD_TOKEN_ERR		0x08

#define SDC_TRANS_WR_NO_ERR			0x00	// Write Report Error[5:4]
#define SDC_TRANS_WR_CMD_ERR		0x10	//
#define SDC_TRANS_WR_DATA_ERR		0x20
#define SDC_TRANS_WR_BUSY_ERR		0x30

#define SDC_DIRECT_ACCESS_REG		(*(volatile unsigned char *)0xAFEA06)	// SPI Direct Read and Write - Set DATA before initiating direct Access Transaction

#define SDC_SD_ADDR_7_0_REG			(*(volatile unsigned char *)0xAFEA07)	// Set the ADDR before a block read or block write
#define SDC_SD_ADDR_15_8_REG		(*(volatile unsigned char *)0xAFEA08)	// Addr0 [8:0] Always should be 0, since each block is 512Bytes
#define SDC_SD_ADDR_23_16_REG		(*(volatile unsigned char *)0xAFEA09)
#define SDC_SD_ADDR_31_24_REG		(*(volatile unsigned char *)0xAFEA0A)

// Not used
//#define SDC_SPI_CLK_DEL_REG			(*(volatile unsigned char *)0xAFEA0B)
//...
#define SDC_RX_FIFO_DATA_REG		(*(volatile BYTE *)0xAFEA10)	// Data from the Block Read
#define SDC_RX_FIFO_DATA_CNT_HI		(*(volatile unsigned char *)0xAFEA12)	// How many Bytes in the FIFO HI
#define SDC_RX_FIFO_DATA_CNT_LO		(*(volatile unsigned char *)0xAFEA13)	// How many Bytes in the FIFO LO
#define SDC_RX_FIFO_CTRL_REG 		(*(volatile unsigned char *)0xAFEA14)	// Bit0 Force Empty - Set to 1 to clear FIFO, self clearing (the bit)
//...
#define SDC_TX_FIFO_DATA_REG		(*(volatile unsigned char *)0xAFEA20)	// Write Data Block here
#define SDC_TX_FIFO_CTRL_REG		(*(volatile unsigned char *)0xAFEA24)	// Bit0 Force Empty - Set to 1 to clear FIFO, self clearing (the bit)

#define SDC_RXTX_FIFO_CTRL_CLEAR	0x1;


//====================== Sofware defines
// Based on the document:
//
// SD Specifications
// Part 1
// Physical Layer
// Simplified Specification
// Version 2.00
// September 25, 2006
//
// www.sdcard.org/developers/tech/sdcard/pls/Simplified_Physical_Layer_Spec.pdf
//------------------------------------------------------------------------------

// SD card commands
// GO_IDLE_STATE - init card in spi mode if CS low 
#define CMD0 (BYTE)0X00
// SEND_IF_COND - verify SD Memory Card interface operating condition.
#define CMD8 (BYTE)0X08
// SEND_CSD - read the Card Specific Data (CSD register) 
#define CMD9 (BYTE)0X09
// SEND_CID - read the card identification information (CID register) 
#define CMD10 (BYTE)0X0A
// SEND_STATUS - read the card status register 
#define CMD13 (BYTE)0X0D
// READ_BLOCK - read a single data block from the card 
#define CMD17 (BYTE)0X11
// WRITE_BLOCK - write a single data block to the card 
#define CMD24 (BYTE)0X18
// WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION 
#define CMD25 (BYTE)0X19
// ERASE_WR_BLK_START - sets the address of the first block to be erased 
#define CMD32 (BYTE)0X20
// ERASE_WR_BLK_END - sets the address of the last block of the continuous range to be erased
#define CMD33 (BYTE)0X21
// ERASE - erase all previously selected blocks 
#define CMD38 (BYTE)0X26
// APP_CMD - escape for application specific command 
#define CMD55 (BYTE)0X37
// READ_OCR - read the OCR register of a card 
#define CMD58 (BYTE)0X3A
// SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be pre-erased before writing 
#define ACMD23 (BYTE)0X17
// SD_SEND_OP_COMD - Sends host capacity support information and activates the card's initialization process 
#define ACMD41 (BYTE)0X29
//------------------------------------------------------------------------------
// /** status for card in the ready state */
// #define R1_READY_STATE (BYTE)0X00
// /** status for card in the idle state */
// #define R1_IDLE_STATE (BYTE)0X01
// status bit for illegal command
#define R1_ILLEGAL_COMMAND (BYTE)0X04
// /** start data token for read or write single block*/
// #define DATA_START_BLOCK (BYTE)0XFE
// /** stop token for write multiple blocks*/
// #define STOP_TRAN_TOKEN (BYTE)0XFD
// /** start data token for write multiple blocks*/
// #define WRITE_MULTIPLE_TOKEN (BYTE)0XFC
// /** mask for data response tokens after a write block operation */
// #define DATA_RES_MASK (BYTE)0X1F
// /** write data accepted token */
// #define DATA_RES_ACCEPTED (BYTE)0X05
//------------------------------------------------------------------------------
// typedef struct CID {
// 	// byte 0
// 	BYTE mid;	// Manufacturer ID
// 	// byte 1-2
// 	char oid[2];	// OEM/Application ID
// 	// byte 3-7
// 	char pnm[5];	// Product name
// 	// byte 8
// 	unsigned prv_m : 4;	// Product revision n.m
// 	unsigned prv_n : 4;
// 	// byte 9-12
// 	unsigned long psn;	// Product serial number
// 	// byte 13
// 	unsigned mdt_year_high : 4;	// Manufacturing date
// 	unsigned reserved : 4;
// 	// byte 14
// 	unsigned mdt_month : 4;
// 	unsigned mdt_year_low :4;
// 	// byte 15
// 	unsigned always1 : 1;
// 	unsigned crc : 7;
// } cid_t;

// // CSD for version 1.00 cards
// typedef struct CSDV1 {
// 	// byte 0
// 	unsigned reserved1 : 6;
// 	unsigned csd_ver : 2;
// 	// byte 1
// 	BYTE taac;
// 	// byte 2
// 	BYTE nsac;
// 	// byte 3
// 	BYTE tran_speed;
// 	// byte 4
// 	BYTE ccc_high;
// 	// byte 5
// 	unsigned read_bl_len : 4;
// 	unsigned ccc_low : 4;
// 	// byte 6
// 	unsigned c_size_high : 2;
// 	unsigned reserved2 : 2;
// 	unsigned dsr_imp : 1;
// 	unsigned read_blk_misalign :1;
// 	unsigned write_blk_misalign : 1;
// 	unsigned read_bl_partial : 1;
// 	// byte 7
// 	BYTE c_size_mid;
// 	// byte 8
// 	unsigned vdd_r_curr_max : 3;
// 	unsigned vdd_r_curr_min : 3;
// 	unsigned c_size_low :2;
// 	// byte 9
// 	unsigned c_size_mult_high : 2;
// 	unsigned vdd_w_cur_max : 3;
// 	unsigned vdd_w_curr_min : 3;
// 	// byte 10
// 	unsigned sector_size_high : 6;
// 	unsigned erase_blk_en : 1;
// 	unsigned c_size_mult_low : 1;
// 	// byte 11
// 	unsigned wp_grp_size : 7;
// 	unsigned sector_size_low : 1;
// 	// byte 12
// 	unsigned write_bl_len_high : 2;
// 	unsigned r2w_factor : 3;
// 	unsigned reserved3 : 2;
// 	unsigned wp_grp_enable : 1;
// 	// byte 13
// 	unsigned reserved4 : 5;
// 	unsigned write_partial : 1;
// 	unsigned write_bl_len_low : 2;
// 	// byte 14
// 	unsigned reserved5: 2;
// 	unsigned file_format : 2;
// 	unsigned tmp_write_protect : 1;
// 	unsigned perm_write_protect : 1;
// 	unsigned copy : 1;
// 	unsigned file_format_grp : 1;
// 	// byte 15
// 	unsigned always1 : 1;
// 	unsigned crc : 7;
// } csd1_t;

// // CSD for version 2.00 cards
// typedef struct CSDV2 {
// 	// byte 0
// 	unsigned reserved1 : 6;
// 	unsigned csd_ver : 2;
// 	// byte 1
// 	BYTE taac;
// 	// byte 2
// 	BYTE nsac;
// 	// byte 3
// 	BYTE tran_speed;
// 	// byte 4
// 	BYTE ccc_high;
// 	// byte 5
// 	unsigned read_bl_len : 4;
// 	unsigned ccc_low : 4;
// 	// byte 6
// 	unsigned reserved2 : 4;
// 	unsigned dsr_imp : 1;
// 	unsigned read_blk_misalign :1;
// 	unsigned write_blk_misalign : 1;
// 	unsigned read_bl_partial : 1;
// 	// byte 7
// 	unsigned reserved3 : 2;
// 	unsigned c_size_high : 6;
// 	// byte 8
// 	BYTE c_size_mid;
// 	// byte 9
// 	BYTE c_size_low;
// 	// byte 10
// 	unsigned sector_size_high : 6;
// 	unsigned erase_blk_en : 1;
// 	unsigned reserved4 : 1;
// 	// byte 11
// 	unsigned wp_grp_size : 7;
// 	unsigned sector_size_low : 1;
// 	// byte 12
// 	unsigned write_bl_len_high : 2;
// 	unsigned r2w_factor : 3;
// 	unsigned reserved5 : 2;
// 	unsigned wp_grp_enable : 1;
// 	// byte 13
// 	unsigned reserved6 : 5;
// 	unsigned write_partial : 1;
// 	unsigned write_bl_len_low : 2;
// 	// byte 14
// 	unsigned reserved7: 2;
// 	unsigned file_format : 2;
// 	unsigned tmp_write_protect : 1;
// 	unsigned perm_write_protect : 1;
// 	unsigned copy : 1;
// 	unsigned file_format_grp : 1;
// 	// byte 15
// 	unsigned always1 : 1;
// 	unsigned crc : 7;
// } csd2_t;

// // union of old and new style CSD register
// union csd_t {
// 	csd1_t v1;
// 	csd2_t v2;
// };


BYTE sd_sendCommand(BYTE cmd, DWORD arg);
BYTE sd_receive_direct();

DRESULT sd_disk_read (
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector number (LBA) */
	UINT count		/* Sector count (1..128) */
);

DSTATUS sd_disk_status ();

DRESULT sd_disk_ioctl (
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive data block */
);

DRESULT sd_disk_write (
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector number (LBA) */
	UINT count			/* Sector count (1..128) */
);

DSTATUS sd_disk_initialize ();
BYTE sd_read_csd();

#endif /* SD_H */

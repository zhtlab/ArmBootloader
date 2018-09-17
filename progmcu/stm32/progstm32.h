/*
 * Copyright (c) 2018 zhtlab
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef	_PROGSTM32_H_
#define	_PROGSTM32_H_

/*
 * progstm32.h:  progstm32 header
 *
 */


struct _stStm32ParamGet {
  int		version;
  int		cmd;
};
struct _stStm32ParamGetId {
  int		versionBootloader;
};


#define	PROGSTM32_ERASE_SECTOR_NUM_MASK		0xffff
#define	PROGSTM32_ERASE_SECTOR_BYTES_SHIFT	11
#define	PROGSTM32_ERASE_SECTOR_BYTES		(1 << (PROGSTM32_ERASE_SECTOR_BYTES_SHIFT))


enum STM32_CMD {
  STM32_CMD_SYNC			= 0x7f,
  STM32_CMD_GET				= 0x00,
  STM32_CMD_GET_VERSION			= 0x01,
  STM32_CMD_GET_ID			= 0x02,
  STM32_CMD_READ_MEMORY			= 0x11,
  STM32_CMD_GO				= 0x21,
  STM32_CMD_WRITE_MEMORY		= 0x31,
  /* ERASE and EXTENDED_ERASE are exclusive.  NOT existed both */
  STM32_CMD_ERASE			= 0x43,
  STM32_CMD_EXTENDED_ERASE		= 0x44,
  STM32_CMD_WRITE_PROTECT		= 0x63,
  STM32_CMD_WRITE_UNPROTECT		= 0x73,
  STM32_CMD_READOUT_PROTECT		= 0x82,
  STM32_CMD_READOUT_UNPROTECT		= 0x92,

  STM32_CMD_WRITE_MEMORY_NO_STRETCH	= 0x32,
  STM32_CMD_ERASE_NO_STRETCH		= 0x45,
  STM32_CMD_WRITE_PROTECT_NO_STRETCH	= 0x64,
  STM32_CMD_WRITE_UNPROTECT_NO_STRETCH	= 0x74,
  STM32_CMD_READOUT_PROTECT_NO_STRETCH	= 0x83,
  STM32_CMD_READOUT_UNPROTECT_NO_STRETCH	= 0x93,

  /* extensions */
  STM32_CMD_CHECK_BLANK			= 0x0d,
  STM32_CMD_GET_INFO			= 0x0e,
  STM32_CMD_GET_CRC			= 0x0f,

  STM32_ACK				= 0x79,
  STM32_NACK				= 0x1f,
  STM32_BUSY				= 0x76,
  
};






#if 1
/* STM32 command */
#define	PROGSTM32_COM_GETSTATUS		0x70
#define	PROGSTM32_COM_RESET		0x00
#define	PROGSTM32_COM_SETOSCFREQ	0x90
#define	PROGSTM32_COM_ERASECHIP		0x20
#define	PROGSTM32_COM_ERASEBLOCK	0x22
#define		PROGSTM32_ADDRESS_LEN		6
#define	PROGSTM32_COM_PROGRAMMING	0x40
#define	PROGSTM32_COM_VERIFY		0x13
#define	PROGSTM32_COM_CHECKBLOCKBLANK	0x32
#define	PROGSTM32_COM_GETSIGNATURE	0xc0
#define	PROGSTM32_COM_GETVERSION	0xc5
#define	PROGSTM32_COM_GETCHECKSUM	0xb0
#define	PROGSTM32_COM_SETSECURITY	0xa0
#define		PROGSTM32_LEN_SECURITYDATA	2
#define		PROGSTM32_DATA_SECURITY_BOT	0x03

/* STM32 status */
#define	PROGSTM32_STA_ERROR_COMMAND	0x04
#define	PROGSTM32_STA_ERROR_PARAMETER	0x05
#define	PROGSTM32_STA_ACK		0x06
#define	PROGSTM32_STA_ERROR_CHECKSUM	0x07
#define	PROGSTM32_STA_ERROR_VERIFY	0x0f
#define	PROGSTM32_STA_ERROR_PROTECT	0x10
#define	PROGSTM32_STA_NACK		0x15
#define	PROGSTM32_STA_ERROR_ERASE	0x1a
#define	PROGSTM32_STA_ERROR_VERIFYBLANK	0x1b
#define	PROGSTM32_STA_ERROR_WRITE	0x1c
#define	PROGSTM32_STA_ERROR_READ	0x20
#define	PROGSTM32_STA_BUSY		0xff

#define	PROGSTM32_LEN_HEADER		2
#define	PROGSTM32_LEN_FOOTER		2
#define	PROGSTM32_LEN_LENGTH		1
#define	PROGSTM32_LEN_COMMAND		1
#define	PROGSTM32_LEN_LENGTHCOMMAND	((PROGSTM32_LEN_COMMAND) + (PROGSTM32_LEN_COMMAND))
#define	PROGSTM32_LEN_STATUSCOM		1
#define	PROGSTM32_LEN_STATUSDATA	2
#define	PROGSTM32_LEN_DATAMAX		256
#define	PROGSTM32_LEN_FRAMEMAX		((PROGSTM32_LEN_HEADER) + (PROGSTM32_LEN_DATAMAX) + (PROGSTM32_LEN_FOOTER))

#define	PROGSTM32_LEN_COMMANDFRAME	((PROGSTM32_LEN_HEADER) + (PROGSTM32_LEN_COMMAND) + (PROGSTM32_LEN_FOOTER))
#define	PROGSTM32_LEN_DATAFRAME		((PROGSTM32_LEN_HEADER) + (PROGSTM32_LEN_DATAMAX) + (PROGSTM32_LEN_FOOTER))
#define	PROGSTM32_LEN_STATUSCOMFRAME	((PROGSTM32_LEN_HEADER) + (PROGSTM32_LEN_STATUSCOM) + (PROGSTM32_LEN_FOOTER))
#define	PROGSTM32_LEN_STATUSDATAFRAME	((PROGSTM32_LEN_HEADER) + (PROGSTM32_LEN_STATUSDATA) + (PROGSTM32_LEN_FOOTER))
#endif



#if 0

#define	PROGSTM32_FRAME_DATAMAXBYTES	256
#define	PROGSTM32_FRAME_TXMAXBYTES	(2 + 256 + 2)
#define	PROGSTM32_FRAME_RXMAXBYTES	(2 +   2 + 2)
#endif
#define	PROGSTM32_FRAME_POS_HEADER	0
#define	PROGSTM32_FRAME_POS_LENGTH	1
#define	PROGSTM32_FRAME_SOH		0x01
#define	PROGSTM32_FRAME_STX		0x02
#define	PROGSTM32_FRAME_ETB		0x17
#define	PROGSTM32_FRAME_ETX		0x03


#define	PROGSTM32_TIME_tME		(32*1000000)
#define	PROGSTM32_TIME_tPROG		(100)
#define	PROGSTM32_TIME_tERASE32KB	(800*1000)
#define	PROGSTM32_TIME_tERASE128KB	(2400*1000)
#define	PROGSTM32_TIME_tERASE256KB	(4000*1000)


#ifdef	_PROGSTM32_C_
struct _stProgstm32Unit {
  int			unit;
  int			up;
#define	PROGSTM32_NUMOFCALLBACK		2
  int			(*pCb[PROGSTM32_NUMOFCALLBACK])(int unit,
					int cb, void *ptr, int size);

  int			modeSerial;
  /* SERIALMODE_XXX are defines in prog.h */


  /* reset time  micro sec */
  /*int			tReset;*/

  int			bootloaderVer;
  int			fCmdGet: 1;
  int			fCmdGetVersion: 1;
  int			fCmdGetId: 1;
  int			fCmdReadMemory: 1;
  int			fCmdWriteMemory: 1;
  int			fCmdGo: 1;
  int			fCmdErase: 1;
  int			fCmdExtendedErase: 1;
  int			fCmdWriteProtect: 1;
  int			fCmdWriteUnprotect: 1;
  int			fCmdReadoutProtect: 1;
  int			fCmdReadoutUnprotect: 1;
  int			fCmdWriteMemoryNoStreach: 1;
  int			fCmdExtendedEraseNoStratch: 1;
  int			fCmdWriteProtectNoStratch: 1;
  int			fCmdWriteUnprotectNoStratch: 1;
  int			fCmdReadoutProtectNoStratch: 1;
  int			fCmdReadoutUnprotectNoStratch: 1;

  int			fCmdExtCheckBlank: 1;
  int			fCmdExtGetInfo: 1;
  int			fCmdExtGetCrc: 1;

  int			chipID;
  

  

};

struct _stProgstm32 {
  int			up;
#define	PROGSTM32_NUMOFUNITS		1
  struct _stProgstm32Unit	*sc[PROGSTM32_NUMOFUNITS];
};


struct _stProgstm32Status {
  int		stat;
  int		sub;
};
#endif


/*******************************************************************
 * function macro
 */


/*******************************************************************
 * prototype
 */
int	Progstm32Api(int unit, int req, void *ptr, int size);


/* internal prototype  (for static funciton) */
#ifdef	_PROGSTM32_C_
static int	Progstm32Init(void);
static int	Progstm32UnInit(void);
static int	Progstm32InitUnit(void);
static int	Progstm32UnInitUnit(int unit);
static void	Progstm32Loop(void);

#if 0
static void *	Progstm32Malloc(int size);
static void	Progstm32Free(void *ptr);
#endif

static int	Progstm32SetCallback(int unit, void *ptr);
static int	Progstm32ResetCallback(int unit, void *ptr);
static int	Progstm32IsCallback(int unit, void *ptr);



static int	Progstm32ConnectDevice(int unit);
static int	Progstm32DisconnectDevice(int unit);
/*static int	Progstm32SendGetStatus(int unit);*/
static int	Progstm32Reset(int unit);
static int	Progstm32EraseChip(int unit);
static int	Progstm32EraseBlock(int unit, struct _stProgData *ptr, int size);
static int	Progstm32Program(int unit, struct _stProgData *ptr, int size);
static int	Progstm32Read(int unit, struct _stProgData *ptr, int size);
static int	Progstm32Verify(int unit, struct _stProgData *ptr, int size);
static int	Progstm32CheckBlockBlank(int unit, struct _stProgData *ptr, int size);
static int	Progstm32GetSignature(int unit, void *ptr, int size);
static int	Progstm32GetInfo(int unit, void *ptr, int size);
static int	Progstm32GetAddrInfo(int unit, void *ptr, int size);
static int	Progstm32GetCrc(int unit, void *ptr, int size);
static int	Progstm32CheckBlank(int unit, void *ptr, int size);
static int	Progstm32GetVersion(int unit, void *ptr, int size);
static int	Progstm32GetCheckSum(int unit, struct _stProgData *ptr, int size);
static int	Progstm32SetSecurity(int unit, unsigned char *ptr, int size);
static int	Progstm32SetSerialMode(int unit, int *pMode, int size);



static int	Progstm32Sync(int unit);
static int	Progstm32CmdGet(int unit, uint8_t *ptr, int size);
static int	Progstm32CmdGetVersion(int unit, uint8_t *ptr, int size);
static int	Progstm32CmdGetId(int unit, uint8_t *ptr, int size);
static int	Progstm32CmdWriteMemory(int unit, struct _stProgData *ptr, int size);
static int	Progstm32CmdReadMemory(int unit, struct _stProgData *ptr, int size);
static int	Progstm32CmdGo(int unit, struct _stProgData *ptr, int size);

static int	Progstm32CmdExtendedErase(int unit, struct _stProgData *ptr, int size);
static int	Progstm32CmdGetInfo(int unit, uint8_t *ptr, int size);
/*static int	Progstm32CmdGetInfoVersion(int unit, uint8_t *ptr, int size);*/
static int	Progstm32CmdGetCrc(int unit, struct _stProgData *ptr, int size);
static int	Progstm32CmdCheckBlank(int unit, struct _stProgData *ptr, int size);





static int	Progstm32SendData(int unit, uint8_t *ptr, int size);
static int	Progstm32SendDataWaitAck(int unit, uint8_t *ptr, int size);
static int	Progstm32SendDataWaitAckTout(int unit, uint8_t *ptr, int size, time_t tout);
/*static int	Progstm32SendByte(int unit, unsigned char byte);*/
/*static int	Progstm32RecvData(int unit, uint8_t *ptr, int size, unsigned int tout);*/
static int	Progstm32RecvData(int unit, uint8_t *ptr, int size);
static int	Progstm32CalcCheckSum(unsigned char *ptr, int size);

static void	Progstm32StoreSupportedCommand(struct _stProgstm32Unit *psc,
					       uint8_t *buf, int size);

#endif


#endif	/* __PROGSTM32_H__ */

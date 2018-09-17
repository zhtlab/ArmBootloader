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

#ifndef	_MAIN_H_
#define	_MAIN_H_



/*** parameter list for the external command */
#define	MAIN_EXTCMD_PARAM_ENTER_THIS		"--enter_this"

#define	MAIN_EXTCMD_PARAM_BEFORE_CONNECT	"--before_connect"
#define	MAIN_EXTCMD_PARAM_AFTER_CONNECT		"--after_connect"



#define	MAIN_EXTCMD_PARAM_BEFORE_DISCONNECT	"--before_disconnect"
#define	MAIN_EXTCMD_PARAM_AFTER_DISCONNECT	"--after_disconnect"

#define	MAIN_EXTCMD_PARAM_BEFORE_RESET		"--before_reset"

#define	MAIN_EXTCMD_PARAM_EXIT_THIS	"--exit_this"



#define	MAIN_GETOPT_MAXLEN	128


struct _stAddr {
  uint32_t		start;
  uint32_t		end;
  uint32_t		size;
};

struct _stHidInfo	{
  int			vid;
  int			pid;
  int			epIn;
  int			epOut;
  int			repDataIn;
  int			repDataOut;
  int			repCommandIn;
  int			repCommandOut;
};


struct _stMain {
  int			debug;
#define	DEBUG_SHOW_TOP_PROCEEDING		(1<< 8)
#define	DEBUG_SHOW_DEVICE_PROCEEDING		(1<<16)
#define	DEBUG_SHOW_PACKET			(1<<31)
  struct _stHidInfo	hidInfo;


  int			fTask;
#define	TASK_ERASE		(1 << 0)
#define	TASK_PROGRAM		(1 << 1)
#define	TASK_VERIFY		(1 << 2)
#define	TASK_BLANK		(1 << 3)
#define	TASK_VERSION		(1 << 4)
#define	TASK_SIGNATURE		(1 << 5)
#define	TASK_SECURITY		(1 << 6)
#define	TASK_GETCHECKSUM	(1 << 7)
#define	TASK_RESET		(1 << 8)
#define	TASK_READ		(1 << 9)
#define	TASK_TEST1SEC		(1 << 16)

  uint32_t		addrStart;
  uint32_t		addrEnd;

  int			modelType;
#define	MAIN_MODELTYPE_NONE		(0)
#define	MAIN_MODELTYPE_KINETIS		(1)
#define	MAIN_MODELTYPE_LPC		(2)
#define	MAIN_MODELTYPE_STM32		(3)
  
  int			updateArea;
#define	MAIN_UPDATEAREA_NONE		(1<<0)
#define	MAIN_UPDATEAREA_BOOTLOADER	(1<<1)
#define	MAIN_UPDATEAREA_USER		(1<<2)
#define	MAIN_UPDATEAREA_FIRMWARE	(1<<3)

  int			devInterface;
#define	MAIN_DEVINTERFACE_NONE		0
#define	MAIN_DEVINTERFACE_UART		1
#define	MAIN_DEVINTERFACE_I2C		2
#define	MAIN_DEVINTERFACE_SPI		3
#define	MAIN_DEVINTERFACE_USBHID	4
  
  struct _stAddr	addrSet;        /* address length info by param */
  struct _stAddr	addrFile;       /* address length info in file */

  int			fUserAreaAccess;
  int			fShowHeader;
  int			fSkipHeader;
  int			fShowInfo;
  int			fForce;
  int			fDumpMemories;
  int			fAllArea;
  int			fEnterUpdate;

#define	MAIN_LEN_FNAME	128
#define	MAIN_MAX_FILES	2
  char			fname[MAIN_MAX_FILES][MAIN_LEN_FNAME];
  int			numFname;
#define	MAIN_HEADER_MAXLEN	1024
  char			strFileHeader[MAIN_HEADER_MAXLEN];
  int			cntFileHeader;
#define	MAIN_HEADER_LINE_MAXLEN	384
  char			strFileHeaderType[MAIN_HEADER_LINE_MAXLEN];

  char			strFileHeaderProduct[MAIN_HEADER_LINE_MAXLEN];
  char			strFileHeaderFile[MAIN_HEADER_LINE_MAXLEN];
  char			strFileHeaderFlashStart[MAIN_HEADER_LINE_MAXLEN];
  char			strFileHeaderVersion[MAIN_HEADER_LINE_MAXLEN];
  int			numFileHeaderTarget;
#define	MAIN_FILE_TARGET_UNKNOWN	0
#define	MAIN_FILE_TARGET_BOOTLOADER	1
#define	MAIN_FILE_TARGET_FIRMWARE	2
#define	MAIN_FILE_TARGET_USERAREA	3
#define	MAIN_FILE_TARGET_ALLAREA	4
  

  char			strBootloaderProduct[MAIN_HEADER_LINE_MAXLEN];
  char			strBootloaderVersion[MAIN_HEADER_LINE_MAXLEN];
  char			strFirmwareVersion[MAIN_HEADER_LINE_MAXLEN];
  char			strUserareaVersion[MAIN_HEADER_LINE_MAXLEN];

  char			uartDevName[MAIN_LEN_FNAME];
  char			spiDevName[MAIN_LEN_FNAME];
  char			i2cDevName[MAIN_LEN_FNAME];
  int			i2cDevAddr;

  uint8_t		strExtCmd[MAIN_LEN_FNAME];
};


#ifdef	_MAIN_C_
static int		MainOpenReadFile(void);
static int		MainOpenWriteFile(struct _stProgData *pData);
static int		MainExecuteExternalCommand(int unit, char *param, char *pKey, char *pValue);
static void		Help(void);
static int		MainExecuteFlashOperation(int s, struct _stProgData *pAddr);


void			MainShowData(char *str, uint8_t *ptr, int size);

static void		MainParseFileHeader(char *str);
static int		MainCheckFileInfoUptodate(void);
static int		MainGetBurnedFirmInfo(int s);

#endif


#endif

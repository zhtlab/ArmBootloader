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

#define	_MAIN_C_

#define	MAIN_SHOWCALLBACK_REQ	0
#define	MAIN_SHOWCALLBACK_DATA	0

#define	MAIN_VERSION		"0.00.01.201809170"

#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdint.h>
#include	<getopt.h>
#include	<errno.h>

#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/uio.h>
#include	<unistd.h>
#include        <sys/ioctl.h>
#include	<termios.h>

#include	"device.h"

#include	"prog.h"

#include	"main.h"


struct _stMain		sys;


#define	ERRNO_SUCCESS		0
#define	ERRNO_ERASE		1
#define	ERRNO_PROGRAM		2
#define	ERRNO_VERIFY		3
#define	ERRNO_SECURITY		4
#define	ERRNO_GETCHECKSUM	5
#define	ERRNO_READ		6
#define	ERRNO_FILE		10
#define	ERRNO_NOTSUPPORTED	11
#define	ERRNO_TIMEOUT		12
#define	ERRNO_DEVNOTFOUND	13
#define	ERRNO_PARAMETER		14


#define	LEN_FLASHMAXSIZE	(1024*1024)

#define	MAIN_TIME_RESET		(500 * 1000)
#define	MAIN_TIME_SETBAUD	(500 * 1000)

/*#define	NAME_UARTDEV		"/dev/ttyACM0"*/
#define	NAME_SPIDEV		"/dev/spi0"
#define	NAME_I2CDEV		"/dev/i2c0"
#define	NAME_UARTDEV		"/dev/serial/by-id/usb-STMicroelectronics_STM32_STLink_0676FF525750877267035147-if02"

#define	GetAddressBySignature(buf) \
	((buf[4] & 0x7f) | ((buf[5] & 0x7f) << 7) | ((buf[6] & 0x7f) << 14))


int		DeviceInit(int unit);
void		ShowSignature(unsigned char *buf);
void		ShowVersion(unsigned char *buf);
int		GetCpuValueByText(unsigned char *val, unsigned char *str);

uint8_t		dataProg[LEN_FLASHMAXSIZE];
uint8_t		dataRead[LEN_FLASHMAXSIZE];
int		sizeProg;
int		sizeRead;
/*int			size = 0;*/

int
cb(int unit, int req, void *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;

#if	MAIN_SHOWCALLBACK_REQ
  printf("%s:%d: cb req:%x\n", __FILE__, __LINE__, req);
#endif  
  

  switch(req) {

    /*** GPIO *****************************************/
  case	PROG_CB_GPIO_INIT:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: init port\n", __FILE__, __LINE__);
#endif
    result = DeviceInitPort(unit);
    break;
  case	PROG_CB_GPIO_UNINIT:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: uninit port\n", __FILE__, __LINE__);
#endif
    result = DeviceUninitPort(unit);
    break;

#if 0
  case	PROG_CB_GPIO_SETRESETPORT:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: set reset %d\n", __FILE__, __LINE__, *(int *)ptr);
#endif
    break;
  case	PROG_CB_GPIO_SETFLMD0PORT:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: set flmd0 %d\n", __FILE__, __LINE__, *(int *)ptr);
#endif
    break;
#endif

    /*** UART ****************************************/
    /*
     *
     */
  case	PROG_CB_SERIAL_INIT:
    result = DeviceInitSerial(unit, sys.uartDevName, *(int *)ptr);
    break;
  case	PROG_CB_SERIAL_UNINIT:
    result = DeviceUninitSerial(unit, *(int *)ptr);
    break;

  case	PROG_CB_SERIAL_SETBAUD:
    if(ptr) {
      DeviceWaitMicroseconds(MAIN_TIME_SETBAUD);
      result = DeviceInitSetBaud(*(int *)ptr);
    }
    break;

  case	PROG_CB_SERIAL_SETTERMINAL:
    if(ptr) {
      result = DeviceInitSetTerminal(*(int *)ptr);
    }
    break;

  case	PROG_CB_SERIAL_SENDDATA:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: cb  send data %x %d\n", __FILE__, __LINE__, (int)ptr, size);
#endif
#if	MAIN_SHOWCALLBACK_DATA || 1
    if(sys.debug & DEBUG_SHOW_PACKET) {
      MainShowData("tx", ptr, size);
    }
#endif
    result = DeviceSendData(unit, ptr, size);
    break;

  case	PROG_CB_SERIAL_RECVDATA:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: cb  recv data %x %d\n", __FILE__, __LINE__, (int)ptr, size);
#endif
    result = DeviceRecvData(unit, ptr, size, 100 * 1000);

#if	MAIN_SHOWCALLBACK_DATA || 1
    if(sys.debug & DEBUG_SHOW_PACKET) {
      if(result) MainShowData("rx", ptr, result);
    }
#endif
    break;

    /*** I2C  *****************************************/
    /*** SPI  *****************************************/
    /*** USBHID ***************************************/

  case	PROG_CB_WAIT_MICROSECONDS:
#if	MAIN_SHOWCALLBACK_REQ
    printf("%s:%d: wait micro sec %d\n", __FILE__, __LINE__,
	   *(unsigned int *)ptr);
#endif
    DeviceWaitMicroseconds(*(unsigned int *)ptr);
    break;


  default:
    result = PROG_ERRNO_INVALIDARG;
    break;
  }

    return result;
  }


static int
MainExecuteExternalCommand(int unit, char *pParam, char *pKey, char *pValue)
{
  int			result = -1;
  char			str[MAIN_LEN_FNAME*2];
  char			nil[]="";

  if(sys.strExtCmd[0] && pParam) {
    if(!pKey)   pKey = nil;
    if(!pValue) pValue = nil;

    snprintf(str, sizeof(str), "%s %d %s %s %s", sys.strExtCmd, unit, pParam, pKey, pValue);
    str[MAIN_LEN_FNAME*2-1] = '\0';

    result = system(str);
  }

  return result;
}


#define GETOPT_VALUE_FLAG_INTEGER		0
#define GETOPT_VALUE_FLAG_HEX			1
#define GETOPT_VALUE_FLAG_HEXINT		2
#define GETOPT_VALUE_FLAG_STRINGS		3
static struct option longopts[] = {
#if 0
  {"intelhex", no_argument, &sys.fIntelHex, 1},
  {"binary", no_argument, &sys.fIntelHex, 0},
  {"updatebootarea", no_argument, &sys.fUpdateBootArea, 1},
  

  {"startaddr", required_argument, (int*)&sys.addrSet.start, GETOPT_VALUE_FLAG_HEXINT},
  {"endaddr", required_argument, (int*)&sys.addrSet.end, GETOPT_VALUE_FLAG_HEXINT},



  {"nousbreset", no_argument, &sys.usbreset, 0},
  {"usbreset", no_argument, &sys.usbreset, 1},
  {"vidupdate", required_argument, &sys.usbVidUpdate, GETOPT_VALUE_FLAG_HEXINT},
  {"pidupdate", required_argument, &sys.usbPidUpdate, GETOPT_VALUE_FLAG_HEXINT},
#endif

  {"allarea", no_argument, &sys.fAllArea, 1},

  {"userarea",  no_argument, &sys.fUserAreaAccess, 1},


  {"start-addr", required_argument, (int*)&sys.addrSet.start, GETOPT_VALUE_FLAG_HEXINT},
  {"end-addr",   required_argument, (int*)&sys.addrSet.end,   GETOPT_VALUE_FLAG_HEXINT},
  {"total-size", required_argument, (int*)&sys.addrSet.size,  GETOPT_VALUE_FLAG_HEXINT},

  {"force", no_argument, &sys.fForce, 1},


  /*{"disp-devices", no_argument, &sys.fDispDevices, 1},*/

  {"dump", no_argument, &sys.fDumpMemories, 1},

  {"stm32", no_argument, &sys.modelType, MAIN_MODELTYPE_STM32},
  {"kinetis", no_argument, &sys.modelType, MAIN_MODELTYPE_KINETIS},
  {"lpc", no_argument, &sys.modelType, MAIN_MODELTYPE_LPC},

  {"uart", no_argument, &sys.devInterface, MAIN_DEVINTERFACE_UART},
  {"i2c",  no_argument, &sys.devInterface, MAIN_DEVINTERFACE_I2C},
  {"spi",  no_argument, &sys.devInterface, MAIN_DEVINTERFACE_SPI},
  {"usb",  no_argument, &sys.devInterface, MAIN_DEVINTERFACE_USBHID},
  
  {"uartdev", required_argument, (int *)sys.uartDevName, GETOPT_VALUE_FLAG_STRINGS},
  {"spidev", required_argument, (int *)sys.uartDevName, GETOPT_VALUE_FLAG_STRINGS},
  {"i2cdev", required_argument, (int *)sys.i2cDevName, GETOPT_VALUE_FLAG_STRINGS},
  {"i2caddr", required_argument, &sys.i2cDevAddr, GETOPT_VALUE_FLAG_HEXINT},
  {"vid", required_argument, &sys.hidInfo.vid, GETOPT_VALUE_FLAG_HEXINT},
  {"pid", required_argument, &sys.hidInfo.pid, GETOPT_VALUE_FLAG_HEXINT},

  
  {"enter-update", no_argument, &sys.fEnterUpdate, 1},


  /* terminator */
  {NULL, 0, NULL, 0},
};
void
ParseOptions(int argc, char *argv[])
{
  int			c;
  int			index;

  while ((c = getopt_long(argc, argv, "epbBrvRVNkKD:c:f:hq", longopts, &index)) != -1) {
    switch (c) {
    case	0:
      if(longopts[index].has_arg == no_argument) {
	*longopts[index].flag = longopts[index].val;
      } else {
	switch(longopts[index].val) {
	case	GETOPT_VALUE_FLAG_INTEGER:
	  *longopts[index].flag = atoi(optarg);
	  break;
	case	GETOPT_VALUE_FLAG_HEXINT:
	  if(optarg[0] == '0' && (optarg[1] | 0x20) == 'x') {
	    *longopts[index].flag = strtoul(optarg, NULL, 16);
	  } else {
	    *longopts[index].flag = atoi(optarg);
	  }
	  break;
	case	GETOPT_VALUE_FLAG_STRINGS:
	  strncpy((char *)longopts[index].flag, optarg, (MAIN_GETOPT_MAXLEN)-1);
	  break;
	}
      }
      break;

    case	'f':
      strncpy(sys.fname[sys.numFname], optarg, MAIN_LEN_FNAME);
      sys.fname[sys.numFname][MAIN_LEN_FNAME - 1] = '\0';
      sys.numFname++;
      break;
    case	'e':
      sys.fTask |= TASK_ERASE;
      break;
    case	'p':
      sys.fTask |= TASK_PROGRAM;
      sys.fTask |= TASK_VERIFY;
      sys.fTask |= TASK_BLANK;
      sys.fTask &= ~TASK_READ;
      break;
    case	'b':
      sys.fTask |= TASK_BLANK;
      break;
    case	'B':
      sys.fTask &= ~TASK_BLANK;
      break;
    case	'r':
      sys.fTask |=  TASK_READ;
      sys.fTask &= ~TASK_PROGRAM;
      break;
    case	'v':
      sys.fTask |= TASK_VERIFY;
      break;
    case	'R':
      sys.fTask |= TASK_RESET;
      break;
    case	'V':
      sys.fTask |= TASK_VERSION;
      break;
#if 0
    case	'N':
      sys.fTask |= TASK_VERSIONFILE;
      break;
#endif
    case	'K':
      sys.fShowHeader = 1;	/* do not put break; */
    case	'k':
      sys.fSkipHeader = 1;
      break;

    case	'D':
      sys.debug = strtoul(optarg, NULL, 16);
      break;
    case	'q':
      sys.fShowInfo = 0;
      break;
    case	'h':
    default:
      Help();
      exit(0);
    }
  }
  
  return;
}


static void
Help(void)
{
  printf("progstm320 [-epvbg] [-f name] [-s code] [-qVStkKh]\n");
  printf("  main operation parameters:\n");
  printf("    -e        erase\n");
  printf("    -p        program\n");
  printf("    -v        verify\n");
  printf("    -b        blank check\n");
  printf("    -B        no blank check\n");
  printf("    -g        get check sum\n");
  printf("    -R        reset\n");

  printf("    -k        skip header of program file\n");
  printf("    -f name   program file name\n");
  printf("\n");

  printf("  setting parameters:\n");
  printf("    --uartdev dev    uart device file  (default: %s)\n", NAME_UARTDEV);
  printf("    --spidev dev     spi  device file  (default: %s)\n", NAME_SPIDEV);
  printf("    --i2cdev dev     i2c  device file  (default: %s)\n", NAME_I2CDEV);

  
  printf("    -D debug  set the debug levelity\n");
  printf("                (1<< 8) show the top proceeding\n");
  printf("                (1<<16) show the device proceeding\n");
  printf("                (1<<31) show all packet\n");
  printf("    -s code   set the security\n");
  printf("                xxff -> enable prog/erase\n");
  printf("                xxe8 -> disable prog/erase\n");
  printf("                  xx is value of complement of lower byte\n");

  printf("    -V        show the version of the target chip\n");
  printf("    -S        show the signature of the target chip\n");
  printf("    -t        wait 1 second and exit this program  (for to set -T num)\n");
  printf("    -q        quiet\n");
  printf("    -K        show header of program file\n");
  printf("    -h        show this information\n");

  printf("  target device selection:\n");
  printf("    --stm32             set the STM32 mode\n");
  printf("    --lpc               set the LPC mode\n");
  printf("    --kinetis           set the KINETIS mode\n");
  printf("\n");

  printf("  interface selection:\n");
  printf("    --uart              set the UART mode (by default)\n");
  printf("    --i2c               set the I2C mode\n");
  printf("    --spi               set the SPI mode\n");
  printf("    --usb               set the USB mode\n");
  printf("\n");

  printf("    --enter-update      enter update mode\n");

  printf("    --start-addr        set the start address\n");
  printf("    --end-addr          set the end address\n");
  printf("    --allarea           all area access\n");
  printf("    --unlock [key]      set the unlock key\n");

  printf("  progmcu --stm --uart -e --allarea\n");

  printf("version: %s\n", MAIN_VERSION);

  return;
}





static int
MainExecuteFlashOperation(int s, struct _stProgData *pData)
{
  int			result = ERRNO_SUCCESS, re;


  /*** erase *************************************/
  /* erase region (start--end)  lock
   * erase BOOTLOADER area      lock
   * erase USER area            lock
   * erase FIRMWARE area
   */
  if(sys.fTask & TASK_ERASE) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Erase\n", __FILE__);
    if(sys.fAllArea) {
      fprintf(stderr, "    Erase  chip erase\n");
      re = ProgApi(s, PROG_API_ERASECHIP, PROG_NULL, 0);
    } else {
      struct _stProgData	erase;
      erase = *pData;
      erase.start =  pData->start                             & ~(pData->sizeEraseSector-1);
      erase.end   = (pData->end + (pData->sizeEraseSector-1)) & ~(pData->sizeEraseSector-1);
      erase.size  = erase.end - erase.start;
      fprintf(stderr, "    Erase  start:%04x, end:%04x, size:%04x\n", erase.start, erase.end, erase.size);
      re = ProgApi(s, PROG_API_ERASEBLOCK, pData, sizeof(struct _stProgData));
    }
    if(re < 0) {
      result = ERRNO_ERASE;
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
      }
      goto end;
    }
  }


  /*** blank check *******************************/
  if(sys.fTask & (/*TASK_PROGRAM |*/ TASK_BLANK)) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Blank\n", __FILE__);
    pData->ptr   = dataRead;
    fprintf(stderr, "    Blankcheck\n");
    if((re = ProgApi(s, PROG_API_CHECKBLOCKBLANK, pData, sizeof(struct _stProgData))) < PROG_ERRNO_SUCCESS) {
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
	goto end;
      }
      if(sys.fShowInfo) {
	fprintf(stderr, "the target chip is not blank.\n");
      }
      result = ERRNO_ERASE;
      goto end;
    }
  }


  /*** program ***********************************/
  if((sys.fTask & TASK_PROGRAM) && pData->size>0) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Program\n", __FILE__);
    pData->ptr   = dataProg;
    pData->size  = sizeProg;

    fprintf(stderr, "    Program  start:%04x, end:%04x, size:%04x\n",
	    pData->start, pData->end, pData->size);
    if((re = ProgApi(s, PROG_API_PROGRAM, pData, sizeof(struct _stProgData)))) {
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
	goto end;
      }
      if(sys.fShowInfo) {
	fprintf(stderr, "program fail\n");
      }
      result = ERRNO_PROGRAM;
      goto end;
    }
  }


  /*** read **************************************/
  if(sys.fTask & TASK_READ) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Read\n", __FILE__);
    fprintf(stderr, "    read: start:%04x, end:%04x, size:%04x\n", pData->start, pData->end, pData->size);
    pData->ptr = dataRead;
    if((re = ProgApi(s, PROG_API_READ, pData, sizeof(struct _stProgData)))) {
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
	goto end;
      }
      if(sys.fShowInfo) {
	fprintf(stderr, "read fail\n");
      }

      result = ERRNO_READ;
      goto end;
    }
    MainOpenWriteFile(pData);
  }


  /*** verify or check crc ***********************/
  if(sys.fTask & TASK_VERIFY) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Verify\n", __FILE__);

    fprintf(stderr, "    Verify");
    fflush(stderr);

    pData->ptr   = dataProg;

    re = ProgApi(s, PROG_API_VERIFY, pData, sizeof(struct _stProgData));
    if(re) {
      if(sys.fShowInfo) {
	printf(" failed\n");
      }
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
	goto end;
      }
      result = ERRNO_VERIFY;
      goto end;
    }
    if(sys.fShowInfo) {
      printf(" ok\n");
    }
  }


#if 0
  /*** get check sum *********************************/
  /* get the check sum */
  if(((sys.fTask & TASK_PROGRAM) && size) ||
     sys.fTask & TASK_GETCHECKSUM) {

    if((re = ProgApi(s, PROG_API_GETCHECKSUM, pData, sizeof(struct _stProgData)))) {
      if(re == PROG_ERRNO_TIMEOUT) {
	result = ERRNO_TIMEOUT;
	goto end;
      }
      if(sys.fShowInfo) {
	printf("get check sum fail\n");
      }
      result = ERRNO_GETCHECKSUM;
      goto end;
    }
    if(sys.fShowInfo) {
      printf("check sum 0x%04x at from 0x%x to 0x%x\n",
	     addr.sum, addr.start, addr.end);
    }
  }
#endif


  /*** set the security **************************/
#if 0
  if(sys.fTask & TASK_SECURITY) {
    char	buf[PROGSTM32_LEN_SECURITYDATA];
    unsigned char	sec, secX;

    sec  =  valSecurity       & 0xff;
    secX = (valSecurity >> 8) & 0xff;
    if(sec != (~secX & 0xff)) {
      if(sys.fShowInfo) {
	printf("security value is wrong [%02x%02x]\n", secX, sec);
	printf("                        [%02x%02x]\n", ~sec & 0xff, sec);
      }
      result = ERRNO_SECURITY;
      goto end;
    }	
    if((sec & PROGSTM32_DATA_SECURITY_BASE) != PROGSTM32_DATA_SECURITY_BASE) {
      if(sys.fShowInfo) {
	printf("security base is wrong [%02x]\n", sec);
	printf("               base is [%02x]\n", PROGSTM32_DATA_SECURITY_BASE);
      }
      result = ERRNO_SECURITY;
      goto end;
    }

    /*buf[0] = PROGSTM32_DATA_SECURITY_ENABLEALL;*/
    buf[0] = sec;
    buf[1] = PROGSTM32_DATA_SECURITY_BOT;

#if SUPPORT_SECURITY
    if(ProgApi(s, PROG_API_SETSECURITY, buf, sizeof(buf))) {
      result = ERRNO_SECURITY;
      goto end;
    }
#else
    if(sys.fShowInfo) {
      printf("security is not supported\n");
    }
    result = ERRNO_NOTSUPPORTED;
    goto end;
#endif
  }
#endif

  /*** reset *************************************/
  if(sys.fTask & TASK_RESET) {
    if(sys.debug & DEBUG_SHOW_TOP_PROCEEDING) fprintf(stderr, "# Debug %s Reset\n", __FILE__);
    if((re = ProgApi(s, PROG_API_RESET, pData, sizeof(struct _stProgData)))) {
    }
    goto end;
  }
  
end:
  return result;
}




int
main(int argc, char *argv[])
{
  int		result = ERRNO_SUCCESS, re;


  /*int		fShowInfo = 1;*/

  /*int		valSecurity;*/

  int		s;
  struct _stProgData	addr;
  struct _stProgData	addrMem, addrData;
#define	ADDR_INVALID		((unsigned int)-1)
  int		mode;

  memset(&sys, 0, sizeof(sys));

  s = -1;
  sys.fShowInfo = 1;

  strcpy(sys.uartDevName, NAME_UARTDEV);
  strcpy(sys.spiDevName,  NAME_SPIDEV);
  strcpy(sys.i2cDevName,  NAME_I2CDEV);

  memset(dataProg, 0xff, sizeof(dataProg));
  memset(dataProg, 0xff, sizeof(dataRead));

  ParseOptions(argc, argv);

  if(sys.modelType == MAIN_MODELTYPE_NONE || sys.devInterface == MAIN_DEVINTERFACE_NONE) {
    if(sys.fShowInfo) {
      fprintf(stderr, "the CPU type and/or the serial mode are not specified\n");
    }
    result = ERRNO_PARAMETER;
    goto end;
  }

  MainExecuteExternalCommand(0, MAIN_EXTCMD_PARAM_BEFORE_CONNECT,
			      PROG_NULL, PROG_NULL);

#if 0
  if(sys.fTask & TASK_TEST1SEC) {
    int		unit = 0;
    DeviceInitPort(unit);
    DeviceWaitMicroseconds(1000 * 1000);
    DeviceUninitPort(unit);

    DeviceWaitMicroseconds(1000 * 1000);

    goto fail;
  }
#endif


  /* device open check */
  if(DeviceOpenCheckSerial(sys.uartDevName) < 0) {
    result = ERRNO_DEVNOTFOUND;
    if(sys.fShowInfo) {
      fprintf(stderr, "can not open the serial device [%s %s].\n", sys.uartDevName, strerror(errno));
    }
    goto fail;
  }


  /* open and read the file */
  if(sys.fTask & TASK_PROGRAM) {
    MainOpenReadFile();
    addrData.ptr = dataProg;
    addrData.size = sizeProg;
    addrData.start = 0;
    addrData.end = addrData.start + addrData.size;
  }

  MainParseFileHeader(sys.strFileHeader);
  /*sys.lenDataProg = size;*/
  
  /* exit, if fTask is not set any bits */
  if(!sys.fTask) goto end;


  /* set the device and the serial mode */
  switch(sys.modelType) {
  case MAIN_MODELTYPE_KINETIS: mode = MODELTYPE_KINETIS; break;
  case MAIN_MODELTYPE_LPC:     mode = MODELTYPE_LPC; break;
  case MAIN_MODELTYPE_STM32:   mode = MODELTYPE_STM32; break;
  }
  ProgApi(s, PROG_API_SETMODELTYPE, &mode, sizeof(mode));

  switch(sys.devInterface) {
  case	MAIN_DEVINTERFACE_UART:    mode = SERIALMODE_UART; break;
  case	MAIN_DEVINTERFACE_I2C:     mode = SERIALMODE_I2C;  break;
  case	MAIN_DEVINTERFACE_SPI:     mode = SERIALMODE_SPI;  break;
  case	MAIN_DEVINTERFACE_USBHID:  mode = SERIALMODE_USB;  break;
  }
  ProgApi(s, PROG_API_SETSERIALMODE, &mode, sizeof(mode));


  if(ProgApi(0, PROG_API_INITIALIZE, PROG_NULL, 0) < 0) {
    fprintf(stderr, "init error %d\n", s);
    exit(1);
  }

  if((s = ProgApi(0, PROG_API_INITIALIZE_UNIT, PROG_NULL, 0)) < 0) {
    fprintf(stderr, "init unit error %d\n", result);
    exit(1);
  }

  ProgApi(s, PROG_API_SETCALLBACK, cb, sizeof(cb));


  /* connect device */
  if(ProgApi(s, PROG_API_CONNECTDEVICE, PROG_NULL, 0) != PROG_ERRNO_SUCCESS) {
    result = ERRNO_DEVNOTFOUND; 
    if(sys.fShowInfo) {
      fprintf(stderr, "device not found [%s].\n", sys.uartDevName);
    }
    goto end;
  }







#if 0
  /* get the signature and the version */
  {
    /*unsigned char	buf[PROGSTM32_LEN_DATAMAX];*/

#if 0
    re = ProgApi(s, PROG_API_GETSIGNATURE, buf, PROGSTM32_LEN_SIGNATURE);
    if(re == PROG_ERRNO_TIMEOUT) {
      result = ERRNO_TIMEOUT;
      goto end;
    }
#endif
#if 0
    for(i = 0; i < 2; i++) {
      re = ProgApi(s, PROG_API_GETSIGNATURE, buf, PROGSTM32_LEN_SIGNATURE);
      if(re == PROG_ERRNO_SUCCESS) {
	break;
      }
    }
#endif
    /*addrSig.end = GetAddressBySignature(buf);*/

#if 0
    if(sys.fTask & TASK_SIGNATURE) {
      ShowSignature(buf);
    }
#endif
}
#endif


  re = MainGetBurnedFirmInfo(s);
  if(re != PROG_ERRNO_SUCCESS) {
    result = ERRNO_TIMEOUT;
    goto end;
  }

  if(sys.fTask & TASK_VERSION) {
    ShowVersion((uint8_t *)sys.strBootloaderVersion);
  }

  if((sys.fTask & TASK_VERSION) || (sys.fTask & TASK_SIGNATURE)) {
    result = ERRNO_SUCCESS;
    goto end;
  }


  /* get the address information of flash rom */
  addrMem.mode = sys.numFileHeaderTarget;
  if(sys.fAllArea) {
    addrMem.mode = MAIN_FILE_TARGET_ALLAREA;
  }
  re = ProgApi(s, PROG_API_GETADDRINFO, &addrMem, sizeof(addrMem));
  if(re != PROG_ERRNO_SUCCESS && !(sys.fTask & TASK_READ)) {
    result = ERRNO_FILE;
    goto end;
  }


  if(sys.fTask & TASK_PROGRAM) {
    /* check the product name btw file and flash rom */
    if(!sys.fForce &&
       !MainCheckFileInfoUptodate()
       ) {
      result = ERRNO_FILE;
      goto end;
    }

    if(sys.fAllArea) {
      addr = addrMem;
    } else {
      addr = addrData;
      addr.start = addrMem.start;
      if(*sys.strFileHeaderFlashStart) addr.start = sys.addrFile.start;
      addr.size  = addrData.size;
      addr.end   = addr.start + addr.size;
      addr.sizeEraseSector   = addrMem.sizeEraseSector;
    }

    printf("# mem len: %x, data size: %x\n", addrMem.size, addrData.size);
    if(addrData.size > addrMem.size) {
      fprintf(stderr, "write date is too big.\n");
      result = ERRNO_FILE;
      goto end;
    }
  }

  if(sys.fTask & TASK_READ) {
    addr = addrMem;
    if(sys.addrSet.start) addr.start = sys.addrSet.start;
    if(sys.addrSet.end) {
      addr.end  = sys.addrSet.end;
      addr.size = addr.end - addr.start;
    }
    if(sys.addrSet.size) {
      addr.end  = addr.start + sys.addrSet.size;
      addr.size = sys.addrSet.size;
    }
    addr.sizeEraseSector   = addrMem.sizeEraseSector;
  }

  printf("# dest addr: %x, end addr: %x, data size:%x\n",
	 addr.start, addr.end, addr.size);

  result = MainExecuteFlashOperation(s, &addr);

  
end:
  if(result == ERRNO_TIMEOUT) {
    if(sys.fShowInfo) {
      fprintf(stderr, "communication timeout\n");
    }
  }

  /* disconnect device */
  ProgApi(s, PROG_API_DISCONNECTDEVICE, PROG_NULL, 0);

  ProgApi(s, PROG_API_RESETCALLBACK, PROG_NULL, 0);

  ProgApi(s, PROG_API_UNINITIALIZE_UNIT, PROG_NULL, 0);

  ProgApi(0, PROG_API_UNINITIALIZE, PROG_NULL, 0);

fail:
  MainExecuteExternalCommand(0, MAIN_EXTCMD_PARAM_BEFORE_CONNECT,
			      PROG_NULL, PROG_NULL);
  return result;
}




static int
MainOpenReadFile(void)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		re;

  if(sys.fname[0][0] != '\0') {
    int		p;
    char	c;

    if((p = open(sys.fname[0], O_RDONLY, 0)) < 0) {
      if(sys.fShowInfo) {
	fprintf(stderr, "file not found [%s].\n", sys.fname[0]);
      }
      result = ERRNO_FILE;
      goto end;
    }
    if(sys.fSkipHeader) {
      while(1) {
	re = read(p, &c, 1);
	if(re < 0) break;
	if(sys.cntFileHeader < sizeof(sys.strFileHeader) && c != 0x1a) {
	  sys.strFileHeader[sys.cntFileHeader++] = c;
	}
	if(sys.fShowHeader && isascii(c)) fputc(c, stdout);
	if(!c) break;
      }
      sys.strFileHeader[sizeof(sys.strFileHeader)-1] = '\0';
    }
    sizeProg = read(p, dataProg, LEN_FLASHMAXSIZE);
    
    close(p);
  }

  result = PROG_ERRNO_SUCCESS;

end:
  return result;
}
static int
MainOpenWriteFile(struct _stProgData *pData)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		re = -1;

  if(sys.fname[0][0] != '\0') {
    int		p;

    if((p = open(sys.fname[0], O_WRONLY | O_CREAT, 0666)) < 0) {
      if(sys.fShowInfo) {
	fprintf(stderr, "%s [%s]\n", strerror(errno), sys.fname[0]);
      }
      result = ERRNO_FILE;
      goto end;
    }

    re = write(p, pData->ptr, pData->size);

    close(p);
  }

  if(re >= 0) result = PROG_ERRNO_SUCCESS;

end:
  return result;
}

#define	DEVICE_NAME_LENGTH		10
void
ShowSignature(unsigned char *buf)
{
  unsigned long		addr;
  char			name[DEVICE_NAME_LENGTH + 1];
  int			i;

  addr = GetAddressBySignature(buf);

  for(i = 0; i < DEVICE_NAME_LENGTH; i++) {
    name[i] = buf[i+7] & 0x7f;
  }
  name[DEVICE_NAME_LENGTH] = '\0';

  printf("vender code:    %02x\n", buf[0] & 0x7f);
  printf("extention code: %02x\n", buf[1] & 0x7f);
  printf("function code:  %02x\n", buf[2] & 0x7f);
  printf("device code:    %02x\n", buf[3] & 0x7f);
  printf("end address:    0x%lx\n", addr);
  printf("device name:    [%s]\n", name);
  printf("security info:  %02x\n", buf[17] & 0x7f);
  printf("boot block num: %02x\n", buf[18] & 0x7f);

  return;
}


void
ShowVersion(unsigned char *buf)
{
  printf("device ver:   %x.%x_%x\n", buf[0], buf[1], buf[2]);
  printf("firmware ver: %x.%x_%x\n", buf[3], buf[4], buf[5]);

  return;
}


#if 0

int
GetCpuValueByText(unsigned char *val, unsigned char *str)
{
  int		result = 0;
  int		i;
  for(i = 0; i < 4; i++) {
    if(!isdigit(*str)) goto fail;
    *val++ = *str++ & 0xf;
  }
  result = 1;

fail:
  return result;
}
#endif


#if 1
static int	factorUSec;
void
DeviceSetMicrosecondsFactor(int n)
{
  factorUSec = n;
  return;
}
/*static int		factorUSec = 50;*/
void
DeviceWaitMicroseconds(int n)
{
  int		i;

  n *= factorUSec;

  for(i = 0; i < n; i++);
  return;
}
#endif


void
MainShowData(char *str, uint8_t *ptr, int size)
{
  int	i;
  uint8_t	*pBuf;

  pBuf = ptr;
  printf("%s[", str);
  for(i = 0; i < size; i++) {
    if(!(i & 0x3)) printf(" ");
    if(!(i & 0xf) && i) printf("\n    ");
    printf("%02x ", *pBuf++ & 0xff);
  }
  printf("]\n");

  return;
}


static void
MainParseFileHeader(char *str)
{
  char		tmp[MAIN_HEADER_MAXLEN];
  char		*p, *av[100];
  int		i;

  strncpy(tmp, str, sizeof(tmp));
  tmp[MAIN_HEADER_MAXLEN] = '\0';

  i = 0;
  p = strtok(tmp, " :\n");
  av[i++] = p;
  do {
    p = strtok(NULL, " :\n");
    av[i] = p;
    i++;
  } while(p != NULL);

  for(i = 0; av[i] != NULL; i++) {
    /*printf("%s\n", av[i]);*/

    if(!strcmp(av[i], "Product")) {
      i++;
      strncpy(sys.strFileHeaderProduct, av[i], MAIN_HEADER_LINE_MAXLEN);
    } else if(!strcmp(av[i], "Type")) {
      i++;
      strncpy(sys.strFileHeaderType, av[i], MAIN_HEADER_LINE_MAXLEN);
    } else if(!strcmp(av[i], "File")) {
      i++;
      strncpy(sys.strFileHeaderFile, av[i], MAIN_HEADER_LINE_MAXLEN);
    } else if(!strcmp(av[i], "FlashStart")) {
      i++;
      strncpy(sys.strFileHeaderFlashStart, av[i], MAIN_HEADER_LINE_MAXLEN);
      sys.addrFile.start = strtoul(&sys.strFileHeaderFlashStart[2], NULL, 16);
    } else if(!strcmp(av[i], "Version")) {
      i++;
      strncpy(sys.strFileHeaderVersion, av[i], MAIN_HEADER_LINE_MAXLEN);
    }
  }

  if(sys.fAllArea && 0) {
    printf("aa 1\n");
    sys.numFileHeaderTarget = MAIN_FILE_TARGET_ALLAREA;
  } else {    
    if(!strcmp(sys.strFileHeaderType, "Bootloader")) {
      sys.numFileHeaderTarget = MAIN_FILE_TARGET_BOOTLOADER;
    } else if(!strcmp(sys.strFileHeaderType, "Firmware")) {
      sys.numFileHeaderTarget = MAIN_FILE_TARGET_FIRMWARE;
    } else if(!strcmp(sys.strFileHeaderType, "Userarea1")) {
      sys.numFileHeaderTarget = MAIN_FILE_TARGET_USERAREA;
    }
  }

#if 1
  printf("Product: %s\n", sys.strFileHeaderProduct);
  printf("Type:    %s (num: %d)\n", sys.strFileHeaderType, sys.numFileHeaderTarget);
  printf("File:    %s\n", sys.strFileHeaderFile);
  printf("FlashStart: %s\n", sys.strFileHeaderFlashStart);
  printf("Version: %s\n", sys.strFileHeaderVersion);
#endif

  return;
}


/*
 * 0: file is NOT up to date 
 */
static int
MainCheckFileInfoUptodate(void)
{
  int		result = 0;		/* not machted */
  char		*p;

  p = (char *) 0;
  switch(sys.numFileHeaderTarget) {
  case	MAIN_FILE_TARGET_BOOTLOADER:
    p = sys.strBootloaderVersion;
    break;
  case	MAIN_FILE_TARGET_FIRMWARE:
    p = sys.strFirmwareVersion;
    break;
  case	MAIN_FILE_TARGET_USERAREA:
    /*p = sys.strFirmwareVersion;*/
    break;
  }
  if(!p) {
    fprintf(stderr, "  the file information was not found\n");
    goto end;		/* no information, no update */
  }

  printf("# target [%s][%s]\n", sys.strBootloaderProduct, p);
  printf("# file   [%s][%s]\n",
	 sys.strFileHeaderProduct, sys.strFileHeaderVersion);

  if(strcmp(sys.strFileHeaderProduct, sys.strBootloaderProduct)) {
    fprintf(stderr, "  target product was not matched\n");
    goto end;
  }
  if(!sys.strFileHeaderVersion[0] || !sys.numFileHeaderTarget) goto end;
  if(strcmp(sys.strFileHeaderVersion, p) <= 0) {
    fprintf(stderr, "  the file was same or older version\n");
    goto end;
  }

  result = 1;		/* file is up to date */

end:
  return result;
}


static int
MainGetBurnedFirmInfo(int s)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		re;

  /* get the product */
  sys.strBootloaderProduct[0] = 1;
  re = ProgApi(s, PROG_API_GETINFO,
	       sys.strBootloaderProduct, sizeof(sys.strBootloaderProduct));
  if(re == PROG_ERRNO_TIMEOUT) {
    result = ERRNO_TIMEOUT;
    goto end;
  }
  /* get the firmware version */
  sys.strBootloaderVersion[0] = 2;
  re = ProgApi(s, PROG_API_GETINFO,
	       sys.strBootloaderVersion, sizeof(sys.strBootloaderVersion));
  if(re == PROG_ERRNO_TIMEOUT) {
    result = ERRNO_TIMEOUT;
    goto end;
  }
  /* get the firmware version */
  sys.strFirmwareVersion[0] = 3;
  re = ProgApi(s, PROG_API_GETINFO,
	       sys.strFirmwareVersion, sizeof(sys.strFirmwareVersion));
  if(re == PROG_ERRNO_TIMEOUT) {
    result = ERRNO_TIMEOUT;
    goto end;
  }

  result = PROG_ERRNO_SUCCESS;

end:
  return result;
}

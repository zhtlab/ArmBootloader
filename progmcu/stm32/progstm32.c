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

#define	_PROGSTM32_C_
/*
 * progstm32.c:  
 *
 */

/*

int
cb(int s, int req, void *ptr, int size) {
  int		result = PROG_ERRNO_SUCCESS;

  switch(req) {
  case		XXXXX_CB_XX:

  }

  return result;
}


int
main(int argc, char argv[])
{
  int		result;
  int		s;
  int		i;

  if((Progstm32Api(0, PROG_API_INITIALIZE, PROG_NULL, 0)) < 0) {
    fprintf(stderr, "init error %d\n", s);
    exit(1);
  }

  if((s = Progstm32Api(0, PROG_API_INITIALIZE_UNIT, PROG_NULL, 0)) < 0) {
    fprintf(stderr, "init unit error %d\n", result);
    exit(1);
  }
 
  Progstm32Api(s, PROG_API_SETCALLBACK, cb, sizeof(cb));

  i = 0;
  while(1) {
    Progstm32Api(0, PROG_API_HEARTBEAT, PROG_NULL, 0);
    if(i++ > 100) break;
  }

  Progstm32Api(s, PROG_API_RESETCALLBACK, PROG_NULL, 0);

  Progstm32Api(s, PROG_API_UNINITIALIZE_UNIT, PROG_NULL, 0);

  Progstm32Api(0, PROG_API_UNINITIALIZE, PROG_NULL, 0);
  
  exit(0);
}



 */



#define	PROGSTM32_USE_MALLOC		0

#define	PROGSTM32_CHECKADDRESSRANGE	0


/*
 * debug
 */
#define	PROGSTM32_DEBUG_SHOWCOMMAND	0


/***************************************************************
 * include header file
 */

#include		<stdio.h>
#include		<stdlib.h>
#include		<string.h>
#include		<stdint.h>
#include		<unistd.h>
#include		<termios.h>
#include		<errno.h>
#include		<sys/time.h>


#include	        "crc32.h"
#include	        "main.h"
#include	        "prog.h"
#include	        "idtbl.h"

#include	        "progstm32.h"


/***************************************************************
 * variable
 */
extern struct _stMain			sys;

static struct _stProgstm32		progstm32;
#if !PROGSTM32_USE_MALLOC
static struct _stProgstm32Unit	progstm32sc[PROGSTM32_NUMOFUNITS];
#endif


/***************************************************************
 * function macro
 */





#if 1
#define	Progstm32Memcpy(dst, src, sz)	memcpy(dst, src, sz)
#define	Progstm32Memset(src, c, sz)	memset(src, c, sz)
#endif


/***************************************************************
 * function
 */


/*
 * static int Progstm32Api(int unit, int req, void *ptr, int size)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
int
Progstm32Api(int unit, int req, void *ptr, int size)
{
  int			result = PROG_ERRNO_UNKNOWN;

  if(req == PROG_API_INITIALIZE) {
    result = Progstm32Init();
    goto end;
  }

  if(!progstm32.up) {
    result = PROG_ERRNO_PERM;
    goto end;
  }

  switch(req) {
    /* initialize */
#if 0
  case	PROG_API_INITIALIZE:
    result = Progstm32Init();
    break;
#endif
  case	PROG_API_UNINITIALIZE:
    result = Progstm32UnInit();
    break;
  case	PROG_API_INITIALIZE_UNIT:
    result = Progstm32InitUnit();
    break;
  case	PROG_API_UNINITIALIZE_UNIT:
    result = Progstm32UnInitUnit(unit);
    break;

    /* callback */
  case	PROG_API_SETCALLBACK:
    if(ptr) {
      result = Progstm32SetCallback(unit, ptr);
    }
    break;
  case	PROG_API_RESETCALLBACK:
    if(ptr) {
      result = Progstm32ResetCallback(unit, ptr);
    }
    break;

  case	PROG_API_HEARTBEAT:
    Progstm32Loop();
    result = PROG_ERRNO_SUCCESS;
    break;

  case	PROG_API_CONNECTDEVICE:
    result = Progstm32ConnectDevice(unit);
    break;
  case	PROG_API_DISCONNECTDEVICE:
    result = Progstm32DisconnectDevice(unit);
    break;


  case	PROG_API_RESET:
    result = Progstm32Reset(unit);
    break;

  case	PROG_API_ERASECHIP:
    result = Progstm32EraseChip(unit);
    break;

  case	PROG_API_ERASEBLOCK:
    result = Progstm32EraseBlock(unit, ptr, size);
    break;

  case	PROG_API_PROGRAM:
    result = Progstm32Program(unit, ptr, size);
    break;

  case	PROG_API_READ:
    result = Progstm32Read(unit, ptr, size);
    break;

  case	PROG_API_VERIFY:
    result = Progstm32Verify(unit, ptr, size);
    break;

  case	PROG_API_CHECKBLOCKBLANK:
    result = Progstm32CheckBlockBlank(unit, ptr, size);
    break;

  case	PROG_API_GETSIGNATURE:
    result = Progstm32GetSignature(unit, ptr, size);
    break;

  case	PROG_API_GETVERSION:
    result = Progstm32GetVersion(unit, ptr, size);
    break;

  case	PROG_API_GETINFO:
    result = Progstm32GetInfo(unit, ptr, size);
    break;

  case	PROG_API_GETADDRINFO:
    result = Progstm32GetAddrInfo(unit, ptr, size);
    break;

  case	PROG_API_GETCRC:
    result = Progstm32GetCrc(unit, ptr, size);
    break;

  case	PROG_API_CHECK_BLANK:
    result = Progstm32CheckBlank(unit, ptr, size);
    break;

  case	PROG_API_GETCHECKSUM:
    result = Progstm32GetCheckSum(unit, ptr, size);
    break;

  case	PROG_API_SETSECURITY:
    result = Progstm32SetSecurity(unit, ptr, size);
    break;

  case	PROG_API_SETSERIALMODE:
    Progstm32SetSerialMode(unit, ptr, size);
    break;

  default:
    result = PROG_ERRNO_INVALIDARG;
    break;
  }

end:
  return result;
}


/*
 * static int Progstm32Init(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progstm32Init(void)
{
  int			result = PROG_ERRNO_SUCCESS;

  Progstm32Memset(&progstm32, 0, sizeof(progstm32));

  progstm32.up = 1;

  return result;
}


/*
 * static int Progstm32UnInit(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progstm32UnInit(void)
{
  int			result = PROG_ERRNO_SUCCESS;
  int			i;

  /*  struct _stProgstm32Unit	**psc;*/

  progstm32.up = 0;


  for(i = 0; i < PROGSTM32_NUMOFUNITS; i++) {
    Progstm32UnInitUnit(i);
  }

  return result;
}


/*
 * static int Progstm32InitUnit(void)
 * input:  void
 * output: unit number
 *
 * description:
 *	
 */
static int
Progstm32InitUnit(void)
{
  int		result = PROG_ERRNO_NOSPACE;
  int		i;

  struct _stProgstm32Unit	**psc;

  for(i = 0; i < PROGSTM32_NUMOFUNITS; i ++) {
    psc = &progstm32.sc[i];
    if(!*psc) {
      result = i;
#if	PROGSTM32_USE_MALLOC
      *psc = Progstm32Malloc(sizeof(struct _stProgstm32Unit));
#else
      *psc = &(progstm32sc[i]);
#endif
      if(*psc) {
	Progstm32Memset(*psc, 0, sizeof(struct _stProgstm32Unit));
	(*psc)->unit = i;

	/* initialize */
	/*(*psc)->tReset = xxxx;*/

	result = i;
      } else {
	result = PROG_ERRNO_NOMEMORY;
      }
      break;
    }
  }

  return result;
}


/*
 * static int Progstm32UnInitUnit(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progstm32UnInitUnit(int unit)
{
  int			result = PROG_ERRNO_UNKNOWN;

  struct _stProgstm32Unit	**psc;

  psc = &progstm32.sc[unit];
#if PROGSTM32_USE_MALLOC
  if(*psc) {
    Progstm32Free(*psc);
  }
#endif
  *psc = PROG_NULL;

  return result;
}


/*
 * static void Progstm32Loop(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static void
Progstm32Loop(void)
{
  return;
}


#if 0
/*
 * static void *Progstm32Malloc(int size)
 * static void  Progstm32Free(void *ptr)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static void *
Progstm32Malloc(int size)
{
#if PROGSTM32_USE_MALLOC
  return malloc(size);
#else
  return PROG_NULL;
#endif
}
static void
Progstm32Free(void *ptr)
{
#if PROGSTM32_USE_MALLOC
  free(ptr);
#endif
  return;
}
#endif


/*
 * static int Progstm32SetCallback(int unit, void *ptr)
 * static int Progstm32ResetCallback(int unit, void *ptr)
 * static int Progstm32IsCallback(int unit, void *ptr)
 * static int Progstm32GoCallback(int unit, int req, void *ptr, int size)
 * input:  void
 * output: void
 *
 * description:
 *
 */
static int
Progstm32SetCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgstm32Unit	*sc;

  sc = progstm32.sc[unit];

  result = PROG_ERRNO_NOSPACE;

  if(Progstm32IsCallback(unit, ptr) == PROG_ERRNO_SUCCESS) {
    result = PROG_ERRNO_SUCCESS;
    goto end;
  }

  for(i = 0; i < PROGSTM32_NUMOFCALLBACK; i++) {
    if(!sc->pCb[i]) {
      sc->pCb[i] = ptr;
      result = PROG_ERRNO_SUCCESS;
      break;
    }
  }

end:
  return result;
}
static int
Progstm32ResetCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgstm32Unit	*sc;

  sc = progstm32.sc[unit];

  result = PROG_ERRNO_PARAMEXCEEDED;

  for(i = 0; i < PROGSTM32_NUMOFCALLBACK; i++) {
    if((void *)sc->pCb[i] == ptr) {
      sc->pCb[i] = PROG_NULL;
      result = PROG_ERRNO_SUCCESS;
      break;
    }
  }

  return result;
}
static int
Progstm32IsCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgstm32Unit	*sc;

  sc = progstm32.sc[unit];

  result = PROG_ERRNO_NOSPACE;

  for(i = 0; i < PROGSTM32_NUMOFCALLBACK; i++) {
    if((void *)sc->pCb[i] == ptr) {
      result = PROG_ERRNO_SUCCESS;
      break;
    }
  }

  return result;
}
static int
Progstm32GoCallback(int unit, int req, void *ptr, int size)
{
  int			result;
  int			i;

  struct _stProgstm32Unit	*sc;

  sc = progstm32.sc[unit];

  result = PROG_ERRNO_SUCCESS;

  for(i = 0; i < PROGSTM32_NUMOFCALLBACK; i++) {
    if(sc->pCb[i]) {
      result = sc->pCb[i](unit, req, ptr, size);
    }
  }

  return result;
}




/******************************************************************
 *
 ******************************************************************/

/*
 * static int Progstm32ConnectDevice(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *	
 *
 */
static int
Progstm32ConnectDevice(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgstm32Unit	*psc;
  uint8_t	buf[256];

  int		n;

  psc = progstm32.sc[unit];

#if 0
  if((result = Progstm32GoCallback(unit, PROG_CB_GPIO_INIT, PROG_NULL, 0)) !=
     PROG_ERRNO_SUCCESS) {
    goto fail;
  }
#endif

  n = psc->modeSerial;
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_INIT, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  n = 115200;
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_SETBAUD, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  n = CS8 | PARENB;	/* CS8, PARENB, !PARODD, !CSTOPB */
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_SETTERMINAL, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;

  result = Progstm32Sync(unit);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  result = Progstm32CmdGet(unit, buf, sizeof(buf));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  Progstm32StoreSupportedCommand(psc, buf, result);
  
  result = Progstm32CmdGetVersion(unit, buf, sizeof(buf));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  printf("# Mask version: %x %x %x\n", buf[0], buf[1], buf[2]);

  result = Progstm32CmdGetId(unit, buf, sizeof(buf));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  printf("# Chip ID: %x %x \n", buf[0], buf[1]);
  psc->chipID = (buf[0] << 8) | buf[1];

  result = PROG_ERRNO_SUCCESS;
fail:
  return result;
}


/*
 * static int Progstm32DisconnectDevice(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32DisconnectDevice(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgstm32Unit	*psc;
  int		n;

  psc = progstm32.sc[unit];
  n = psc->modeSerial;

  Progstm32GoCallback(unit, PROG_CB_SERIAL_UNINIT, &n, sizeof(n));

  Progstm32GoCallback(unit, PROG_CB_GPIO_UNINIT, PROG_NULL, 0);

  return result;
}


/*
 * static int Progstm32Reset(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *
 */
static int
Progstm32Reset(int unit)
{
  int				result = PROG_ERRNO_UNKNOWN;
  struct _stProgData		d;

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32Reset()\n", __FILE__, __LINE__);
#endif

  d.start = 0x08000000;
  result = Progstm32CmdGo(unit, &d, sizeof(d));
  
  return result;
}




/*
 * static int Progstm32EraseChip(int unit)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32EraseChip(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;

  struct _stProgstm32Unit	*psc;
  struct _stProgData		d;

  psc = progstm32.sc[unit];
  memset(&d, 0, sizeof(d));

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32EraseChip()\n", __FILE__, __LINE__);
#endif

  if(psc->fCmdExtendedErase) {
    d.cnt = 0xffff;		/* table length */
    result = Progstm32CmdExtendedErase(unit, &d, sizeof(d));
  } else if(psc->fCmdErase) {
  }

  return result;
}


/*
 * static int Progstm32EraseBlock(int unit, struct _stProgData *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32EraseBlock(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;

  struct _stProgstm32Unit	*psc;
  struct _stProgData		d;
  uint32_t			addr, addrSect;
  int				num;

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32EraseBlock()\n", __FILE__, __LINE__);
#endif

  psc = progstm32.sc[unit];
  memset(&d, 0, sizeof(d));
  addr = ptr->start;


  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  if(psc->fCmdExtendedErase) {
    while(addr < ptr->end ) {
      addrSect = addr;
      num = IdtblStm32GetSectorByAddr(psc->chipID, &addrSect);
      if(num < 0) break;
      d.start = num & PROGSTM32_ERASE_SECTOR_NUM_MASK;
      d.size  = 0;
      d.cnt = 1;
      result = Progstm32CmdExtendedErase(unit, &d, sizeof(d));
      addr = addrSect;
    }
  } else if(psc->fCmdErase) {
  }

fail:
  return result;
}


/*
 * static int Progstm32Program(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32Program(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  int		packets = 0;
#endif

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32Program()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progstm32CmdWriteMemory(unit, ptr, size);

fail:  
  return result;
}


/*
 * static int Progstm32Read(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32Read(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  int		packets = 0;
#endif

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32Read()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progstm32CmdReadMemory(unit, ptr, size);

fail:  
  return result;
}


/*
 * static int Progstm32Verify(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32Verify(int unit, struct _stProgData *ptr, int size)
{
  int			result = PROG_ERRNO_UNKNOWN;

  struct _stProgData	d;
  uint32_t		crc;
  int			pos;

  struct _stProgstm32Unit	*psc;

  psc = progstm32.sc[unit];

  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32Verify()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  d.start = ptr->start;
  d.end   = ptr->end;
  d.size  = ptr->size;

  if(psc->fCmdExtGetCrc) {
    result = ProgApi(unit, PROG_API_GETCRC, &d, sizeof(d));
    if(result == PROG_ERRNO_SUCCESS) {

      crc = 0;
      crc = Crc32Calc(crc, ptr->ptr, (ptr->size + 0x7ffUL) & ~0x7ffUL);

      if(d.crc != crc) result = PROG_ERRNO_DATACHECK;
    }
  }

#if 1
  if(result != PROG_ERRNO_SUCCESS) {
    int		sz;
    uint8_t	buf[PROGSTM32_ERASE_SECTOR_BYTES];
    struct _stProgData	d;

    d.ptr = buf;
    d.start = ptr->start;
    sz = ptr->size;

    pos = 0;
    while(sz > 0) {
      d.size = sizeof(buf);
      if(sz < sizeof(buf)) d.size = sz;
      d.end = d.start + d.size;

      result = Progstm32CmdReadMemory(unit, &d, sizeof(d));
      if(result < PROG_ERRNO_SUCCESS) {
	break;
      }
      if(memcmp(buf, &ptr->ptr[pos], d.size)) {
	result = PROG_ERRNO_DATACHECK;
	break;
      }

      sz -= sizeof(buf);
      d.start += sizeof(buf);
      pos += sizeof(buf);
    }
  }
#endif
fail:
  return result;
}


/*
 * static int Progstm32CheckBlockBlank(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32CheckBlockBlank(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		i, sz;
  uint8_t       valBlank = 0xff;

  uint8_t	buf[PROGSTM32_ERASE_SECTOR_BYTES];
  struct _stProgData	d;

  struct _stProgstm32Unit	*psc;

  psc = progstm32.sc[unit];

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32CheckBlockBlank()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  if(psc->fCmdExtCheckBlank) {
    result = Progstm32CmdCheckBlank(unit, ptr, size);

  } else {
    d.ptr = buf;
    d.start = ptr->start;
    sz = ptr->size;

    valBlank = IdtblStm32GetBlankValueById(psc->chipID);

    while(sz > 0) {
      d.size = sizeof(buf);
      if(sz < sizeof(buf)) d.size = sz;
      d.end = d.start + d.size;

      result = Progstm32CmdReadMemory(unit, &d, sizeof(d));
      if(result < PROG_ERRNO_SUCCESS) {
	break;
      }
      for(i = 0; i < d.size; i++) {
	if(d.ptr[i] != valBlank) {
	  sz = 0;
	  result = PROG_ERRNO_DATACHECK;
	  break;
	}
      }
      sz -= sizeof(buf);
      d.start += sizeof(buf);
    }
  }

fail:
  return result;
}


/*
 * static int Progstm32GetSignature(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32GetSignature(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetSignature()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }


  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progstm32GetVersion(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32GetVersion(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetVersion()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  /*Progstm32CmdGetInfoVersion(unit, ptr, size);*/
  
  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progstm32GetInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32GetInfo(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetInfo()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progstm32CmdGetInfo(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progstm32GetAddrInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32GetAddrInfo(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  struct _stProgData		*addr;

#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetAddrInfo()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  addr = (struct _stProgData *)ptr;

  result = PROG_ERRNO_SUCCESS;
  switch(addr->mode) {
  case	MAIN_FILE_TARGET_BOOTLOADER:
    addr->start = 0x000000;
    addr->end   = 0x018000;
    break;
  case	MAIN_FILE_TARGET_FIRMWARE:
    addr->start = 0x040000;
    addr->end   = 0x100000;
    break;
  case	MAIN_FILE_TARGET_USERAREA:
    addr->start = 0x018000;
    addr->end   = 0x020000;
    break;
  case	MAIN_FILE_TARGET_ALLAREA:
    addr->start = 0x000000;
    addr->end   = 0x200000;
    break;
  default:
    addr->start = 0x00000;
    addr->end   = 0x40000;
    result = PROG_ERRNO_UNKNOWN;
    break;
  }
  addr->start += 0x08000000;
  addr->end   += 0x08000000;
  addr->size = addr->end - addr->start;
  addr->sizeBlock = PROGSTM32_ERASE_SECTOR_BYTES;
  addr->sizeEraseSector = PROGSTM32_ERASE_SECTOR_BYTES;

fail:
  return result;
}


/*
 * static int Progstm32GetInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32GetCrc(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetCrc()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progstm32CmdGetCrc(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progstm32GetInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32CheckBlank(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32CheckBlank()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progstm32CmdCheckBlank(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progstm32GetCheckSum(int unit, struct _stProgData *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32GetCheckSum(int unit, struct _stProgData *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32GetCheckSum()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }


  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progstm32SetSecurity(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progstm32SetSecurity(int unit, unsigned char *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGSTM32_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progstm32SetSecurity()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progstm32SetSerialMode(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progstm32SetSerialMode(int unit, int *pMode, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgstm32Unit	*psc;
  int		mode;

  psc = progstm32.sc[unit];

  if(pMode) {
    mode = *pMode;
    if(mode < SERIALMODE_NONE || mode > SERIALMODE_USB) {
      result = PROG_ERRNO_PARAMEXCEEDED;
      goto fail;
    }

    psc->modeSerial = mode;
    result = PROG_ERRNO_SUCCESS;
  }

fail:
  return result;
}


/******************************************************************
 *
 ******************************************************************/
static int
Progstm32Sync(int unit)
{
  int		result;
  int		i;
  uint8_t	c;

  i = 0;
  while(1) {
    c = STM32_CMD_SYNC;
    result = Progstm32SendDataWaitAckTout(unit, &c, sizeof(c), 500*1000);
    if(result == PROG_ERRNO_SUCCESS) break;
#if 0
    if(result == PROG_ERRNO_NACK_RECV) {
      result = PROG_ERRNO_SUCCESS;
      break;
    }
#endif
    /*if(result == PROG_ERRNO_TIMEOUT) continue;*/

    if(i > 10) {
      result = PROG_ERRNO_TIMEOUT;
      break;
    }
    i++;
  }

  return result;
}

static int
Progstm32CmdGet(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], c;
  int		sz;
  int		i;

  if(!ptr || size < 0) goto fail;

  sz = size;

  buf[0] =  STM32_CMD_GET;
  buf[1] = ~STM32_CMD_GET;

  /* send command */
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* get size of data */
  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  c++;
  if(sz > c) sz = c;
  /* recv data */
  result = Progstm32RecvData(unit, ptr, sz);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* wait ack */
  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;

  result = sz;

  /* flush remain data, if exist */
  sz = c - size;
  for(i = 0; i < sz; i++) {
    Progstm32RecvData(unit, &c, 1);
  }
fail:
  return result;
}


static int
Progstm32CmdGetVersion(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], c;

  if(!ptr || size < 3) goto fail;

  buf[0] =  STM32_CMD_GET_VERSION;
  buf[1] = ~STM32_CMD_GET_VERSION;

  /* send command */
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv data */
  result = Progstm32RecvData(unit, ptr, 3);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* wait ack */
  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;

fail:
  return result;
}


static int
Progstm32CmdGetId(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], c;

  if(!ptr || size < 2) goto fail;

  buf[0] =  STM32_CMD_GET_ID;
  buf[1] = ~STM32_CMD_GET_ID;

  /* send command */
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* get size of data */
  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv data */
  result = Progstm32RecvData(unit, ptr, 2);
  /* wait ack */
  result = Progstm32RecvData(unit, &c, 1);

fail:
  return result;
}


static int
Progstm32CmdReadMemory(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], *p;
  int		sz, n;
  uint32_t	addr;

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  p    = ptr->ptr;
  addr = ptr->start;
  sz   = ptr->size;

  while(sz > 0) {
    /* send command */
    buf[0] =  STM32_CMD_READ_MEMORY;
    buf[1] = ~STM32_CMD_READ_MEMORY;
    result = Progstm32SendDataWaitAck(unit, buf, 2);
    if(result < PROG_ERRNO_SUCCESS) goto fail;
    /* send the address */
    buf[0] =  addr >> 24;
    buf[1] =  addr >> 16;
    buf[2] =  addr >>  8;
    buf[3] =  addr >>  0;
    buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
    result = Progstm32SendDataWaitAck(unit, buf, 5);
    if(result < PROG_ERRNO_SUCCESS) goto fail;
    /* send the size */
    n = sz;
    if(n >= 256) n = 256;
    buf[0] =  n - 1;
    buf[1] = ~buf[0];
    result = Progstm32SendDataWaitAck(unit, buf, 2);
    if(result < PROG_ERRNO_SUCCESS) goto fail;
    /* recv data */
    result = Progstm32RecvData(unit, p, n);

    sz   -= n;
    p    += n;
    addr += n;
  }

  result = PROG_ERRNO_SUCCESS;

  fail:
  return result;
}


static int
Progstm32CmdGo(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8];
  uint32_t	addr;

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  /*p    = d->ptr;*/
  addr = ptr->start;

  /* send command */
  buf[0] =  STM32_CMD_GO;
  buf[1] = ~STM32_CMD_GO;
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result != PROG_ERRNO_SUCCESS) goto fail;
  /* send the address */
  buf[0] =  addr >> 24;
  buf[1] =  addr >> 16;
  buf[2] =  addr >>  8;
  buf[3] =  addr >>  0;
  buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
  result = Progstm32SendDataWaitAck(unit, buf, 5);
  /*if(result != PROG_ERRNO_SUCCESS) goto fail;*/

fail:
  return result;
}


static int
Progstm32CmdWriteMemory(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], *p;
  int		i, sz, n;
  uint32_t	addr;
  int		align;
  int		fWrite;

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  p    = ptr->ptr;
  addr = ptr->start;
  sz   = ptr->size;

  align = ((sz + 7) & ~7) - sz;

  while(sz > 0) {
    n = sz;
    if(n > 256) n = 256;

    /* set fWrite = 1, the all 256 bytes data are not 0xff */
    fWrite = 0;
    for(i = 0; i < n; i++) {
      if(p[i] != 0xff) fWrite = 1;
    }

    if(fWrite) {
      /* send command */
      buf[0] =  STM32_CMD_WRITE_MEMORY;
      buf[1] = ~STM32_CMD_WRITE_MEMORY;
      result = Progstm32SendDataWaitAck(unit, buf, 2);
      if(result != PROG_ERRNO_SUCCESS) goto fail;
      /* send the address */
      buf[0] =  addr >> 24;
      buf[1] =  addr >> 16;
      buf[2] =  addr >>  8;
      buf[3] =  addr >>  0;
      buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
      result = Progstm32SendDataWaitAck(unit, buf, 5);
      if(result != PROG_ERRNO_SUCCESS) goto fail;
      /* send the size */
      if(n < 256 && align) {
	buf[0] =  n-1 + align;
	result = Progstm32SendData(unit, buf, 1);
	/* send data */
	result = Progstm32SendData(unit, p, n);
	memset(buf, 0xff, align);
	result = Progstm32SendData(unit, buf, align);
	/* send the checksum */
	buf[0] = Progstm32CalcCheckSum(p, n) ^ Progstm32CalcCheckSum(buf, align) ^ ((n-1) + align);
      } else {		/* 256byte filled data */
	buf[0] =  n-1;
	result = Progstm32SendData(unit, buf, 1);
	/* send data */
	result = Progstm32SendData(unit, p, n);
	/* send the checksum */
	buf[0] = Progstm32CalcCheckSum(p, n) ^ (n-1);
      }
      result = Progstm32SendDataWaitAckTout(unit, buf, 1, 256*PROGSTM32_TIME_tPROG);

      if(result != PROG_ERRNO_SUCCESS) goto fail;
    }

    sz   -= n;
    p    += n;
    addr += n;
  }

fail:
  return result;
}


/*
 * ### special erase
 *   cnt = 0xfff0 -- 0xffff    
 * ### 1 sector erase
 *   addr = num of sector
 *   cnt  = 1
 *   size = 0
 * ### multiple sector erase
 *   ptr = pointer of list of sectors
 *   size = sizeof(list)
 *   cnt = count of sectors
 *
 */
static int
Progstm32CmdExtendedErase(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8], *p, sum;
  int		sz, n, cnt;
  int		i;
  uint32_t	addr;

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  addr = ptr->start;		/* number of sectors */
  p    = ptr->ptr;		/* list table */
  sz   = ptr->size;		/* table length */
  cnt  = ptr->cnt;		/* entry count */
  if(addr >= 0x200000) {
    result = PROG_ERRNO_PARAMEXCEEDED;
    goto fail;
  }

  /* send command */
  buf[0] =  STM32_CMD_EXTENDED_ERASE;
  buf[1] = ~STM32_CMD_EXTENDED_ERASE;
  result = Progstm32SendDataWaitAckTout(unit, buf, 2, 100*1000);
  if(result != PROG_ERRNO_SUCCESS) goto fail;

  buf[0] =  cnt>>8;
  buf[1] =  cnt;
  if(cnt == 1) {
    /* number of sectors */
    buf[1] = 0;
    buf[2] =  addr >>  8;
    buf[3] =  addr >>  0;
    buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
    result = Progstm32SendDataWaitAckTout(unit, buf, 5, PROGSTM32_TIME_tERASE256KB);
    if(result != PROG_ERRNO_SUCCESS) goto fail;
  } else if(cnt < 0xfff0) {
    while(sz > 0) {
      if(addr < 0xfff0 && sz >= 0) {
	/* send the size */
	n = sz;
	if(n >= 256) n = 256-1;
	buf[0] =  n>>8;
	buf[1] =  n;
	result = Progstm32SendData(unit, buf, 2);
	/* send data */
	result = Progstm32SendData(unit, p, n);
	/* send the checksum */
	for(i = 0; i < sz; i++) {
	  sum ^= p[i];
	}
	result = Progstm32SendDataWaitAck(unit, &sum, 1);
	if(result != PROG_ERRNO_SUCCESS) goto fail;
      }
    }
  } else if((cnt & 0xfff0) == 0xfff0) {
    buf[2] =  buf[0] ^ buf[1];
    result = Progstm32SendDataWaitAckTout(unit, buf, 3, PROGSTM32_TIME_tME);
  }

fail:
  return result;
}


static int
Progstm32CmdGetInfo(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[260], *p, c;
  int		n;
  struct _stProgstm32Unit	*psc;

  psc = progstm32.sc[unit];
  
  if(!psc->fCmdExtGetInfo) {
    result = PROG_ERRNO_NOT_SUPPORTED;
    goto fail;
  }

  if(!ptr) goto fail;

  c = *(uint8_t *) ptr;
  *(uint8_t *) ptr = '\0';

  /* send command */
  buf[0] =  STM32_CMD_GET_INFO;
  buf[1] = ~STM32_CMD_GET_INFO;
  result = Progstm32SendDataWaitAck(unit, buf, 2);

  if(result < PROG_ERRNO_SUCCESS) goto fail;

  /* info number */
  buf[0] =  c;
  buf[1] = ~c;
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;

  /* recv data */
  n = 1;
  result = Progstm32RecvData(unit, buf, n);
  n = buf[0];
  if(n > sizeof(buf)) n = sizeof(buf);
  result = Progstm32RecvData(unit, buf, n);
  buf[n] = '\0';
  p = &buf[0];
  while((*ptr++ = *p++) && size > 0) size--;

  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS || c != STM32_ACK) goto fail;
  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


#if 0
static int
Progstm32CmdGetInfoVersion(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[32], *p, c;
  int		n;


  if(!ptr) goto fail;

  /* send command */
  buf[0] =  STM32_CMD_GET_INFO;
  buf[1] = ~STM32_CMD_GET_INFO;
  result = Progstm32SendDataWaitAck(unit, buf, 2);

  if(result < PROG_ERRNO_SUCCESS) goto fail;

  /* info number */
  buf[0] =  2;
  buf[1] = ~2;
  result = Progstm32SendDataWaitAck(unit, buf, 2);

  /* recv data */
  n = 1;
  result = Progstm32RecvData(unit, buf, n);
  n = buf[0];
  if(n > sizeof(buf)) n = sizeof(buf);
  result = Progstm32RecvData(unit, buf, n);
  buf[n] = '\0';
  p = &buf[0];
  while((*ptr++ = *p++) && size > 0) size--;

  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


static int
Progstm32CmdGetInfoFirmwareVersion(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[260], *p, c;
  int		n;


  if(!ptr) goto fail;

  /* send command */
  buf[0] =  STM32_CMD_GET_INFO;
  buf[1] = ~STM32_CMD_GET_INFO;
  result = Progstm32SendDataWaitAck(unit, buf, 2);

  if(result < PROG_ERRNO_SUCCESS) goto fail;

  /* info number */
  buf[0] =  3;
  buf[1] = ~3;
  result = Progstm32SendDataWaitAck(unit, buf, 2);

  /* recv data */
  n = 1;
  result = Progstm32RecvData(unit, buf, n);
  n = buf[0];
  if(n > sizeof(buf)) n = sizeof(buf);
  result = Progstm32RecvData(unit, buf, n);
  buf[n] = '\0';
  p = &buf[0];
  while((*ptr++ = *p++) && size > 0) size--;

  result = Progstm32RecvData(unit, &c, 1);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}
#endif

static int
Progstm32CmdGetCrc(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	buf[8];
  int		sz, n;
  uint32_t	addr;
  uint32_t	crc;

  struct _stProgstm32Unit	*psc;

  psc = progstm32.sc[unit];
  
  if(!psc->fCmdExtGetCrc) {
    result = PROG_ERRNO_NOT_SUPPORTED;
    goto fail;
  }

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  addr = ptr->start;
  sz = ptr->size;

  /* send command */
  buf[0] =  STM32_CMD_GET_CRC;
  buf[1] = ~STM32_CMD_GET_CRC;
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* send the address */
  buf[0] =  addr >> 24;
  buf[1] =  addr >> 16;
  buf[2] =  addr >>  8;
  buf[3] =  addr >>  0;
  buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
  result = Progstm32SendDataWaitAck(unit, buf, 5);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* send the size */
  buf[0] =  sz >> 24;
  buf[1] =  sz >> 16;
  buf[2] =  sz >>  8;
  buf[3] =  sz >>  0;
  buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
  result = Progstm32SendDataWaitAck(unit, buf, 5);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv data */
  n = 5;
  result = Progstm32RecvData(unit, buf, n);

  crc  = buf[0] << 24;
  crc |= buf[1] << 16;
  crc |= buf[2] <<  8;
  crc |= buf[3] <<  0;

  ptr->crc = crc;

  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


static int
Progstm32CmdCheckBlank(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		re;
  uint8_t	buf[8];
  int		sz, n;
  uint32_t	addr;

  struct _stProgstm32Unit	*psc;

  psc = progstm32.sc[unit];
  
  if(!psc->fCmdExtCheckBlank) {
    result = PROG_ERRNO_NOT_SUPPORTED;
    goto fail;
  }

  if(!ptr || size != sizeof(struct _stProgData)) goto fail;

  addr = ptr->start;
  sz = ptr->size;

  /* send command */
  buf[0] =  STM32_CMD_CHECK_BLANK;
  buf[1] = ~STM32_CMD_CHECK_BLANK;
  result = Progstm32SendDataWaitAck(unit, buf, 2);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* send the address */
  buf[0] =  addr >> 24;
  buf[1] =  addr >> 16;
  buf[2] =  addr >>  8;
  buf[3] =  addr >>  0;
  buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
  result = Progstm32SendDataWaitAck(unit, buf, 5);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* send the size */
  buf[0] =  sz >> 24;
  buf[1] =  sz >> 16;
  buf[2] =  sz >>  8;
  buf[3] =  sz >>  0;
  buf[4] =  buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
  result = Progstm32SendDataWaitAck(unit, buf, 5);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv data */
  n = 5;
  re = Progstm32RecvData(unit, buf, n);

  if(re != 5 ||
     (buf[0] ^ buf[1] ^ buf[2] ^ buf[3]) != buf[4] ||
     (buf[0] | buf[1] | buf[2] | buf[3]) != 0) {
    result = PROG_ERRNO_DATACHECK;
    goto fail;
  }

fail:
  return result;
}


/******************************************************************
 * send and recv data
 ******************************************************************/

static int
Progstm32SendDataWaitAck(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	c;

  /* send */
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_SENDDATA, ptr, size);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv */
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_RECVDATA, &c, sizeof(c));
  if(result >= 1) {
    if(c == STM32_ACK) {
      result = PROG_ERRNO_SUCCESS;
    } else if(c == STM32_NACK) {
      result = PROG_ERRNO_NACK_RECV;
    } else {
      result = PROG_ERRNO_COMMUNICATION;
    }
  } else result = PROG_ERRNO_COMMUNICATION;

fail:
  return result;
}
static int
Progstm32SendDataWaitAckTout(int unit, uint8_t *ptr, int size, time_t tout)
{
  int		result = PROG_ERRNO_UNKNOWN;
  uint8_t	c;
  struct timeval	tv;
  time_t		t, tPrev;

  /* send */
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_SENDDATA, ptr, size);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  /* recv */
  gettimeofday(&tv, NULL);
  tPrev = tv.tv_sec * 1000000 + tv.tv_usec;
  while(1) {
    result = Progstm32GoCallback(unit, PROG_CB_SERIAL_RECVDATA, &c, sizeof(c));
    if(result >= 1) {
      if(c == STM32_ACK) {
	result = PROG_ERRNO_SUCCESS;
      } else if(c == STM32_NACK) {
	result = PROG_ERRNO_NACK_RECV;
      } else {
        fprintf(stderr, "# WARN Progstm32SendDataWaitAckTout() return value is invalid.  recv size:%d val:%02x\r\n", result, c);
	result = PROG_ERRNO_COMMUNICATION;
      }
      break;
    }
    gettimeofday(&tv, NULL);
    t = tv.tv_sec * 1000000 + tv.tv_usec;
    if((t - tPrev) > tout) {
      result = PROG_ERRNO_TIMEOUT;
      break;
    }
  }
  
fail:
  return result;
}
static int
Progstm32SendData(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  
  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_SENDDATA, ptr, size);

  return result;
}


/*
 * static int Progstm32RecvData(int unit, char *ptr, int size unsigned int tout)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 *
 */
static int
Progstm32RecvData(int unit, uint8_t *ptr, int size)
{
  int	result = PROG_ERRNO_UNKNOWN;

  result = Progstm32GoCallback(unit, PROG_CB_SERIAL_RECVDATA, ptr, size);

  return result;
}


/*
 * static int Progstm32CalcCheckSum(char *ptr, int size)
 * input:  ptr   data pointer
 *         size  calculate length
 * output: check sum
 *
 * description:
 *	
 *
 */
static int
Progstm32CalcCheckSum(unsigned char *ptr, int size)
{
  unsigned int	sum = 0;
  int		i;

  for(i = 0; i < size; i++) {
    sum = sum ^ *ptr++;
  }

  return sum & 0xff;
}


static void
Progstm32StoreSupportedCommand(struct _stProgstm32Unit *psc,
			       uint8_t *buf, int size)
{
  int		i;

  psc->bootloaderVer = buf[0];
  for(i = 1; i < size; i++) {
    switch(buf[i]) {
    case	STM32_CMD_GET:
      psc->fCmdGet = 1;
      break;
    case	STM32_CMD_GET_VERSION:
      psc->fCmdGetVersion = 1;
      break;
    case	STM32_CMD_GET_ID:
      psc->fCmdGetId = 1;
      break;
    case	STM32_CMD_READ_MEMORY:
      psc->fCmdReadMemory = 1;
      break;
    case	STM32_CMD_GO:
      psc->fCmdGo = 1;
      break;
    case	STM32_CMD_WRITE_MEMORY:
      psc->fCmdWriteMemory = 1;
      break;
      /* ERASE and EXTENDED_ERASE are exclusive.  NOT existed both */
    case	STM32_CMD_ERASE:
      psc->fCmdErase = 1;
      break;
    case	STM32_CMD_EXTENDED_ERASE:
      psc->fCmdExtendedErase = 1;
      break;
    case	STM32_CMD_WRITE_PROTECT:
      psc->fCmdWriteProtect = 1;
      break;
    case	STM32_CMD_WRITE_UNPROTECT:
      psc->fCmdWriteUnprotect = 1;
      break;
    case	STM32_CMD_READOUT_PROTECT:
      psc->fCmdReadoutProtect = 1;
      break;
    case	STM32_CMD_READOUT_UNPROTECT:
      psc->fCmdReadoutUnprotect = 1;
      break;

    case	STM32_CMD_WRITE_MEMORY_NO_STRETCH:
      psc->fCmdWriteMemoryNoStreach = 1;
      break;
    case	STM32_CMD_ERASE_NO_STRETCH:
      psc->fCmdExtendedEraseNoStratch = 1;
      break;
    case	STM32_CMD_WRITE_PROTECT_NO_STRETCH:
      psc->fCmdWriteProtectNoStratch = 1;
      break;
    case	STM32_CMD_WRITE_UNPROTECT_NO_STRETCH:
      psc->fCmdWriteUnprotectNoStratch = 1;
      break;
    case	STM32_CMD_READOUT_PROTECT_NO_STRETCH:
      psc->fCmdReadoutProtectNoStratch = 1;
      break;
    case	STM32_CMD_READOUT_UNPROTECT_NO_STRETCH:
      psc->fCmdReadoutUnprotectNoStratch = 1;
      break;

    case	STM32_CMD_CHECK_BLANK:
      psc->fCmdExtCheckBlank = 1;
      break;
    case	STM32_CMD_GET_INFO:
      psc->fCmdExtGetInfo = 1;
      break;
    case	STM32_CMD_GET_CRC:
      psc->fCmdExtGetCrc = 1;
      break;
    default:
      printf("Chip has unknown command [%x]\n", buf[i]);
    }
  }

  return;
}


/*
 * end of file
 */

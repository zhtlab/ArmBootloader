/*
 * Copyright (c) 2014-2018 zhtlab
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

#define	_PROGMKL43_C_

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

  if((Progmkl43Api(0, PROG_API_INITIALIZE, PROG_NULL, 0)) < 0) {
    fprintf(stderr, "init error %d\n", s);
    exit(1);
  }

  if((s = Progmkl43Api(0, PROG_API_INITIALIZE_UNIT, PROG_NULL, 0)) < 0) {
    fprintf(stderr, "init unit error %d\n", result);
    exit(1);
  }
 
  Progmkl43Api(s, PROG_API_SETCALLBACK, cb, sizeof(cb));

  i = 0;
  while(1) {
    Progmkl43Api(0, PROG_API_HEARTBEAT, PROG_NULL, 0);
    if(i++ > 100) break;
  }

  Progmkl43Api(s, PROG_API_RESETCALLBACK, PROG_NULL, 0);

  Progmkl43Api(s, PROG_API_UNINITIALIZE_UNIT, PROG_NULL, 0);

  Progmkl43Api(0, PROG_API_UNINITIALIZE, PROG_NULL, 0);
  
  exit(0);
}

 */



#define	PROGMKL43_USE_MALLOC		0

#define	PROGMKL43_CHECKADDRESSRANGE	0


/*
 * debug
 */
#define	PROGMKL43_DEBUG_SHOWCOMMAND	0
#define	PROGSTM32_DEBUG_SHOWALLPACKETS	0


/***************************************************************
 * include header file
 */

#include		<stdio.h>
#include		<stdlib.h>
#include		<string.h>
#include		<stdint.h>
#include		<unistd.h>
#include		<errno.h>
#include		<termios.h>


#include	        "crc32.h"
#include	        "crc16.h"
#include	        "main.h"
#include	        "prog.h"

#include	        "progmkl43.h"


/***************************************************************
 * variable
 */
extern struct _stMain			sys;

static struct _stProgmkl43		progmkl43;
#if !PROGMKL43_USE_MALLOC
static struct _stProgmkl43Unit	progmkl43sc[PROGMKL43_NUMOFUNITS];
#endif


/***************************************************************
 * function macro
 */





#if 1
#define	Progmkl43Memcpy(dst, src, sz)	memcpy(dst, src, sz)
#define	Progmkl43Memset(src, c, sz)	memset(src, c, sz)
#endif


/***************************************************************
 * function
 */

#if	PROGSTM32_DEBUG_SHOWALLPACKETS
static void	Progmkl43PacketShow(struct _stMkl43Pkt *pkt, char *str);
#endif

/*
 * static int Progmkl43Api(int unit, int req, void *ptr, int size)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
int
Progmkl43Api(int unit, int req, void *ptr, int size)
{
  int			result = PROG_ERRNO_UNKNOWN;

  if(req == PROG_API_INITIALIZE) {
    result = Progmkl43Init();
    goto end;
  }

  if(!progmkl43.up) {
    result = PROG_ERRNO_PERM;
    goto end;
  }

  switch(req) {
    /* initialize */
#if 0
  case	PROG_API_INITIALIZE:
    result = Progmkl43Init();
    break;
#endif
  case	PROG_API_UNINITIALIZE:
    result = Progmkl43UnInit();
    break;
  case	PROG_API_INITIALIZE_UNIT:
    result = Progmkl43InitUnit();
    break;
  case	PROG_API_UNINITIALIZE_UNIT:
    result = Progmkl43UnInitUnit(unit);
    break;

    /* callback */
  case	PROG_API_SETCALLBACK:
    if(ptr) {
      result = Progmkl43SetCallback(unit, ptr);
    }
    break;
  case	PROG_API_RESETCALLBACK:
    if(ptr) {
      result = Progmkl43ResetCallback(unit, ptr);
    }
    break;

  case	PROG_API_HEARTBEAT:
    Progmkl43Loop();
    result = PROG_ERRNO_SUCCESS;
    break;

  case	PROG_API_CONNECTDEVICE:
    result = Progmkl43ConnectDevice(unit);
    break;
  case	PROG_API_DISCONNECTDEVICE:
    result = Progmkl43DisconnectDevice(unit);
    break;


  case	PROG_API_RESET:
    result = Progmkl43Reset(unit);
    break;

  case	PROG_API_ERASECHIP:
    result = Progmkl43EraseChip(unit);
    break;

  case	PROG_API_ERASEBLOCK:
    result = Progmkl43EraseBlock(unit, ptr, size);
    break;

  case	PROG_API_PROGRAM:
    result = Progmkl43Program(unit, ptr, size);
    break;

  case	PROG_API_READ:
    result = Progmkl43Read(unit, ptr, size);
    break;

  case	PROG_API_VERIFY:
    result = Progmkl43Verify(unit, ptr, size);
    break;

  case	PROG_API_CHECKBLOCKBLANK:
    result = Progmkl43CheckBlockBlank(unit, ptr, size);
    break;

  case	PROG_API_GETSIGNATURE:
    result = Progmkl43GetSignature(unit, ptr, size);
    break;

  case	PROG_API_GETVERSION:
    result = Progmkl43GetVersion(unit, ptr, size);
    break;

  case	PROG_API_GETINFO:
    result = Progmkl43GetInfo(unit, ptr, size);
    break;

  case	PROG_API_GETADDRINFO:
    result = Progmkl43GetAddrInfo(unit, ptr, size);
    break;

  case	PROG_API_GETCRC:
    result = Progmkl43GetCrc(unit, ptr, size);
    break;

  case	PROG_API_CHECK_BLANK:
    result = Progmkl43CheckBlank(unit, ptr, size);
    break;

  case	PROG_API_GETCHECKSUM:
    result = Progmkl43GetCheckSum(unit, ptr, size);
    break;

  case	PROG_API_SETSECURITY:
    result = Progmkl43SetSecurity(unit, ptr, size);
    break;

  case	PROG_API_SETSERIALMODE:
    Progmkl43SetSerialMode(unit, ptr, size);
    break;

  default:
    result = PROG_ERRNO_INVALIDARG;
    break;
  }

end:
  return result;
}


/*
 * static int Progmkl43Init(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progmkl43Init(void)
{
  int			result = PROG_ERRNO_SUCCESS;

  Progmkl43Memset(&progmkl43, 0, sizeof(progmkl43));

  progmkl43.up = 1;

  return result;
}


/*
 * static int Progmkl43UnInit(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progmkl43UnInit(void)
{
  int			result = PROG_ERRNO_SUCCESS;
  int			i;

  /*  struct _stProgmkl43Unit	**psc;*/

  progmkl43.up = 0;


  for(i = 0; i < PROGMKL43_NUMOFUNITS; i++) {
    Progmkl43UnInitUnit(i);
  }

  return result;
}


/*
 * static int Progmkl43InitUnit(void)
 * input:  void
 * output: unit number
 *
 * description:
 *	
 */
static int
Progmkl43InitUnit(void)
{
  int		result = PROG_ERRNO_NOSPACE;
  int		i;

  struct _stProgmkl43Unit	**psc;

  for(i = 0; i < PROGMKL43_NUMOFUNITS; i ++) {
    psc = &progmkl43.sc[i];
    if(!*psc) {
      result = i;
#if	PROGMKL43_USE_MALLOC
      *psc = Progmkl43Malloc(sizeof(struct _stProgmkl43Unit));
#else
      *psc = &(progmkl43sc[i]);
#endif
      if(*psc) {
	Progmkl43Memset(*psc, 0, sizeof(struct _stProgmkl43Unit));
	(*psc)->unit = i;

	/* initialize */

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
 * static int Progmkl43UnInitUnit(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static int
Progmkl43UnInitUnit(int unit)
{
  int			result = PROG_ERRNO_UNKNOWN;

  struct _stProgmkl43Unit	**psc;

  psc = &progmkl43.sc[unit];
#if PROGMKL43_USE_MALLOC
  if(*psc) {
    Progmkl43Free(*psc);
  }
#endif
  *psc = PROG_NULL;

  return result;
}


/*
 * static void Progmkl43Loop(void)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static void
Progmkl43Loop(void)
{
  return;
}


#if 0
/*
 * static void *Progmkl43Malloc(int size)
 * static void  Progmkl43Free(void *ptr)
 * input:  void
 * output: void
 *
 * description:
 *	
 */
static void *
Progmkl43Malloc(int size)
{
#if PROGMKL43_USE_MALLOC
  return malloc(size);
#else
  return PROG_NULL;
#endif
}
static void
Progmkl43Free(void *ptr)
{
#if PROGMKL43_USE_MALLOC
  free(ptr);
#endif
  return;
}
#endif


/*
 * static int Progmkl43SetCallback(int unit, void *ptr)
 * static int Progmkl43ResetCallback(int unit, void *ptr)
 * static int Progmkl43IsCallback(int unit, void *ptr)
 * static int Progmkl43GoCallback(int unit, int req, void *ptr, int size)
 * input:  void
 * output: void
 *
 * description:
 *
 */
static int
Progmkl43SetCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgmkl43Unit	*sc;

  sc = progmkl43.sc[unit];

  result = PROG_ERRNO_NOSPACE;

  if(Progmkl43IsCallback(unit, ptr) == PROG_ERRNO_SUCCESS) {
    result = PROG_ERRNO_SUCCESS;
    goto end;
  }

  for(i = 0; i < PROGMKL43_NUMOFCALLBACK; i++) {
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
Progmkl43ResetCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgmkl43Unit	*sc;

  sc = progmkl43.sc[unit];

  result = PROG_ERRNO_PARAMEXCEEDED;

  for(i = 0; i < PROGMKL43_NUMOFCALLBACK; i++) {
    if((void *)sc->pCb[i] == ptr) {
      sc->pCb[i] = PROG_NULL;
      result = PROG_ERRNO_SUCCESS;
      break;
    }
  }

  return result;
}
static int
Progmkl43IsCallback(int unit, void *ptr)
{
  int			result;
  int			i;

  struct _stProgmkl43Unit	*sc;

  sc = progmkl43.sc[unit];

  result = PROG_ERRNO_NOSPACE;

  for(i = 0; i < PROGMKL43_NUMOFCALLBACK; i++) {
    if((void *)sc->pCb[i] == ptr) {
      result = PROG_ERRNO_SUCCESS;
      break;
    }
  }

  return result;
}
static int
Progmkl43GoCallback(int unit, int req, void *ptr, int size)
{
  int			result;
  int			i;

  struct _stProgmkl43Unit	*sc;

  sc = progmkl43.sc[unit];

  result = PROG_ERRNO_SUCCESS;

  for(i = 0; i < PROGMKL43_NUMOFCALLBACK; i++) {
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
 * static int Progmkl43ConnectDevice(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *	
 *
 */
static int
Progmkl43ConnectDevice(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgmkl43Unit	*psc;
  uint32_t	params[PROGKL43_NUM_OF_PARAMS];
  int		cnt;

  int		n, i;

  psc = progmkl43.sc[unit];

#if 0
  if((result = Progmkl43GoCallback(unit, PROG_CB_GPIO_INIT, PROG_NULL, 0)) !=
     PROG_ERRNO_SUCCESS) {
    goto fail;
  }
#endif

  n = psc->modeSerial;
  result = Progmkl43GoCallback(unit, PROG_CB_SERIAL_INIT, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  n = 115200;
  result = Progmkl43GoCallback(unit, PROG_CB_SERIAL_SETBAUD, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  n = CS8;	/* CS8, !PARENB, !PARENB, !CSTOPB */
  result = Progmkl43GoCallback(unit, PROG_CB_SERIAL_SETTERMINAL, &n, sizeof(n));
  if(result < PROG_ERRNO_SUCCESS) goto fail;

  i = 0;
  while(1) {
    result = Progmkl43CmdPing(unit);
    if(result == PROG_ERRNO_SUCCESS) break;
    else if(i > 10) goto fail;
    sleep(1);
    i++;
  }

  params[0] = API_PROPERTY_AVAILABLE_COMMANDS;
  cnt = PROGKL43_NUM_OF_PARAMS;
  result = Progmkl43CmdGetProperty(unit, params, &cnt);
  if(result < PROG_ERRNO_SUCCESS) goto fail;
  Progmkl43StoreSupportedCommand(psc, params[1]);
  
  result = PROG_ERRNO_SUCCESS;
fail:
  return result;
}


/*
 * static int Progmkl43DisconnectDevice(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43DisconnectDevice(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgmkl43Unit	*psc;
  int		n;

  psc = progmkl43.sc[unit];
  n = psc->modeSerial;

  Progmkl43GoCallback(unit, PROG_CB_SERIAL_UNINIT, &n, sizeof(n));

  Progmkl43GoCallback(unit, PROG_CB_GPIO_UNINIT, PROG_NULL, 0);

  return result;
}


/*
 * static int Progmkl43Reset(int unit)
 * input:  unit number
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43Reset(int unit)
{
  int				result = PROG_ERRNO_UNKNOWN;
  struct _stProgData		d;

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43Reset()\n", __FILE__, __LINE__);
#endif

  d.start = 0x08000000;
  result = Progmkl43CmdGo(unit, &d, sizeof(d));
  
  return result;
}




/*
 * static int Progmkl43EraseChip(int unit)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43EraseChip(int unit)
{
  int		result = PROG_ERRNO_UNKNOWN;

  struct _stProgData		d;


  memset(&d, 0, sizeof(d));

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43EraseChip()\n", __FILE__, __LINE__);
#endif

  result = Progmkl43CmdFlashEraseAll(unit);

  return result;
}


/*
 * static int Progmkl43EraseBlock(int unit, struct _stProgData *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43EraseBlock(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43EraseBlock()\n", __FILE__, __LINE__);
#endif

  result = Progmkl43CmdEraseRegion(unit, ptr, size);

  return result;
}


/*
 * static int Progmkl43Program(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43Program(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  int		packets = 0;
#endif

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43Program()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progmkl43CmdWriteMemory(unit, ptr, size);

fail:  
  return result;
}


/*
 * static int Progmkl43Read(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43Read(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  int		packets = 0;
#endif

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43Read()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progmkl43CmdReadMemory(unit, ptr, size);

fail:  
  return result;
}


/*
 * static int Progmkl43Verify(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43Verify(int unit, struct _stProgData *ptr, int size)
{
  int			result = PROG_ERRNO_UNKNOWN;

  struct _stProgData	d;
  uint32_t		crc;
  int			pos;

  struct _stProgmkl43Unit	*psc;

  psc = progmkl43.sc[unit];

  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43Verify()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  d.start = ptr->start;
  d.end   = ptr->end;
  d.size  = ptr->size;

  if(psc->supportedCmd & CMDTAG_MASK(API_CMDTAG_FLASH_CHECK_CRC)) {
    result = ProgApi(unit, PROG_API_GETCRC, &d, sizeof(d));
    if(result == PROG_ERRNO_SUCCESS) {

      crc = 0;
      crc = Crc32Calc(crc, ptr->ptr, (ptr->size + 0x7ffUL) & ~0x7ffUL);

      if(d.crc != crc) result = PROG_ERRNO_DATACHECK;
    }
  }

  if(result != PROG_ERRNO_SUCCESS &&
     (psc->supportedCmd & CMDTAG_MASK(API_CMDTAG_READ_MEMORY))) {
    int		sz;
    uint8_t	buf[PROGMKL43_ERASE_SECTOR_BYTES];
    struct _stProgData	d;

    d.ptr = buf;
    d.start = ptr->start;
    sz = ptr->size;

    pos = 0;
    while(sz > 0) {
      d.size = sizeof(buf);
      if(sz < sizeof(buf)) d.size = sz;
      d.end = d.start + d.size;

      result = Progmkl43CmdReadMemory(unit, &d, sizeof(d));
      if(result < PROG_ERRNO_SUCCESS) {
	break;
      }
      if(memcmp(buf, &ptr->ptr[pos], d.size)) {
	result = PROG_ERRNO_DATACHECK;
	break;
      }

      sz      -= sizeof(buf);
      d.start += sizeof(buf);
      pos     += sizeof(buf);
    }
  }

fail:
  return result;
}


/*
 * static int Progmkl43CheckBlockBlank(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43CheckBlockBlank(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		i, sz;

  uint8_t	buf[PROGMKL43_ERASE_SECTOR_BYTES];
  struct _stProgData	d;

  struct _stProgmkl43Unit	*psc;

  psc = progmkl43.sc[unit];

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43CheckBlockBlank()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  if(psc->supportedCmd & CMDTAG_MASK(API_CMDTAG_FLASH_BLANK_CHECK)) {
    result = Progmkl43CmdCheckBlank(unit, ptr, size);

  } else if(psc->supportedCmd & CMDTAG_MASK(API_CMDTAG_READ_MEMORY)) {
    d.ptr = buf;
    d.start = ptr->start;
    sz = ptr->size;

    while(sz > 0) {
      d.size = sizeof(buf);
      if(sz < sizeof(buf)) d.size = sz;
      d.end = d.start + d.size;

      result = Progmkl43CmdReadMemory(unit, &d, sizeof(d));
      if(result < PROG_ERRNO_SUCCESS) {
	break;
      }
   
      for(i = 0; i < d.size; i++) {
	if(d.ptr[i] !=0xff) {
	  if((d.start + i) != PROGMKL43_FSEC_ADDR) {
	    sz = 0;
	    result = PROG_ERRNO_DATACHECK;
	    break;
	  }
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
 * static int Progmkl43GetSignature(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43GetSignature(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetSignature()\n", __FILE__, __LINE__);
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
 * static int Progmkl43GetVersion(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43GetVersion(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetVersion()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  /*Progmkl43CmdGetInfoVersion(unit, ptr, size);*/
  
  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progmkl43GetInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43GetInfo(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetInfo()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progmkl43CmdGetInfo(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progmkl43GetAddrInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43GetAddrInfo(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  struct _stProgData		*addr;

#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetAddrInfo()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  addr = (struct _stProgData *)ptr;

  result = PROG_ERRNO_SUCCESS;
  switch(addr->mode) {
  case	MAIN_FILE_TARGET_BOOTLOADER:
    addr->start = 0x00000;
    addr->end   = 0x07800;
    break;
  case	MAIN_FILE_TARGET_FIRMWARE:
    addr->start = 0x08000;
    addr->end   = 0x40000;
    break;
  case	MAIN_FILE_TARGET_USERAREA:
    addr->start = 0x07800;
    addr->end   = 0x08000;
    break;
  case	MAIN_FILE_TARGET_ALLAREA:
    addr->start = 0x00000;
    addr->end   = 0x40000;
    break;
  default:
    addr->start = 0x00000;
    addr->end   = 0x40000;
    result = PROG_ERRNO_UNKNOWN;
    break;
  }
  addr->size = addr->end - addr->start;
  addr->sizeBlock = PROGMKL43_ERASE_SECTOR_BYTES;
  addr->sizeEraseSector = PROGMKL43_ERASE_SECTOR_BYTES;

  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


/*
 * static int Progmkl43GetCrc(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43GetCrc(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetCrc()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progmkl43CmdGetCrc(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progmkl43GetInfo(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43CheckBlank(int unit, void *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43CheckBlank()\n", __FILE__, __LINE__);
#endif

  if(!ptr) {
    result = PROG_ERRNO_INVALIDARG;
    goto fail;
  }

  result = Progmkl43CmdCheckBlank(unit, ptr, size);
  
  /*result = PROG_ERRNO_SUCCESS;*/

fail:
  return result;
}


/*
 * static int Progmkl43GetCheckSum(int unit, struct _stProgData *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43GetCheckSum(int unit, struct _stProgData *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43GetCheckSum()\n", __FILE__, __LINE__);
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
 * static int Progmkl43SetSecurity(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *
 */
static int
Progmkl43SetSecurity(int unit, unsigned char *ptr, int size)
{
  int				result = PROG_ERRNO_UNKNOWN;
  
#if	PROGMKL43_DEBUG_SHOWCOMMAND
  printf("%s:%d: Progmkl43SetSecurity()\n", __FILE__, __LINE__);
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
 * static int Progmkl43SetSerialMode(int unit, void *ptr, int size)
 * input:  unit number
 *         
 * output: status
 *
 * description:
 *	
 */
static int
Progmkl43SetSerialMode(int unit, int *pMode, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stProgmkl43Unit	*psc;
  int		mode;

  psc = progmkl43.sc[unit];

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
Progmkl43CmdPing(int unit)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;
  int			re;
  struct _stMkl43Pkt	pktTx;
  struct _stMkl43PktPingRes	pktRx;

  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_PING, 0, 0, NULL, 0);

#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow(&pktTx, "<<Tx");
#endif
  Progmkl43GoCallback(unit, PROG_CB_SERIAL_SENDDATA, &pktTx, 2);

  re = Progmkl43GoCallback(unit, PROG_CB_SERIAL_RECVDATA, &pktRx, 10);
  if(re != 10) goto fail;
#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow((struct _stMkl43Pkt *)&pktRx, ">>Rx");
#endif

  result = PROGMKL43_RESULT_SUCCESS;

fail:
  return result;
}


static int
Progmkl43CmdGetProperty(int unit, uint32_t *params, int *cnt)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;
  int			re;
  struct _stMkl43Pkt	pktTx;
  struct _stMkl43Pkt	pktRx;

  /*int			type, tag;*/

  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_COMMAND, 0,
			  API_CMDTAG_GET_PROPERTY, params, 1);
  re = Progmkl43SendPktWaitAck(unit, &pktTx);
  if(re < 0) goto fail;

  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, cnt);

  if(re >= 0) {
    result = PROGMKL43_RESULT_SUCCESS;
  }

fail:
  return result;
}


static int
Progmkl43CmdFlashEraseAll(int unit)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;
  int			re;
  struct _stMkl43Pkt	pktTx;
  struct _stMkl43Pkt	pktRx;
  uint32_t		params[PROGKL43_NUM_OF_PARAMS];
  int			cnt;

  /*int			type, tag;*/

  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  re = Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_COMMAND, 0,
			       API_CMDTAG_FLASH_ERASE_ALL, NULL, 0);
  if(re < 0) goto fail;
  Progmkl43SendPktWaitAck(unit, &pktTx);

  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);

  if(re >= 0) {
    result = PROGMKL43_RESULT_SUCCESS;
  }

fail:
  return result;
}


static int
Progmkl43CmdEraseRegion(int unit, struct _stProgData *ptr, int size)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;

  struct _stMkl43Pkt	pktTx;
  struct _stMkl43Pkt	pktRx;

  int			re;
  uint32_t		params[PROGKL43_NUM_OF_PARAMS];
  int			cnt;
  uint32_t		start, end;
  int			sz;
  int			szSector;

  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  szSector = PROGMKL43_ERASE_SECTOR_BYTES;
  start = ptr->start & ~(szSector-1);
  end   = ptr->start + ptr->size;
  sz    = (end - start+(szSector-1)) & ~(szSector-1);

  params[0] = start;
  params[1] = sz;
  cnt = 2;
  re = Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_COMMAND, 0,
			       API_CMDTAG_FLASH_ERASE_REGION, params, cnt);
  if(re < 0) goto fail;

  re = Progmkl43SendPktWaitAck(unit, &pktTx);
  if(re < 0) goto fail;

  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);

  if(cnt >= 2 && params[0] == API_ERROR_SUCCESS) {
    result = PROGMKL43_RESULT_SUCCESS;
  } else {
    Progmkl43ShowErrorCode(params[0]);
  }

fail:
  return result;
}


static int
Progmkl43CmdReadMemory(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stMkl43Pkt	pktTx;
  struct _stMkl43Pkt	pktRx;
  int			re;
  int			cnt;
  uint32_t		params[7];
  int			type, szData, szRecv;
  uint8_t		*pData;

  if(!ptr) goto fail;

#if 0
  fprintf(stderr, "Progmkl43CmdReadMemory(): unit:%d start:%x size:%x\n",
	  unit, ptr->start, ptr->size);
#endif

  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  /* send READ_MEMORY command with address and size info */
  params[0] = ptr->start;
  params[1] = ptr->size;
  cnt = 2;
  re = Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_COMMAND, 0,
			       API_CMDTAG_READ_MEMORY, params, cnt);
  if(re < 0) goto fail;
  Progmkl43SendPktWaitAck(unit, &pktTx);

  /* recv the generic response for command */
  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);
  if(cnt < 2 || params[0] != 0) {
    Progmkl43ShowErrorCode(params[0]);
#if 0
    Progmkl43ShowErrorCode(params[0]);
    switch(params[0]) {
    case	kStatus_UnknownCommand:
      fprintf(stderr, "  Unknown command\n");
      break;
    case	kStatus_SecurityViolation:
      fprintf(stderr, "  Security violation\n");
      break;
    case	kStatus_AbortDataPhase:
      fprintf(stderr, "  Abort data phase\n");
      break;
    case	kStatus_NoRespons:
      fprintf(stderr, "  No response\n");
      break;
    case	kStatus_NoResponseExpected:
      fprintf(stderr, "  No response expected\n");
      break;
    }
#endif
    goto fail;
  }

  szRecv = ptr->size;
  pData  = ptr->ptr;
  while(szRecv > 0) {
    /* recv data packet */
    re = Progmkl43RecvPktSendAck(unit, &pktRx);
    if(re < 0) goto fail;
    Progmkl43PacketDisassemble(&pktRx, &type, &szData,
			       NULL, (uint32_t *)pData, NULL);

    pData  += szData;
    szRecv -= szData;
  }

  /* recv the final generic response */
  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);

  result = PROGMKL43_RESULT_SUCCESS;

fail:
  return result;
}


static int
Progmkl43CmdWriteMemory(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct _stMkl43Pkt	pktTx;
  struct _stMkl43Pkt	pktRx;
  int			re;
  int			cnt;
  uint32_t		params[7];
  int			szData, szSend;
  uint8_t		*pData;

  if(!ptr) goto fail;

#if 0
  fprintf(stderr, "Progmkl43CmdWriteMemory(): unit:%d start:%x size:%x\n",
	  unit, ptr->start, ptr->size);
#endif
  if(unit > PROGMKL43_NUM_OF_UNIT) {
    result = PROGMKL43_RESULT_INVALIDARG;
    goto fail;
  }

  /* send READ_MEMORY command with address and size info */
  params[0] = ptr->start;
  params[1] = ptr->size;
  cnt = 2;
  re = Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_COMMAND, 0,
			       API_CMDTAG_WRITE_MEMORY, params, cnt);
  if(re < 0) goto fail;
  Progmkl43SendPktWaitAck(unit, &pktTx);

  /* recv the generic response for command */
  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);

  szSend = ptr->size;
  pData  = ptr->ptr;
  szData = PROGMKL43_PACKET_PAYLOAD_SIZE;
  while(szSend > 0) {
    Progmkl43PacketAssemble(&pktTx, PROGMKL43_PACKET_TYPE_DATA, szData,
			    0, (uint32_t *)pData, 0);

    re = Progmkl43SendPktWaitAck(unit, &pktTx);
    if(re < 0) goto fail;

    pData  += szData;
    szSend -= szData;
  }

  /* recv the final generic response */
  re = Progmkl43RecvPktSendAck(unit, &pktRx);
  if(re < 0) goto fail;
  Progmkl43PacketDisassemble(&pktRx, NULL, NULL, NULL, params, &cnt);

  result = PROGMKL43_RESULT_SUCCESS;

fail:
  return result;
}


static int
Progmkl43CmdGo(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}






#if 1

static int
Progmkl43CmdGetInfo(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}


#if 0
static int
Progmkl43CmdGetInfoVersion(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}


static int
Progmkl43CmdGetInfoFirmwareVersion(int unit, uint8_t *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}
#endif

static int
Progmkl43CmdGetCrc(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}


static int
Progmkl43CmdCheckBlank(int unit, struct _stProgData *ptr, int size)
{
  int		result = PROG_ERRNO_UNKNOWN;
  return result;
}
#endif





/******************************************************************
 * basic packet send and recv
 ******************************************************************/
static int
Progmkl43PacketAssemble(struct _stMkl43Pkt *pkt, int type, int size, int tag, uint32_t *params, int cnt)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;

  int			i;
  uint8_t		*ptr;
  uint16_t		crc16;
  crc16_data_t		crc16Conf;
  int			sz;		/* total bytes */

  if(pkt == NULL) goto fail;

  /*memset(pkt, 0, sizeof(*pkt));*/

  ptr = (uint8_t *)pkt;

  /* assemble the framing */
  pkt->frame.start    = PROGMKL43_PACKET_START_BYTE;
  pkt->frame.type     = type;

  if(type == PROGMKL43_PACKET_TYPE_DATA) {
    if(size > PROGMKL43_PACKET_PAYLOAD_SIZE) goto fail;
    memcpy(pkt->data, params, size);
    pkt->frame.size[0] = size & 0xff;
    pkt->frame.size[1] = size >> 8;
    sz = size;

  } else {
    if(cnt > PROGKL43_NUM_OF_PARAMS) goto fail;

    sz = 4 + 4*cnt;
    pkt->frame.size[0] = sz & 0xff;
    pkt->frame.size[1] = sz >> 8;

    /* assemble the command */
    pkt->cmdres.tag   = tag;
    pkt->cmdres.flags = 0;
    pkt->cmdres.reserved = 0;
    pkt->cmdres.count = cnt;
    if(params != NULL && cnt > 0) {
      for(i = 0; i < cnt; i++) {
	pkt->cmdres.param[i][0] =  params[i]        & 0xff;
	pkt->cmdres.param[i][1] = (params[i] >>  8) & 0xff;
	pkt->cmdres.param[i][2] = (params[i] >> 16) & 0xff;
	pkt->cmdres.param[i][3] = (params[i] >> 24) & 0xff;
      }
    }
  }

  crc16_init(&crc16Conf);
  crc16_update(&crc16Conf, ptr, 4);
  crc16_update(&crc16Conf, ptr+6, sz);
  crc16_finalize(&crc16Conf, &crc16);
  pkt->frame.crc16[0] = crc16 & 0xff;
  pkt->frame.crc16[1] = crc16 >> 8;

  result = PROGMKL43_RESULT_SUCCESS;
fail:
  return result;
}


static int
Progmkl43PacketDisassemble(struct _stMkl43Pkt *pkt, int *type, int *size, int *tag, uint32_t *params, int *pCnt)
{
  int			result = PROGMKL43_RESULT_UNKNOWN;

  int			cnt;
  int			i;
  int			sz;

  unsigned long		param;

  if(!pkt) goto fail;

  if(type) *type = pkt->frame.type;
  sz  =  pkt->frame.size[0]       & 0x00ff;
  sz |= (pkt->frame.size[1] << 8) & 0xff00;
  if(size) *size = sz;
  if(pkt->frame.type == PROGMKL43_PACKET_TYPE_DATA) {
    memcpy(params, pkt->data, sz);

  } else {
    if(tag)  *tag  = pkt->cmdres.tag;
    if(params && pCnt) {
      cnt = pkt->cmdres.count;
      if(cnt > *pCnt) cnt = *pCnt;

      for(i = 0; i < cnt; i++) {
	param  =  pkt->cmdres.param[i][0]        & 0x000000ff;
	param |= (pkt->cmdres.param[i][1] <<  8) & 0x0000ff00;
	param |= (pkt->cmdres.param[i][2] << 16) & 0x00ff0000;
	param |= (pkt->cmdres.param[i][3] << 24) & 0xff000000;
	params[i] = param;
      }
      *pCnt = cnt;
    }
  }

  result = PROGMKL43_RESULT_SUCCESS;
fail:
  return result;
}


static int
Progmkl43SendPktWaitAck(int unit, struct _stMkl43Pkt *pkt)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		size;
  struct _stMkl43Pkt	pktRx;
  
  if(!pkt) goto fail;

  size  =  pkt->frame.size[0]       & 0x00ff;
  size |= (pkt->frame.size[1] << 8) & 0xff00;
  size += sizeof(pkt->frame);

#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow(pkt, "<<Tx");
#endif
  Progmkl43GoCallback(unit, PROG_CB_SERIAL_SENDDATA, pkt, size);

  Progmkl43GoCallback(unit, PROG_CB_SERIAL_RECVDATA, &pktRx, 2);
#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow(&pktRx, ">>Rx");
#endif
  if(pktRx.frame.type == PROGMKL43_PACKET_TYPE_ACK) {
    result = PROG_ERRNO_SUCCESS;
  }

fail:
  return result;
}
static int
Progmkl43RecvPktSendAck(int unit, struct _stMkl43Pkt *pkt)
{
  int		result = PROG_ERRNO_UNKNOWN;
  int		size;
  /*struct _stMkl43Pkt	pktRx;*/
  struct _stMkl43Pkt	pktAck;
#if 0
  const uint8_t		ack[] = {PROGMKL43_PACKET_START_BYTE,
				 PROGMKL43_PACKET_TYPE_ACK};
#endif

  if(!pkt) goto fail;

  Progmkl43GoCallback(unit, PROG_CB_SERIAL_RECVDATA,
		      &pkt->frame, sizeof(pkt->frame));
  size  =  pkt->frame.size[0]       & 0x00ff;
  size |= (pkt->frame.size[1] << 8) & 0xff00;
  Progmkl43GoCallback(unit, PROG_CB_SERIAL_RECVDATA, &pkt->cmdres, size);
#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow(pkt, ">>Rx");
#endif  

  pktAck.frame.start = PROGMKL43_PACKET_START_BYTE;
  pktAck.frame.type = PROGMKL43_PACKET_TYPE_ACK;
#if	PROGSTM32_DEBUG_SHOWALLPACKETS
  Progmkl43PacketShow(&pktAck, "<<Tx");
#endif  
  Progmkl43GoCallback(unit, PROG_CB_SERIAL_SENDDATA, &pktAck, 2);

  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


#if	PROGSTM32_DEBUG_SHOWALLPACKETS
static void
Progmkl43PacketShow(struct _stMkl43Pkt *pkt, char *str)
{
  struct _stMkl43PktPingRes  *ping;
  uint8_t	*ptr;
  uint32_t	size;
  uint16_t	crc16;
  crc16_data_t	crc16Conf;
  int		i;
  uint32_t	val;
  
  ptr = (uint8_t *)pkt;


  if(pkt->frame.type == PROGMKL43_PACKET_TYPE_PING_RESPONSE) {
    crc16_init(&crc16Conf);
    crc16_update(&crc16Conf, ptr, 8);
    crc16_finalize(&crc16Conf, &crc16);

    printf("%s\n", str);
    ping = (struct _stMkl43PktPingRes *) pkt;
    printf("  start: %2x", ping->start);
    printf("  type: %2x", ping->type);
    printf(", bugfix: %x", ping->bugfix);
    printf(", major: %x", ping->major);
    printf(", minor: %x\n", ping->minor);
    printf("  name: %02x", ping->name);
    printf(", opt: %02x%02x", ping->option[1], ping->option[0]);
    printf(", crc16: %02x%02x (calc: %04x)\n",
	   ping->crc16[1],  ping->crc16[0], crc16);
  } else {
    printf("%s\n", str);
    size =  (pkt->frame.size[1]  << 8) | pkt->frame.size[0];
    crc16 = (pkt->frame.crc16[1] << 8) | pkt->frame.crc16[0];
    printf("  framing ");
    printf(" start: %2x", pkt->frame.start);
    printf(", type: %2x", pkt->frame.type);
    if(pkt->frame.type > PROGMKL43_PACKET_TYPE_ACK_ABOART &&
       pkt->frame.type != PROGMKL43_PACKET_TYPE_PING) {
      printf(", size: %x", size);
      printf(", crc16: %04x\n", crc16);
    }
    if(pkt->frame.type == PROGMKL43_PACKET_TYPE_COMMAND) {
      printf("  command ");
      printf(" tag: %2x", pkt->cmdres.tag);
      printf(", flags: %2x", pkt->cmdres.flags);
      printf(", reserved: %2x", pkt->cmdres.reserved);
      printf(", count: %2x", pkt->cmdres.count);
      printf("\n  params: ");
      for(i = 0; i < pkt->cmdres.count; i++) {
	val  = pkt->cmdres.param[i][0];
	val |= pkt->cmdres.param[i][1] <<  8;
	val |= pkt->cmdres.param[i][2] << 16;
	val |= pkt->cmdres.param[i][3] << 24;
	printf(" %08x", val);
      }
    } else if(pkt->frame.type == PROGMKL43_PACKET_TYPE_DATA) {
      printf("  data:");
      for(i = 0; i < size; i++) {
	if(i && !(i&3)) printf(" ");
	if(i == 0x10) printf("\n       ");
	printf(" %02x", pkt->data[i]);
      }
    }

    printf("\n");
  }
 
  return;
}
#endif





static void
Progmkl43StoreSupportedCommand(struct _stProgmkl43Unit *psc,
			       uint32_t val)
{
  psc->supportedCmd = val;
  printf("  supported commands(0x%x) are\n", val);

  printf("        ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_ERASE_ALL)) printf("EraseAll, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_ERASE_REGION)) printf("EraseRegion, ");
  if(val & CMDTAG_MASK(API_CMDTAG_READ_MEMORY)) printf("Read, ");
  if(val & CMDTAG_MASK(API_CMDTAG_WRITE_MEMORY)) printf("Write, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FILL_MEMORY)) printf("Fill, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_SECURITY_DISABLE)) printf("SecurityDis, ");
  if(val & CMDTAG_MASK(API_CMDTAG_GET_PROPERTY)) printf("GetProp, ");
  printf("\n        ");

  if(val & CMDTAG_MASK(API_CMDTAG_RECEIVE_SB_FILE)) printf("RecvSb, ");
  if(val & CMDTAG_MASK(API_CMDTAG_EXECUTE)) printf("Exec, ");
  if(val & CMDTAG_MASK(API_CMDTAG_CALL)) printf("Call, ");
  if(val & CMDTAG_MASK(API_CMDTAG_RESET)) printf("Reset, ");
  if(val & CMDTAG_MASK(API_CMDTAG_SET_PROPERTY)) printf("SetProp, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_ERASE_ALL_UNSECURE)) printf("EraseAllUnsec, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_PROGARM_ONCE)) printf("ProgOnce, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_RESET_ONCE)) printf("ResetOnce, ");
  printf("\n        ");

  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_READ_RESOURCE)) printf("ReadResources, ");
  if(val & CMDTAG_MASK(API_CMDTAG_RESERVED11)) printf("Reserved11, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_BLANK_CHECK)) printf("Blank, ");
  if(val & CMDTAG_MASK(API_CMDTAG_FLASH_CHECK_CRC)) printf("CheckCrc, ");
  printf("\n");
   
  return;
}


static void
Progmkl43ShowAllProperties(int unit)
{
  int		i, j;
  uint32_t	params[PROGKL43_NUM_OF_PARAMS];
  int		cnt;

  printf("# GetProperty informations\n");
  for(i = 1; i <= API_PROPERTY_END_OF_DEFAULT; i++) {
    params[0] = i;
    memset(&params[1], 0, (PROGKL43_NUM_OF_PARAMS-1)*sizeof(uint32_t));
    cnt = PROGKL43_NUM_OF_PARAMS;
    Progmkl43CmdGetProperty(unit, params, &cnt);
    printf("%4x: %6d", i, params[0]);
    for(j = 1; j < cnt; j++) {
      printf(" %08x", params[j]);
    }
    printf("\n");
  }
  for(i = API_PROPERTY_STAART_OF_EXTENDED; i <= API_PROPERTY_END_OF_EXTENDED; i++) {
    params[0] = i;
    memset(&params[1], 0, (PROGKL43_NUM_OF_PARAMS-1)*sizeof(uint32_t));
    cnt = PROGKL43_NUM_OF_PARAMS;
    Progmkl43CmdGetProperty(unit, params, &cnt);
    printf("%4x: %6d", i, params[0]);
    for(j = 1; j < cnt; j++) {
      printf(" %08x", params[j]);
    }
    printf("\n");
  }

  return;
}


static void
Progmkl43ShowErrorCode(int param)
{
  switch(param) {
  case	kStatus_UnknownCommand:
    fprintf(stderr, "  Unknown command\n");
    break;
  case	kStatus_SecurityViolation:
    fprintf(stderr, "  Security violation\n");
    break;
  case	kStatus_AbortDataPhase:
    fprintf(stderr, "  Abort data phase\n");
    break;
  case	kStatus_NoResponse:
    fprintf(stderr, "  No response\n");
    break;
  case	kStatus_NoResponseExpected:
    fprintf(stderr, "  No response expected\n");
    break;
  }

  return;
}

/*
 * end of file
 */

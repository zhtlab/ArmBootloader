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

#ifndef	_PROG_H_
#define	_PROG_H_

/*******************************************************************
 * definition
 */
#define	PROG_ERRNO_SUCCESS		0
#define	PROG_ERRNO_UNKNOWN		(-1)	/* unknow error */
#define	PROG_ERRNO_NOMEMORY		(-2)	/* can't allocate memory */
#define	PROG_ERRNO_NOSPACE		(-3)	/* no space in the system */
#define	PROG_ERRNO_INVALIDARG		(-4)	/* invalid argument */
#define	PROG_ERRNO_PARAMEXCEEDED	(-5)	/* parameter exceeded */
#define	PROG_ERRNO_PERM			(-6)	/* operation not permitted */
#define	PROG_ERRNO_TIMEOUT		(-7)	/* time out */
#define	PROG_ERRNO_DATACHECK		(-8)	/* data check error */
#define	PROG_ERRNO_COMMUNICATION	(-9)	/* busy */
#define	PROG_ERRNO_NACK_RECV		(-10)   /* nack received */
#define	PROG_ERRNO_NOT_SUPPORTED	(-11)   /* command not supported */
/* network */
#define	PROG_ERRNO_SOCKET		(-100)
#define	PROG_ERRNO_CONNECT		(-101)
#define	PROG_ERRNO_BIND			(-102)
#define	PROG_ERRNO_ACCEPT		(-103)
#define	PROG_ERRNO_LISTEN		(-104)

#define	PROG_ERRNO_EXTEND		(-4096)



/* api number */
#define	PROG_API_INITIALIZE		0x0101
#define	PROG_API_UNINITIALIZE		0x0102
#define	PROG_API_INITIALIZE_UNIT	0x0103
#define	PROG_API_UNINITIALIZE_UNIT	0x0104
#define	PROG_API_SETCALLBACK		0x0108
#define	PROG_API_RESETCALLBACK		0x0109
#define	PROG_API_HEARTBEAT		0x0110
#define	PROG_API_SETRESETWIDTH		0x0111

#define	PROG_API_CONNECTDEVICE		0x0291
#define	PROG_API_DISCONNECTDEVICE	0x0292

#define	PROG_API_SETOSCFREQ		0x0203


#define	PROG_API_RESET			0x0202
#define	PROG_API_ERASECHIP		0x0204
#define	PROG_API_ERASEBLOCK		0x0205
#define	PROG_API_PROGRAM		0x0206
#define	PROG_API_VERIFY			0x0207
#define	PROG_API_CHECKBLOCKBLANK	0x0208
#define	PROG_API_GETSIGNATURE		0x0209
#define		PROG_LEN_SIGNATURE		19
#define	PROG_API_GETVERSION		0x020a
#define		PROG_LEN_VERSION		6
#define	PROG_API_GETCHECKSUM		0x020b
#define	PROG_API_SETSECURITY		0x020c
#define		PROG_DATA_SECURITY_MASK	0x17
#define		PROG_DATA_SECURITY_BASE	0xe8
#define		PROG_DATA_SECURITY_PROGRAMBOOT	(1 << 4)
#define		PROG_DATA_SECURITY_PROGRAM	(1 << 2)
#define		PROG_DATA_SECURITY_ERASEBLOCK	(1 << 1)
#define		PROG_DATA_SECURITY_ERASECHIP	(1 << 0)
#define		PROG_DATA_SECURITY_ENABLEALL	0xff
#define	PROG_API_READ			0x020d
#define	PROG_API_GETINFO		0x020e
#define	PROG_API_GETADDRINFO		0x020f
#define	PROG_API_GETCRC			0x0210
#define	PROG_API_CHECK_BLANK		0x0211

#define	PROG_API_SETMODELTYPE	0x0301
#define		MODELTYPE_NONE		0
#define		MODELTYPE_KINETIS	1
#define		MODELTYPE_LPC		2
#define		MODELTYPE_STM32	3
#define		MODELTYPE_END		4
#define	PROG_API_SETSERIALMODE	0x0302
#define		SERIALMODE_NONE		0
#define		SERIALMODE_UART		1
#define		SERIALMODE_I2C		2
#define		SERIALMODE_SPI		3
#define		SERIALMODE_USB		4
#define		SERIALMODE_END		5


/* callback number */
/*#define	PROG_CB_			0x0000*/
#define	PROG_CB_GPIO_INIT		0x8101
#define	PROG_CB_GPIO_UNINIT		0x8102
#define	PROG_CB_WAIT_MICROSECONDS	0x810f
#define	PROG_CB_GPIO_SETRESETPORT	0x8111
#define	PROG_CB_GPIO_SETFLMD0PORT	0x8112

#define	PROG_CB_SERIAL_INIT		0x8201
#define	PROG_CB_SERIAL_UNINIT		0x8202
#define	PROG_CB_SERIAL_SETBAUD		0x8203
#define	PROG_CB_SERIAL_SETTERMINAL	0x8204
#define	PROG_CB_SERIAL_SENDDATA		0x8211
#define	PROG_CB_SERIAL_RECVDATA		0x8212

#define	PROG_CB_I2C_INIT		0x8301
#define	PROG_CB_I2C_UNINIT		0x8302
#define	PROG_CB_I2C_SETSPEEDUD		0x8303
#define	PROG_CB_I2C_SENDDATA		0x8311
#define	PROG_CB_I2C_RECVDATA		0x8312

#define	PROG_CB_SPI_INIT		0x8401
#define	PROG_CB_SPI_UNINIT		0x8402
#define	PROG_CB_SPI_SETBAUD		0x8403
#define	PROG_CB_SPI_SETTERMINAL		0x8404
#define	PROG_CB_SPI_SENDDATA		0x8411
#define	PROG_CB_SPI_RECVDATA		0x8412

#define	PROG_CB_USBHID_INIT		0x8501
#define	PROG_CB_USBHID_UNINIT		0x8502
#define	PROG_CB_USBHID_SENDDATA		0x8511
#define	PROG_CB_USBHID_RECVDATA		0x8512

#define	PROG_NULL			((void *) 0)


struct _stProgData {
  int			mode;	/* boot, firm, user, allarea */
  unsigned int		start;
  unsigned int		end;
  unsigned char		*ptr;		/* data ptr */
  int			size;		/* data size */
  int			cnt;		/* count of entry */
  unsigned short	sum;		/* sum */
  unsigned long		crc;		/* crc */
  int			sizeBlock;	/* block size */
  int			sizeEraseSector;	/* erase sector size */
};


int		ProgApi(int unit, int req, void *ptr, int size);


#endif

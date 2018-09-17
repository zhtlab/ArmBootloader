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

#define	_SERIAL_C_


#define	DEVICE_DEBUG_SHOW_RESETPORT	0
#define	DEVICE_DEBUG_SHOW_DATA		0

#define	DEVICE_CONTROL_PORT_RTS_DTR	1


#if	DEVICE_DEBUG_SHOW_RESETPORT || DEVICE_DEBUG_SHOW_DATA
#include	<stdio.h>
#endif

#include	<stdint.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/uio.h>
#include        <sys/select.h>
#include	<unistd.h>
#include        <sys/ioctl.h>
#include	<termios.h>
#include	<errno.h>

#include	<string.h>


#include	"device.h"
#include	"prog.h"


static int		s = -1;

struct termios	tcBackup;

extern char	nameSerialDev[];

int
DeviceOpenCheckSerial(char *nameSerialDev)
{
  int		d = 0;

  if((d = open(nameSerialDev, O_RDWR | O_NONBLOCK)) < 0) {
    goto end;
  }
  close(d);

end:
  return d;
}


int
DeviceInitSerial(int unit, char *nameSerialDev, int mode)
{
  int		result = PROG_ERRNO_UNKNOWN;
  struct termios	tc;

  if(s < 0) {
    /* serial init */
    if((s = open(nameSerialDev, O_RDWR | O_NONBLOCK | O_NOCTTY)) < 0) {
      result = PROG_ERRNO_UNKNOWN;
      goto fail;
    }
  }

  tcgetattr(s, &tc);
  memcpy(&tcBackup, &tc, sizeof(tcBackup));

  /*memset(&tc, 0, sizeof(tc));*/
  cfmakeraw(&tc);
  cfsetospeed(&tc, B115200);
  cfsetispeed(&tc, B115200);

  tc.c_cflag |= CLOCAL;
  tc.c_cflag &= ~(PARENB | PARODD);
  tc.c_cflag |= ( PARENB );
  tc.c_cflag &= ~(CRTSCTS /*| CIGNORE*/);

  tcsetattr(s, TCSANOW, &tc);

  result = PROG_ERRNO_SUCCESS;

fail:
  return result;
}


int
DeviceUninitSerial(int unit, int mode)
{
  int		result = PROG_ERRNO_SUCCESS;

  tcsetattr(s, TCSANOW, &tcBackup);
  close(s);

  return result;
}


int
DeviceInitSetBaud(int baud)
{
  int			result = PROG_ERRNO_UNKNOWN;
  struct termios	tc;
  int			flag = 0;

  switch(baud) {
  case     9600: flag =    B9600; break;
  case   115200: flag =  B115200; break;
  case   230400: flag =  B230400; break;
  case   460800: flag =  B460800; break;
  case   921600: flag =  B921600; break;
  case  2000000: flag = B2000000; break;
  }

  if(flag) {
    tcgetattr(s, &tc);
    cfmakeraw(&tc);
    cfsetospeed(&tc, flag);
    cfsetispeed(&tc, flag);
    tcsetattr(s, TCSANOW, &tc);
    result = PROG_ERRNO_SUCCESS;
  }

  return result;
}


int
DeviceInitSetTerminal(int flag)
{
  int			result = PROG_ERRNO_UNKNOWN;
  struct termios	tc;
  uint32_t		mask;

  mask = (CSIZE | PARENB | PARODD | CSTOPB);
  tcgetattr(s, &tc);
  tc.c_cflag &= ~mask;
  tc.c_cflag |= flag & mask;
  tcsetattr(s, TCSANOW, &tc);
  result = PROG_ERRNO_SUCCESS;

  return result;
}



int
DeviceSendData(int unit, unsigned char *ptr, int n)
{
#if DEVICE_DEBUG_SHOW_DATA
  int		i;

  printf("DeviceSendData: [");
  for(i = 0; i < n; i++) {
    printf("%02x ", ptr[i]);
    if(!(~i & 3)) printf(" ");
    if(i && !(i & 15)) printf("\n");
  }
  printf("]\n");
#endif

  return write(s, ptr, n);
}


int
DeviceRecvData(int unit, unsigned char *ptr, int n, unsigned int tout)
{
  int			re;
  int			i;
  fd_set		fd;
  struct timeval	tv;
  unsigned char		*pBuf;
#if DEVICE_DEBUG_SHOW_DATA
  int			cnt;
#endif


  pBuf = (unsigned char *) ptr;
  for(i = 0; i < n; i++) {
    FD_ZERO(&fd);
    tv.tv_sec  = (tout) / 1000000;
    tv.tv_usec = (tout) % 1000000;
    FD_SET(s, &fd);
    if(select(s+1, &fd, NULL, NULL, &tv) <= 0) break;
    re = read(s, pBuf++, 1);
    if(re < 0) break;
  }

#if DEVICE_DEBUG_SHOW_DATA
  if(i) {
    printf("DeviceRecvData: [");
    for(cnt = 0; cnt < i; cnt++) {
      printf("%02x ", ptr[cnt]);
      if(!(~cnt & 3)) printf(" ");
      if(cnt && !(cnt & 15)) printf("\n");
    }
    printf("]\n");
  }
#endif

  return i;
}



int
DeviceInitPort(int unit)
{
  int			result;

  /*DeviceInitSerial(unit, 0);*/

  result = PROG_ERRNO_SUCCESS;

  return result;
}


int
DeviceUninitPort(int unit)
{
  return PROG_ERRNO_SUCCESS;
}






/*
 * end of file
 */

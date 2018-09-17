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

#define	_PROG_C_

#include	<stdint.h>

#include	"stm32/progstm32.h"
#include	"kinetis/progmkl43.h"
#include	"prog.h"

static int	modelType;
static int	serialMode;

static int	(*api[][SERIALMODE_END-1])(int unit, int req, void *ptr, int size) = {
  /* 1  kinetis */
  {Progmkl43Api, PROG_NULL, PROG_NULL, PROG_NULL},
  /* 2  LPC */
  {PROG_NULL, PROG_NULL, PROG_NULL, PROG_NULL},
  /* 3  STM32 */
  {Progstm32Api, PROG_NULL, PROG_NULL, PROG_NULL},
};


int
ProgApi(int unit, int req, void *ptr, int size)
{
  int		re = PROG_ERRNO_UNKNOWN;
  int		val;

  switch(req) {
  case	PROG_API_SETMODELTYPE:
    if(!ptr || size > sizeof(int)) goto end;
    val = *(int *)ptr;
    if(val < MODELTYPE_NONE || val >= MODELTYPE_END) {
      re = PROG_ERRNO_PARAMEXCEEDED;
      goto end;
    }
    modelType = val;
    re = PROG_ERRNO_SUCCESS;
    break;
  case	PROG_API_SETSERIALMODE:
    if(!ptr || size > sizeof(int)) goto end;
    val = *(int *)ptr;
    if(val < MODELTYPE_NONE || val >= MODELTYPE_END) {
      re = PROG_ERRNO_PARAMEXCEEDED;
      goto end;
    }
    serialMode = val;
    re = PROG_ERRNO_SUCCESS;
    break;
  default:
    if(api[modelType-1][serialMode-1]) {
      re = api[modelType-1][serialMode-1](unit, req, ptr, size);
    }
    break;
  }

end:
  return re;
}


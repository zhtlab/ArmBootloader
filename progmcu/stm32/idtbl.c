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

#define	_IDTBL_C_

#include	<stdint.h>
#include	<stdlib.h>
#include	<string.h>

struct _stMemeory {
  uint32_t	start;
  uint32_t	size;
  int		times;
  int		sector;
};

struct _stIdtbl {
  int			id;
  char			name[16];
  struct _stMemeory	mem[8];
  int			boot[8];
  int			user;
  int			firm;
};


static struct _stIdtbl		idtbl[] = {
  {0x451,		/* chip id */
   "STM32F767xI",	/* chip name */
   {			/* memory map */
     {0x08000000, 0x08000, 4, 0},
     {0x08020000, 0x20000, 1, 4},
     {0x08040000, 0x40000, 7, 5},
     {-1, -1, -1, -1},
   },
   {0, 1, 2, 4,  -1, -1, -1, -1},
   3,
   5
  },

  {0x450,		/* chip id */
   "STM32H432xI",	/* chip name */
   {			/* memory map */
     {0x08000000, 0x20000, 8, 0},
     {0x08100000, 0x20000, 8, 8},
     {-1, -1, -1, -1},
   },
   {0, 1, 2, 4,  -1, -1, -1, -1},
   1,
   2
  },
  
  {0x435,		/* chip id */
   "STM32L433",		/* chip name */
   {			/* memory map */
     {0x08000000, 0x00800, 256, 0},
     {-1, -1, -1, -1},
   },
   {0, 1, 2, 3,  -1, -1, -1, -1},
   7,
   8
  },

  {-1,			/* chip id */
   "NULL",		/* chip name */
   {			/* memory map */
     {-1, -1, -1, -1},
   },
   {-1, -1, -1, -1,  -1, -1, -1, -1},
   -1,
   -1
  },
};


int
IdtblStm32GetSectorByAddr(int chipId, uint32_t *addr)
{
  int		num = -1;
  
  struct _stIdtbl	*p = idtbl;
  struct _stMemeory	*mem;
  uint32_t		start, end;
  int			times;
  int			j, i;

  while(1) {
    if(p->id == -1) break;
    if(p->id == chipId) {
      for(i = 0; i < 8; i++) {
	mem = &p->mem[i];
	if(mem->start != -1) {

	  start = mem->start;
	  end   = mem->start + mem->size;
	  times = mem->times;
	  for(j = 0; j < times; j++) {
	    if(start <= *addr && end > *addr) {
	      num = mem->sector + j;
	      *addr = end;
	      goto end;
	    }
	    start  = end;
	    end   += mem->size;
	  }
	}
      }
      break;
    }
    p++;
  }

end:
  return num;
}


void
IdtblStm32GetChipName(int chipId, char *ptr, int len)
{
  struct _stIdtbl	*p = idtbl;
  int			i = 0;

  if(!ptr || len <= 0) goto fail;
  
  *ptr = '\0';

  while(1) {
    if(p->id == -1) break;
    if(p->id == chipId) {
      strncpy(ptr, idtbl[i].name, len);
      break;
    }
    i++;
    p++;
  }

fail:
  return;
}

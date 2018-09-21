#
# Copyright (c) 2018 zhtlab
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

# Makefile for gcc

COMPILER	= COMPILER_GCC
PORTABLE_CC	= GCC
PORTABLE_ARCH	= ARM$(FREERTOS_ARCH)
PORTABLE_ARCHDIR	= ARM_$(FREERTOS_ARCH)
PORTABLE_DIR	= $(FREERTOS_DIR)/Source/portable/$(PORTABLE_CC)/$(PORTABLE_ARCHDIR)
PORTABLE_TARGET	= $(PORTABLE_CC)_$(PORTABLE_ARCH)

CFLAGS_DEV	= -S -O -mthumb -mcpu=$(CPU_CORTEX_ARCH) -mlittle-endian \
			-mfloat-abi=hard -D__ARMVFP__

CFLAGS		= -O -mthumb -mcpu=cortex-m0plus

ASFLAGS		= -acdghlmns=${@:.o=.lst}

LDCONFIG	= main.ld
LDFLAGS		= -Wl,-Map=${@:.elf=.map} -T $(LDCONFIG) -nostdlib

ARFLAGS		= rcs

ELFTOOLFLAGS_SREC	= -S -O srec --srec-forceS3 --srec-len=32
ELFTOOLFLAGS_BIN	= -S -O binary

CC		= $(CROSS)gcc
CPP		= $(CROSS)gcc
AS		= $(CROSS)as
LD		= $(CROSS)gcc
AR		= $(CROSS)ar
CCDEP		= $(CROSS)gcc
GCC		= $(CROSS)cc
STRIP		= $(CROSS)strip
ELFTOOL		= $(CROSS)objcopy
SIZE		= $(CROSS)size

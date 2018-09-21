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

# Makefile for iar

COMPILER	= COMPILER_IAR

LDCONFIG	= $(TARGET).icf

CFLAGS_DEV	= --silent -Oh \
		--cpu=$(CPU_CORTEX_ARCH) -e --fpu=None \
		-DCPU_$(DEVICE_NAME) \
		-lA ${@:.o=.ss} -lCN ${@:.o=.lst}

ASFLAGS		= --cpu $(CPU_CORTEX_ARCH) --fpu None \
		-S -s+ -M\<\> -w+ -r \
		-L. -t8


LDFLAGS		= --silent --config $(LDCONFIG) \
		--log libraries,initialization,modules,redirects,sections,veneers,unused_fragments \
		--log_file $(OBJDIR)/$(TARGET).log  --map $(OBJDIR)/$(TARGET).map \
		--redirect __aeabi_idiv=__iar_small___aeabi_idiv --redirect \
			__aeabi_uidiv=__iar_small___aeabi_uidiv --redirect \
			__aeabi_idivmod=__iar_small___aeabi_idivmod --redirect \
			__aeabi_uidivmod=__iar_small___aeabi_uidivmod \
		--entry Reset_Handler \
		--no_exceptions --merge_duplicate_sections --vfe \
		--diag_suppress Lp048,Lp049

#		--redirect _Printf=_PrintfSmall --redirect _Scanf=_ScanfSmall \
		--config_def __ram_vector_table__=1 \
		--config_def __stack_size__=0x400 \
		--config_def __heap_size__=0x100 \
#		--semihosting \
#		--entry __iar_program_start --vfe

LIBS		= $(IAR)/lib/rt6M_tl.a $(IAR)/lib/shb_l.a

ELFTOOLFLAGS_SREC	= --silent --srec --srec-s3only --srec-len=32
ELFTOOLFLAGS_BIN	= --silent --bin


CC		= iccarm
CPP		= iccarm
AS		= iasmarm
LD		= ilinkarm
CCDEP		= gcc
GCC		= cc
CP		= cp
STRIP		= strip
RM		= /bin/rm
ELFTOOL		= ielftool
SIZE		= size

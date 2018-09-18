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

### target infromation

### target
TARGET		= progmcu
TARGET_KINETIS	= 0
TARGET_LPC	= 0
TARGET_STM	= 1

### sources
SOURCESASM	= 
SOURCES		= base/main.c base/serial.c base/prog.c base/crc32.c \

ifeq ($(TARGET_KINETIS), 1)
SOURCES		+= kinetis/progmkl43.c \
			kinetis/fsl_crc/src/crc16.c
endif
ifeq ($(TARGET_STM), 1)
SOURCES		+= stm32/progstm32.c stm32/idtbl.c
endif

#
# select compile environment
# compiler dependent sources
#

CFLAGS_WARN	= -Wall -Wunused-result #-Wunused-function
CFLAGS		= $(CFLAGS_WARN) -O -I . -I base \
			-I kinetis \
			-I kinetis/fsl_crc \
			-I lpc -I stm32 \
			-DTARGET_KINETIS=$(TARGET_KINETIS) \
			-DTARGET_LPC=$(TARGET_LPC) \
			-DTARGET_STM=$(TARGET_STM)

ASFLAGS		=


LDFLAGS		= 



CC		= $(CROSS)gcc
CPP		= $(CROSS)gcc
AS		= $(CROSS)as
LD		= $(CROSS)gcc
CCDEP		= $(CROSS)gcc
GCC		= $(CROSS)cc
STRIP		= $(CROSS)strip
ELFTOOL		= $(CROSS)objcopy
SIZE		= $(CROSS)size
LN		= $(CROSS)ln
PUSHD		= pushd
POPD		= popd


CONFIGFILE	= ../../../config
LOGFILE		= log
DEPENDFILE	= _depend
TMPDIR		= /tmp
TOOLBINDIR	= ..

#TARGET		= 
LOGFILE		= log



SOURCESCPP	= 

#SOURCES		= $(wildcard src/*.c)
ARCH		= $(shell uname -s)-$(shell uname -p)
OBJDIR		= obj/$(ARCH)


OBJS		= $(addprefix $(OBJDIR)/, $(SOURCESASM:.s=.o)) \
			$(addprefix $(OBJDIR)/, $(SOURCES:.c=.o))

DIRS		= $(dir $(OBJS))

DEPENDS		= $(addprefix $(OBJDIR)/, $(SOURCES:.c=.d))

LIBS		= 


CFLAGSDEP	= -I . -I ..

INSTALLDIR	= ../../binary

CP		= cp
RM		= /bin/rm
MKDIR		= /bin/mkdir

SHELL		= bash

.SUFFIXES:	.c .cpp .o .d
.PHONY:		clean depend

all: $(OBJDIR)/$(TARGET)


$(OBJDIR)/$(TARGET): $(OBJDIR) $(DEPENDS) $(OBJS)
	echo $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS)
	$(PUSHD) $(OBJDIR)/..; $(LN) -fs $(ARCH)/$(TARGET) $(TARGET).$(ARCH); $(POPD)

$(OBJS): Makefile

$(OBJDIR)/%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<


$(OBJDIR):
	$(MKDIR) -p $(OBJDIR)
	$(MKDIR) -p $(DIRS)


rmlog:
	$(RM) -f $(LOGFILE)

log: $(OBJS)
	$(RM) -f $(LOGFILE)

clean:
	$(RM) -f  $(DEPENDFILE)
	$(RM) -f  $(OBJS)
	$(RM) -f  $(OBJDIR)/$(TARGET)
	$(RM) -fr $(OBJDIR)

depend: $(DEPENDS)

$(OBJDIR)/%.d: %.c
	$(CC) -MM -MT "${@:.d=.o} $@" -MF $@ $(CFLAGS) $<
$(OBJDIR)/%.d: ../../../boot2/%.c
	$(CC) -MM -MT "${@:.d=.o} $@" -MF $@ $(CFLAGS) $<

ifeq ($(OBJDIR)/$(DEPENDFILE), $(wildcard $(OBJDIR)/$(DEPENDFILE)))
-include $(DEPENDS)
endif


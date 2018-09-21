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

######################################################
# directory definitions
#
MK_DIR		= ../mk
TMPDIR		= /tmp
OBJDIR		= obj/$(DEVICE_TYPE)_$(ENV_CC)/$(TARGET)
DEVICE_DIR	= target/$(DEVICE_TYPE)/$(TARGET)
COMMON_DIR	= common
PRINTF_DIR	= lib/printf-MarcoPaland


######################################################
# include
#
include		../config
include		target/$(DEVICE_TYPE)/config
include		$(MK_DIR)/cortex-m.mk


ifeq ($(ENV_CC), gcc)			### GCC
MAKEFILE_DEP	= $(MK_DIR)/boot.gcc.mk
SOURCESASM	= $(COMMON_DIR)/start_gcc.s

else ifeq ($(ENV_CC), iar)		### IAR
MAKEFILE_DEP	= $(MK_DIR)/boot.iar.mk
SOURCESASM	= $(COMMON_DIR)/start_iar.s

else					### no environment
$(error no environment)

endif
include		$(MAKEFILE_DEP)


#
# select target environments
#
ifeq	($(TARGET), boot1)
CRC32_BUILD_TABLE	= 1

else ifeq	($(TARGET), boot2)
CRC32_BUILD_TABLE	= 0

else
$(error unknown target)

endif


######################################################
# sources
#
include		target/$(DEVICE_TYPE)/$(TARGET)/sources.mk
vpath		%.c $(sort $(dir $(SOURCES)))
vpath		%.s $(sort $(dir $(SOURCESASM)))

OBJS		= $(addprefix $(OBJDIR)/, $(notdir $(SOURCESASM:.s=.o))) \
			$(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))

CFLAGS		= $(CFLAGS_DEV) $(DEFINES) $(INCDIRS) $(DEPFLAGS) $(LSTFLAGS)

LDFLAGS		= $(LDFLAGS_CFG) $(LDFLAGS_SEC)  $(LDFLAGS_MAP)

DEPENDS		= $(OBJS:.o=.d)

include		target/$(DEVICE_TYPE)/$(TARGET)/sources.mk

######################################################
# tool definitions
#
CP		= /bin/cp
RM		= /bin/rm
SHELL		= /bin/bash


######################################################
# make special target definitions
#
.SUFFIXES:	.c .s .o .d
.PHONY:		clean depend


######################################################
# entry point
#
all: $(OBJDIR)/$(TARGET).srec $(OBJDIR)/$(TARGET).bin


######################################################
# ld, file convert
#
$(OBJDIR)/$(TARGET).srec: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_SREC) $< $@

$(OBJDIR)/$(TARGET).bin: $(OBJDIR)/$(TARGET).elf
	$(ELFTOOL) $(ELFTOOLFLAGS_BIN) $< $@

$(OBJDIR)/$(TARGET).elf: $(OBJS) $(LDCONFIG)
	$(LD) $(CPU) $(OBJS) $(LIBS) $(LDFLAGS) -o $@
	$(SIZE)	$@


######################################################
# cc
#
ifeq ($(ENV_CC), gcc)			### GCC
$(OBJDIR)/%.o: %.c
	$(CC) -c $(CPU) -o $@ $(CFLAGS) $<

else ifeq ($(ENV_CC), iar)		### IAR
$(OBJDIR)/%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<

else
$(error no environment)

endif

$(OBJDIR)/%.o: %.s
	$(AS)  -c $(CPU) $(ASFLAGS) -o $@ $<


######################################################
# Makefile
#
$(OBJS): $(MK_DIR)/boot.mk $(MAKEFILE_DEP)


######################################################
# clean, etc
#
clean:
	$(RM) -f $(OBJS) ${OBJS:.o=.ss} ${OBJS:.o=.lst}


######################################################
# depend
#
$(OBJDIR)/%.d: %.s
	echo "${@:.d=.o} $@: $(OBJDIR)/../config.inc" > $@

-include $(DEPENDS)

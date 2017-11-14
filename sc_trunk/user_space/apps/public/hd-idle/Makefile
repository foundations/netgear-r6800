###############################################################################
#
# General Definitions
#
###############################################################################

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/Rules.mak

#include ../../Rules.mak

DESTBIN = $(BINDIR)


LIB_DIRS   =  

CC = $(CROSS)gcc
CFLAGS    += -Wall

LD         = $(CC)
LDFLAGS   += $(LIB_DIRS)

###############################################################################
#
# Main Dependencies
#
###############################################################################

TARGET  = hd-idle

LIBS    = 

SRCS    = hd-idle.c

OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

distclean: clean

clean:
	rm -f $(OBJS) $(TARGET)

install: 
	#install -D -g root -o root $(TARGET) $(TARGET_DIR)/sbin/$(TARGET)
	#install -D -g root -o root $(TARGET).1 $(TARGET_DIR)/share/man/man1/$(TARGET).1
	$(STRIP) $(TARGET)
	cp -a  $(TARGET) $(DESTBIN)

hd-idle.o:     hd-idle.c

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIB_DIRS) $(LIBS)



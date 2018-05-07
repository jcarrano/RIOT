SRC := $(filter-out loadlib.c lua.c luac.c,$(wildcard *.c))

# This builds using POSIX system calls and some extra libraries, and
# removes a compiler warning that warns against using tmpnam().
ifeq ($(BOARD),native)
CFLAGS += -DLUA_USE_LINUX
endif

# This is very important because lua defaults to 64 bit integers
CFLAGS += -DLUA_32BITS -fstack-usage -Wstack-usage=2048 -DLUAL_BUFFERSIZE=64

include $(RIOTBASE)/Makefile.base

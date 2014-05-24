
CC=gcc
PLATFORM=$(shell uname)
STRIP=strip
CFLAGS =-Wall -fPIC -ldl
CFLAGS+=-DDEBUG
CFLAGS+=-DBLOCK_LIST=\"$(BLOCK_LIST)\"
LDFLAGS=

SRC=$(wildcard *.c)
OBJ=$(subst .c,.o,$(SRC))
DST=preload.so

.PHONY: all clean

all: $(DST)

$(DST): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $< -ldl
	$(STRIP) $@

test: $(DST)
	@if [ $(PLATFORM) = "Darwin" ]; then \
		DYLD_INSERT_LIBRARIES$$(pwd)/$(DST) bash; \
	else \
		LD_PRELOAD=$$(pwd)/$(DST) bash; \
	fi

clean:
	rm -rf $(DST) $(OBJ)

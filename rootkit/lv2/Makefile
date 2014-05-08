BLOCK_LIST="WHAT_YOU_WANT_TO_BLOCK /etc/ld.so.preload"

CC=gcc
STRIP=strip
CFLAGS =-Wall -fPIC -ldl
#CFLAGS+=-DDEBUG
CFLAGS+=-DBLOCK_LIST=\"$(BLOCK_LIST)\"
#CFLAGS+=-DUID=$(UID) -DGID=$(GID)
LDFLAGS=

SRC=$(wildcard src/*.c)
OBJ=$(subst .c,.o,$(SRC))
DST=preload.so

.PHONY: all clean

all: $(DST)

$(DST): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $<
	$(STRIP) $@

test: $(DST)
	@echo "Test ls... "
	@LD_PRELOAD=./$(DST) ls > /dev/null
	@echo "Test ps... "
	@LD_PRELOAD=./$(DST) ps > /dev/null

clean:
	rm -rf $(DST) $(OBJ)

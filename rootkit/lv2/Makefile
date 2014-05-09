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
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $< -ldl
	$(STRIP) $@

test: $(DST)
	@echo -e "\033[36mTest ls... \033[m"
	@LD_PRELOAD=./$(DST) ls > /dev/null
	@echo -e "\033[36mTest ps... \033[m"
	@LD_PRELOAD=./$(DST) ps > /dev/null
	@echo -e "\033[36mTest netstat... \033[m"
	@LD_PRELOAD=./$(DST) netstat > /dev/null
	@echo -e "\033[36mTest NOT_FOUND... \033[m"
	@LD_PRELOAD=./$(DST) NOT_FOUND || true

clean:
	rm -rf $(DST) $(OBJ)

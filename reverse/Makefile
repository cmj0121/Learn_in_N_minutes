CC=gcc
CFLAGS=-Wall
LDFLAGS=

DST=scenario1 scenario2 scenario3 scenario4

.PHONY: all $(DST)

all: $(DST)

scenario1: scenario1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
scenario2: scenario2.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
scenario3: scenario3.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
scenario4: scenario4.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

.PHONY: cleanall


cleanall: clean

clean:
	rm -rf $(DST)

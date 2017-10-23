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

.PHONY: install
install: upload

.PHONY: upload
upload:
	avrdude -C/usr/local/Cellar/avrdude/6.3/etc/avrdude.conf -v -patmega328p -carduino -P/dev/cu.usbserial-DN01JMER -b115200 -D -Uflash:w:src/bin/Demo.hex:i 
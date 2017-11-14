all:
#	$(MAKE) -C kernel
	$(MAKE) -C lib
#	$(MAKE) -C util

install:
#	$(MAKE) -C kernel install
	$(MAKE) -C lib	install
#	$(MAKE) -C util	install
	
clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C lib clean
#	$(MAKE) -C util clean

prepare:
	@echo "fuse prepare nothing"

GPL:
	cp -a $(shell pwd) $(GPL_DIR)/common/

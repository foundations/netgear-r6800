CURR_DIR := $(shell pwd)
BUILD_DIR1:=$(subst /user_space, /user_space,$(CURR_DIR))
BUILD_DIR2:=$(word 1, $(BUILD_DIR1))

include $(BUILD_DIR2)/Rules.mak

SRC_CODE_DIR = lzo-2.06

all: auto_config  build_targe

auto_config:
	@echo "----------"GCC:$(CC)"---------------------"
	cd $(SRC_CODE_DIR); ./build.sh $(CC)
build_targe:
	make -C $(SRC_CODE_DIR)
	cp $(SRC_CODE_DIR)/src/.libs/liblzo2.a ./
install:
	
clean:
	make -C $(SRC_CODE_DIR) clean
	rm -f ./liblzo2.a
total_clean:
	make -C $(SRC_CODE_DIR) distclean
	rm -f ./liblzo2.a

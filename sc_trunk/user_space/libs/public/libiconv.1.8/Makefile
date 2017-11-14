#Makefile
#include $(TOP)/.config
include cross.mak

#-------------------------------------------------------#
#                       SUBLIBS                         #
#-------------------------------------------------------#
SUBLIBS  =
SUBLIBS  +=  lib
#SUBLIBS  +=  src

#-------------------------------------------------------#
#                       TARGETS                         #
#-------------------------------------------------------#

all: libs
	@echo All LIBS are OK!


libs : ${SUBLIBS}
	@for i in ${SUBLIBS} ;  do                                                    \
            echo "-----------------------------------------------------------------"; \
            echo -e "\033[;34m               libiconv-1.8/$$i                 \033[;0m"; \
            echo "-----------------------------------------------------------------"; \
            $(MAKE) -C $$i || exit 1 ;                                         \
        done

install:
	cp -f lib/libiconv.so.0.0 $(TARGET_DIR)/lib/
	cd $(TARGET_DIR)/lib/
	ln -sf libiconv.so.0.0 libiconv.so.0
	ln -sf libiconv.so.0 libiconv.so
	ln -sf libiconv.so.0.0 libiconv.so.2
	mv libiconv.so.0 $(TARGET_DIR)/lib/
	mv libiconv.so $(TARGET_DIR)/lib/
	mv libiconv.so.2 $(TARGET_DIR)/lib/
clean:
	for i in ${SUBLIBS} ; do $(MAKE) -C $$i clean || exit 1 ; done
	rm -rf $(TARGET_DIR)/lib/libiconv.so*

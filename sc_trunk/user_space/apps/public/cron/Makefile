#
# Makefile for cron daemon
#
#


CC=$(CROSS)gcc

all: crond 

crond : cron.o cron-parent.o
	$(CC) -s -o $@ $(LDFLAGS)  $^ 

install: all
	cp -a crond $(ROOT)/target/usr/sbin
	cp -a crond $(ROOT)/target/usr/sbin
	# will chown for all file in target by root makefile, not do this here because we will chown in fakeroot
	#chown root $(ROOT)/target/usr/etc/crontab
	chmod 0644 $(ROOT)/target/usr/etc/crontab
	
clean:
	rm -f *~ crond *.o


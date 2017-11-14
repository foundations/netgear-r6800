#!/bin/sh
rc firewall stop
rmmod ipt_REJECT
insmod /tmp/ipt_REJECT.o
rc firewall start

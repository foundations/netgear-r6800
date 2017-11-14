#!/bin/sh -x

DEVPATH=$1
USRDEVPATH=$2

echo "use fakeroot to generate the filesystem"

# ONLY need console and null in dev/ for booting (before copy nodes from /usr/dev)
# We will copy all nodes form /usr/dev to /dev, so we need to make all nodes here
cd $USRDEVPATH

mknod console c 4 64

mknod mtd0 c 90 0
mknod mtd0ro c 90 1
mknod mtd1 c 90 2
mknod mtd1ro c 90 3
mknod mtd2 c 90 4
mknod mtd2ro c 90 5
mknod mtd3 c 90 6
mknod mtd3ro c 90 7
mknod mtd4 c 90 8
mknod mtd4ro c 90 9
mknod mtd5 c 90 10
mknod mtd5ro c 90 11
mknod mtd6 c 90 12
mknod mtd6ro c 90 13
mknod mtd7 c 90 14
mknod mtd7ro c 90 15
mknod mtd8 c 90 16
mknod mtd8ro c 90 17
mknod mtd9 c 90 18
mknod mtd9ro c 90 19
mknod mtd10 c 90 20
mknod mtd10ro c 90 21
mknod mtd11 c 90 22
mknod mtd11ro c 90 23
mknod mtd12 c 90 24
mknod mtd12ro c 90 25
mknod mtd13 c 90 26
mknod mtd13ro c 90 27
mknod mtd14 c 90 28
mknod mtd14ro c 90 29
mknod mtd15 c 90 30
mknod mtd15ro c 90 31
mknod mtd16 c 90 32
mknod mtd16ro c 90 33
mknod mtd17 c 90 34
mknod mtd17ro c 90 35
mknod mtd18 c 90 36
mknod mtd18ro c 90 37
mknod mtd19 c 90 38
mknod mtd19ro c 90 39
mknod mtd20 c 90 40
mknod mtd20ro c 90 41

mknod mtdblock0 b 31 0
mknod mtdblock1 b 31 1
mknod mtdblock2 b 31 2
mknod mtdblock3 b 31 3
mknod mtdblock4 b 31 4
mknod mtdblock5 b 31 5
mknod mtdblock6 b 31 6
mknod mtdblock7 b 31 7
mknod mtdblock8 b 31 8
mknod mtdblock9 b 31 9
mknod mtdblock10 b 31 10
mknod mtdblock11 b 31 11
mknod mtdblock12 b 31 12
mknod mtdblock13 b 31 13
mknod mtdblock14 b 31 14
mknod mtdblock15 b 31 15
mknod mtdblock16 b 31 16
mknod mtdblock17 b 31 17
mknod mtdblock18 b 31 18
mknod mtdblock19 b 31 19
mknod mtdblock20 b 31 20

mknod sda  b 8 0
mknod sda1 b 8 1
mknod sda2 b 8 2
mknod sda3 b 8 3
mknod sda4 b 8 4
mknod sdb  b 8 16
mknod sdb1 b 8 17
mknod sdb2 b 8 18
mknod sdb3 b 8 19
mknod sdb4 b 8 20

mknod null c 1 3
mknod ppp c 108 0
mknod ptmx c 5 2

mknod ptyp0 c 2 0
mknod ptyp1 c 2 1
mknod ptyp2 c 2 2
mknod ptyp3 c 2 3
mknod ptyp4 c 2 4
mknod ptyp5 c 2 5
mknod ptyp6 c 2 6
mknod ptyp7 c 2 7
mknod ptyp8 c 2 8
mknod ptyp9 c 2 9
mknod ptypa c 2 10
mknod ptypb c 2 11
mknod ptypc c 2 12
mknod ptypd c 2 13
mknod ptype c 2 14
mknod ptypf c 2 15

mknod random c 1 8
mknod sc_led c 225 0
mknod nvram  c 253 0
mknod gpio   c 254 0

mknod ttyp0 c 3 0
mknod ttyp1 c 3 1
mknod ttyp2 c 3 2
mknod ttyp3 c 3 3
mknod ttyp4 c 3 4
mknod ttyp5 c 3 5
mknod ttyp6 c 3 6
mknod ttyp7 c 3 7
mknod ttyp8 c 3 8
mknod ttyp9 c 3 9
mknod ttypa c 3 10
mknod ttypb c 3 11
mknod ttypc c 3 12
mknod ttypd c 3 13
mknod ttype c 3 14
mknod ttypf c 3 15

mknod urandom c 1 9

cd -

cd $DEVPATH
#mknod console c 4 64
#mknod null c 1 3
#mknod ttyS0 c 4 64
cd -


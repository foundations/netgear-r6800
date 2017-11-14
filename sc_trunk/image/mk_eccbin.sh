#!/bin/sh

ecc_tool=./scEcc
platform=mt7620
block_size=128
page_size=2048
ecc_cal_len=512
input=$1
output=$2

echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
echo $ecc_tool will generate a $2 for PLATFORM $platform
echo '<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<'

if [ "$1" == "" -o "$2" == "" ]
then
	echo "$0 <input_file> <output_file>"
	exit 0
fi

$ecc_tool $platform $block_size $page_size $ecc_cal_len $output $input

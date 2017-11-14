#!/bin/sh

#
# A tool to change the marco definition in a head file into a config file.
#                                     Nick Chen 2015.06
#
file_name=${1%.*}
grep '^#define' "$1"> sc_config.tmp1
cat sc_config.tmp1 | awk '{print $2"="$3}' > sc_config.tmp2
cat sc_config.tmp2 | grep '".*"$' > sc_${file_name}_config.sh
rm -rf sc_config.tmp*

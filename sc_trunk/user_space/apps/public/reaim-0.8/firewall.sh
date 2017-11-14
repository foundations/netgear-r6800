#!/bin/sh
#
# Project: ReAim
#
# Release : 0.8 pre
#
# Homepage:  http://reaim.sourceforge.net/
#
# Copyright: Mark Cooke, March 2002.  All Rights Reserved.
# 
# License: GNU General Public License, Version 2.
#
# CVS Data: $Id: firewall.sh,v 1.6 2003/03/27 14:39:36 mark-c Exp $
#
# Description:
#
# Sample script to configure the necessary firewall rules
# for ReAim using iptables.  See the source for an ipt/NetBSD example.
#
# Final words:
#
# No warranty. Patches, bug reports, success stories and other contributions
# most welcome.


# -------------------------
# User configurable section
# -------------------------
#
# The user must uncomment the INSIDE_IF and OUTSIDE_IF
# lines, and select the appropriate network interfaces
# for this script to use to configure networking.
#
# Example ethernet-connected cable / corporate firewall:
# INSIDE_IF=eth0
# OUTSIDE_IF=eth1
#
# Example home LAN with dialup:
INSIDE_IF=br0
OUTSIDE_IF=ppp0

#--------------------------------------------------------------
# SHOULD NOT HAVE TO CHANGE ANYTHING BELOW HERE
#--------------------------------------------------------------

IPT_X="/tmp/iptables"
IPT=IPT_X
newchain() {
    # Check syntax
    if [ $# = 0 ]; then
	echo "Usage: newchain {name}"
    fi

    # If chain already exists just flush it, else create it.
    $IPT $2 -L -n | grep "$1 " > /dev/null
    if test "$?" = 0
    then
	$IPT_X $2 -F "$1"
    else
	$IPT_X $2 -N "$1"
	$IPT_X $2 -F "$1"
    fi
}

# Just in case we don't have the sbin directories in the
# path, tag them to the end.
export PATH=$PATH:/sbin:/usr/sbin

# Sanity check the user supplied interfaces...
#
if [ X${INSIDE_IF} = "X" ];
then
	echo "INSIDE_IF was not defined. Please adjust the start of the script."
	exit 1
else
	grep -q "${INSIDE_IF}:" /proc/net/dev
	if [ $? != 0 ];
	then
		echo "INSIDE_IF (${INSIDE_IF}) is not listed as an interface in /proc/net/dev."
		exit 1
	fi
fi

if [ X${OUTSIDE_IF} = "X" ];
then
	echo "OUTSIDE_IF was not defined. Please adjust the start of the script."
	exit 1
else
	grep -q "${OUTSIDE_IF}:" /proc/net/dev
	if [ $? != 0 ];
	then
		echo "OUTSIDE_IF (${OUTSIDE_IF}) is not listed as an interface in /proc/net/dev."
		exit 1
	fi
fi

if [ ${INSIDE_IF} = ${OUTSIDE_IF} ];
then
	echo "INSIDE_IF and OUTSIDE_IF are not allowed to be the same device!"
	exit 1
fi

# IPT_X=$IPT

# Create new chains or flush existing ones with these names.
#
newchain REAIM_IN
newchain REAIM_PRE "-t nat"

# Add the AIM accept rules to the outside interface...
$IPT_X -I REAIM_IN 1  -i ${OUTSIDE_IF} -p tcp --dport 4443 -j ACCEPT
$IPT_X -I REAIM_IN 1  -i ${OUTSIDE_IF} -p tcp --dport 5190 -j ACCEPT
$IPT_X -I REAIM_IN 1  -i ${OUTSIDE_IF} -p tcp --dport 5566 -j ACCEPT

# Add the MSN accept rules to the outside interface...
$IPT_X -I REAIM_IN 1  -i ${OUTSIDE_IF} -p tcp --dport 1864 -j ACCEPT

# Add the DYNAMIC DCC port range to the outside interface...
$IPT_X -I REAIM_IN 1  -i ${OUTSIDE_IF} -p tcp --dport 40000:40099 -j ACCEPT

# Add the AIM port interception rules to the inside interface...
$IPT_X -I REAIM_PRE 1 -t nat  -i ${INSIDE_IF} -p tcp --dport 5190 -j REDIRECT --to-port 5190
$IPT_X -I REAIM_IN 1  -i ${INSIDE_IF} -p tcp --dport 4443 -j ACCEPT
$IPT_X -I REAIM_IN 1  -i ${INSIDE_IF} -p tcp --dport 5190 -j ACCEPT
$IPT_X -I REAIM_IN 1  -i ${INSIDE_IF} -p tcp --dport 5566 -j ACCEPT

# Add the MSN port interception rules to the inside interface...
$IPT_X -I REAIM_PRE 1 -t nat  -i ${INSIDE_IF} -p tcp --dport 1863 -j REDIRECT --to-port 1863
$IPT_X -I REAIM_IN 1 -i ${INSIDE_IF} -p tcp --dport 1863:1864 -j ACCEPT


# Add the REAIM_* chains to the appropriate place, but only if they
# didn't already exist.
$IPT -L INPUT -n | grep "^REAIM_IN " > /dev/null
if [ $? = 1 ];
then
	$IPT_X -I INPUT 1 -j REAIM_IN
fi

$IPT -L PREROUTING -t nat -n | grep "^REAIM_PRE " > /dev/null
if [ $? = 1 ];
then
	$IPT_X -I PREROUTING 1 -t nat -j REAIM_PRE
fi

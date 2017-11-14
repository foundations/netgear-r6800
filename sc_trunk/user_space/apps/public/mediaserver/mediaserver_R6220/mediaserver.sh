#!/bin/sh

case $1 in
    start)
	if [ "$2" = "fast" ]; then
		/etc/mediaupnp -C /etc/media.conf -F &>/dev/null
	else
		/etc/mediaupnp -C /etc/media.conf &>/dev/null
	fi
	;;
    stop)
	pid=`cat /var/run/media_server.pid`
	if test "$pid" != ""; then
	    kill $pid
	fi
	;;
    *)
	echo "Usage: mediaserver.sh {start|stop}"
	;;
esac

#! /bin/sh
#
# rootd		Start/Stop the ROOT file serving daemon
#
# chkconfig: 345 20 80
# description:	The rootd server provides remote access to ROOT files.
#
# processname: rootd
# pidfile: /var/run/rootd.pid
# config:

ROOTD=$ROOTSYS/bin/rootd

# Source function library.
. /etc/init.d/functions

# Get config.
. /etc/sysconfig/network

# Get rootd config

[ -f /etc/sysconfig/rootd ] && . /etc/sysconfig/rootd

# Check that networking is up.
if [ ${NETWORKING} = "no" ]
then
	exit 0
fi

[ -x $ROOTD ] || exit 0

RETVAL=0
prog="rootd"

start() {
        echo -n $"Starting $prog: "
        # when not built with --prefix, add as last argument the
        # path where ROOT is installed
        daemon $ROOTD $ROOTDOPTS
        RETVAL=$?
        echo
        [ $RETVAL -eq 0 ] && touch /var/lock/subsys/rootd
	return $RETVAL
}

stop() {
    	test ! -f /var/lock/subsys/rootd && return 0 || true
        echo -n $"Stopping $prog: "
        killproc rootd
        RETVAL=$?
        echo
        [ $RETVAL -eq 0 ] && rm -f /var/lock/subsys/rootd
	return $RETVAL
}

# See how we were called.
case "$1" in
  start)
	start
	;;
  stop)
	stop
	;;
  status)
	status rootd
	;;
  restart|reload)
	stop
	start
	;;
  condrestart)
	if [ -f /var/lock/subsys/rootd ]; then
            stop
            start
        fi
	;;
  *)
	echo  $"Usage: $0 {start|stop|status|restart|reload|condrestart}"
	exit 1
esac

exit $RETVAL

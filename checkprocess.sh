#!/bin/bash
PATH="${PATH}:/sbin:/usr/bin:/bin"

# find /tmp -name "rcon_*.pid" -type f -mmin +1 -exec /root/devel/rconlog/checkprocess.sh {} \;

PID=$(cat $1)
kill -9 $PID && rm -rf $1

echo "0x0D killed" >> /root/devel/rconlog/logs/rconlog.log

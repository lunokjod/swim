#!/bin/bash
#
# See the LICENSE file for copyright and license details. 
#

xidfile="$HOME/tmp/tabbed-surf.xid"
uri=""

if [ "$#" -gt 0 ];
then
	uri="$1"
else
    uri="https://duckduckgo.com/?kae=t&kaq=-1&kp=-2&kz=-1&kav=1&k1=-1&kaj=m&kak=-1&kax=-1&kap=-1&kau=-1&kg=p"
fi

runtabbed() {
	echo "Opening: " uri
	tabbed -dn tabbed-surf -r 2 surf -e '' "${uri}" >"$xidfile" \
#	tabbed -dn tabbed-surf -r 2 surf -e '' >"$xidfile" \
		2>/dev/null &
}

if [ ! -r "$xidfile" ];
then
	runtabbed
else
	xid=$(cat "$xidfile")
	xprop -id "$xid" >/dev/null 2>&1
	if [ $? -gt 0 ];
	then
		runtabbed
	else
#		surf -e "$xid" >/dev/null 2>&1 &
		surf -e "$xid" "${uri}" >/dev/null 2>&1 &
	fi
fi


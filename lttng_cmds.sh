#!/bin/sh

#install lttng
#install trace compass

# $1 is the name of tracing, any name is ok
lttng create $1
lttng enable-event -k --syscall --all
lttng enable-event -k sched_switch,sched_wak'*',irq_'*',net_'*',sock_'*',skb_'*',napi_poll
lttng add-context -k -t vtid -t vpid -t procname -t prio
lttng start
sleep 10
lttng stop
lttng destroy

perf stat -ae 'tcp:*,sock:*'
perf record -ae 'sock:inet_sock_set_state' --call-graph dwarf
perf script
perf report

#Ftrace:
cd /sys/kernel/debug/tracing 
echo 0 > tracing_on
echo > trace
echo function_graph > current_tracer
echo tcp_v4_syn_recv_sock > set_graph_function
echo 1 > tracing_on
# --- start tcp transmission ---
echo 0 > tracing_on
cat /sys/kernel/debug/tracing/trace > trace-2
vim trace-2

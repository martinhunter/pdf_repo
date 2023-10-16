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
echo tcp_connect > set_graph_function
echo tcp_v4_do_rcv > set_graph_function
echo 1 > tracing_on
# --- start tcp transmission ---
echo 0 > tracing_on
cat /sys/kernel/debug/tracing/trace > trace-2
vim trace-2

我在linux上使用以下示例代码创建服务器后, 当客户端发起连接请求后，服务器的tcp层调用了什么函数栈，当客户端发起关闭连接请求后，服务器的tcp层调用了什么函数栈，
当客户端发起第二次连接后，服务器的tcp层调用了什么函数栈
"""
sock_fd = socket(AF_INET, SOCK_STREAM, 0);
setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,...);
bind(sock_fd,...);
listen(sock_fd,1024);
"""
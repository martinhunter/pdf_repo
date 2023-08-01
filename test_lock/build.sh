set -e
rm -rf /tmp/unix_socket
gcc client.c -g -o client
gcc server.c daemon.c -g -lpthread -o daemon

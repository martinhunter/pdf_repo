#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <fcntl.h>

#include "daemon.h"
#include "server.h"

#define SOCKET_PATH "/tmp/unix_socket" // Unix域套接字文件路径
#define MAX_CLIENTS 5 // 最大客户端连接数
#define BUFFER_SIZE 1024 // 缓冲区大小

unsigned int gen_rand(){
    unsigned int random_data;
    int random_fd = open("/dev/urandom", O_RDONLY);
    if (random_fd == -1) {
        perror("Failed to open /dev/urandom");
        return 1;
    }

    if (read(random_fd, &random_data, sizeof(random_data)) != sizeof(random_data)) {
        perror("Failed to read random data");
        close(random_fd);
        return 1;
    }

    close(random_fd);
    return random_data;
}

void handle_start(char *msg, ExtendProbe *probe) {
  if (probe->exist == 1) {
    sprintf(msg, "already started MUTEX");
  } else {
    pthread_create(&probe->tid, NULL, SingleProbe, probe);
  }
  sprintf(msg, "Start:%d", probe->exist);
}

void handle_stop(char *msg, ExtendProbe *probe) {
  if (probe->exist == 0) {
    sprintf(msg, "already stopped MUTEX");
  } else {
    pthread_cancel(probe->tid);
    // probe->tid = 0;
    probe->exist = 0;
  }
  sprintf(msg, "Stop:%d", probe->exist);
}



int handler(char *msg, char*buf, ExtendProbe *probe){
  unsigned int rn = gen_rand();
  printf("----------s0:%d\n", rn);
  pthread_mutex_lock(&probe->mutex);
  printf("----------s1:%d\n", rn);
    if (strcmp(buf, "start") == 0) {
      handle_start(msg, probe);
    } else if (strcmp(buf, "stop") == 0) {
      handle_stop(msg, probe);
    } else {
      sprintf(msg, "unknown:%s", buf);
    }
  printf("----------s2:%d\n", rn);
  pthread_mutex_unlock(&probe->mutex);
  printf("----------s3:%d\n", rn);
    return 0;
}

void *serve(void *arg) {
    ExtendProbe *probe = (ExtendProbe *)arg;

    int sockfd, client_sockfd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // 创建Socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    // 设置服务器地址
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // 绑定Socket到指定的Unix域套接字文件
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding");
        close(sockfd);
        exit(1);
    }

    // 开始监听客户端连接
    listen(sockfd, 5);

    printf("Server listening on Unix socket: %s\n", SOCKET_PATH);

    // 接受并处理客户端连接请求
    while (1) {
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd < 0) {
            perror("Error accepting connection");
            close(sockfd);
            exit(1);
        }

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("Error receiving data");
            close(client_sockfd);
            continue;
        } else if (bytes_received == 0) {
            close(client_sockfd);
            break;
        }
        char msg[1024] = {0};
        
        printf("Received: %s\n", buffer);
        int ret = handler(msg, buffer, probe);
        printf("Send: %s\n", msg);
        send(client_sockfd, msg, strlen(msg), 0);

        close(client_sockfd);
    }

    // 关闭Socket连接
    close(sockfd);

    // 删除Unix域套接字文件
    unlink(SOCKET_PATH);

}


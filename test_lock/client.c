#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket" // Unix域套接字文件路径
#define BUFFER_SIZE 1024 // 缓冲区大小

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

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

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting");
        close(sockfd);
        exit(1);
    }

    // 与服务器进行通信
        sprintf(buffer, "%s", argv[1]);

        // 发送消息给服务器
        int bytes_sent = send(sockfd, buffer, strlen(buffer), 0);
        if (bytes_sent < 0) {
            perror("Error sending data");
            exit(1);
        }

        // 接收服务器的回复
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("Error receiving data");
            exit(1);
        }

        printf("Received from server: %s\n", buffer);

    // 关闭Socket连接
    close(sockfd);

    return 0;
}


import socket
import threading

from entry import process


def handle_client(client_socket, client_address):
    # 处理客户端请求的函数
    # 这里可以根据你的需求进行自定义的处理逻辑
    # 例如，接收和发送数据等操作

    # 接收客户端发送的数据
    data = client_socket.recv(1024)
    # 处理数据...
    response = process(data)

    # 发送响应给客户端
    client_socket.send(response.encode())

    # 关闭客户端连接
    client_socket.close()


def start_server(host, port):
    # 创建服务器套接字
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 绑定地址和端口
    server_socket.bind((host, port))

    # 开始监听连接
    server_socket.listen(5)
    print(f"Server listening on {host}:{port}")

    try:
        while True:
            # 接受客户端连接
            client_socket, client_address = server_socket.accept()
            print(f"New connection from {client_address[0]}:{client_address[1]}")

            # 创建线程来处理客户端请求
            client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
            client_thread.start()
    finally:
        # 关闭服务器套接字
        server_socket.close()

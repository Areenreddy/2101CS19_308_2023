from socket import *
server_host = "127.0.0.1"
server_port = 12345

client_socket = socket(AF_INET,SOCK_STREAM)
client_socket.connect((server_host,server_port))

message = input()
message = "2" + message
client_socket.send(message.encode())

response_message = client_socket.recv(1024).decode()
print(f"Server's response: {response_message}")

client_socket.close()

from socket import *

def q1(sentence, client_socket):
    print(sentence)
    sentence = sentence[1::]
    response_message = "Have a good day"
    client_socket.send(response_message.encode())
        
def q2(sentence, client_socket):
    print(sentence)
    sentence = sentence[1::]
    vowels = ["a","e","i","o","u","A","E","I","O","U"]
    response_message = 0
    for i in sentence:
        if i in vowels:
            response_message+=1
    response_message = str(response_message)
    client_socket.send(response_message.encode())

def q3(sentence, client_socket):
    print(sentence[1:len(sentence)-1:])
    if sentence == sentence[::-1]:
        response_message = "String is palindrome"
    else:
        response_message = "String is not palindrome"
    client_socket.send(response_message.encode())

def q4(sentence, client_socket):
    sentence = sentence[1:]
    print(sentence)
    l = list(sentence.strip().split())
    l1 = []
    for i in l:
        l1.append(int(i))
    response_message = str(sum(l1))
    client_socket.send(response_message.encode())

host = "127.1.1.12"
port = 22222

server_socket = socket(AF_INET, SOCK_STREAM)
server_socket.bind((host, port))

server_socket.listen(3)
print(f"Server is listening on {host}:{port}")

while True:
    client_socket, client_address = server_socket.accept()
    sentence = client_socket.recv(1024).decode()

    if sentence[0] == "1":
        q1(sentence, client_socket)
    elif sentence[0] == "2":
        q2(sentence, client_socket)
    elif sentence[0] == "3":
        q3(sentence, client_socket)
    elif sentence[0] == "a":
        q4(sentence, client_socket)

    client_socket.close()

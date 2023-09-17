#include <string>
#include <vector>
#include <iostream>
#include <winsock2.h>
#include <cstring>


void q1(std::string sentence, int client_socket) {
    std::cout << sentence << std::endl;
    sentence = sentence.substr(1);
    std::string response_message = "Have a good day";
    send(client_socket, response_message.c_str(), response_message.size(), 0);
}

void q2(std::string sentence, int client_socket) {
    std::cout << sentence << std::endl;
    sentence = sentence.substr(1);
    std::string vowels = "aeiouAEIOU";
    int response_message = 0;
    for (char i : sentence) {
        if (vowels.find(i) != std::string::npos) {
            response_message++;
        }
    }
    send(client_socket, std::to_string(response_message).c_str(), sizeof(int), 0);
}

void q3(std::string sentence, int client_socket) {
    std::cout << sentence.substr(1, sentence.length() - 2) << std::endl;
    std::string response_message;
    if (sentence == std::string(sentence.rbegin(), sentence.rend())) {
        response_message = "String is palindrome";
    } else {
        response_message = "String is not palindrome";
    }
    send(client_socket, response_message.c_str(), response_message.size(), 0);
}

void q4(std::string sentence, int client_socket) {
    sentence = sentence.substr(1);
    std::cout << sentence << std::endl;
    std::string delimiter = " ";
    size_t pos = 0;
    int response_message = 0;
    while ((pos = sentence.find(delimiter)) != std::string::npos) {
        response_message += std::stoi(sentence.substr(0, pos));
        sentence.erase(0, pos + delimiter.length());
    }
    response_message += std::stoi(sentence);
    send(client_socket, std::to_string(response_message).c_str(), sizeof(int), 0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(22222);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    listen(server_socket, 3);
    std::cout << "Server is listening on port 22222" << std::endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        recv(client_socket, buffer, sizeof(buffer), 0);
        std::string sentence(buffer);
        
        if (sentence[0] == '1') {
            q1(sentence, client_socket);
        } else if (sentence[0] == '2') {
            q2(sentence, client_socket);
        } else if (sentence[0] == '3') {
            q3(sentence, client_socket);
        } else if (sentence[0] == 'a') {
            q4(sentence, client_socket);
        }

        close(client_socket);
    }

    close(server_socket);
    return 0;
}

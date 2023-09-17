
// importing required header files

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <vector>


//predefined macros
#define SERVER_IP "127.0.0.1"
#define PORT_TASK1 12345
using namespace std;

int main() 
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    // Create socket
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) 
   {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    // Configure server address
    serverAddr.sin_family = AF_INET;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    serverAddr.sin_port = htons(PORT_TASK1);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Try connecting to the selected task server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof serverAddr) == 0) 
    {
                // Send a message to the server for Task 1
                string message = "Hello IITP";
                send(clientSocket, message.c_str(), message.length(), 0);
                // Receive and display the server's response for Task 1
                char response[1024];
                recv(clientSocket, response, sizeof(response), 0);
                cout << "Server response (Task 1): " << response << std::endl;
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}



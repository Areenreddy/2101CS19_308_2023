#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstdio>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define PORT_TASK4 12348
using namespace std;

int main(int argc, char* argv[]) 
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
    serverAddr.sin_port = htons(PORT_TASK4);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Try connecting to the selected task server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof serverAddr) == 0) 
    {
               std::vector<int> integer_array = {1, 2, 3, 4, 5};
                send(clientSocket, integer_array.data(), integer_array.size() * sizeof(int), 0);

                // Receive and display the server's response for Task 4
                int response;
                recv(clientSocket, &response, sizeof(response), 0);
                std::cout << "Sum of elements in the array (Task 4): " << response << std::endl;
                
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}



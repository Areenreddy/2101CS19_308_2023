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
#define PORT_TASK3 12347
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
    serverAddr.sin_port = htons(PORT_TASK3);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Try connecting to the selected task server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof serverAddr) == 0) 
    {
               if (argc < 1) 
		{
                    std::cerr << "Usage: <input_string>" << std::endl;
                    close(clientSocket);
                    return 1;
                }

                std::string input_string = argv[1];
                send(clientSocket, input_string.c_str(), input_string.length(), 0);

                // Receive and display the server's response for Task 3
               int response;
                recv(clientSocket, &response, sizeof(response), 0);
               if(response==0)
		{
			cout<<"Not palindrome"<<endl;
		}
		else{
			cout<<"Palindrome"<<endl;						
	        }
		
    }

    // Close the client socket
    close(clientSocket);

    return 0;
}



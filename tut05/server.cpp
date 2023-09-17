

// importing required header files
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

//predefined macros
#define SERVER_IP "127.0.0.1"
#define PORT_TASK1 12345
#define PORT_TASK2 12346
#define PORT_TASK3 12347
#define PORT_TASK4 12348

using namespace std;

// Function to check if a string is a palindrome
bool isPalindrome(const char* str) {
    int start = 0;
    int end = strlen(str) - 1;

    while (start < end) 
{
        if (str[start] != str[end]) {
            return false;
        }
        start++;
        end--;
    }

    return true;
}
int task1()
{
    // Server configuration
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer1[1024];

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Port for Task 1
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error listening" << std::endl;
        return 1;
    }

    std::cout << "Listening for Task 1 on port 12345..." << std::endl;

    // Accept a connection from a client
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }

    // Receive data from the client
    ssize_t bytesRead = recv(clientSocket, buffer1, sizeof(buffer1), 0);

    if (bytesRead < 0) {
        std::cerr << "Error receiving data" << std::endl;
        return 1;
    }

    // Display the received message
    std::cout << "Received message (Task 1): " << buffer1 << std::endl;

    // Send a response message to the client
    const char* response = "Have a good day";
    send(clientSocket, response, strlen(response), 0);

    // Close the client socket and server socket
    close(clientSocket);
    close(serverSocket);

    return 0;
}
int task2()
{
    // Server configuration
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer2[1024];

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12346);  // Port for Task 2
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error listening" << std::endl;
        return 1;
    }

    std::cout << "Listening for Task 2 on port 12346..." << std::endl;

    // Accept a connection from a client
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket < 0) {
        std::cerr << "Error accepting connection" << std::endl;
        return 1;
    }

    // Receive data from the client
    ssize_t bytesRead = recv(clientSocket, buffer2, sizeof(buffer2), 0);

    if (bytesRead < 0) {
        std::cerr << "Error receiving data" << std::endl;
        return 1;
    }
	
cout<<"Received input string "<<buffer2<<endl;
    // Count the number of vowels in the received string
    int vowelCount = 0;
    for (int i = 0; i < bytesRead; ++i) {
        char c = tolower(buffer2[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowelCount++;
        }
    }

    // Display the number of vowels
    std::cout << "Number of vowels (Task 2): " << vowelCount << std::endl;
	send(clientSocket,&vowelCount,sizeof(vowelCount),0);
    // Close the client socket and server socket
    close(clientSocket);
    close(serverSocket);

    return 0;
}
int task3()
{

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);
    char buffer3[1024];

    // Create socket
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_TASK3);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    std::cout << "Listening for Task 3 on port 12347..." << std::endl;

    // Accept a connection from a client
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addr_size);

    if (clientSocket < 0) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    // Receive the string from the client
    ssize_t bytesRead = recv(clientSocket, buffer3, sizeof(buffer3), 0);

    if (bytesRead < 0) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

     std::cout << "Received input (Task 3): " << buffer3 << std::endl;

    // Check if the received string is a palindrome
    bool is_palindrome = isPalindrome(buffer3);
	
    // Send the result to the client
    if (is_palindrome) {
	int response =1;

        send(clientSocket, &response,sizeof(int),0);
    } else {
int response=0;
        send(clientSocket,&response ,sizeof(int), 0);
    }
	
    // Close the client socket and server socket
    close(clientSocket);
    close(serverSocket);

    return 0;
}
int task4()
{
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;

    // Create socket
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_TASK4);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    // Bind the socket to the address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof serverAddr) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 10) == 0) {
        std::cout << "Listening on port " << PORT_TASK4 << " for Task 4..." << std::endl;
    } else {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    addr_size = sizeof serverStorage;
    newSocket = accept(serverSocket, (struct sockaddr *)&serverStorage, &addr_size); // Accept a new connection

    // Receive the integer array from the client
    int array[5];
    int bytesReceived = recv(newSocket, array, sizeof(array), 0);
    if (bytesReceived < 0) {
        perror("Error receiving data");
        exit(EXIT_FAILURE);
    }

    // Calculate the sum of elements in the array
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += array[i];
    }

    // Send the sum back to the client
    send(newSocket, &sum, sizeof(sum), 0);

    // Close the sockets
    close(newSocket);
    close(serverSocket);

    return 0;
}
int main(int argc,char* argv[]) 
{

for(int i=0;i<4;i++)
{
int tasknumber;
cout<<"Enter Task Number"<<endl;

cin>>tasknumber;
if(tasknumber==1)
{
	task1();
}else if(tasknumber==2)
{
	task2();
}
else if(tasknumber==3)
{
	task3();
}
else if(tasknumber==4)
{
task4();
}
}
return 0;
	
}


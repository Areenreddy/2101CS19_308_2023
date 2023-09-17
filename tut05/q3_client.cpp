#include <iostream>
#include <winsock2.h>

using namespace std;

int main() {
  // Initialize Winsock
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  // Create a TCP socket
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket == INVALID_SOCKET) {
    cout << "Error creating socket: " << WSAGetLastError() << endl;
    return 1;
  }

  // Connect to the server
  SOCKADDR_IN server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(12345);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (connect(client_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
    cout << "Error connecting to server: " << WSAGetLastError() << endl;
    return 1;
  }

  // Send the message to the server
  string message = "MALAYALAM";
  message = "3" + message + "3";
  send(client_socket, message.c_str(), message.length(), 0);

  // Receive the response message from the server
  char response_message[1024];
  int response_message_size = recv(client_socket, response_message, sizeof(response_message), 0);
  if (response_message_size == SOCKET_ERROR) {
    cout << "Error receiving response message: " << WSAGetLastError() << endl;
    return 1;
  }

  // Print the response message
  cout << "Server's response: " << response_message << endl;

  // Close the socket
  closesocket(client_socket);

  // Shutdown Winsock
  WSACleanup();

  return 0;
}

//Compile with ws2_32 library
//gcc server.c -o server -lws2_32

#include <stdio.h>
#include <stdlib.h>
#include <Winsock2.h>

#define BUFFER_SIZE 1024

void tcp_server(int port, int secret) {
    WSADATA wsaData;
    SOCKET server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error: %d\n", WSAGetLastError());
        return;
    }
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed. Error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    printf("TCP Server: Waiting for a connection...\n");

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
        printf("Accept failed. Error: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return;
    }

    // Receive data from the client
    int valread = recv(new_socket, buffer, BUFFER_SIZE, 0);
    if (valread > 0) {
        printf("Message received: %s\n", buffer);
        int received_secret = atoi(buffer);
        if(received_secret == secret){
            printf("Success!");
        }
        else{
            while(1){} //The bot will wait forever. Sometimes my genious is frightening
        }
    }

    // Close the connection
    closesocket(new_socket);
    closesocket(server_fd);
    WSACleanup();  
    printf("TCP Server: Connection closed.\n");
}

int main(int argc, char *argv[]){
    int port;
    int secret;

    if(argc != 3){
        exit(1);
    }
    else{
        port = atoi(argv[1]);
        secret = atoi(argv[2]);
    }

    tcp_server(port, secret);

    return 0;
}
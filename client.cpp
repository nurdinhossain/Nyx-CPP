#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"

int connectSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
    }
	
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP.c_str(), &(serverAddr.sin_addr)) <= 0) {
        std::cerr << "Invalid address/Address not supported\n";
    }

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
    }

    return sock;
}

void closeSocket(int sock)
{
    close(sock);
}

std::thread initListenThread(int sock, std::string& buffer)
{
    return std::thread([sock, &buffer]() {
        while (true) {
            char buf[BUFFER_SIZE];
            int bytesReceived = recv(sock, buf, BUFFER_SIZE, 0);
            if (bytesReceived == -1) {
                std::cerr << "Error in recv()\n";
                break;
            }
            if (bytesReceived == 0) {
                std::cout << "Client disconnected\n";
                break;
            }
            buffer = std::string(buf, bytesReceived);
        }
    });
}

void sendMessage(int sock, std::string message)
{
    send(sock, message.c_str(), message.length(), 0);
}

void killListenThread(std::thread& listenThread)
{
    listenThread.detach();
}
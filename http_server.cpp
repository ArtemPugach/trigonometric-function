#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <errno.h>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <signal.h>
#include <atomic>
#include "FuncA.h"

#define PORT 8081

std::atomic<bool> running(true);

void handle_sigint(int sig) {
    running = false;
    printf("\nShutting down server...\n");
}

char HTTP_200HEADER[] = "HTTP/1.1 200 Ok\r\n";
char HTTP_404HEADER[] = "HTTP/1.1 404 Not Found\r\n";

void sendGETresponse(int fdSocket, char strFilePath[], char strResponse[]);
void sendComputeResponse(int fdSocket, char strResponse[]);

int CreateHTTPserver() {
    signal(SIGINT, handle_sigint); // Set up SIGINT handler

    int connectionSocket, clientSocket, pid;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((connectionSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket open failed\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(connectionSocket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("socket bind failed\n");
        close(connectionSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(connectionSocket, 10) < 0) {
        perror("socket listen failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port %d\n", PORT);

    // Use select() to manage accept()
    fd_set readfds;
    struct timeval timeout;

    while (running) {
        FD_ZERO(&readfds);
        FD_SET(connectionSocket, &readfds);

        // Set timeout to 1 second
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(connectionSocket + 1, &readfds, NULL, NULL, &timeout);

        if (activity < 0 && errno != EINTR) {
            perror("select error");
            break;
        }

        // If there was a termination signal, exit the loop
        if (!running) break;

        // Check if there is a new connection
        if (FD_ISSET(connectionSocket, &readfds)) {
            clientSocket = accept(connectionSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            if (clientSocket < 0) {
                perror("Error accept()");
                continue;
            }

            pid = fork();
            if (pid < 0) {
                perror("Error on fork");
                close(clientSocket);
                continue;
            }

            if (pid == 0) { // Child process
                close(connectionSocket); // Child closes the main socket

                char buffer[30000] = { 0 };
                int bytesRead = read(clientSocket, buffer, sizeof(buffer));
                if (bytesRead <= 0) {
                    printf("Error reading request or connection closed by client.\n");
                    close(clientSocket);
                    exit(0);
                }

                printf("\nClient message:\n%s\n", buffer);

                char strResponse[500] = { 0 };

                // Handle GET request
                if (strstr(buffer, "GET /compute")) {
                    sendComputeResponse(clientSocket, strResponse);
                }
                else {
                    sprintf(strResponse, "%sContent-Type: text/plain\r\nContent-Length: 13\r\nConnection: close\r\n\r\n404 Not Found", HTTP_404HEADER);
                    write(clientSocket, strResponse, strlen(strResponse));
                }

                close(clientSocket);
                exit(0);
            }
            else {
                close(clientSocket); // Parent closes the client socket
            }
        }
    }

    printf("Closing server socket...\n");
    close(connectionSocket);
    return 0;
}

void sendComputeResponse(int fdSocket, char strResponse[]) {
    auto t1 = std::chrono::high_resolution_clock::now();

    FuncA funcA;
    const int terms = 1000;
    const double x = 0.5;

    std::vector<double> values;
    for (int i = 0; i < 200000; ++i) {
        values.push_back(funcA.arctg(x, terms));
    }

    for (int i = 0; i < 100; ++i) {
        std::sort(values.begin(), values.end());
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    char strElapsed[50];
    sprintf(strElapsed, "%lld", elapsed_ms.count());

    sprintf(strResponse,
        "%sContent-Type: text/plain\r\nContent-Length: %ld\r\nConnection: close\r\n\r\n",
        HTTP_200HEADER, strlen(strElapsed));

    write(fdSocket, strResponse, strlen(strResponse));
    write(fdSocket, strElapsed, strlen(strElapsed));

    printf("\nElapsed time: %s ms\n", strElapsed);
}


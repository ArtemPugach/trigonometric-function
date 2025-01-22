#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <csignal>
#include <sys/wait.h>
#include <regex>

// Include the server function
extern int CreateHTTPserver();

#define SERVER_PORT 8081
#define SERVER_ADDRESS "127.0.0.1"

// Function to send an HTTP request and validate the response
bool testHTTPServer() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Client socket creation failed");
        return false;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);

    // Convert address
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serverAddress.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(clientSocket);
        return false;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Connection to the server failed");
        close(clientSocket);
        return false;
    }

    std::cout << "Connected to server..." << std::endl;

    // Send a request to `/compute`
    const char* request = "GET /compute HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    send(clientSocket, request, strlen(request), 0);

    char buffer[4096] = { 0 }; // Buffer for the response
    int bytesRead;
    std::string response;

    // Read the response from the server
    while ((bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the string
        response += buffer;       // Append to the full response string
    }

    if (bytesRead < 0) {
        perror("Read from server failed");
        close(clientSocket);
        return false;
    }

    close(clientSocket);

    // Validate the response
    std::cout << "Server response:\n" << response << std::endl;

    // Check if the response contains HTTP 200 OK
    if (response.find("HTTP/1.1 200 Ok") == std::string::npos) {
        std::cerr << "Response does not contain 200 OK.\n";
        return false;
    }

    // Check if the response contains the elapsed time as a number
    std::regex elapsedTimeRegex(R"(\r\n\r\n(\d+))"); // Regex to find a numeric value after the headers
    std::smatch match;
    if (std::regex_search(response, match, elapsedTimeRegex) && match.size() > 1) {
        std::cout << "Elapsed time received: " << match[1] << " ms" << std::endl;
        return true; // Test passed
    }

    std::cerr << "Response does not contain elapsed time.\n";
    return false;
}

int main() {
    std::cout << "Starting HTTP server test..." << std::endl;

    // Fork to create a child process for the server
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {
        // Child process: Run the server
        CreateHTTPserver();
        exit(0); // Exit child process after the server stops
    }
    else {
        // Parent process: Wait for the server to start
        sleep(1); // Give the server time to initialize

        // Run the client test
        bool testResult = testHTTPServer();

        // Stop the server by sending SIGINT
        kill(pid, SIGINT);

        // Wait for the child process to terminate
        waitpid(pid, NULL, 0);

        if (testResult) {
            std::cout << "Test passed: Server responded correctly." << std::endl;
        }
        else {
            std::cout << "Test failed: Server response was incorrect." << std::endl;
        }

        std::cout << "Server test finished and server shut down." << std::endl;
    }

    return 0;
}


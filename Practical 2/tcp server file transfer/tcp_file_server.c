#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 10035
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUF_SIZE];
    int addrlen = sizeof(address);

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("TCP File Server listening on port %d...\n", PORT);

    // Accept
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Receive filename first
    memset(buffer, 0, BUF_SIZE);
    read(new_socket, buffer, BUF_SIZE);
    printf("Receiving file: %s\n", buffer);
    
    // Create or overwrite the file
    int file_fd = open(buffer, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) {
        perror("file create failed");
        exit(EXIT_FAILURE);
    }

    // Receive file content
    ssize_t bytes_received;
    while ((bytes_received = read(new_socket, buffer, BUF_SIZE)) > 0) {
        write(file_fd, buffer, bytes_received);
    }

    printf("File received successfully!\n");
    
    close(file_fd);
    close(new_socket);
    close(server_fd);
    return 0;
}

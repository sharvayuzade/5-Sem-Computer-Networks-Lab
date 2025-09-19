#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 10035
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connection failed");
        return 1;
    }

    // Send filename first
    char *filename = argv[1];
    send(sock, filename, strlen(filename), 0);

    // Open and send file
    int file_fd = open(filename, O_RDONLY);
    if (file_fd < 0) {
        perror("file open failed");
        return 1;
    }

    ssize_t bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUF_SIZE)) > 0) {  // Fixed this line
        send(sock, buffer, bytes_read, 0);
    }

    printf("File sent successfully!\n");

    close(file_fd);
    close(sock);
    return 0;
}

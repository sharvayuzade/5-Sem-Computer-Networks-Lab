#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Use the port number you specified
#define PORT 10035
#define BUF_SIZE 1024

void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];
    socklen_t client_len = sizeof(client_addr);
    ssize_t bytes_received;
    FILE *fp;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        die("socket creation failed");
    }

    // Clear and set up server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on any available interface

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        die("bind failed");
    }

    printf("UDP File Server listening on port %d...\n", PORT);

    while (1) {
        printf("\nWaiting for a filename from a client...\n");

        // Receive the filename from the client
        bytes_received = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received < 0) {
            die("recvfrom failed");
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received filename

        printf("Client requested file: '%s'\n", buffer);

        // Try to open the requested file in binary read mode
        fp = fopen(buffer, "rb");
        if (fp == NULL) {
            // If file doesn't exist, send an error message
            printf("File not found. Sending error message to client.\n");
            sendto(sockfd, "ERROR: File not found", 22, 0, (struct sockaddr *)&client_addr, client_len);
            continue; // Wait for the next request
        }

        // File exists, start sending it in chunks
        printf("Sending file contents...\n");
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
            if (sendto(sockfd, buffer, bytes_read, 0, (struct sockaddr *)&client_addr, client_len) < 0) {
                die("sendto failed");
            }
        }

        // Send a zero-byte packet to signal the end of the file transfer
        printf("End of file reached. Sending EOF signal.\n");
        sendto(sockfd, "", 0, 0, (struct sockaddr *)&client_addr, client_len);
        
        fclose(fp);
        printf("File transfer complete.\n");
    }

    close(sockfd);
    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Use the same port number as the server
#define PORT 10035
#define BUF_SIZE 1024

// Change this to the server's IP address if not running on the same machine
#define SERVER_IP "127.0.0.1" 

void die(char *s) {
    perror(s);
    exit(1);
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];
    char source_filename[100];
    char dest_filename[100];
    socklen_t server_len = sizeof(server_addr);
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
    // Convert IP address from text to binary form
    if (inet_aton(SERVER_IP, &server_addr.sin_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    
    // Get filenames from user
    printf("Enter filename to request from server: ");
    scanf("%s", source_filename);
    
    printf("Enter filename to save as on client: ");
    scanf("%s", dest_filename);

    // Send the requested filename to the server
    if (sendto(sockfd, source_filename, strlen(source_filename), 0, (struct sockaddr *)&server_addr, server_len) < 0) {
        die("sendto failed");
    }

    printf("\nRequest sent for file '%s'. Waiting for server response...\n", source_filename);

    // Open the destination file in binary write mode
    fp = fopen(dest_filename, "wb");
    if (fp == NULL) {
        die("fopen failed for destination file");
    }

    // Receive the file content
    while (1) {
        bytes_received = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&server_addr, &server_len);
        
        if (bytes_received < 0) {
            die("recvfrom failed");
        }

        // A zero-byte packet signifies the end of the file
        if (bytes_received == 0) {
            printf("EOF signal received. File transfer complete.\n");
            break;
        }

        // Check for an error message from the server
        if (strncmp(buffer, "ERROR:", 6) == 0) {
            printf("Server error: %s\n", buffer);
            fclose(fp);
            remove(dest_filename); // Delete the empty file
            close(sockfd);
            return 1;
        }
        
        // Write the received chunk to the file
        fwrite(buffer, 1, bytes_received, fp);
    }
    
    fclose(fp);
    close(sockfd);
    
    printf("File '%s' received successfully and saved as '%s'.\n", source_filename, dest_filename);
    return 0;
}

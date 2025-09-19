#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 10035 
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUF_SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUF_SIZE);
        read(sock, buffer, BUF_SIZE);
        printf("Echo from server: %s", buffer);
    }

    close(sock);
    return 0;
}


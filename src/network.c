/* Jacobus Burger (2023)
 * This source file handles peer-to-peer networking and associated
 * ADT's for both single/multi-player games of battleship. The
 * former case uses localhost for networking, while the latter
 * looks for available clients advertising themselves.
 *
 * This us currently a WIP
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    char buffer[256];

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }
    printf("Socket opened\n");

    // Set up server address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = 8080; // Port number for communication
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    inet_pton(AF_INET, "127.0.0.1", &(serv_addr.sin_addr)); // IP address of the server

    // Connect to server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Error connecting to the server");
    }
    printf("Connected to server\n");

    while (1) {
        printf("Enter a message: ");
        memset(buffer, 0, sizeof(buffer));
        scanf("%255[^\n]", buffer); // Read input from the keyboard

        // Send data to the server
        write(sockfd, buffer, strlen(buffer));

        // Receive response from the server
        memset(buffer, 0, sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer) - 1);
        printf("Recieved Message: %s\n", buffer);
    }

    // Close the socket
    close(sockfd);

    return 0;
}

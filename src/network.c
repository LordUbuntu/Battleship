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
#include <pthread.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void *handle_client(void *arg) {
    int newsockfd = *((int *)arg);
    char buffer[256];
    int n;

    // Read and write data
    memset(buffer, 0, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        error("Error reading from socket");
    }
    printf("Client: %s\n", buffer);

    write(newsockfd, "Message received by the server", 30);

    // Close the socket
    close(newsockfd);

    pthread_exit(NULL);
}

int main() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread_id;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening socket");
    }

    // Set up server address struct
    memset(&serv_addr, 0, sizeof(serv_addr));
    portno = 12345; // Port number for communication
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("Error on binding");
    }

    // Listen for incoming connections
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        // Accept incoming connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("Error on accept");
        }

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&newsockfd) != 0) {
            error("Error creating thread");
        }
    }

    // Close the main socket
    close(sockfd);

    return 0;
}

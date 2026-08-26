/*
* Jacobus Burger (2025)
* Coming back to this after so long. Will be quick to implement a simple
*       message-passing networking feature over LAN and maybe also WAN
* 
* Helpful resources for anyone doing networking in C:
* - https://beej.us/guide/bgnet/html/
*/
// 2026-08-25, back again to start. This is a good project to get back into habit. Gonna implement LAN multiplayer.
// thx to "Think and Learn" channel, actually useful socket programming tutorial
// start this file just as standalone like a main.c file
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char *argv[])
{
        int sockfd, newsockfd, portno;
        socklen_t clilen;
        socklen_t srvlen;
        char buf[1024]; // may be excessive but can shrink down later alongside encoding methods
        // no 'return 0;' needed under normal program end conditions for C99+
        printf("closed");
}

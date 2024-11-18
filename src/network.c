/* Jacobus Burger (2023)
 * This source file handles peer-to-peer networking and associated
 * ADT's for both single/multi-player games of battleship. The
 * former case uses localhost for networking, while the latter
 * looks for available clients advertising themselves.
 *
 * This us currently a WIP
 *
 * Resources:
 * - Beej's Guide to Network Programming
 *   https://beej.us/guide/bgnet/html/split/index.html
 * - Socket Programming in C for Beginners
 *   https://www.youtube.com/watch?v=KEiur5aZnIM
 * References:
 * - Simple netcat in C
 *   https://github.com/guzlewski/netcat
 *
 * Design Plan:
 * The goal of the networking is a simple LAN network application.
 * Each move, A sends a packet of the move they made to B. That simple.
 */

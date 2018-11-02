//
//  client.cpp
//  client
//
//  Created by VeeVee Wang on 6/6/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <iostream>         // input / output stream library
#include <stdlib.h>         // general utilites library
#include <unistd.h>         // access to POSIX
#include <string.h>         // string library
#include <stdio.h>          // needed for client code
#include <sys/types.h>      // needed for client code
#include <sys/socket.h>     // needed for client code
#include <netinet/in.h>     // needed for client code
#include <netdb.h>          // needed for client code


// FUNCTION PROTOTYPES
void error(const char*);                // error message and exits
bool isNumber(std::string);             // checks if input is valid


int main(int argc, const char * argv[]) {

    int sockfd, portno, n;          // socket and port vars
    struct sockaddr_in serv_addr;   // address of server we want to connect to
    struct hostent *server;         // pointer to a structure of type hostent
    char nameBuffer[256];           // holds char array for player's name
    char guessBuffer[256];          // holds char array for player's guess
    char congratsBuffer[256];       // holds char array of congrats message
    char leaderBuffer[1000];        // holds char array of leaderboard
    int turnNumber = 1;             // tracks the current turn number
    int closenessValue;             // holds "closeness" value
    int guessValue = -1;            // holds user's guess value
    int guessToSend;                // holds htons(guessValue)
    bool isValid;                   // checks if guess is valid input

    // if user fails to pass 2 command line argugments, display error and exit
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);                     // store port number
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   // create a socket

    // check socket create was unsuccessful
    if (sockfd < 0) error("ERROR opening socket");

    // store name of host on the Internet
    server = gethostbyname(argv[1]);

    // check if host is valid
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host");
        exit(0);
    }

    // set fields in serv_addr
    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // attempt to connect with server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }


    // CONVERSATION WITH SERVER BEGINS
    printf("\nWelcome to the Number Guessing Game!\n");

    // GET USERS NAME
    printf("Please enter you name: ");
    bzero(nameBuffer, 256);
    fgets(nameBuffer, 255, stdin);
    n = (int)write(sockfd,nameBuffer,255);
    if (n < 0 ) error("ERROR writing to socket");


    // NUMBER GUESSING
    do {

        printf("\nTurn: %u\n", turnNumber++);

        // ensure that guess value in within range
        do {

            printf("Enter a guess: ");             // get user's number guess
            bzero(guessBuffer, 256);               // clear the buffer
            fgets(guessBuffer, 255, stdin);        // read message from stdin

            std::string userGuess = guessBuffer;
            isValid = true;

            if (isNumber(userGuess)) {
                guessValue = stoi(userGuess);

                if (guessValue < 0 || guessValue > 9999) {
                    isValid = false;
                    printf("Invalid Input!\n");
                }
            }
            else {
                isValid = false;
                printf("Invalid Input!\n");
            }

        } while (!isValid);


        // WRITE GUESS
        guessToSend = htons(guessValue);
        n = (int)write(sockfd, &guessToSend, sizeof(guessToSend));
        if (n < 0 ) error("ERROR writing to socket");

        // READ CLOSENESS
        n = (int)read(sockfd, &closenessValue, sizeof(closenessValue));
        if (n < 0) error("ERROR reading from socket");
        closenessValue = ntohs(closenessValue);

        // PRINT CLOSENESS
        printf("Result of guess: %u\n", closenessValue);

    } while(closenessValue != 0);


    // THE GAME HAS BEEN WON!


    // READ THE CONGRATS MESSAGE
    bzero(congratsBuffer, 256);
    n = (int)read(sockfd, congratsBuffer, 255);
    if (n < 0) error("ERROR reading from socket");
    printf("\n\n%s\n\n", congratsBuffer);

    // READ AND PRINT LEADERBOARD
    bzero(leaderBuffer, 1000);
    n = (int)read(sockfd, leaderBuffer, 999);
    if (n < 0) error("ERROR reading from socket");
    printf("\nLEADERBOARD:\n");
    printf("%s\n\n", leaderBuffer);

    close(sockfd);
    return 0;
}


// error message
void error (const char *msg) {
    perror(msg);
    exit(0);
}

// checks if input is a number
bool isNumber(std::string s) {

    for (unsigned int i = 0; i < s.length() - 1; i++) {
        if (!(isdigit(s[i]))) {
            return false;
        }
    }

    return true;
}

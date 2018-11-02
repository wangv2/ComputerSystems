//
//  server.cpp
//  server
//
//  Created by VeeVee Wang on 6/6/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <iostream>         // input / output stream library
#include "pthread.h"        // thread library
#include <semaphore.h>      // semaphore library
#include <vector>           // vector library (for leaderboard)
#include <sstream>          // string stream
#include <stdlib.h>         // general utilities library
#include <string.h>         // string library
#include <unistd.h>         // access to POSIX
#include <stdio.h>          // needed for server code
#include <sys/types.h>      // needed for server code
#include <sys/socket.h>     // needed for server code
#include <netinet/in.h>     // needed for server code
#include <time.h>           // time


// Structure used to pass into thread function.
struct ThreadArgs {

    int clientSock;

};

// struct for content of leader board, holds a player's name and score
struct leaderboardContent {

    std::string name;
    int score;

};


// FUNCTION PROTOTYPES
void error(const char*);            // error message and exits
void errorOnRead(const char*);      //
void *threadMain(void*);            // function called when a thread is made
void processClient(int&);           // converstaion between server and client
int calculateCloseness(int, int);   // calculate's closeness value
void updateLeaderboard(leaderboardContent&);// updates leaderboard


// GLOBAL VARIABLES
sem_t leaderboardAccess;                    // used to update the leaderboard
std::vector <leaderboardContent> board;     // leaderboard


int main(int argc, const char * argv[]) {

    int sockfd;                     // file descriptor socket system call
    int portno;                     // stores port # for accept connections
    struct sockaddr_in serv_addr;   // contains address of the server
    srand ((int)time(NULL));        // for random number generation

    // if user fails to pass port number, display error message
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided");
        exit(1);
    }

    // create a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // if socket creation is unsuccessful, display error message
    if (sockfd < 0) {
        fprintf(stderr, "ERROR, no port provided");
    }

    // sets all values in buffer to zero
    bzero((char*) &serv_addr, sizeof(serv_addr));

    // port number on which the server will listen for connections
    portno = atoi(argv[1]);

    // setting fields of serv_addr
    serv_addr.sin_family = AF_INET;         // code for address family
    serv_addr.sin_port = htons(portno);     // contains port numbers
    serv_addr.sin_addr.s_addr = INADDR_ANY; // contains IP address of the host

    // binds a socket to an address
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }

    // initialize semaphore
    sem_init(&leaderboardAccess, 0, 1);

    // allows process to listen on socket for connections
    if (listen(sockfd, 5) < 0)
        error("ERROR on listening");

    // infinite loop of accepting clients
    while (true) {

        // Accept connection from client
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientSock = accept(sockfd,(struct sockaddr *)&clientAddr, &addrLen);
        if (clientSock < 0){
            error("ERROR on accepting");
            exit(-1);
        }

        // Create and initialize argument struct
        ThreadArgs* threadArgs =  new ThreadArgs;
        threadArgs->clientSock = clientSock;

        // Create client thread
        pthread_t threadID;
        int status = pthread_create(&threadID, NULL, threadMain,(void *)threadArgs);
        if (status != 0) exit(-1); // Note: status == 0 is GOOD

    }

    sem_destroy(&leaderboardAccess);
    return 0;
}


// function called when a new thread is created
void *threadMain(void *args) {

    // Extract socket file descriptor from argument
    struct ThreadArgs *threadArgs = (struct ThreadArgs *) args;
    int clientSock = threadArgs->clientSock;
    delete threadArgs;

    // Communicate with client
    processClient(clientSock);

    // Reclaim resources before finishing
    pthread_detach(pthread_self());
    close(clientSock);
    return NULL;
}

// function called from thread; holds converation bewteen server and client
void processClient(int& clientSock) {

    leaderboardContent player;              // player struct of current player
    int mysteryNumber = rand() % 10000;     // the mystery number
    int closeness;                          // holds calculated closeness value
    int n;                                  // holds result of read() & write()
    char nameBuffer[256];                   // holds read() and write() values
    char leaderBuffer[1000];                // holds char array of leaderboard
    int playersGuess;                       // holds the player's guess value
    int totalRounds = 0;                    // tracks total guesses from player
    std::string nameAsString;               // holds players name as String


    // read user's name from client
    bzero(nameBuffer, 256);
    n = (int)read(clientSock, nameBuffer, 255);
    if (n < 0) errorOnRead("ERROR reading from socket");
    nameBuffer[strlen(nameBuffer) - 1] = 0;
    player.name = nameBuffer;

    std::cout << std::endl << mysteryNumber << std::endl;

    // START GAME
    do {

        // get the user's guess
        n = (int)read(clientSock, &playersGuess, sizeof(playersGuess));
        playersGuess = ntohs(playersGuess);
        if (n < 0) errorOnRead("ERROR reading from socket");

        closeness = calculateCloseness(mysteryNumber, playersGuess);

        // write closeness value back to client
        closeness = htons(closeness);
        int n = (int)write(clientSock, &closeness, sizeof(closeness));
        if (n < 0) error("ERROR writing to socket");

        // track total rounds
        totalRounds++;

    } while (closeness != 0);


    // GAME HAS BEEN WON!

    // update player's total score
    player.score = totalRounds;

    // WRITE CONGRATS MESSAGE
    char congratsBuffer[256];
    bzero(congratsBuffer, 256);
    std::stringstream congratsMessage;
    congratsMessage << "Congratulations! It took ";
    congratsMessage << totalRounds;
    congratsMessage << " to guess the number!\n";
    strcpy(congratsBuffer, congratsMessage.str().c_str());

    n = (int)write(clientSock, congratsBuffer, 255);
    if (n < 0) error("ERROR writing to socket");

    // UPDATE LEADERBOARD
    sem_wait(&leaderboardAccess);
    updateLeaderboard(player);
    sem_post(&leaderboardAccess);

    // SENDS LEADERBOARD
    bzero(leaderBuffer, 1000);
    std::stringstream leaderString;

    for (unsigned int i = 0; i < board.size(); i++) {
        leaderString << std::to_string(i + 1);
        leaderString << ". ";
        leaderString << board.at(i).name;
        leaderString << " ";
        leaderString << board.at(i).score;
        leaderString << "\n";
    }

    strcpy(leaderBuffer, leaderString.str().c_str());
    n = (int)write(clientSock, leaderBuffer, 999);
    if (n < 0) error("ERROR writing to socket");
}

// prints error message
void error(const char *msg) {

    perror(msg);
    exit(1);

}

// prints error message
void errorOnRead(const char *msg) {

    perror(msg);
    pthread_exit(nullptr);

}

// calculates the closeness between two numbers and return the difference
int calculateCloseness(int mysteryNum, int guessVal) {


    int difference = 0; // holds sum of 'difference' between 2 four-digit ints
    int r, g;           // holds result of modulo operation

    for (int i = 0; i < 4; i++) {

        r = mysteryNum % 10;
        g = guessVal % 10;

        difference += abs(r - g);

        mysteryNum /= 10;
        guessVal /= 10;

    }

    return difference;
}

// called to update leaderboard, contains critical section
void updateLeaderboard(leaderboardContent& player) {

    // if the board is empty, put players as number 1
    if (board.size() == 0) {
        board.push_back(player);
    }

    // else if the board has one item, order the new player accordingly
    else if (board.size() == 1) {

        board.push_back(player);

        if (player.score < board.at(0).score) {
            std::swap(board.at(1), board.at(0));
        }
    }

    // two items in leader board
    else if (board.size() == 2) {

        board.push_back(player);

        if (player.score < board.at(1).score) {
            std::swap(board.at(2), board.at(1));
        }

        if (board.at(1).score < board.at(0).score) {
            std::swap(board.at(1), board.at(0));
        }

    }

    // three items in leader board
    else {

        if (player.score < board.at(2).score) {
            std::swap(player, board.at(2));
        }

        if (board.at(2).score < board.at(1).score) {
            std::swap(board.at(2), board.at(1));
        }

        if (board.at(1).score < board.at(0).score) {
            std::swap(board.at(1), board.at(0));
        }
    }
}

//
//  main.cpp
//  ClientServerGame
//
//  Created by VeeVee Wang on 6/5/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <iostream>
#include <cstdlib>          // to generate random number
#include <cmath>            // to calculate absolute value
#include <stdio.h>          // needed for server code
#include <sys/types.h>      // needed for server code
#include <sys/socket.h>     // needed for server code
#include <netinet/in.h>     // needed for server code

using namespace std;


// calculate how "close" the guess is from the mystery number.
int getDifference(int rand, int guess) {
    
    // guess must be same number of digts are random, or else error  ASK MISHRA about this
    
    int difference = 0; // holds sum of 'difference' between 2 four-digit ints
    int r, g;           // holds result of modulo operation
    
    for (int i = 0; i < 4; i++) {
        
        r = rand % 10;
        g = guess % 10;
        
        difference += abs(r - g);
        
        rand /= 10;
        guess /= 10;
        
    }
    
    //difference += abs(rand - guess);
    
    return difference;
}

int main(int argc, const char * argv[]) {
    
    int sockfd;         // file descriptor
    int newsockfd;      // file descriptor
    int portNo;         // port number on which server accepts connections
    int clilen;         // stores size of address of clinent
    int n;              // contains number of character read or written
    char buffer[256];   // server reads characters from socket connection
    struct sockaddr_in serv_addr, cli_addr;
    
    // if user fails to pass port number, display error message
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided");
        exit(1);
    }
    
    // create a new socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    // if socket create is unsuccessful, display error message
    if (sockfd < 0) {
        fprintf(stderr, "ERROR, no port provided");
    }
    
    // sets all values in buffer to zero
    bzero((char*) &serv_addr, sizeof(serv_addr));
    
    // port number on which the server will listen for connections
    portNo = atoi(argv[1]);
    
    // setting fields of serv_addr
    serv_addr.sin_family = AF_INET;         // code for address family
    serv_addr.sin_port = htons(portNo);     // contains port numbers
    serv_addr.sin_addr.s_addr = INADDR_ANY; // contains IP address of the host
    
    // binds a socket to an address (Slide 41)
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
    }
    
    // allows process to listen on socket for connections
    listen(sockfd, 5);
    
    //
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
    if (newsockfd < 0) {
        error("ERROR on accept");
    }
    
    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
        error("ERROR reading from socket");
        printf("Here is the message: %s", buffer);
    }
    
    n = write(newsockfd, "I got your message", 18);
    if (n < 0) {
        error("ERROR writing to socket");
    }
    
    
//    int guess= -1;
//    int randomNum = rand() % 10000;
//
//    cout << "Random number: " << randomNum << endl;
//
//    while (guess != randomNum) {
//
//        cout << "Guess number:  ";
//        cin >> guess;
//        int diff = getDifference(randomNum, guess);
//        cout << diff << endl;
//    }
//    cout << "You guessed the right number!" << endl;

    return 0;
}

// This function is called when a system call fails
void error (char *msg) {
    perror(msg);
    exit(1);
}








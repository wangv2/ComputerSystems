//
//  server.cpp
//  ClientServerGame
//
//  Created by VeeVee Wang on 6/5/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <stdio.h>


// struct used to pass into thread function
struct ThreadArgs {
    int clientSock;         // socket to communicate with client
};


class server {
    
    while (true) {
        
        // Accept connection from client
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        
        clientSock = accept(sock,(struct sockaddr *)&clientAddr, &addrLen);
        
        if (clientSock < 0) {
            exit(-1);
        }
        
        
        // Create and initialize argument struct
        threadArgs =  new ThreadArgs;
        
        threadArgs -> clientSock = clientSock;
        
        
        
        // Create client thread
        pthread_t threadID;
        
        int status = pthread_create(&threadID, NULL, threadMain,(void *) threadArgs);
        
        if (status != 0) {
            exit(-1);
        } // Note: status == 0 is GOOD
        
    }
};

void processClient(int clientSock) {
    
}


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

//
//  main.cpp
//  threadExample
//
//  Created by VeeVee Wang on 5/1/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <pthread.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int val = 8;

void *inc(void *parmPtr) {
    val++;
    return(NULL);
}

void *dec(void *parmPtr) {
    val--;
    return(NULL);
}



int main(int argc, const char * argv[]) {
    
    for (int i = 0; i < 10000; i++) {
        val = 8;
        
        pthread_t threadID1, threadID2;
        
        /* Create the threads */
        pthread_create(&threadID1, NULL, &inc, NULL);
        pthread_create(&threadID2, NULL, &dec, NULL);
        
        /* Wait for them to complete */
        pthread_join(threadID1, NULL);
        pthread_join(threadID2, NULL);

        if (val != 8) {
            cout << "PROBLEM ";
        }
        cout << "val = " << val << endl;

    }
    
    exit(0);
}

//
//  lazyDentist.cpp
//  LazyDentist Assignemnt: Code the solution for the lazy dentist problem.
//
//  Created by VeeVee Wang on 5/13/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <sstream>

using namespace std;


sem_t dentistReady;         // represents when the dentist is ready

sem_t seatCountWriteAccess; // used to update number of seats in waiting room

sem_t patientReady;         // represents number of patients in waiting room

int numberOfFreeWRSeats;    // represents number of free seats in room

void *Dentist(void*);       // dentist function prototype

void *Customer(void*);      // customer function prototype

void errorMessage();        // error message function prototype


int main(int argc, const char * argv[])
{
    
    // if command line didn't include a 2 inputs, print error message and exit
    if (argc < 3)
    {
        errorMessage();
        
        exit(-1);
    }
    
    
    // else, continue with the program
    else
    {
        
        stringstream convert;       // To convert commmand line input
        int numCustomerThreads;     // holds the number of patients
        
        // parse first command line input
        convert << argv[1];
        convert >> numberOfFreeWRSeats;
        convert.clear();
        
        // parse second command line input
        convert << argv[2];
        convert >> numCustomerThreads;
        
        // initialize semaphores
        sem_init(&dentistReady, 0, 0);
        sem_init(&seatCountWriteAccess, 0, 1);
        sem_init(&patientReady, 0, 0);
        
        // declare threads
        pthread_t dentistThread;
        pthread_t* customerThread = new pthread_t[numCustomerThreads];
        
        // create dentist thread
        pthread_create(&dentistThread, NULL, Dentist, NULL);
        
        // create customer threads
        for (int i = 0; i < numCustomerThreads; i++)
        {
            pthread_create(&customerThread[i], NULL, Customer, &i);
        }
        
        // join dentist thread
        pthread_join(dentistThread, NULL);
        
        // join customer threads
        for (int i = 0; i < numCustomerThreads; i++)
        {
            pthread_join(customerThread[i], NULL);
        }
        
        // deallocate memory
        delete[] customerThread;
    }
    
    return 0;
}
    
// Dentist thread
void* Dentist(void* args)
{
    
    // Run in an infinite loop.
    while (true)
    {
        
        // Try to acquire a patient - if none is available, go to sleep
        printf("Dentist trying to acquire patient...\n");
        sem_wait(&patientReady);
        
        // Awake; Try to access to modify # available seats, otherwise sleep
        printf("Dentist trying to acquire seatCountWriteAccess...\n");
        sem_wait(&seatCountWriteAccess);
        
        // One waiting room chair becomes free
        numberOfFreeWRSeats += 1;
        printf("Incremented free seats to %i\n", numberOfFreeWRSeats);
        
        // I am ready to consult
        printf("Dentist ready to consult...\n");
        sem_post(&dentistReady);
        
        // Don't need the lock on the chairs anymore
        printf("Dentist releasing seatCountWriteAccess...\n");
        sem_post(&seatCountWriteAccess);
        
        printf("Dentist consulting patient...\n");
        // (Talk to patient here.)
    }
    
    return NULL;
}

// Customer Thread
void* Customer(void* args_p)
{
    
    // convert parameter to integer
    int patientNum = *reinterpret_cast<int*>(args_p);
    
    // Run in an infinite loop to simulate multiple patients.
    while (true)
    {
        
        // Try to get access to the waiting room chairs.
        printf("Customer %i trying to acquire seatCountWriteAccess...\n", patientNum);
        sem_wait(&seatCountWriteAccess);
        
        // If there are any free seats:
        if (numberOfFreeWRSeats > 0)
        {
            // sit down in a chair
            numberOfFreeWRSeats -= 1;
            printf("Customer %i seated; Remaining chairs=%i\n", patientNum, numberOfFreeWRSeats);
            
            // notify the dentist, who's waiting until there is a patient
            printf("Customer %i notifying dentist patientReady...\n", patientNum);
            sem_post(&patientReady);
            
            // don't need to lock the chairs anymore
            printf("Customer %i releasing seatCountWriteAccess...\n", patientNum);
            sem_post(&seatCountWriteAccess);
            
            // wait until the dentist is ready
            printf("Customer %i waiting for dentist...\n", patientNum);
            sem_wait(&dentistReady);
            
            // (Consult dentist here.)
            printf("Customer %i consulting dentist...\n", patientNum);
        }
        
        // otherwise, there are no free seats; tough luck!
        else
        {
            printf("Customer %i leaving without consulting; no chairs available...\n", patientNum);
            // don't forget to release lock on seats!
            sem_post(&seatCountWriteAccess);
            
            // Leave without consulting the dentist.
        }
    }
    
    return NULL;
}

// Error Message
void errorMessage()
{
    
    cout << "\nInvalid command line input! Aborting program." << endl << endl;
    cout << "Please enter 2 numbers with the first representing the number\n";
    cout << "chairs and the second representing the number of patients.\n\n";
    cout << "Example: ./a.out 3 5" << endl << endl;
    
}

//
//  vowels.cpp
//
//  This is a multithreaded program that counts the number of times each
//  vowel cumulatively appear is 20 different text files.
//
//  Created by VeeVee Wang on 4/16/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <fstream>

using namespace std;

// global variables
pthread_mutex_t mutexID;                                // global mutex var
const int NUM = 20;                                     // number of files
int aTotal = 0;                                         // total 'a' count
int eTotal = 0;                                         // total 'e' count
int iTotal = 0;                                         // total 'i' count
int oTotal = 0;                                         // total 'o' count
int uTotal = 0;                                         // total 'u' count

// global string array of hard-coded file names
string file[NUM] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt",
                    "file5.txt", "file6.txt", "file7.txt", "file8.txt",
                    "file9.txt", "file10.txt","file11.txt","file12.txt",
                    "file13.txt","file14.txt","file15.txt","file16.txt",
                    "file17.txt","file18.txt","file19.txt","file20.txt"};

// function prototypes
void* countVowels(void *);          // function called when a thread is created
void welcome();                     // welcomes user to program
void farewell();                    // thanks user for using program
void printCommandLineError();       // prints error if command line error
void printThreadErrorMessage();     // prints error if thread error
void printStats();                  // prints statistics


int main(int argc, const char * argv[]) {
    
    // if the command line did not include a filepath, print error message
    if (argc < 2) {
        printCommandLineError();
        exit(-1);
    }
    
    // else, continue on with program
    else {
    
        pthread_t id[NUM];                      // array of p_threads
        int status;                             // status of thread
        pthread_mutex_init(&mutexID, NULL);     // initialize mutex variable
        string directoryName = (string) argv[1];// name of directory with files
        
        // welcome user to program
        welcome();
        
        // update directory to file based on user command line input
        for (int h = 0; h < NUM; h++) {
            file[h] = directoryName + file[h];
        }
        
        // create 20 threads for each file
        for (int i = 0; i < NUM; i++) {
            
            status = pthread_create(&id[i], NULL, countVowels, &file[i]);
            
            // if status is not zero, print error message and exit
            if (status) {
                printThreadErrorMessage();
                exit(-1);
            }
        }
        
        // join all threads before exiting program
        for (int j = 0; j < NUM; j++) {
            status = pthread_join(id[j], NULL);
                
            // if status is not zero, print error message and exit
            if (status) {
                printThreadErrorMessage();
                exit(-1);
            }
        }

        printStats();
        farewell();
    }
    return 0;
}

// This function is called when a thread is created. This function takes a
// file path as an argument and opens the file. The file is parsed, and the
// number of vowels is counted. Onces the number of vowels have been counted,
// the function enters a critical section where the local count of vowels is
// added to the global count of total vowels.
void* countVowels(void *args_p) {
    
    int a = 0;          // local counts of vowel 'a'
    int e = 0;          // local counts of vowel 'e'
    int i = 0;          // local counts of vowel 'i'
    int o = 0;          // local counts of vowel 'o'
    int u = 0;          // local counts of vowel 'u'
    string fileName;    // name of file to open
    char character;     // holds each character from the file
    
    // cast and assign parameter to local variable
    fileName = *reinterpret_cast<string*>(args_p);

    // create fstream object and open file
    fstream file;
    file.open(fileName);
    
    // if the file cannot be opened, print an error message, and exit
    if (!file) {
        cout << endl << "File does not exist! Aborting program." <<endl <<endl;
        exit(1);
    }
    
    // else, the process the contents of the file
    else {
        
        // get each character in the file
        while (file.get(character)) {
            
            // set each character to lower
            char c = tolower(character);
            
            if (c == 'a') {
                a++;
            }
            else if (c == 'e') {
                e++;
            }
            else if (c == 'i') {
                i++;
            }
            else if (c == 'o') {
                o++;
            }
            else if (c == 'u') {
                u++;
            }
        }
    }
    
    file.close();
    
    // enter critical section, aquire lock
    pthread_mutex_lock(&mutexID);
    
    // add vowel counts for the file to global count of vowels
    aTotal += a;
    eTotal += e;
    iTotal += i;
    oTotal += o;
    uTotal += u;
    
    // leaving critical section, remove lock
    pthread_mutex_unlock(&mutexID);
    
    return NULL;
}

// prints welcome message to user
void welcome() {
    cout  << endl << "Welcome to HW1: Count Vowels" << endl << endl;
}

// print goodbye message to user
void farewell() {
    cout << endl << "Thank you for visitng Count Vowels!" << endl << endl;
}

// prints error if there is a command line error
void printCommandLineError() {
    cout << endl << "ERROR: " <<
                    "Invalid command line. Command line input must include " <<
                    "a file path to files.\nExample: ./a.out /Users/vilia/" <<
                    "Desktop/files/\n\nAborting program.\n" << endl;
}

// print error is there is a problem with a thread function
void printThreadErrorMessage() {
    cout << endl << "ERROR: There has been an error with the pthread. " <<
                    "Aborting program." << endl;
}

// prints statistics
void printStats() {
    cout << endl;
    cout << "total A/a: " << aTotal << endl;
    cout << "total E/e: " << eTotal << endl;
    cout << "total I/i: " << iTotal << endl;
    cout << "total O/o: " << oTotal << endl;
    cout << "total U/u: " << uTotal << endl << endl;
}

// AUTHOR: Vilia Wang
// FILENAME: hw2.cpp
// DATE: 6 February 2018
// VERSION: 1

// PURPOSE:
// This is the driver file that tests the DynamicList class. The
// driver tests the class by reading instructions from a file in order to
// create a dynamic list.

// ASSUMPTIONS:
// Assumes that the file only contains one number that represents the list's
// capacity. Assumes that this number appears as the first line in the file.
// Assumes that the file only contains one "EXIT" command and that the
// commands appears at the end of the file. Assumes no other commands will
// appear after the "EXIT" command.


#include <iostream>
#include <fstream>
#include <sstream>
#include "DynamicList.h"

using namespace std;

void readExecuteFile();
// description: Opens & reads file; performs actions as specified by input file
// on the Dynamiclist class
// input: None.
// processing: Reads text file, create and manipulates DynamicList object.
// output: Void.

void welcome();
// description: Welcomes user.
// input: None.
// processing: Prints to screen.
// output: Void.

void farewell();
// description: Thanks users and says goodbye.
// input: None.
// processing: Void.

int main() {
    welcome();
    readExecuteFile();
    farewell();
    return 0;
}

void readExecuteFile() {
    int number;                         // holds to number to add
    string line;                        // holds each line in the file
    string command;                     // holds each line in the file
    string file = "input_trace.txt";    // input file
    
    // open input file
    ifstream infile;
    infile.open(file);
    
    // read the first line for list capacity, and create list object
    infile >> number;
    infile.ignore();
    DynamicList list(number);
    
    // parse through the rest of the input file
    while (getline(infile, line) && line != "EXIT") {
        stringstream ss;
        ss << line;
        ss >> command;
        
        if (command == "ADD") {
            ss << line;
            ss >> number;
            list.add_item(number);
        }
        else if (command == "PRINT") {
            list.print();
            cout << endl;
        }
        else if (command == "DELETE") {

            list.remove_item();
        }
        
    }
    infile.close();
}

void welcome() {
    cout << endl << "Welcome to HW2!" << endl << endl;
    cout << "This program will read instructions for an text file" << endl
    << "to create and populate a DynamicList." << endl << endl;
}

void farewell() {
    cout << endl << "Thank you for using HW2! Goodbye!" << endl << endl;
}

//
//  SJF.cpp
//
//  PURPOSE:
//  SJF.cpp takes a filename from the command line. If possible, the file is
//  opened, it's contents are extracted into Processes, and the shortest job
//  first scheudling is applied to the Processes. If the file is not able to be
//  opened or a filename is not specified, the program will abort.
//
//  ASSUMPTIONS:
//  A filename is provided from the command line.
//  The file is well-formatted (3 number on each line seperated by commas).
//
//  Created by Vilia Wang on 3/14/18.

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

// This struct represents a Process. A Process has fields of jobID, burst,
// and arrival. The struct overloads < and () operators.
struct Process {
    int jobID, burst, arrival;

    Process(int id, int b, int a) {
        jobID = id;
        burst = b;
        arrival = a;
    }
    
    bool operator< (const Process& a) const {
        if (burst > a.burst) {
            return true;
        }
        else if (burst == a.burst) {
            if (jobID > a.jobID) {
                return true;
            }
        }
        return false;
    }
    
    bool operator() (const Process& a) const {
        if (burst > a.burst) {
            return true;
        }
        else if (burst == a.burst) {
            if (jobID > a.jobID) {
                return true;
            }
        }
        return false;
    }
};

// This function opens the given file. If the file is not able to be opened,
// an error message is printed and the function returns false. Else if,the file
// is able to open, the function continues to process the file and then returns
// true. The file is processed by having each line read, and extracts info
// to create Processes. Each process is pushed onto a priority queue.
bool openFileSaveData(int &totalJobs, priority_queue<Process> &q,
                      string &fileName) {
    
    string line;                 // holds each line in file
    string token;                // holds int seperated by comma
    int val;                     // holds string converted to int
    int id = 0;                  // holds id value
    int burst = 0;               // holds burst value
    int arrival = 0;             // holds arrival value
    bool continueProgram = true; // flag to see if file issue occured
    
    fstream file;
    file.open(fileName);
    
    // if the file does not open, print error message and return false
    if (!file) {
        cout << endl << "File does not exist! Aborting program." <<endl <<endl;
        continueProgram = false;
        return continueProgram;
    }
    
    // else, the file does open, continue and return true at the end
    else {
        
        // Continue while there are lines in the file
        while (getline(file, line)) {
            stringstream ss (line);     // get a line
            totalJobs++;                // increment number of jobs
            
            int column = 1;             // track which value is being extracted
            while (getline(ss, token, ',')) {
                val = stoi(token);
                
                // the first value is jobID
                if (column == 1) {
                    id = val;
                }
                // the second value is burst
                else if (column == 2) {
                    burst = val;
                }
                // the third value is arrival
                else {
                    arrival = val;
                }
                column++;
            }
            Process p(id, burst, arrival);  // create a Process
            q.push(p);                      // add the new process to pri queue
        }
    }
    file.close();
    return continueProgram;
}

// This function executes shortest job first scheduling, and then prints
// statistics at the end.
void SJF(priority_queue<Process> &q, int &totalJobs) {
    
    int time = 0;           // tracks time
    int totalTA = 0;        // tracks total turn around time
    int totalWT = 0;        // tracks total wait time
    int turnAround = 0;     // tracks current turn around time
    int waitTime = 0;       // tracks current wait time
    vector<int> order;      // holds the order at which processes execute
    vector<Process> temp;   // temporarily holds processes that haven't arrived
    
    // while the priority queue is not empty
    while (!q.empty()) {
        
        // while top item in queue hasn't arrived, put it in the temp vector
        while (!q.empty() && q.top().arrival > time) {
            temp.push_back(q.top());
            q.pop();
        }
        
        if (!q.empty()) {
            
            // if the top item is queue has arrived
            if (q.top().arrival <= time) {
                time += q.top().burst;                  // increment time
                turnAround = time - q.top().arrival;    // calculate TA
                waitTime = turnAround - q.top().burst;  // calculate WT
                totalTA += turnAround;                  // increment total TA
                totalWT += waitTime;                    // increment total WT
                order.push_back(q.top().jobID);         // add process to order
                q.pop();                                // pop proccess off q
            }
        }
        
        // else, no process has arrived at the current time - increment time
        else {
            time++;
        }
        
        // add all remaining items in temp vector back onto the priority queue
        while (!temp.empty()) {
            q.push(temp.back());
            temp.pop_back();
        }
    }
    
    // print statisics
    cout << endl << "SHORTEST JOB FIRST" << endl << endl;
    cout << "A) Avg. turn around:   " << totalTA/(totalJobs * 1.0) << endl;
    cout << "B) Overall throughput: " << (totalJobs * 1.0)/time << endl;
    cout << "C) Avg. wait time:     " << totalWT/(totalJobs * 1.0) << endl;
    cout << "D) Order of execution: ";
    for (unsigned int i = 0; i < order.size(); i++)
        cout << order[i] << " ";
    cout << endl << endl;
}

int main(int argc, const char * argv[]) {
    
    int totalJobs = 0;          // total number of jobs
    bool continueProgram;       // tracks if file was able to be opened
    priority_queue<Process> q;  // holds processes in a priorty queue
    string file;                // name of file from command line
    
    // if there is an invalid command line - abort the program
    if (argc < 2) {
       cout << endl << "Invalid command line. Aborting program.\n" << endl;
    }
    
    // else, command line was valid - continue with program
    else {
        
        // get the command line input
        file = (string) argv[1];
        
        // open file and save data into the priority queue
        continueProgram = openFileSaveData(totalJobs, q, file);

        // if the file was able to be opened, continue the program
        if (continueProgram) {
            
            // do the SJF scheduling algorithem and print statistics
            SJF(q, totalJobs);
        }
    }
    return 0;
}

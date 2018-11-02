//
//  RoundRobin.cpp
//
//  PURPOSE:
//  RoundRobin.cpp takes a filename from the command line. If possible, the
//  file is opened, it's contents are extracted into Processes, and the RR
//  scheudling is applied to the Processes. If the file is not able to be
//  opened or a filename is not specified, the program will abort.
//
//  ASSUMPTIONS:
//  A filename is provided from the command line.
//  The file is well-formatted (3 number on each line seperated by commas).
//
//  Created by VeeVee Wang on 3/16/18.
//

using namespace std;
#include <iostream>
#include <queue>
#include <sstream>
#include <fstream>
#include <vector>

// This struct represents a Process. A Process has fields of jobID, burst,
// arrival and remaining time.
struct Process {
    int jobID, burst, arrival, remainingTime;
    
    Process(int id, int b, int a) {
        jobID = id;
        burst = b;
        arrival = a;
        remainingTime = burst;
    };
};

// This function opens the given file. If the file is not able to be opened,
// an error message is printed and the function returns false. Else if,the file
// is able to open, the function continues to process the file and then returns
// true. The file is processed by having each line read, and extracts info
// to create Processes. Each process is pushed onto a vector.
bool openFileSaveData(int &totalJobs, vector<Process> &vect, string &fileName){
    
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
        cout << endl << "File does not exist! Aborting proram." << endl <<endl;
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
            vect.push_back(p);              // add the new process to vector
        }
    }
    file.close();
    return continueProgram;
}

// swap is a helper method for the sortData method that swaps two Processes.
void swap(int &minIndex, int &i, vector<Process> &v) {
    Process temp = v[i];
    v[i] = v[minIndex];
    v[minIndex] = temp;
}

// sortData sorts Processes into a queue. This function sorts by arrival time.
// If two Processes arrive at the same time, it sorts by jobID.
void sortData(int &totalJobs, vector<Process> &vect, queue<Process> &arrivalQ){
    
    int min;            // holds minimum Process
    
    for (int i = 0; i < totalJobs; i++) {
        
        min = i;
        for (int j = i + 1; j < totalJobs; j++) {
            
            // if the arrival is less than min arrival, swap
            if (vect[j].arrival < vect[min].arrival) {
                min = j;
            }
            
            // else if, arrials are the same and jobId < min jobID, swap
            else if (vect[j].arrival == vect[min].arrival) {
                if (vect[j].jobID < vect[min].jobID)
                    min = j;
            }
        }
        swap(min, i, vect);
    }

    // push ordered Processes onto a queue
    for (int k = 0; k < totalJobs; k++) {
        arrivalQ.push(vect[k]);
    }
}

// This function executes round robin scheduling, and then prints
// statistics at the end
void RobinRobin(int &totalJobs, queue<Process> &arrivalQ, queue<Process> &q,
                vector<Process> &processOrder) {
    
    int total = totalJobs;  // tracks the number of jobs added to q
    int time = 0;           // tracks time
    int totalTA = 0;        // tracks total turn around time
    int totalWT = 0;        // tracks total wait time
    int turnAround = 0;     // tracks current turn around time
    int waitTime = 0;       // tracks current wait time
    
    // add all processes that have arrived at time 0 to q
    while (!arrivalQ.empty() && arrivalQ.front().arrival <= time) {
        q.push(arrivalQ.front());
        arrivalQ.pop();
        total--;
    }
    
    // Keep executing processes while q is not empty OR not all jobs have been
    // added to q
    while (!q.empty() || total > 0) {
        
        // add processes that have arrived at the current time
        while (!arrivalQ.empty() && arrivalQ.front().arrival <= time) {
            q.push(arrivalQ.front());
            arrivalQ.pop();
            total--;
        }
        
        // if the queue is empty, it means no processes have arrived at the
        // current time (CPU is idle), so increment time
        if (q.empty()) {
            time++;
        }
        
        // else, execute the process at the front of the queue
        else {
            processOrder.push_back(q.front());
            q.front().remainingTime = q.front().remainingTime - 1;
            
            // if the process has no remaining time, calculate statistics and
            // pop it off
            if (q.front().remainingTime == 0) {
                turnAround = (time + 1) - q.front().arrival;
                waitTime = turnAround - q.front().burst;
                totalTA += turnAround;
                totalWT += waitTime;
                q.pop();
            }
            
            // else, if the process has remaining time, pop it off the front,
            // and add it to the end of the queue
            else {
                q.push(q.front());
                q.pop();
            }
            time++;
        }
    }
    
    // print statisics
    cout << endl << "ROUND ROBIN" << endl << endl;
    cout << "A) Avg. turn around:   " << totalTA/(totalJobs * 1.0) << endl;
    cout << "B) Overall throughput: " << (totalJobs * 1.0)/time << endl;
    cout << "C) Avg. wait time:     " << (totalWT* 1.0)/totalJobs << endl;
    cout << "D) Order of execution: ";
    for (unsigned int i = 0; i < processOrder.size(); i++)
        cout << processOrder[i].jobID << " ";
    cout << endl << endl;
}

int main(int argc, const char * argv[]) {
    
    int totalJobs = 0;              // total number of jobs
    bool continueProgram;           // tracks if file was able to be opened
    vector<Process> orderedVector;  // used to order Processes by arrival
    vector<Process> processOrder;   // the order at which Processes execute
    queue<Process> arrivalQ;        // holds Processes in order
    queue<Process> q;               // used to run round robin scheduling
    string file;                    // holds name of file
    
    // if there is an invalid command line - abort the program
    if (argc < 2) {
        cout << endl << "Invalid command line. Aborting program.\n" << endl;
    }
    
    // else, command line was valid - continue with program
    else {
        
        // get the command line input
        file = (string) argv[1];
    
        // open file and save data into the queue
        continueProgram = openFileSaveData(totalJobs, orderedVector, file);
        
        if (continueProgram) {
        
            // sort vector into queue by arrival time
            sortData(totalJobs, orderedVector, arrivalQ);

            // do the RR scheduling algorithem and print statistics
            RobinRobin(totalJobs, arrivalQ, q, processOrder);
        }
    }
    
    return 0;
}

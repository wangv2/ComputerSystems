//
//  SRTF.cpp
//  SRTF
//
//  PURPOSE:
//  SRTF.cpp takes a filename from the command line. If possible, the file is
//  opened, it's contents are extracted into Processes, and the SRTF
//  scheudling is applied to the Processes. If the file is not able to be
//  opened or a filename is not specific, the program will abort.
//
//  ASSUMPTIONS:
//  A filename is provided from the command line.
//  The file is well-formatted (3 number on each line seperated by commas).
//
//  Created by VeeVee Wang on 3/17/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

// This struct represents a Process. A Process has fields of jobID, burst,
// and arrival. The struct overloads < and () operators.
struct Process {
    int jobID, burst, arrival, remainingTime;
    
    Process(int id, int b, int a) {
        jobID = id;
        burst = b;
        arrival = a;
        remainingTime = burst;
    }
    
    Process(int id, int b, int a, int rt) {
        jobID = id;
        burst = b;
        arrival = a;
        remainingTime = rt;
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
// to create Processes. Each process is pushed onto a vector.
bool openFileSaveData(int &totalJobs, vector<Process> &vect,
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
            vect.push_back(p);              // add the new process to vector
        }
    }
    file.close();
    return continueProgram;
}

// swap is a helper method for the softData method that swaps two Processes.
void swap(int &minIndex, int &i, vector<Process> &v) {
    Process temp = v[i];
    v[i] = v[minIndex];
    v[minIndex] = temp;
}

// sortData sorts Processes into a queue. This function sorts by arrival time.
// If two Processes arrive at the same time, it sorts by job ID.
void orderProcesses(vector<Process> &vect, int &totalJobs,
                    queue<Process> &arrivalQ) {
    
    int min;            // holds minimum Process
    
    for (int i = 0; i < totalJobs; i++) {
        
        min = i;
        for (int j = i + 1; j < totalJobs; j++) {
            
            // for the arrival is less than min arrival, swap
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

// This function executes SRTF scheduling, and then prints
// statistics at the end.
void SRTF(queue<Process> &arrivalQ, int &totalJobs,
          priority_queue<Process> &pq, vector<int> &processOrder) {
    
    int total = totalJobs;  // tracks the number of jobs added to q
    int time = 0;           // tracks time
    int totalTA = 0;        // tracks total turn around time
    int totalWT = 0;        // tracks total wait time
    int turnAround = 0;     // tracks current turn around time
    int waitTime = 0;       // tracks current wait time
    
    // add all processes that have arrived at time 0 to q
    while (!arrivalQ.empty() && arrivalQ.front().arrival <= time) {
        pq.push(arrivalQ.front());
        arrivalQ.pop();
        total--;
    }
    
    // Keep executing processes while q is not empty OR not all jobs have been
    // added to q
    while (!pq.empty() || total > 0) {
        
        // add processes that have arrived at the current time
        while (!arrivalQ.empty() && arrivalQ.front().arrival <= time) {
            pq.push(arrivalQ.front());
            arrivalQ.pop();
            total--;
        }
        
        // if the queue is empty, it means no processes have arrived at the
        // current time (CPU is idle), so increment time
        if (pq.empty()) {
            time++;
        }
        
        // else, execute the process at the front of the pri queue
        else {
            
            if (processOrder.size() == 0) {
                processOrder.push_back(pq.top().jobID);
            }
            else {
                if (processOrder.back() != pq.top().jobID)
                    processOrder.push_back(pq.top().jobID);
            }
            
            // create a copy process of the top priority item
            Process p(pq.top().jobID, pq.top().burst,
                      pq.top().arrival, pq.top().remainingTime);
            pq.pop();
            int timeItRuns = 0;     // track time process runs before preempted
            bool Continue = true;   // flag -see if process should be preempted
            
            // while the process is not yet finished, and flag is true
            while (p.remainingTime - timeItRuns != 0 && Continue) {
                
                // if front item has arrived and has shorter remaining time,
                // set flag as false
                if ((!arrivalQ.empty()) &&
                    (arrivalQ.front().arrival < time ||
                    arrivalQ.front().remainingTime >
                    p.remainingTime - timeItRuns))
                    Continue = false;
                
                // if there is an item in priorty queue which has less time.
                // set flag as false
                if (!pq.empty() && pq.top().remainingTime <
                    p.remainingTime - timeItRuns)
                    Continue = false;
                
                // increment time trackers
                timeItRuns++;
                time++;
            }
            
            // if the process is not yet finished, add it back onto pri queue
            // with an updated remaining time
            if (p.remainingTime - timeItRuns != 0) {
                Process finalP(p.jobID, p.burst, p.arrival,
                               p.remainingTime - timeItRuns);
                pq.push(finalP);
            }
            
            // else if, the process is finished, record TA and WT
            else {
                turnAround = time - p.arrival;
                waitTime = turnAround - p.burst;
                totalTA += turnAround;
                totalWT += waitTime;
            }
        }
    }
    
    // print statisics
    cout << endl << "SHORTEST REMAINING TIME FIRST" << endl << endl;
    cout << "A) Avg. turn around:   " << totalTA/(totalJobs * 1.0) << endl;
    cout << "B) Overall throughput: " << (totalJobs * 1.0)/time << endl;
    cout << "C) Avg. wait time:     " << totalWT/(totalJobs * 1.0) << endl;
    cout << "D) Order of execution: ";
    for (unsigned int i = 0; i < processOrder.size(); i++)
        cout << processOrder[i] << " ";
    cout << endl << endl;
}


int main(int argc, const char * argv[]) {

    int totalJobs = 0;              // total number of jobs
    bool continueProgram;           // tracks if file was able to be opened
    priority_queue<Process> pq;     // holds processes in a priorty queue
    vector<Process> orderedVector;  // used to order Processes by arrival
    vector<int> processOrder;       // the order at which Processes execute
    queue<Process> arrivalQ;        // holds Processes in order
    string file;                    // name of file from command line
    
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

        // if the file was able to be opened, continue the program
        if (continueProgram) {
            
            // order the process by arrival time and put into queue
            orderProcesses(orderedVector, totalJobs, arrivalQ);
            
            // do the SJF scheduling algorithem and print statistics
            SRTF(arrivalQ, totalJobs, pq, processOrder);
        }
    }
    
    return 0;
}

//
//  main.cpp
//  ReadFilePractice
//
//  Created by VeeVee Wang on 3/6/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


void openFileSaveData(vector<int> &id, vector<int> &burst, vector<int> &arrival,
                      int &totalJobs) {
    
    string line;            //
    string token;           //
    int val;                //
    
    fstream file;
    file.open("jobs.txt");
    
    if (!file) {
        cout << "File does not exist!" << endl;
    }
    
    else {
        
        while (getline(file, line)) {
            stringstream ss (line);
            totalJobs++;                // increment number of jobs
            
            int column = 1;
            while (getline(ss, token, ',')) {
                val = stoi(token);      // convert string to integer
                
                if (column == 1) {
                    id.push_back(val);
                }
                else if (column == 2) {
                    burst.push_back(val);
                }
                else {
                    arrival.push_back(val);
                }
                column++;
            }
        }
    }
    file.close();
}

void swap(vector<int> &arr, int i, int minIndex) {
    int temp = arr[i];
    arr[i] = arr[minIndex];
    arr[minIndex] = temp;
}

void sortByBurst(vector<int> &id, vector<int> &burst, vector<int> &arrival,
                 int &totalJobs) {
    
    int minIndex;
    
    for (int i = 0; i < totalJobs; i++) {
        
        minIndex = i;
        for (int j = i + 1; j < totalJobs; j++) {
            if (burst[j] < burst[minIndex])
                minIndex = j;
        }
        
        swap(id, i, minIndex);
        swap(burst, i, minIndex);
        swap(arrival, i, minIndex);
    }
}

void SJF(vector<int> &id, vector<int> &burst, vector<int> &arrival,
         int &totalJobs) {
    
    vector<int> order;
    int index;
    int time = 0;
    int totalTA = 0;
    int totalWT = 0;
    
    for (int i = 0; i < totalJobs; i++) {
        index = -1;
        for (int j = 0; j < totalJobs; j++) {
            if (burst[j] != 0) {
                if (arrival[j] <= time) {
                    if (index == -1)
                        index = j;
                    else if (burst[j] < burst[index])
                        index = j;
                    else if (burst[j] == burst[index] && id[j] < id[index])
                        index = j;
                }
            }
        }
        time += burst[index];
        int turnAround = time - arrival[index];
        int waitTime = turnAround - burst[index];
        totalTA += turnAround;
        totalWT += waitTime;
        order.push_back(id[index]);
        burst[index] = 0;
    }
    
    
    cout << endl << "A) Avg. turn around:   " << totalTA/(totalJobs*1.0) << endl;
    cout << "B) Overall throughput: " << (totalJobs*1.0)/time << endl;
    cout << "C) Avg. wait time:     " << totalWT/(totalJobs*1.0) << endl;
    cout << "D) Order of execution: ";
    for (int k = 0; k < totalJobs; k++) {
        cout << order[k] << " ";
    }
    cout << endl << endl;
    
}

int main() {
    int totalJobs = 0;      // total jobs in the given file
    vector<int> jobID;      // vector that holds all jobID data
    vector<int> burst;      // vector that holds bust time data
    vector<int> arrival;    // vector that holds arrival time data

    // read file, save data into vectors
    openFileSaveData(jobID, burst, arrival, totalJobs);
//    // printing contents of the vectors
//    for (int i = 0; i < totalJobs; i++) {
//        cout << jobID[i] << " " << burst[i] << " " << arrival[i] << endl;
//    }
//    cout << endl << endl;

    // sorts vectors by burst
    sortByBurst(jobID, burst, arrival, totalJobs);
//    // printing contents AFTER ORDERING BY BURST
//    for (int i = 0; i < totalJobs; i++) {
//        cout << jobID[i] << " " << burst[i] << " " << arrival[i] << endl;
//    }
    
    SJF(jobID, burst, arrival, totalJobs);
    
    //vectors are currently sorted by burst time - TODO next, run the data
    // though "CPU" simulator to get order of processing, wait time, etc.

    return 0;
}


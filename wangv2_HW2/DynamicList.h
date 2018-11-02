// AUTHOR: Vilia Wang
// FILENAME: DynamicList.h
// DATE: 6 February 2018
// VERSION: 1

// PURPOSE: This header file outlines the public and private methods and
// fields in the DynamicList of integers class.

// ASSUMPTIONS: The void print() method has been added to allow printing of
// statistics. The destructor has been added to properly deallocated memory
// and to prevent memory leaks.

#ifndef DynamicList_h
#define DynamicList_h

#include <stdio.h>

class DynamicList {
    
public:
    DynamicList(int);   // constructor that accepts capacity of list as input
    ~DynamicList();     // destructor for deallocating memory
    void add_item(int); // adds the specified item to the end of the list
    void remove_item(); // removes the last item on the list
    void expand_list(); // the list is doubled in size if it becomes > 50% full
    void shrink_list(); // the list is halved in size if it becomes < 25% full
    void print();       // prints size, capacity and contents of list
    
private:
    int list_capacity;  // max number of integers the list can currectly store
    int size;           // the number of item currently stored in the list
    int* list;          // used for dynamically allocating an array of integers
    
};

#endif /* DynamicList_h */

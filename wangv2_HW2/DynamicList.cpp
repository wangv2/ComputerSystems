// AUTHOR: Vilia Wang
// FILENAME: DynamicList.cpp
// DATE: 6 February 2018
// VERSION: 1
//
// PURPOSE: This class implements the methods and initializes fields that are
// outlined in the header class.

// ASSUMPTIONS:
// The input for the constructor and add_item will be postive integers.
// If a user tries to remove an item from and empty list, an error message will
// be printed.

#include "DynamicList.h"
#include <iostream>

using namespace std;

DynamicList::DynamicList(int capacity) {
    this->list_capacity = capacity;
    this->size = 0;
    this->list = new int[list_capacity];
}

DynamicList::~DynamicList() {
    delete[] list;
}

void DynamicList::add_item(int item) {
    // add item to list and increment size
    list[size++] = item;
    
    // check list's capacity and expand if capacity > 50%
    if ((double)size / list_capacity > .5)
        expand_list();
}

void DynamicList::remove_item() {
    // if list is empty, print error message
    if (size == 0)
        cout << "ERROR: Cannot remove from an empty list." << endl;
    
    else {
        // else, remove last item stored in list and decrement size
        list[--size] = 0;
        
        // check list's capcity, and shrink if capacity < 25%
        if ((double)size / list_capacity < .25)
            shrink_list();
    }
}

void DynamicList::expand_list() {
    // allocate new dynamic memory for an array twice the current size
    int* temp = new int[list_capacity * 2];
    
    // copy contents of current array into new array
    for (int i = 0; i < size; i++)
        temp[i] = list[i];
    
    // delete smaller array
    delete[] list;
    
    // update list capacity
    this->list_capacity = list_capacity * 2;
    
    // make int* list point to the new array
    list = temp;
    temp = nullptr;
}

void DynamicList::shrink_list() {
    // allocate new dynamic memory for an array half the current size
    int* temp = new int[list_capacity / 2];
    
    // copy contents of current array into new array
    for (int i = 0; i < size; i++)
        temp[i] = list[i];
    
    // delete smaller array
    delete[] list;
    
    // update list capacity
    this->list_capacity = list_capacity / 2;
    
    // make int* list pinter to the new array
    list = temp;
    temp = nullptr;
}

void DynamicList::print() {
    cout << "List max capacity = " << list_capacity << endl;
    
    cout << "List current size = " << size << endl;
    
    cout << "List contents: ";
    for (int i = 0; i < size; i++) {
        cout << list[i];
        
        // ensures an extra comma is not printed
        if (i != size - 1)
            cout << ", ";
    }
    cout << endl;
}

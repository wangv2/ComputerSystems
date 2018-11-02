//
//  main.cpp
//  PlayingWithPointers
//
//  Created by VeeVee Wang on 1/23/18.
//  Copyright © 2018 VeeVee Wang. All rights reserved.
//
//  Messing around with pointers and referenes

#include <iostream>

using namespace std;

void foo(int*);
int sum(int, int[]);

int value = 100;    // global variable

namespace first {
    int value = 1;  // namespace first variable
}

int main() {
    int value = 10; // local variable
    
    cout << "first namespace -> first::value = " << first::value << endl;
    cout << "local           -> value        = " << value << endl;
    cout << "global          -> ::value      = "  << ::value << endl << endl;
    
    // pointer practice
    int x = 8;
    int* y = &x;
    foo(y);
    cout << "x = " << x << ", *y = " << *y << endl;
    
    // passing array practice
    int size = 5;
    int arr[size];
    
    for (int i = 0; i < size; i++)
        arr[i] = i;
    cout << "Summ of arr = " << sum(size, arr) << endl;
    
    return 0;
}

int sum(int size, int arr[]) {
    int sum = 0;
    for (int i = 0; i < size; i++)
        sum += arr[i];
    return sum;
}

void foo(int* p) {
    int q = 19;
    *p = 15;
    p = &q;
}

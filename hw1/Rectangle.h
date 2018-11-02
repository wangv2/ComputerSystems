// AUTHOR: Vilia Wang 
// FILENAME: Rectangle.h
// DATE: 23 January 2018
// VERSION: 1

// PURPOSE: This header file outlines the public and private methods and
// fields in the Rectangle class.

using namespace std;

class Rectangle {
 
public:
    Rectangle();                 // no-arg constructor
    Rectangle(int, int);         // ctor that takes length and width parameters
    void setLength(int);         // setter for length
    void setWidth(int);          // setter for width
    int perimeter();             // getter for perimeter
    int area();                  // getter for area
    void show();                 // prints length and width
    int sameArea(Rectangle);     // returns true if 2 rectangles have same area
 
private:
    int length, width;           // holds a Rectangle's length and width
}; 

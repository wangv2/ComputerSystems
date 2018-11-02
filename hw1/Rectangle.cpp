// AUTHOR: Vilia Wang
// FILENAME: Rectangle.cpp
// DATE: 23 January 2018
// VERSION: 1

// PURPOSE: This file implements the methods for the Rectangle class. 

#include "Rectangle.h"
#include <iostream> 

Rectangle::Rectangle() {
   this->length = 0; 
   this->width = 0; 
}

Rectangle::Rectangle(int l, int w) {
   this->length = l; 
   this->width = w;
}

void Rectangle::setLength(int l) {
   this->length = l; 
}

void Rectangle::setWidth(int w) {
   this->width = w; 
}

int Rectangle::perimeter() {
   return (length * 2) + (width * 2);
}

int Rectangle::area() {
   return length * width; 
}

void Rectangle::show() {
   cout << "Length = " << length << ", Width = " << width;
}

int Rectangle::sameArea(Rectangle r) {
   if ((r.length*r.width) == (this->length*this->width))
      return 1; 
   else 
      return 0; 
}
   

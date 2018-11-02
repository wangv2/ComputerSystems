// AUTHOR: Vilia Wang 
// FILENAME: hw1.cpp
// DATE: 23 January 2018
// VERSION: 1

// PURPOSE: This is the driver file that tests the Rectangle class. The driver
// tests the class by creating an array of Rectangles, and then prints
// information about each Rectangle.

// ASSUMPTIONS: Assumes input for Rectangle lengths and widths will only be
// positive integers. When printing information for the smallest area and
// perimeter and the largest area and perimeter, if there are duplicates, the
// first instance of the smallest/largest will be printed.

#include "Rectangle.h"
#include <iostream> 
#include <fstream> 

using namespace std; 

void welcome();
// description: Welcome's user to the program. 
// input: None.
// processing: Prints to screen. 
// output: Void.

void getInsertData(int, Rectangle*, int*, int*);
// description: Accomplishes objectives 2, 3, 4. For 2, it gets user input for
// 5 Rectangles. For 3, it creates a file and stores Rectangle's length and
// width. For 4, it initalizes length and width of the 5 rectangles.  
// input: int as array size, Rectanlge pointer to Rectangle array, int* of 
// length array, and int* of width array. 
// processing: Populates Rectangle and int arrays with user input. 
// output: Void. 

void printMinAndMax(int, Rectangle*, int*, int*);
// description: Accomplishes objectives 6, 7, 8, 9 by printing the rectangles
// with the smallest area, largest area, smallest perimeter, and largest 
// perimeter. 
// input: int as array size, Rectanlge pointer to Rectangle array, int* of 
// length array, and int* of width array. 
// processing: Uses Rectangles and int arrays to find smallest/largest of
// area/perimeter and prints.  
// output: Void.

void printDuplicates(int, Rectangle*, int*, int*); 
// description: Accomplishes objectives 10, and 11 by findind and printing
// rectangles with duplicate areas and perimeters, when applipliable. 
// input: int as array size, Rectanlge pointer to Rectangle array, int* of 
// length array, and int* of width array. 
// processing: Uses input to find Rectangles with smallest and largest areas.
// output: Void. 

void print(Rectangle*, int, int*, int*); 
// description: Prints statistics of Rectangles. 
// input: Rectangle* of Rectangle array, int as size of array, int* of length
// array and int* of width array. 
// processing: Uses input to print length, width, area, and perimeter of a
// given rectangle.   
// output: Void. 

void goodbye(); 
// description: Bids farewell to the user. 
// input: None. 
// processing: Prints to screen. 
// output: Void. 

int main() {
    int size = 5;                          // size of arrays
    Rectangle* ptr = new Rectangle[size];  // #1 - array of 5 Rectangles
    int* length = new int[size];           // array of lengths
    int* width = new int[size];            // array of widths
   
    welcome();
   
    // #2, 3, 4 - get user input, create and write to file, initialize arrays
    cout << "---------------------------------------------------" << endl;
    cout << "Please enter lengths and widths for five Rectangles" << endl;
    cout << "---------------------------------------------------" << endl;
    getInsertData(size, ptr, length, width);
   
    // #5 - print all rectangles
    cout << endl << "-------------------------------------" << endl;
    cout << "Lengths, widths, areas and perimeters" << endl;
    cout << "-------------------------------------" << endl;
    for (int i = 0; i < size; i++)
        print(ptr, i, length, width);
   
    // #6, 7, 8, 9 - print small and large areas and perimeters
    printMinAndMax(size, ptr, length, width);
   
    // #10, 11 - print duplicates
    printDuplicates(size, ptr, length, width);
    
    goodbye();

    delete [] ptr;
    delete [] length;
    delete [] width;
    return 0;
}

void printDuplicates(int size, Rectangle* ptr, int* l, int* w) {
    bool areaAlreadyExists = false;        // duplicate area exists
    bool perimeterAlreadyExists = false;   // duplicate perimeter exists
    int dupAreaIndex = 0;                  // index of duplicated area
    int dupPerimeterIndex = 0;             // index of duplicated perimeter
    int dupArea[5];                        // array of duplicated areas
    int dupPerimeter[5];                   // array of duplicated perimeter
   
    // initailize dupArea and dupPerimeter arrays to -1
    for (int m = 0; m < size; m++)
        dupArea[m] = -1;
    for (int n = 0; n < size; n++)
        dupPerimeter[n] = -1;

    // check for duplicate areas and perimeters
    for (int i = 0; i < size; i++) {
        for (int j = (i + 1); j < size; j++) {
            areaAlreadyExists = false;
            perimeterAlreadyExists = false;
         
        // check for duplicate areas
        if (ptr[i].sameArea(ptr[j])) {
            for (int k = 0; k < size; k++) {
                if (dupArea[k] == ptr[i].area())
                    areaAlreadyExists = true;
            }
            if (!areaAlreadyExists) {
                dupArea[dupAreaIndex] = ptr[i].area();
                dupAreaIndex++;
            }
         } 

        // check for duplicate perimeters
        if (ptr[i].perimeter() == ptr[j].perimeter()) {
            for (int x = 0; x < size; x++) {
                if (dupPerimeter[x] == ptr[i].perimeter())
                    perimeterAlreadyExists = true;
            }
            if (!perimeterAlreadyExists) {
                dupPerimeter[dupPerimeterIndex] = ptr[i].perimeter();
                dupPerimeterIndex++;
            }
         }
      }
   }

    // printing rectangles with duplicates areas
    cout << endl << endl << "---------------" << endl;
    cout << "Duplicate areas" << endl;
    cout << "---------------" << endl;
    
    int noAreaDups = 0;
    for (int l = 0; l < size; l++) {
        if (dupArea[l] != -1) {
          cout << "Rectangles ";
          for (int q = 0; q < size; q++) {
             if (ptr[q].area() == dupArea[l])
                cout << "r" << q << ", ";
          }
          cout << "have equal areas of " << dupArea[l]
               << " units each." << endl;
        }
        else
            noAreaDups++;
    }
    if (noAreaDups == size)
        cout << "No rectangles with equal areas found" << endl;
   
   
    // printing rectangles with duplicate perimeters
    cout << endl << endl << "--------------------" << endl;
    cout << "Duplicate perimeters" << endl;
    cout << "--------------------" << endl;
    int noPerimeterDups = 0;
    for (int y = 0; y < size; y++) {
        if (dupPerimeter[y] != -1) {
            cout << "Rectangles ";
            for (int r = 0; r < size; r++) {
                if (ptr[r].perimeter() == dupPerimeter[y])
                    cout << "r" << r << ", ";
            }
            cout << "have equal perimeters of " << dupPerimeter[y]
                 << " units each." << endl;
        }
        else
            noPerimeterDups++;
    }
    if (noPerimeterDups == size)
        cout << "No rectangles with equal perimeters found" << endl;
}

void printMinAndMax(int size, Rectangle* ptr, int* l, int* w) {
   int minArea = 0;        // holds index of minimum area
   int maxArea = 0;        // holds index of maximum area
   int minPerimeter = 0;   // holds index of minimum perimeter
   int maxPerimeter = 0;   // holds index of maximum perimeter

   // search for min and max areas and perimters 
   for (int i = 1; i < size; i++) {
      if (ptr[i].area() < ptr[minArea].area())  
         minArea = i;
      if (ptr[i].area() > ptr[maxArea].area())
         maxArea = i; 
      if (ptr[i].perimeter() < ptr[minPerimeter].perimeter())
         minPerimeter = i; 
      if (ptr[i].perimeter() > ptr[maxPerimeter].perimeter())
         maxPerimeter = i;
   }
   
   // prints findings
   cout << endl << endl << "----------------------------" << endl;
   cout << "Rectangle with smallest area" << endl;
   cout << "----------------------------" << endl;
   print(ptr, minArea, l, w);
    
   cout << endl << endl << "---------------------------" << endl;
   cout << "Rectangle with largest area" << endl;
   cout << "---------------------------" << endl;
   print(ptr, maxArea, l, w);
    
   cout << endl << endl << "-------------------------------------" << endl;
   cout << "Rectangle with the smallest perimeter" << endl;
   cout << "-------------------------------------" << endl;
   print(ptr, minPerimeter, l, w);
    
   cout << endl << endl << "------------------------------------" << endl;
   cout << "Rectangle with the largest perimeter" << endl;
   cout << "------------------------------------" << endl;
   print(ptr, maxPerimeter, l, w); 
}

void print(Rectangle* ptr, int index, int* l, int* w) {
    cout << "Rectangle " << index << ": ";
    ptr[index].show();
    cout  << ", Area = " << ptr[index].area()
          << ", Perimeter = " << ptr[index].perimeter() << endl;
}

void getInsertData(int size, Rectangle* ptr, int* l, int* w) {
   int length, width;   // holds user's input for length and width

   ofstream outputFile; 
   outputFile.open("lengthwidth.txt");

   for (int i = 0; i < size; i++)  {
      cout << "Enter length for rectangle " << i << "   ";
      cin >> length; 
      outputFile << length << ", ";
      l[i] = length; 
      
      cout << "Enter  width for rectangle " << i << "   ";
      cin >> width; 
      outputFile << width << endl;
      w[i] = width; 

      // Create Rectangle object and insert into Rectangle array 
      ptr[i] = Rectangle(length, width);  
   }
   
   cout << endl; 
   outputFile.close();
}

void welcome () {
   cout << endl << "Welcome to homework 1!" << endl << endl;    
}  

void goodbye() {
   cout << endl << "Goodbye!" << endl << endl; 
}

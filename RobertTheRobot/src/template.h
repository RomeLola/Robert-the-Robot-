//This is a template for a header file.
//The header file is used to declare variables and functions. It is included in the source file using #include "template.h"
//at the top of the source file. The source file is the one ending in .c or .cpp and contains your variable definitions 
//(different than declerations) function definitions and all the other code. You can include the header file in multiple 
//different source files so that you have access to global variables and functions. 
//Use extern before a global variable and place them in the header. Then d
//Example: extern int globalVariable;
#ifndef TEMPLATE_H
#define TEMPLATE_H

//include any applicable header files here. You probably want the two I added below
#include <Arduino.h>
#include "debug.h"

//variable declarations
extern int globalVariable;

//function declarations
void exampleFunction();

#endif
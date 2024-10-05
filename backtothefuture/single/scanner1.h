#ifndef SCANNER_H 
#define SCANNER_H

//--------------------------------------------------------------
// unit Scanner1

//--------------------------------------------------------------
// interface

//--------------------------------------------------------------
// Recognize an Addition Operator

int IsAddop(char);

//--------------------------------------------------------------
// Recognize a Multiplication Operator

int IsMulop(char);

//--------------------------------------------------------------
// Match One Character

void Match(char *);

//--------------------------------------------------------------
// Get an Identifier

char GetName();

//--------------------------------------------------------------
// Get a Number

char GetNumber();

#endif

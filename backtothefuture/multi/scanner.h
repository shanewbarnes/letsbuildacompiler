#ifndef SCANNER_H 
#define SCANNER_H

#define MAX_EXPR 8

//--------------------------------------------------------------
// unit Scanner

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

char *GetName();

//--------------------------------------------------------------
// Get a Number

char *GetNumber();

#endif

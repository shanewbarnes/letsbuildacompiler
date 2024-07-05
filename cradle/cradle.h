#ifndef CRADLE_H
#define CRADLE_H

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;

//--------------------------------------------------------------
// Read New Character From Input Stream

void GetChar();

//--------------------------------------------------------------
// Report an Error

void Error(char *s);

//--------------------------------------------------------------
// Report Error and Halt

void Abort(char *s);

//--------------------------------------------------------------
// Report What Was Expected

void Expected(char *s);

//--------------------------------------------------------------
// Match a Specific Input Character

void Match(char *x); 

//--------------------------------------------------------------
// Get an Identifier

char GetName();

//--------------------------------------------------------------
// Get a Number

char GetNum();

//--------------------------------------------------------------
// Output a String with Tab

void Emit(char *s);

//--------------------------------------------------------------
// Output a String with TAB and CRLF

void EmitLn(char *s);

//--------------------------------------------------------------
// Initialize

void Init();

#endif

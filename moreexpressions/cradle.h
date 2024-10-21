#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;
extern char buf[MAX_BUF];

//--------------------------------------------------------------
// Read New Character From Input Stream

void GetChar();

//--------------------------------------------------------------
// Report an Error

void Error(char *);

//--------------------------------------------------------------
// Report Error and Halt

void Abort(char *);

//--------------------------------------------------------------
// Report What Was Expected

void Expected(char *);

//--------------------------------------------------------------
// Match a Specific Input Character

void Match(char *); 

//--------------------------------------------------------------
// Get an Identifier

char GetName();

//--------------------------------------------------------------
// Get a Number

char GetNum();

//--------------------------------------------------------------
// Output a String with Tab

void Emit(char *);

//--------------------------------------------------------------
// Output a String with TAB and CRLF

void EmitLn(char *);

//--------------------------------------------------------------
// Initialize

void Init();

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add();

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract();

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply();

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide();

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor();

//--------------------------------------------------------------
// Parse and Translate an Idenifier

void Ident();

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char);

#endif

#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100
#define LETTERS 26

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
extern int Table[LETTERS];

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
// Recognize and Skip Over a Newline

void NewLine(); 

//--------------------------------------------------------------
// Get an Identifier

char GetName();

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
// Recognize an Addop

int Addop(char);

//--------------------------------------------------------------
// Get a Number

int GetNum();

//--------------------------------------------------------------
// Parse and Translate an Expression

int Expression();

//--------------------------------------------------------------
// Parse and Translate a Math Term

int Term();

//--------------------------------------------------------------
// Parse and Translate a Math Factor

int Factor();

//--------------------------------------------------------------
// Initialize the Variable Area

void InitTable();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

//--------------------------------------------------------------
// Input Routine

void Input();

//--------------------------------------------------------------
// Output Routine

void Output();

#endif

#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;

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
// Recognize an Addop

int Addop(char);

//--------------------------------------------------------------
// Parse and Translate A Program

void Prog();

//--------------------------------------------------------------
// Write the Prolog

void Prolog();

//--------------------------------------------------------------
// Write the Epilog

void Epilog(char);

//--------------------------------------------------------------
// Parse and Translate a Pascal Block

void DoBlock(char);

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char);

//--------------------------------------------------------------
// Parse and Translate the Declaration Part

void Declarations();

void Statements();

//--------------------------------------------------------------
// Process Label Statement

void Labels();

//--------------------------------------------------------------
// Process Const Statement

void Constants();

//--------------------------------------------------------------
// Process Type Statement

void Types();

//--------------------------------------------------------------
// Process Var Statement

void Variables();

//--------------------------------------------------------------
// Process Procedure Statement

void DoProcedure();

//--------------------------------------------------------------
// Process Function Statement

void DoFunction();

#endif


#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100
#define LCOUNT_SIZE 2

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;
extern char buf[MAX_BUF];
extern int LCount;

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
// Recognize and Translate an "Other"

void Other();

//--------------------------------------------------------------
// Parse and Translate a Program

void DoProgram();

//--------------------------------------------------------------
// Recognize and Translate a Statement Block

void Block();

//--------------------------------------------------------------
// Generate a Unique Label

char *NewLabel();

//--------------------------------------------------------------
// Post a Label to Output

void PostLabel();

//--------------------------------------------------------------
// Recognize and Translate an IF Construct

void DoIf();

//--------------------------------------------------------------
// Recognize and Translate an WHILE Construct

void DoWhile();

//--------------------------------------------------------------
// Recognize and Translate an LOOP Construct

void DoLoop();

//--------------------------------------------------------------
// Recognize and Translate an REPEAT Construct

void DoRepeat();

//--------------------------------------------------------------
// Recognize and Translate an FOR Construct

void DoFor();

//--------------------------------------------------------------
// Recognize and Translate an DO Construct

void DoDo();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Parse and Translate a Boolean Condition

void Condition();

#endif

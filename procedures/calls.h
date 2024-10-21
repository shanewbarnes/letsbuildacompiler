#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100
#define ALPHA_SIZE 26

//--------------------------------------------------------------
// program Calls

//--------------------------------------------------------------
// Constant Definitions

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Variable Definitions

extern char Look;		    // Lookahead Character
extern char buf[MAX_BUF];
extern char ST[ALPHA_SIZE];

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
// Report an Undefined Identifer

void Undefined(char n);

//--------------------------------------------------------------
// Report an Duplicate Identifer

void Duplicate(char n);

//--------------------------------------------------------------
// Get Type of Symbol

char TypeOf(char n);

//--------------------------------------------------------------
// Look for Symbol in Table

int InTable(char n);

//--------------------------------------------------------------
// Add a New Symbol to Table

void AddEntry(char Name, char T);

//--------------------------------------------------------------
// Check an Entry to Make Sure It's a Variable

void CheckVar(char Name);
		
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
// Post a Label To Output

void PostLabel(char L);

//--------------------------------------------------------------
// Initialize

void Init();

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char c);

//--------------------------------------------------------------
// Recognize an Mulop

int IsMulop(char c);

//--------------------------------------------------------------
// Recognize a Boolean Orop

int IsOrop(char c);

//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char c);

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c);

//--------------------------------------------------------------
// Skip Over Leading White Space

void SkipWhite();

//--------------------------------------------------------------
// Skip Over an End-of-Line

void Fin();

//--------------------------------------------------------------
// Load a Variable to the Primary Register

void LoadVar(char Name);

//--------------------------------------------------------------
// Store the Primary Register

void StoreVar(char Name);

//--------------------------------------------------------------
// Parse and Translate an Expression
// Vestigial Version

void Expression();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment(char);

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void AssignOrProc();

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void DoBlock();

//--------------------------------------------------------------
// Parse and Translate a Begin-Block

void BeginBlock();

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(char N);

//--------------------------------------------------------------
// Parse and Translate a Data Declaration

void Decl();

//--------------------------------------------------------------
// Parse and Translate Global Declarations

void TopDecls();

//--------------------------------------------------------------
// Parse and Translate Procedure Declaration

void DoProc();

//--------------------------------------------------------------
// Emits an RTS Instruction

void Return();

//--------------------------------------------------------------
// Process a Procedure Call

void CallProc(char);

//--------------------------------------------------------------
// Write the Prolog

void Prolog();

//--------------------------------------------------------------
// Write the Epilog

void Epilog();

//--------------------------------------------------------------
// Process the Formal Parameter List of a Procedure

void FormalList();

//--------------------------------------------------------------
// Process a Formal Parameter

void FormalParam();

//--------------------------------------------------------------
// Process an Actual Paramter

void Param();

//--------------------------------------------------------------
// Process the Parameter List for a Procudure Call

int ParamList();

//--------------------------------------------------------------
// Initialize Parameter Table to Null

void ClearParams();

//--------------------------------------------------------------
// Find the Parameter Number

int ParamNumber(char);

//--------------------------------------------------------------
// See if an Identifier is a Parameter

int IsParam(char);

//--------------------------------------------------------------
// Add a New Parameter to Table

void AddParam(char);

//--------------------------------------------------------------
// Load a Parameter to the Primary Register

void LoadParam(int);

//--------------------------------------------------------------
// Store a Parameter from the Primary Register

void StoreParam(int);

//--------------------------------------------------------------
// Push The Primary Register to the Stack

void Push();

//--------------------------------------------------------------
// Adjust the Stack Pointer Upwards by N Bytes

void CleanStack(int N);

//--------------------------------------------------------------
// Write the Prolog for a Procedure

void ProcProlog(char);

//--------------------------------------------------------------
// Write the Epilog for a Procedure

void ProcEpilog();

#endif


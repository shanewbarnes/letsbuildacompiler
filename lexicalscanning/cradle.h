#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <string.h>

#define MAX_BUF 100
#define NAME_SIZE 8
#define NUM_SIZE 16

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;
extern const char KWcode[5];

//--------------------------------------------------------------
// Type Declarations

typedef char Symbol[8];
typedef Symbol SymTab[4];
typedef const SymTab *TabPtr;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;
extern char buf[MAX_BUF];
extern char Token;
extern char ValueArray[16];	// String Token of Look
extern char *Value;

//--------------------------------------------------------------
// Definition of Keywords and Token Types

extern const Symbol KWList[4];

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

void GetName();

//--------------------------------------------------------------
// Get a Number

void GetNum();

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char);

//--------------------------------------------------------------
// Skip Over Leading White Space

void SkipWhite();

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
// Get an Operator

void GetOp();

//--------------------------------------------------------------
// Lexical Scanner

char *Scan();

//--------------------------------------------------------------
// Skip a CR

void Fin();

//--------------------------------------------------------------
// Recognize Any Operator

int IsOp(char);

//--------------------------------------------------------------
// Skip Over a Comma

void SkipComma();

//--------------------------------------------------------------
// Table Lookup

// If the input string matches a table entry, return the entry index. If not, return a zero.

int Lookup(TabPtr, char *, int);

#endif

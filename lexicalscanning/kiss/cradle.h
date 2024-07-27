#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"

#define MAX_BUF 100
#define NAME_SIZE 16
#define LCOUNT_SIZE 2

//--------------------------------------------------------------
// program KISS

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Type Definitions

typedef char Symbol[8];
typedef Symbol SymTab[4];
typedef const SymTab *TabPtr;

//--------------------------------------------------------------
// Variable Definitions

extern char Look;		// Lookahead Character
extern int LCount;		// Label Counter
extern char buf[MAX_BUF];

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
// Match a Specific Input Character

int Lookup(TabPtr, char *, int);

//--------------------------------------------------------------
// Get an Identifier

void GetName();

//--------------------------------------------------------------
// Get a Number

void GetNum();

//--------------------------------------------------------------
// Get an Identifier and Scane it for Keywords

void Scan();

//--------------------------------------------------------------
// Generate a Unique Label

void MatchString(char *);

//--------------------------------------------------------------
// Generate a Unique Label

char *NewLabel();

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char *L); 

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c); 

//--------------------------------------------------------------
// Skip Over Leading White Space 

void SkipWhite();

//--------------------------------------------------------------
// Output a String with Tab

void Emit(char *s);

//--------------------------------------------------------------
// Output a String with TAB and CRLF

void EmitLn(char *s);

//--------------------------------------------------------------
// Parse and Translate an Identifier

void Ident();

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor();

//--------------------------------------------------------------
// Parse and Translate the First Math Factor

void SignedFactor();

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply();

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide();

//--------------------------------------------------------------
// Completion of Term Processing (called by Term and FirstTerm)

void Term1();

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term();

//--------------------------------------------------------------
// Parse and Translate a Math Term with Possible Leading Sign

void FirstTerm();

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add();

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Parse and Translate a Boolean Condition

void Condition();

//--------------------------------------------------------------
// Recognize and Translate an IF Construct

void DoIf();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

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
// Skip a CR

void Fin();

//--------------------------------------------------------------
// Recognize and Translate a Statement Block

void Block();

//--------------------------------------------------------------
// Parse and Translate a Program

void DoProgram();

#endif

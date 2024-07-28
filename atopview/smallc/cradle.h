#ifndef CRADLE_H
#define CRADLE_H

#include <stdio.h>

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
// Parse and Translate a Program

void Prog();

//--------------------------------------------------------------
// Get a Storage Class Specifier

void GetClass();

//--------------------------------------------------------------
// Get a Type Specifier

void GetType();

//--------------------------------------------------------------
// Process a Top-Level Declaration

void TopDecl();

//--------------------------------------------------------------
// Process a Function Definition

void DoFunc(char);

//--------------------------------------------------------------
// Process a Data Declaration

void DoData(char);

//--------------------------------------------------------------
// Process a Int Declaration

void IntDecl();

//--------------------------------------------------------------
// Process a Char Declaration

void CharDecl();

//--------------------------------------------------------------
// Process a Preprocessor Declaration

void PreProc();

#endif

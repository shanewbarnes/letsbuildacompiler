#ifndef CODEGEN_H 
#define CODEGEN_H

#include "errors.h"

//--------------------------------------------------------------
// unit CodeGen

//--------------------------------------------------------------
// interface

extern char buf[MAX_BUF];

//--------------------------------------------------------------
// Load the Primary Register with a Constant

void LoadConstant(char *);

//--------------------------------------------------------------
// Load a Variable to the Primary Register

void LoadVariable(char *);

//--------------------------------------------------------------
// Negate Primary

void Negate();

//--------------------------------------------------------------
// Push Primary to Stack

void Push();

//--------------------------------------------------------------
// Add TOS to Primary

void PopAdd();

//--------------------------------------------------------------
// Subtract TOS from Primary

void PopSub();

//--------------------------------------------------------------
// Multiply TOS by Primary

void PopMul();

//--------------------------------------------------------------
// Divide TOS by Primary

void PopDiv();

//--------------------------------------------------------------
// Store the Primary Register to a Variable

void StoreVariable(char *Name);

//--------------------------------------------------------------
// Or TOS with Primary

void PopOr();

//--------------------------------------------------------------
// Exclusive-Or TOS with Primary

void PopXor();

//--------------------------------------------------------------
// And TOS with Primary

void PopAnd();

//--------------------------------------------------------------
// Bitwise Not Primary

void NotIt();

#endif

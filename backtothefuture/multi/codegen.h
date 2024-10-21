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

#endif

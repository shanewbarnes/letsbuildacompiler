#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "output.h"
#include "errors.h"
#include "codegen.h"

//--------------------------------------------------------------
// unit CodeGen

char buf[MAX_BUF];

//--------------------------------------------------------------
// Load the Primary Register with a Constant

void LoadConstant(char *n)
{
	snprintf(buf, MAX_BUF, "MOVE #%s,D0", n);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Load a Variable to the Primary Register

void LoadVariable(char *Name)
{
	snprintf(buf, MAX_BUF, "MOVE %s(PC),D0", Name);
	EmitLn(buf);
}	

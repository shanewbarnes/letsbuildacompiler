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

//--------------------------------------------------------------
// Negate Primary

void Negate()
{
	EmitLn("NEG D0");
}

//--------------------------------------------------------------
// Push Primary to Stack

void Push()
{
	EmitLn("MOVE D0,-(SP)");
}

//--------------------------------------------------------------
// Add TOS to Primary

void PopAdd()
{
	EmitLn("ADD (SP)+,D0");
}

//--------------------------------------------------------------
// Subtract TOS from Primary

void PopSub()
{
	EmitLn("SUB (SP)+,D0");
	Negate();
}

//--------------------------------------------------------------
// Multiply TOS by Primary

void PopMul()
{
	EmitLn("MULS (SP)+,D0");
}

//--------------------------------------------------------------
// Divide Primary by TOS

void PopDiv()
{
	EmitLn("MOVE (SP)+,D7");
	EmitLn("EXT.L D7");
	EmitLn("DIVS D0,D7");
	EmitLn("MOVE D7,D0");
}

//--------------------------------------------------------------
// Store the Primary Register to Variable

void StoreVariable(char *Name)
{
	snprintf(buf, MAX_BUF, "LEA %s(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

//--------------------------------------------------------------
// Or TOS with Primary

void PopOr()
{
	EmitLn("OR (SP)+,D0");
}

//--------------------------------------------------------------
// Exclusive-Or TOS with Primary

void PopXor()
{
	EmitLn("EOR (SP)+,D0");
}

//--------------------------------------------------------------
// And TOS with Primary

void PopAnd()
{
	EmitLn("AND (SP)+,D0");
}

//--------------------------------------------------------------
// Bitwise Not Primary

void NotIt()
{
	EmitLn("EOR #-1,D0");
}

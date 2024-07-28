#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Definitions

const char TAB = '\t';

//--------------------------------------------------------------
// Variable Definitions

char Look;
char buf[MAX_BUF];

//--------------------------------------------------------------
// Read New Character From Input Stream

void GetChar()
{
	Look = getchar();
}

//--------------------------------------------------------------
// Report an Error

void Error(char *s)
{
	printf("\n");
	printf("Error: %s.\n", s);
}

//--------------------------------------------------------------
// Report Error and Halt

void Abort(char *s)
{
	Error(s);
	exit(-1);
}

//--------------------------------------------------------------
// Report What Was Expected

void Expected(char *s)
{
	char tmp[MAX_BUF];
	snprintf(tmp, MAX_BUF, "%s Expected", s);
	Abort(tmp); 
}

//--------------------------------------------------------------
// Match a Specific Input Character

void Match(char *x)
{
	char tmp[MAX_BUF];
	if (Look == *x) {
		GetChar();
	} else {
		snprintf(tmp, MAX_BUF, "\"%s\"", x); 
		Expected(tmp);
	}
}

//--------------------------------------------------------------
// Get an Identifier

char GetName()
{
	if (!isalpha(Look)) {
		Expected("Name");
	}
	char look = toupper(Look);
	GetChar();
	return look;
}

//--------------------------------------------------------------
// Get a Number

char GetNum()
{
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	char look = Look;
	GetChar();
	return look;
}

//--------------------------------------------------------------
// Output a String with Tab

void Emit(char *s)
{
	printf("%c%s", TAB, s);
}

//--------------------------------------------------------------
// Output a String with TAB and CRLF

void EmitLn(char *s)
{
	Emit(s);
	printf("\n");
}

//--------------------------------------------------------------
// Initialize

void Init()
{
	GetChar();
}	

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char c)
{
	return c == '+' || c == '-';
}

//--------------------------------------------------------------
// Parse and Translate A Program

void Prog()
{
	char Name;
	Match("p");		// Handles program header part
	Name = GetName();
	Prolog();
	DoBlock(Name);
	Match(".");
	Epilog(Name);
}

//--------------------------------------------------------------
// Write the Prolog

void Prolog()
{
	EmitLn("WARMST EQU $A01E");
}

//--------------------------------------------------------------
// Write the Epilog

void Epilog(char Name)
{
	EmitLn("DC WARMST");
	snprintf(buf, MAX_BUF, "END %c", Name);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Parse and Translate a Pascal Block

void DoBlock(char Name)
{
	Declarations();
	PostLabel(Name);
	Statements();
}

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char L)
{
	printf("%c:", L);
}

//--------------------------------------------------------------
// Parse and Translate the Delcaration Part

void Declarations()
{
	while (Look == 'l' || Look == 'c' || Look == 't' || Look == 'v' || Look == 'p' || Look == 'f') {
		switch (Look) {
			case 'l':
				Labels();
				break;
			case 'c':
				Constants();
				break;
			case 't':
				Types();
				break;
			case 'v':
				Variables();
				break;
			case 'p':
				DoProcedure();
				break;
			case 'f':
				DoFunction();
				break;
		}
	}
}

void Statements()
{
	Match("b");
	while (Look != 'e') {
		GetChar();
	}
	Match("e");	
}

//--------------------------------------------------------------
// Process Label Statement

void Labels()
{
	Match("l");
}

//--------------------------------------------------------------
// Process Const Statement

void Constants()
{
	Match("c");
}

//--------------------------------------------------------------
// Process Type Statement

void Types()
{
	Match("t");
}

//--------------------------------------------------------------
// Process Var Statement

void Variables()
{
	Match("v");
}

//--------------------------------------------------------------
// Process Procedure Statement

void DoProcedure()
{
	Match("p");
}

//--------------------------------------------------------------
// Process Function Statement

void DoFunction()
{
	Match("f");
}

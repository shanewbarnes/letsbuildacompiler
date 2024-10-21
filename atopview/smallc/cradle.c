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
char Class;
char Typ;
char Sign;
char Int = 0;

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
	while (Look != '\n') {
		switch (Look) {
			case '#':
				PreProc();
				break;
			case 'i':
				IntDecl();
				break;
			case 'c':
				CharDecl();
				break;
			default:
				DoFunc(Int);
				break;
		}
	}
}

//--------------------------------------------------------------
// Process an Int Declaration

void IntDecl()
{
	GetChar();
}

//--------------------------------------------------------------
// Process a Char Declaration

void CharDecl()
{
	GetChar();
}

//--------------------------------------------------------------
// Process a Preprocessor Declaration

void PreProc()
{
	GetChar();
}

//--------------------------------------------------------------
// Get a Storage Class Specifier

void GetClass()
{
	if (Look == 'a' || Look == 'x' || Look == 's') {
		Class = Look;
		GetChar();
	}
	else {
		Class = 'a';
	}
}

//--------------------------------------------------------------
// Get a Type Specifier

void GetType()
{
	Typ = ' ';
	if (Look == 'u') {
		Sign = 'u';
		Typ = 'i';
		GetChar();
	} else {
		Sign = 's';
	}
	if (Look == 'i' || Look == 'l' || Look == 'c') {
		Typ = Look;
		GetChar();
	}
}

//--------------------------------------------------------------
// Process a Top-Level Declaration

void TopDecl()
{
	char Name;
	Name = GetName();
	if (Look == '(') {
		DoFunc(Name);
	} else {
		DoData(Name);
	}
}

//--------------------------------------------------------------
// Process a Function Declaration

void DoFunc(char n)
{
	Match("(");
	Match(")");
	Match("{");
	Match("}");
	if (Typ == ' ') {
		Typ = 'i';
	}
	printf("%c %c %c function %c\n", Class, Sign, Typ, n);
}

//--------------------------------------------------------------
// Process a Data Declaration

void DoData(char n)
{
	if (Typ == ' ') {
		Expected("Type declaration");
	}
	printf("%c %c %c data %c\n", Class, Sign, Typ, n);
	while (Look == ',') {
		Match(",");
		n = GetName();
		printf("%c %c %c data %c\n", Class, Sign, Typ, n);
	}
	Match(";");
}


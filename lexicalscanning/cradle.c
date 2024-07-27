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
const char CR = '\n';
const Symbol KWlist[4] = {"IF", "ELSE", "ENDIF", "END"};
const char KWcode[5] = "xilee";

//--------------------------------------------------------------
// Variable Definitions

char Look;
char buf[MAX_BUF];
char Token;
char ValueArray[16];		// String Token of Look
char *Value = ValueArray;

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

void GetName()
{
	char *c = Value;
	if (!isalpha(Look)) {
		Expected("Name");
	}
	while (isalnum(Look)) {
		*c++ = toupper(Look);
		GetChar();
	}
	*c = '\0';
	SkipWhite();
	Token = KWcode[Lookup(&KWlist, Value, 4)];
}

//--------------------------------------------------------------
// Get a Number

void GetNum()
{
	char *n = Value;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look)) {
		*n++ = Look;
		GetChar();
	}
	*n = '\0';
	SkipWhite();
	Token = '#';
}

//--------------------------------------------------------------
// Get an Operator

void GetOp()
{
	char *o = Value;
	if (!IsOp(Look)) {
		Expected("Operator");
	}
	while (IsOp(Look)) {
		*o++ = Look;
		GetChar();
	}
	*o = '\0';
	if (strlen(Value) == 1) {
		Token = Value[0];
	} else {
		Token = '?';
	}
}

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c) 
{
	return c == ' ' || c == TAB; 
}

//--------------------------------------------------------------
// Skip Over Leading White Space 

void SkipWhite() {
	while (IsWhite(Look)) {
		GetChar();
	}
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
// Lexical Scanner

char *Scan()
{
	int k;
	while (Look == CR) {
		Fin();
	}
	if (isalpha(Look)) {
		GetName();
	} else if (isdigit(Look)) {
		GetNum();
	} else if (IsOp(Look)) {
		GetOp();
	} else {
		char look = Look;
		Value = &look;
		Token = '?';
		GetChar();
	}
	SkipWhite();
}

//--------------------------------------------------------------
// Skip a CR

void Fin()
{
	if (Look == CR) {
		GetChar();
	}
}

//--------------------------------------------------------------
// Recognize Any Operator

int IsOp(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' || c == ':' || c == '=';
}

//--------------------------------------------------------------
// Skip Over a Comma

void SkipComma()
{
	SkipWhite();
	if (Look == ',') {
		GetChar();
		SkipWhite();
	}
}

//--------------------------------------------------------------
// Table Lookup

// If the input string matches a table entry, return the entry index. If not, return a zero.

int Lookup(TabPtr T, char *s, int n)
{
	int i;
	int found;

	found = 0;
	i = n;
	while (i > 0 && !found) {
		if (strcmp(s, (*T)[i - 1]) == 0) {
			found = 1;
		} else {
			i--;
		}
	}
	return i;
}

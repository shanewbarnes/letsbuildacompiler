#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cradle.h"

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

const char TAB = '\t';
const char CR = '\n';

//--------------------------------------------------------------
// Variable Declarations

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
	if (Look != *x) {
		snprintf(tmp, MAX_BUF, "\"%s\"", x); 
		Expected(tmp);
	} else {
		GetChar();
		SkipWhite();
	}
}

//--------------------------------------------------------------
// Get an Identifier

char *GetName()
{
	char *Token = (char *) malloc(MAX_EXPR * sizeof(char));
	int i = 0;
	if (!isalpha(Look)) {
		Expected("Name");
	}
	while (isalnum(Look) && i < MAX_EXPR) {
		Token[i++] = toupper(Look);
		GetChar();
	}
	Token[i] = '\0';
	SkipWhite();
	return Token;
}

//--------------------------------------------------------------
// Get a Number

char *GetNum()
{
	char *Value = (char *) malloc(MAX_EXPR * sizeof(char));
	int i = 0;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look) && i < MAX_EXPR) {
		Value[i++] = Look;
		GetChar();
	}
	Value[i] = '\0';
	SkipWhite();
	return Value;
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
	SkipWhite();
}	

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor()
{
	if (Look == '(') {
		Match("(");
		Expression();
		Match(")");
	} else if (isalpha(Look)) {
		Ident();
	} else {
		snprintf(buf, MAX_BUF, "MOVE #%s,D0", GetNum());
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate an Identifier

void Ident()
{
	char *Name = GetName();
	if (Look == '(') {
		Match("(");
		Match(")");
		snprintf(buf, MAX_BUF, "BSR %s", Name);
		EmitLn(buf);
	} else {
		snprintf(buf, MAX_BUF, "MOVE %s,(PC),D0", Name);
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression() 
{
	if (IsAddop(Look)) {
		EmitLn("CLR D0");
	} else {
		Term();
	}
	while (IsAddop(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment()
{
	char *Name = GetName();
	Match("=");
	Expression();
	snprintf(buf, MAX_BUF, "LEA %s(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add()
{
	Match("+");
	Term();
	EmitLn("ADD (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract()
{
	Match("-");
	Term();
	EmitLn("SUB (SP)+,D0");
	EmitLn("NEG D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply()
{
	Match("*");
	Factor();
	EmitLn("MULS (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide()
{
	Match("/");
	Factor();
	EmitLn("MOVE (SP)+,D1");
	EmitLn("EXS.L D0");
	EmitLn("DIVS D1,D0");
}

//--------------------------------------------------------------
// Parse ad Translate a Math Term
void Term()
{
	Factor();
	while (Look == '*' || Look == '/') {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
		}
	}
}

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char c) 
{
	if (c == '+' || c == '-') {
		return 1;
	} else {
		return 0;
	}
}

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c) 
{
	return c == ' ' || c == TAB || c == CR;
}

//--------------------------------------------------------------
// Skip Over Leading White Space 

void SkipWhite() {
	while (IsWhite(Look)) {
		GetChar();
	}
}
//--------------------------------------------------------------

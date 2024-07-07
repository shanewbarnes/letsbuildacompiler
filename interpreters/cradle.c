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

//--------------------------------------------------------------
// Variable Definitions

char Look;
char buf[MAX_BUF];
int Table[LETTERS];

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
	snprintf(buf, MAX_BUF, "%s Expected", s);
	Abort(buf); 
}

//--------------------------------------------------------------
// Match a Specific Input Character

void Match(char *x)
{
	if (Look == *x) {
		GetChar();
	} else {
		snprintf(buf, MAX_BUF, "\"%s\"", x); 
		Expected(buf);
	}
}

//--------------------------------------------------------------
// Recognize and Skip Over a Newline

void NewLine()
{
	if (Look == CR) {
		GetChar();
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
	InitTable();
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
// Get a Number

int GetNum()
{
	int Value = 0;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look)) {
		Value = 10 * Value + Look - '0';
		GetChar();
	}
	return Value;
}
  
//--------------------------------------------------------------
// Parse and Translate an Expression

int Expression()
{
	int Value;
	if (IsAddop(Look)) {
		Value = 0;
	} else {
		Value = Term();
	}
	while (IsAddop(Look)) {
		switch (Look) {
			case '+':
				Match("+");
				Value += Term();
				break;
			case '-':
				Match("-");
				Value -= Term();
				break;
		}
	}
	return Value; 
}

//--------------------------------------------------------------
// Parse and Translate an Math Term

int Term()
{
	int Value = Factor();
	while (Look == '*' || Look == '/') {
		switch (Look) {
			case '*':
				Match("*");
				Value *= Factor();
				break;
			case '/':
				Match("/");
				Value /= Factor();
				break;
		}
	}
	return Value;
}

//--------------------------------------------------------------
// Parse and Translate a Math Factor

int Factor()
{
	int factor;
	if (Look == '(') {
		Match("(");
		factor = Expression();
		Match(")");
	} else if (isalpha(Look)) {
		factor = Table[GetName() - 'A'];
	} else {
		factor = GetNum();
	}
	return factor;
}

//--------------------------------------------------------------
// Initialize the Variable Area

void InitTable()
{
	for (int i = 0; i < LETTERS; i++) {
		Table[i] = 0;
	}
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment()
{
	char Name = GetName();
	Match("=");
	Table[Name - 'A'] = Expression();
}

//--------------------------------------------------------------
// Input Routine

void Input()
{
	Match("?");
	scanf("%d", &Table[GetName() - 'A']);
}

//--------------------------------------------------------------
// Output Routine

void Output()
{
	Match("!");
	printf("%d\n", Table[GetName() - 'A']);
}

//--------------------------------------------------------------

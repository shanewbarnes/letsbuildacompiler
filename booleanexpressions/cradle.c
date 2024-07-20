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
// Recognize a Boolean Literal

int IsBoolean(char c)
{
	return toupper(c) == 'T' || toupper(c) == 'F';
}

//--------------------------------------------------------------
// Get a Boolean Literal

int GetBoolean()
{
	int c;
	if (!IsBoolean(Look)) {
		Expected("Boolean Literal");
	}
	c = toupper(Look) == 'T';
	GetChar();
	return c;
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Term

void BoolTerm()
{
	NotFactor();
	while (Look == '&') {
		EmitLn("MOVE D0,-(SP)");
		Match("&");
		NotFactor();
		EmitLn("AND (SP)+,D0");
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr()
{
	Match("|");
	BoolTerm();
	EmitLn("OR (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate an Exclusive OR

void BoolXor()
{
	Match("~");
	BoolTerm();
	EmitLn("EOR (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize a Boolean OrOp

int IsOrOp(char c)
{
	return c == '|' || c == '~';
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression()
{
	BoolTerm();
	while (IsOrOp(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
			case '|':
				BoolOr();
				break;
			case '~':
				BoolXor();
				break;
			}
	}
}
	
//--------------------------------------------------------------
// Parse and Translate a Boolean Factor with NOT

void NotFactor()
{
	if (Look == '!') {
		Match("!");
		BoolFactor();
		EmitLn("EOR #-1,D0");
	} else {
		BoolFactor();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor

void BoolFactor()
{
	if (IsBoolean(Look)) {
		if (GetBoolean()) {
			EmitLn("MOVE #-1,D0");
		} else {
			EmitLn("CLR D0");
		}
	} else {
		Relation();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation()
{
	Expression();
	if (IsRelop(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
			case '=':
				Equals();
				break;
			case '#':
				NotEquals();
				break;
			case '<':
				Less();
				break;
			case '>':
				Greater();
				break;
		}
		EmitLn("TST D0");
	}
}

//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char c)
{
	return c == '=' || c == '#' || c == '<' || c == '>';
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Equals"

void Equals()
{
	Match("=");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SEQ D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Not Equals"

void NotEquals()
{
	Match("#");
	Expression();
	EmitLn("CMP(SP)+,D0");
	EmitLn("SNE D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Less Than"

void Less()
{
	Match("<");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SGE D0");
}

//--------------------------------------------------------------
// Recognize and Transalte a Relational "Greater Than"

void Greater()
{
	Match(">");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SLE D0");
}

//--------------------------------------------------------------
// Parse and Translate an Identifier

void Ident()
{
	char Name = GetName();
	if (Look == '(') {
		Match("(");
		Match(")");
		snprintf(buf, MAX_BUF, "BSR %c", Name);
		EmitLn(buf);
	} else {
		snprintf(buf, MAX_BUF, "MOVE %c(PC),D0", Name);
		EmitLn(buf);
	}
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
		snprintf(buf, MAX_BUF, "MOVE #%c,D0", GetNum());
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate the First Math Factor

void SignedFactor()
{
	if (Look == '+') {
		GetChar();
	}
	if (Look == '-') {
		GetChar();
		if (isdigit(Look)) {
			snprintf(buf, MAX_BUF, "MOVE #-%d,D0", GetNum());
			EmitLn(buf);
		} else {
			Factor();
			EmitLn("NEG D0");
		}
	} else {
		Factor();
	}
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
	SignedFactor();
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
// Parse and Translate an Expression

void Expression() 
{
	Term();
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

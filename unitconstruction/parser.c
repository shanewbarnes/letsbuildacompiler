#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "scanner.h"
#include "error.h"
#include "codegen.h"
#include "parser.h"

//--------------------------------------------------------------
// unit Parser

//--------------------------------------------------------------
// Parse and Translate a Factor

void Factor()
{
	if (isdigit(Look)) {
		LoadConstant(GetNumber());
	} else if (isalpha(Look)) {
		LoadVariable(GetName());
	} else {
		snprintf(buf, MAX_BUF, "Unrecognized character %c", Look);
		Error(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate a Term with Optional Sign

void SignedTerm()
{
	char Sign;
	Sign = Look;
	if (IsAddop(Look)) {
		GetChar();
	}
	Term();
	if (Sign == '-') {
		Negate();
	}
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression()
{
	SignedTerm();
	while (IsAddop(Look)) {
		switch (Look) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
			case '|':
				_Or();
				break;
			case '~':
				_Xor();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate an Addition Operation

void Add()
{
	Match("+");
	Push();
	Term();
	PopAdd();
}

//--------------------------------------------------------------
// Parse and Translate an Subtraction Operation

void Subtract()
{
	Match("-");
	Push();
	Term();
	PopSub();
}

//--------------------------------------------------------------
// Parse and Translate a Term

void Term()
{
	NotFactor();
	while (IsMulop(Look)) {
		switch (Look) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
			case '&':
				_And();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate a Multiplication Operation

void Multiply()
{
	Match("*");
	Push();
	NotFactor();
	PopMul();
}

//--------------------------------------------------------------
// Parse and Translate a Division Operation

void Divide()
{
	Match("/");
	Push();
	NotFactor();
	PopDiv();
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment()
{
	char *Name;
	Name = GetName();
	Match("=");
	Expression();
	StoreVariable(Name);
}

//--------------------------------------------------------------
// Parse and Translate an Or Operation

void _Or()
{
	Match("|");
	Push();
	Term();
	PopOr();
}

//--------------------------------------------------------------
// Parse and Translate an Xor Operation

void _Xor()
{
	Match("~");
	Push();
	Term();
	PopXor();
}

//--------------------------------------------------------------
// Parse and Translate a Boolean And Operation

void _And()
{
	Match("&");
	Push();
	NotFactor();
	PopAnd();
}

//--------------------------------------------------------------
// Parse and Translate a Factor with Optional "Not"

void NotFactor()
{
	if (Look == '!') {
		Match("!");
		Factor();
		NotIt();
	} else {
		Factor();
	}
}

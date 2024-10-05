#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner1.h"
#include "input.h"
#include "errors.h"

//--------------------------------------------------------------
// unit Scanner1

//--------------------------------------------------------------
// Recognize an Addition Operator

int IsAddop(char c)
{
	return c == '+' || c == '-';
}

//--------------------------------------------------------------
// Recognize a Multiplication Operator

int IsMulop(char c)
{
	return c == '*' || c == '/';
}

//--------------------------------------------------------------
// Match One Character

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
	char look = Look;
	GetChar();
	return look;
}

//--------------------------------------------------------------
// Get an Number

char GetNumber()
{
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	char look = Look;
	GetChar();
	return look;
}

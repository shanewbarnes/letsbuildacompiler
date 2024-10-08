#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "input.h"
#include "errors.h"

//--------------------------------------------------------------
// unit Scanner1

//--------------------------------------------------------------
// Recognize an Addition Operator

int IsAddop(char c)
{
	return c == '+' || c == '-' || c == '|' || c == '~';
}

//--------------------------------------------------------------
// Recognize a Multiplication Operator

int IsMulop(char c)
{
	return c == '*' || c == '/' || c == '&';
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

char *GetName()
{
	char *n;
	int i;
	n = (char *) malloc(MAX_EXPR * sizeof(char));
	i = 0;
	if (!isalpha(Look)) {
		Expected("Name");
	}
	while (isalnum(Look) && i < MAX_EXPR) {
		n[i++] = Look;
		GetChar();
	}
	n[i] = '\0';
	return n;
}

//--------------------------------------------------------------
// Get an Number

char *GetNumber()
{	
	char *n;
	int i;
	n = (char *) malloc(MAX_EXPR * sizeof(char));
	i = 0;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look) && i < MAX_EXPR) {
		n[i++] = Look;
		GetChar();
	}
	n[i] = '\0';
	return n;
}

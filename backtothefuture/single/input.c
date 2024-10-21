#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"

//--------------------------------------------------------------
// unit Input

char Look;		// Lookahead character

//--------------------------------------------------------------
// Read New Character From Input Stream

void GetChar()
{
	Look = getchar();
}

//--------------------------------------------------------------
// Unit Initialization

void Init()
{
	GetChar();
}

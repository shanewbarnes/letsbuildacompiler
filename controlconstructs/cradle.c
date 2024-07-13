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
int LCount;

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
	LCount = 0;
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
// Recognize and Translate an "Other"

void Other() 
{
	char name = GetName();
	snprintf(buf, MAX_BUF, "%c", name);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Parse and Translate a Program

void DoProgram()
{
	Block();
	if (Look != 'e') {
		Expected("End");
	}
	EmitLn("END");
}

//--------------------------------------------------------------
// Generate a Unique Label

char *NewLabel()
{
	char *S = (char *) malloc((LCOUNT_SIZE + 1) * sizeof(char));
	snprintf(S, LCOUNT_SIZE + 1, "L%d", LCount);
	LCount++;
	return S;
}

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char *L)
{
	printf("%s:", L);
}

//--------------------------------------------------------------
// Recognize and Translate and IF Construct

void DoIf() 
{
	char *L1;
	char *L2;
	Match("i");
	Condition();
	L1 = NewLabel();
	L2 = L1;
	snprintf(buf, MAX_BUF, "BEQ %s", L1);
	EmitLn(buf);
	Block();
	if (Look == 'l') {
		Match("l");
		L2 = NewLabel();
		snprintf(buf, MAX_BUF, "BRA %s", L2);
		EmitLn(buf);
		PostLabel(L1);
		Block();
	}
	Match("e");
	PostLabel(L2);
}	

//--------------------------------------------------------------
// Parse and Translate a WHILE Statement

void DoWhile()
{
	char *L1;
	char *L2;
	Match("w");
	L1 = NewLabel();
	L2 = NewLabel();
	PostLabel(L1);
	Condition();
	snprintf(buf, MAX_BUF, "BEQ %s", L2);
	EmitLn(buf);
	Block();
	Match("e");
	snprintf(buf, MAX_BUF, "BRA %s", L1);
	EmitLn(buf);
	PostLabel(L2);
}

//--------------------------------------------------------------
// Parse and Translate a LOOP Statement

void DoLoop()
{
	char *L;
	Match("p");
	L = NewLabel();
	PostLabel(L);
	Block();
	Match("e");
	snprintf(buf, MAX_BUF, "BRA %s", L);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Parse and Translate a REPEAT Statement

void DoRepeat()
{
	char *L;
	Match("r");
	L = NewLabel();
	PostLabel(L);
	Block();
	Match("u");
	Condition();
	snprintf(buf, MAX_BUF, "BEQ %s", L);
	EmitLn(buf);
}
	
//--------------------------------------------------------------
// Recognize and Transalate a Statement Block

void Block() 
{
	while (Look != 'e' && Look != 'l' && Look != 'u') {
		switch (Look) {
			case 'i':
				DoIf();
				break;
			case 'w':
				DoWhile();
				break;
			case 'p':
				DoLoop();
				break;
			case 'r':
				DoRepeat();	
				break;
			default:
				Other();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Condition
// This version is a dummy

void Condition() 
{
	EmitLn("<condition>");
}

//--------------------------------------------------------------

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
	Block("");
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

void DoIf(char *L) 
{
	char *L1;
	char *L2;
	Match("i");
	Condition();
	L1 = NewLabel();
	L2 = L1;
	snprintf(buf, MAX_BUF, "BEQ %s", L1);
	EmitLn(buf);
	Block(L);
	if (Look == 'l') {
		Match("l");
		L2 = NewLabel();
		snprintf(buf, MAX_BUF, "BRA %s", L2);
		EmitLn(buf);
		PostLabel(L1);
		Block(L);
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
	Block(L2);
	Match("e");
	snprintf(buf, MAX_BUF, "BRA %s", L1);
	EmitLn(buf);
	PostLabel(L2);
}

//--------------------------------------------------------------
// Parse and Translate a LOOP Statement

void DoLoop()
{
	char *L1;
	char *L2;
	Match("p");
	L1 = NewLabel();
	L2 = NewLabel();
	PostLabel(L1);
	Block(L2);
	Match("e");
	snprintf(buf, MAX_BUF, "BRA %s", L1);
	EmitLn(buf);
	PostLabel(L2);
}

//--------------------------------------------------------------
// Parse and Translate a REPEAT Statement

void DoRepeat()
{
	char *L1;
	char *L2;
	Match("r");
	L1 = NewLabel();
	L2 = NewLabel();
	PostLabel(L1);
	Block(L2);
	Match("u");
	Condition();
	snprintf(buf, MAX_BUF, "BEQ %s", L1);
	EmitLn(buf);
	PostLabel(L2);
}

//--------------------------------------------------------------
// Parse and Translate a FOR Statement

void DoFor()
{
	char *L1;
	char *L2;
	char Name;
	Match("f");
	L1 = NewLabel();
	L2 = NewLabel();
	Name = GetName();
	Match("=");
	Expression();
	EmitLn("SUBQ #1,D0");
	snprintf(buf, MAX_BUF, "LEA %c(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
	Expression();
	EmitLn("MOVE D0,-(SP)");
	PostLabel(L1);
	snprintf(buf, MAX_BUF, "LEA %c (PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE (A0),D0");
	EmitLn("ADDQ #1,D0");
	EmitLn("MOVE D0,(A0)");
	EmitLn("CMP (SP),D0");
	snprintf(buf, MAX_BUF, "BGT %s", L2);
	EmitLn(buf);
	Block(L2);
	Match("e");
	snprintf(buf, MAX_BUF, "BRA %s", L1);
	EmitLn(buf);
	PostLabel(L2);
	EmitLn("ADDQ #2,SP");
}

//--------------------------------------------------------------
// Parse and Translate a DO Statement

void DoDo()
{
	char *L1;
	char *L2;
	Match("d");
	L1 = NewLabel();
	L2 = NewLabel();
	Expression();
	EmitLn("SUBQ #1,D0");
	PostLabel(L2);
	EmitLn("MOVE D0,-(SP)");
	Block(L2);
	EmitLn("MOVE (SP)+,D0");
	snprintf(buf, MAX_BUF, "DBRA D0,%s", L1);
	EmitLn(buf);
	PostLabel(L2);
	EmitLn("ADDQ #2,SP");
}

//--------------------------------------------------------------
// Parse and Translate a BREAK Statement

void DoBreak(char *L)
{
	Match("b");
	if (L != "") {
		snprintf(buf, MAX_BUF, "BRA %s", L);
		EmitLn(buf);
	} else {
		Abort("No loop to break from");
	}
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression()
{
	snprintf(buf, MAX_BUF, "<expr>");
	EmitLn(buf);
}

//--------------------------------------------------------------
// Recognize and Transalate a Statement Block

void Block(char *L) 
{
	while (Look != 'e' && Look != 'l' && Look != 'u') {
		switch (Look) {
			case 'i':
				DoIf(L);
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
			case 'f':
				DoFor();
				break;
			case 'd':
				DoDo();
				break;
			case 'b':
				DoBreak(L);
				break;
			default:
				Other();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Condition

void Condition() 
{
	EmitLn("<condition>");
}

//--------------------------------------------------------------

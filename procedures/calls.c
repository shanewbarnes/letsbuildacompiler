#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "calls.h"

//--------------------------------------------------------------
// program Calls

//--------------------------------------------------------------
// Constant Definitions

const char TAB = '\t';
const char CR = '\n';

//--------------------------------------------------------------
// Variable Definitions

char Look;		// Lookahead Character
char buf[MAX_BUF];
char ST[ALPHA_SIZE];

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
// Report an Undefined Identifer

void Undefined(char n)
{
        snprintf(buf, MAX_BUF, "Undefined Identifer %c", n);
        Abort(buf);
}

//--------------------------------------------------------------
// Report an Duplicate Identifer

void Duplicate(char n)
{
        snprintf(buf, MAX_BUF, "Duplicate Identifer %c", n);
        Abort(buf);
}

//--------------------------------------------------------------
// Get Type of Symbol

char TypeOf(char n)
{
	return ST[n - 'A'];
}

//--------------------------------------------------------------
// Look for Symbol in Table

int InTable(char n)
{
	return ST[n - 'A'] != ' ';
}

//--------------------------------------------------------------
// Add a New Symbol to Table

void AddEntry(char Name, char T)
{
	if (InTable(Name)) {
		Duplicate(Name);
	}
	ST[toupper(Name) - 'A'] = T;
}

//--------------------------------------------------------------
// Check an Entry to Make Sure It's a Variable

void CheckVar(char Name)
{
	if (!InTable(Name)) {
		Undefined(Name);
	}
	if (TypeOf(Name) != 'v') {
        	snprintf(buf, MAX_BUF, "%c is not a variable", Name);
        	Abort(buf);
	}
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
	SkipWhite();
}

//--------------------------------------------------------------
// Get an Identifier

char GetName()
{
	if (!isalpha(Look)) {
		printf("%c\n", Look);
		Expected("Name");
	}
	char look = toupper(Look);
	GetChar();
	SkipWhite();
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
	SkipWhite();
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
// Post a Label To Output

void PostLabel(char L)
{
	printf("%c:", L);
}

//--------------------------------------------------------------
// Initialize

void Init()
{
	GetChar();
	SkipWhite();
	for (int i = 0; i < ALPHA_SIZE; i++) {
		ST[i] = ' ';
	}
}	

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char c)
{
	return c == '+' || c == '-';
}

//--------------------------------------------------------------
// Recognize an Mulop

int IsMulop(char c) 
{
	return c == '*' || c == '/';
}

//--------------------------------------------------------------
// Recognize a Boolean Orop

int IsOrop(char c)
{
	return c == '|' || c == '~';
}

//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char c)
{
	return c == '=' || c == '#' || c == '<' || c == '>';
}

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c)
{
	return c == ' ' || c == '\t';
}

//--------------------------------------------------------------
// Skip Over Leading White Space

void SkipWhite()
{
	while (IsWhite(Look)) {
		GetChar();
	}
}

//--------------------------------------------------------------
// Skip Over an End-of-Line

void Fin()
{
	if (Look == CR) {
		GetChar();
	}
}

//--------------------------------------------------------------
// Load a Variable to the Primary Register

void LoadVar(char Name)
{
	CheckVar(Name);
	snprintf(buf, MAX_BUF, "MOVE %c(PC),D0", Name);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Store the Primary Register

void StoreVar(char Name)
{
	CheckVar(Name);
	snprintf(buf, MAX_BUF, "LEA %c(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

//--------------------------------------------------------------
// Parse and Translate an Expression
// Vestigial Version

void Expression()
{
	LoadVar(GetName());
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment(char Name)
{
	Match("=");
	Expression();
	StoreVar(Name);
}

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void AssignOrProc()
{
	char Name;
	Name = GetName();
	switch (TypeOf(Name)) {
		case ' ':
			Undefined(Name);
			break;
		case 'v':
			Assignment(Name);
			break;
		case 'p':
			CallProc(Name);
			break;
		default:
			snprintf(buf, MAX_BUF, "Identifier %c Cannnot Be Used Here");
			Abort(buf);
			break;
	}
}

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void DoBlock()
{
	while (Look != 'e') {
		AssignOrProc();
		Fin();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Begin-Block

void BeginBlock()
{
	Match("b");
	Fin();
	DoBlock();
	Match("e");
	Fin();
}

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(char N)
{
	if (InTable(N)) {
		Duplicate(N);
	}
	ST[N - 'A'] = 'v';
	printf("%c:%cDC 0\n", N, TAB);
}

//--------------------------------------------------------------
// Parse and Translate a Data Declaration

void Decl()
{
	char Name;
	Match("v");
	Alloc(GetName());
}

//--------------------------------------------------------------
// Parse and Translate a Procedure Declaration

void DoProc()
{
	char N;
	Match("p");
	N = GetName();
	Fin();
	if (InTable(N)) {
		Duplicate(N);
	}
	ST[N] = 'p';
	PostLabel(N);
	BeginBlock();
	Return();
}

//--------------------------------------------------------------
// Emits an RTS Instruction

void Return()
{
	EmitLn("RTS");
}

//--------------------------------------------------------------
// Parse and Translate a Main Program

void DoMain()
{
	char N;
	Match("P");
	N = GetName();
	Fin();
	if (InTable(N)) {
		Duplicate(N);
	}
	Prolog();
	BeginBlock();
} 

//--------------------------------------------------------------
// Parse and Translate Global Declarations

void TopDecls()
{
	while (Look != '.') {
		switch (Look) {
			case 'v':
				Decl();
				break;
			case 'p':
				DoProc();
				break;
			case 'P':
				DoMain();
				break;
			default:
				Abort("Unrecognized Keyword");
				break;
		}
		Fin();
	}
}

//--------------------------------------------------------------
// Call a Procedure

void CallProc(char N)
{
	snprintf(buf, MAX_BUF, "BSR %c", N);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Write the Prolog

void Prolog()
{
	printf("MAIN:");
}


//--------------------------------------------------------------
// Write the Epilog

void Epilog()
{
        EmitLn("DC WARMST");
        EmitLn("END MAIN");
}


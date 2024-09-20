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
int NumParams;
int Base;
char buf[MAX_BUF];
char ST[ALPHA_SIZE];
int Params[ALPHA_SIZE];

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
	if (IsParam(n)) {
		return 'f';
	} else {
		return ST[n - 'A'];
	}
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
	ClearParams();
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
	char Name;
	Name = GetName();
	if (IsParam(Name)) {
		LoadParam(ParamNumber(Name));
	} else {
		LoadVar(Name);
	}
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment(char Name)
{
	Match("=");
	Expression();
	if (IsParam(Name)) {
		StoreParam(ParamNumber(Name));
	} else {
		StoreVar(Name);
	}
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
		case 'f':
			Assignment(Name);
			break;
		case 'p':
			CallProc(Name);
			break;
		default:
			snprintf(buf, MAX_BUF, "Identifier %c Cannnot Be Used Here", Name);
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
	int k;
	Match("p");
	N = GetName();
	if (InTable(N)) {
		Duplicate(N);
	}
	ST[N - 'A'] = 'p';
	FormalList();
	k = LocDecls();
	ProcProlog(N, k);
	BeginBlock();
	ProcEpilog();
	ClearParams();
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

void Call(char N)
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

//--------------------------------------------------------------
// Process the Formal Parameter List of a Procedure

void FormalList() 
{
	Match("(");
	if (Look != ')') {
		FormalParam();
		while (Look == ',') {
			Match(",");
			FormalParam();
		}
	}
	Match(")");
	Fin();
	Base = NumParams;
	NumParams += 4;
	Fin();
}

//--------------------------------------------------------------
// Process a Formal Parameter

void FormalParam()
{
	AddParam(GetName());
}

//--------------------------------------------------------------
// Process an Actual Parameter

void Param()
{
	Expression();
	Push();
}

//--------------------------------------------------------------
// Process the Parameter List for a Procedure Call

int ParamList()
{
	int N;
	N = 0;
	Match("(");
	if (Look != ')') {
		Param();
		N++;
		while (Look == ',') {
			Match(",");
			Param();
			N++;
		}
	}
	Match(")");
	return 2 * N;
}

//--------------------------------------------------------------
// Process a Procedure Call

void CallProc(char Name)
{
	int N;
	N = ParamList();
	Call(Name);
	CleanStack(N);
}

//--------------------------------------------------------------
// Initialize Parameter Table to Null

void ClearParams()
{
	char i;
	for (i = 0; i < ALPHA_SIZE; i++) {
		Params[i] = 0;
	}
	NumParams = 0;
}

//--------------------------------------------------------------
// Find the Parameter Number

int ParamNumber(char N)
{
	return Params[N - 'A'];
}

//--------------------------------------------------------------
// See if an Identifier is a Parameter

int IsParam(char N)
{
	return Params[N - 'A'] != 0;
}

//--------------------------------------------------------------
// Add a New Parameter to Table

void AddParam(char Name)
{
	if (IsParam(Name)) {
		Duplicate(Name);
	}
	NumParams++;
	Params[Name - 'A'] = NumParams;
}

//--------------------------------------------------------------
// Load a Parameter to the Primary Register

void LoadParam(int N)
{
	int Offset;
	Offset = 8 + 2 * (Base - N);
	Emit("MOVE ");
	printf("%d(A6),D0\n", Offset);
}

//--------------------------------------------------------------
// Store a Paramter from the Primary Register

void StoreParam(int N)
{
	int Offset;
	Offset = 8 + 2 * (Base - N);
	Emit("MOVE D0,");
	printf("%d(A6)\n", Offset);
}

//--------------------------------------------------------------
// Push The Primary Register to the Stack

void Push()
{
	EmitLn("MOVE D0,-(SP)");
}

//--------------------------------------------------------------
// Adjust the Stack Pointer Upwards by N Bytes

void CleanStack(int N) 
{
	if (N > 0) {
		Emit("ADD #");
		printf("%d,SP\n", N);
	}
}

//--------------------------------------------------------------
// Write the Prolog for a Procedure

void ProcProlog(char N, int k)
{
	PostLabel(N);
	Emit("LINK A6,#");
	printf("%d\n", -2 * k);
}

//--------------------------------------------------------------
// Write the Epilog for a Procedure

void ProcEpilog()
{
	EmitLn("UNLK A6");
	EmitLn("RTS");
}

//--------------------------------------------------------------
// Parse and Translate a Local Data Declaration

void LocDecl()
{
	char Name;
	Match("v");
	AddParam(GetName());
	Fin();
}

//--------------------------------------------------------------
// Parse and Translate a Local Declaration

int LocDecls()
{
	int n;
	n = 0;
	while (Look == 'v') {
		LocDecl();
		n++;
	}
	return n;
}

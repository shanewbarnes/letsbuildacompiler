#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"

//--------------------------------------------------------------
// program Types

//--------------------------------------------------------------
// Constant Definitions

const char TAB = '\t';
const char CR = '\n';

//--------------------------------------------------------------
// Variable Definitions

char Look;
char ST[ALPHA_SIZE];
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
	SkipWhite();
	return look;
}

//--------------------------------------------------------------
// Get a Number

long GetNum()
{
	long Val;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	Val = 0;
	while (isdigit(Look)) {
		Val = 10 * Val + Look - '0';
		GetChar();
	}
	SkipWhite();
	return Val;
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
	char i;
	for (i = 0; i < ALPHA_SIZE; i++) {
		ST[i] = '?';
	}
	GetChar();
	SkipWhite();
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
	return c == ' ' || c == TAB;
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
	if (Look == '\n') {
		GetChar();
	}
}

//--------------------------------------------------------------
// Dump the Symbol Table

void DumpTable()
{
	char i;
	for (int i = 0; i < ALPHA_SIZE; i++) {
		if (ST[i] != '?') {
			printf("%c %c\n", i + 'A', ST[i]);
		}
	}
}	

//--------------------------------------------------------------
// Report Type of a Variable

char TypeOf(char N)
{
	return ST[N - 'A'];
}

//--------------------------------------------------------------
// Report of a Variable is in the Table

int InTable(char N)
{
	return TypeOf(N) != '?';
}

//--------------------------------------------------------------
// Check for a Duplicate Variable Name

void CheckDup(char N)
{
	if (InTable(N)) {
		snprintf(buf, MAX_BUF, "Duplicate Name %c", N);
		Abort(buf);
	}
}

//--------------------------------------------------------------
// Add Entry to Table

void AddEntry(char N, char T)
{
	CheckDup(N);
	ST[N - 'A'] = T;
}

//--------------------------------------------------------------
// Parse and Translate a Data Declaration

void Decl()
{
	char Typ;
	Typ = GetName();
	Alloc(GetName(), Typ);
}

//--------------------------------------------------------------
// Parse and Translate Global Declarations

void TopDecls()
{
	while (Look != 'B') {
		switch (Look) {
			case 'b':
			case 'w':
			case 'l':
				Decl();
				break;
			default:
				snprintf(buf, MAX_BUF, "Unrecognized Keyword %c", Look);
				Abort(buf);
				break;
		}
		Fin();
	}
}

//--------------------------------------------------------------
// Generate Code for Allocation of a Variable

void AllocVar(char N, char T)
{
	printf("%c:%cDC.%c 0\n", N, TAB, T);
}

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(char N, char T)
{
	AddEntry(N, T);
	AllocVar(N, T);
}

//--------------------------------------------------------------
// Load a Variable to Primary Register

void LoadVar(char Name, char Typ)
{
	char tmp[MAX_BUF];
	snprintf(tmp, MAX_BUF, "%c(PC)", Name);
	Move(Typ, tmp, "D0");
}

//--------------------------------------------------------------
// Generate a Move Instruction

void Move(char Size, char *Source, char *Dest)
{
	snprintf(buf, MAX_BUF, "MOVE.%c %s,%s", Size, Source, Dest);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Recognize a Legal Variable Type

int IsVarType(char c)
{
	return c == 'B' || c == 'W' || c == 'L';
}

//--------------------------------------------------------------
// Get a Variable Type from the Symbol Table

char VarType(char Name)
{
	char Typ;
	Typ = TypeOf(Name);
	if (!IsVarType(Typ)) {
		snprintf(buf, MAX_BUF, "Identifier %c is not a variable", Name);
		Abort(buf);
	}
	return Typ;
}

//--------------------------------------------------------------
// Load a Variable to the Primary Register

char Load(char Name)
{
	char Typ;
	Typ = VarType(Name);
	LoadVar(Name, Typ);
	return Typ;
}

//--------------------------------------------------------------
// Store Primary to Variable

void StoreVar(char Name, char Typ)
{
	snprintf(buf, MAX_BUF, "LEA %c(PC),A0", Name);
	EmitLn(buf);
	Move(Typ, "D0", "(A0)");
}

//--------------------------------------------------------------
// Store a Variable from the Primary Register

void Store(char Name, char T1)
{
	char T2;
	T2 = VarType(Name);
	Convert(T1, T2, "D0");
	StoreVar(Name, T2);
}


//--------------------------------------------------------------
// Parse and Translate a Math Term

char Term()
{
	char Typ;
	Typ = Factor();
	while (IsMulop(Look)) {
		Push(Typ);
		switch (Look) {
			case '*':
				Multiply(Typ);
				break;
			case '/':
				Divide(Typ);
				break;
		}
	}
	return Typ;
}

//--------------------------------------------------------------
// Parse and Translate an Expression

char Expression()
{
	char Typ;
	if (IsAddop(Look)) {
		Typ = Unop();
	} else {
		Typ = Term();
	}
	while (IsAddop(Look)) {
		Push(Typ);
		switch (Look) {
			case '+':
				Typ = Add(Typ);
				break;
			case '-':
				Typ = Subtract(Typ);
				break;
		}
	}
	return Typ;
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment()
{
	char Name;
	Name = GetName();
	Match("=");
	Store(Name, Expression());
}

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void Block()
{
	while (Look != '.') {
		Assignment();
		Fin();
	}
}

//--------------------------------------------------------------
// Convert a Data Item from One Type to Another

void Convert(char Source, char Dest, char *Reg) {
	if (Source != Dest) {
		if (Source == 'B') {
			snprintf(buf, MAX_BUF, "AND.W #$FF,%s", Reg);
			EmitLn(buf);
		}
		if (Dest == 'L') {
			snprintf(buf, MAX_BUF, "EXT.L %s", Reg);
			EmitLn(buf);
		}
	}
}

//--------------------------------------------------------------
// Load a Constant to the Primary Register

char LoadNum(long N)
{
	char Typ;
	if (abs(N) <= 127) {
		Typ = 'B';
	} else if (abs(N) <= 32767) {
		Typ = 'W';
	} else {
		Typ = 'L';
	}
	LoadConst(N, Typ);
	return Typ;
}

//--------------------------------------------------------------
// Load a Constant to the Primary Register

void LoadConst(long N, char Typ)
{
	char tmp[MAX_BUF];
	snprintf(tmp, MAX_BUF, "#%ld", N);
	Move(Typ, tmp, "D0");
}

//--------------------------------------------------------------
// Process a Term with Leading Unary Operator

char Unop()
{
	Clear();
	return 'W';
}

//--------------------------------------------------------------
// Push Primary onto Stack

void Push(char Size)
{
	Move(Size, "D0", "-(SP)");
}

//--------------------------------------------------------------
// Recognize and Translate an Add

char Add(char T1)
{
	Match("+");
	return PopAdd(T1, Term());
}

//--------------------------------------------------------------
// Recognize and Translate a Subtract

char Subtract(char T1)
{
	Match("-");
	return PopSub(T1, Term());
}

//--------------------------------------------------------------
// Pop Stack into Secondary Register

void Pop(char Size)
{
	Move(Size, "(SP)+", "D7");
}

//--------------------------------------------------------------
// Promote the Size of a Register Value

char Promote(char T1, char T2, char *Reg)
{
	char Typ;
	if (T1 != T2) {
		if ((T1 == 'B') || ((T1 == 'W') && (T2 == 'L'))) {
			Convert(T1, T2, Reg);
			Typ = T2;
		}
	}
	return Typ;
}

//--------------------------------------------------------------
// Force both Arguments to Same Type

char SameType(char T1, char T2)
{
	T1 = Promote(T1, T2, "D7");
	return Promote(T2, T1, "D0");
}

//--------------------------------------------------------------
// Generate Code to Add Primary to the Stack

char PopAdd(char T1, char T2)
{
	Pop(T1);
	T2 = SameType(T1, T2);
	GenAdd(T2);
	return T2;
}

//--------------------------------------------------------------
// Generate Code to Subtract Primary from the Stack

char PopSub(char T1, char T2)
{
	Pop(T1);
	T2 = SameType(T1, T2);
	GenSub(T2);
	return T2;
}

//--------------------------------------------------------------
// Add Top of Stack to Primary

void GenAdd(char Size)
{
	snprintf(buf, MAX_BUF, "ADD.%c D7,D0", Size);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Subtract Primary from Top of Stack

void GenSub(char Size)
{
	snprintf(buf, MAX_BUF, "SUB.%c D7,D0", Size);
	EmitLn(buf);
	snprintf(buf, MAX_BUF, "NEG.%c D0", Size);
	EmitLn(buf);
}	

//--------------------------------------------------------------
// Clear the Primary Register

void Clear()
{
	EmitLn("CLR D0");
}

//--------------------------------------------------------------
// Parse and Translate a Factor

char Factor()
{
	if (Look == '(') {
		char exp;
		Match("(");
		exp = Expression();
		Match(")");
		return exp;
	} else if (isalpha(Look)) {
		return Load(GetName());
	} else {
		return LoadNum(GetNum());
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Multiply

char Multiply(char T1)
{
	Match("*");
	return PopMul(T1, Factor());
}

//--------------------------------------------------------------
// Recognize and Translate a Divide

char Divide(char T1)
{
	Match("/");
	return PopDiv(T1, Factor());
}

//--------------------------------------------------------------
// Multiply Top of Stack by Primary (Word)

void GenMult()
{
	EmitLn("MULS D7,D0");
}

//--------------------------------------------------------------
// Multiply Top of Stack by Primary (Long)

void GenLongMult()
{
	EmitLn("JSR MUL32");
}

//--------------------------------------------------------------
// Generate Code to Multiply Primary by Stack

char PopMul(char T1, char T2)
{
	char T;
	Pop(T1);
	T = SameType(T1, T2);
	Convert(T, 'W', "D7");
	Convert(T, 'W', "D0");
	if (T == 'L') {
		GenLongMult();
	} else {
		GenMult();
	}
	if (T == 'B') {
		return 'W';
	} else {
		return 'L';
	}
}

//--------------------------------------------------------------
// Generate Code to Divide Stack by the Primary

char PopDiv(char T1, char T2)
{
	Pop(T1);
	Convert(T1, 'L', "D7");
	if ((T1 == 'L') || (T2 == 'L')) {
		Convert(T2, 'L', "D0");
		GenLongDiv();
		return 'L';
	} else {
		Convert(T2, 'W', "D0");
		GenDiv();
		return T1;
	}
}

//--------------------------------------------------------------
// Divide Top of Stack by Primary (Word)

void GenDiv()
{
	EmitLn("DIVS D0,D7");
	Move('W', "D7", "D0");
}

//--------------------------------------------------------------
// Divide Top of Stack by Primary (Long)

void GenLongDiv()
{
	EmitLn("JSR DIV32");
}

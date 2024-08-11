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
const char CR = '\n';

//--------------------------------------------------------------
// Variable Definitions

char Look;			// Lookahead Character
char buf[MAX_BUF];
char Token;			// Encoded Token
char ValueArray[16];		// Unencoded Token
char *Value = ValueArray;
Symbol ST[MaxEntry];
char SType[MaxEntry];
int LCount;
int NEntry = 0;

//--------------------------------------------------------------
// Definition of Keywords and Token Types

Symbol KWlist[MaxEntry] = {"IF", "ELSE", "ENDIF", "WHILE", "ENDWHILE", "READ", "WRITE", "VAR", "BEGIN", "END", "PROGRAM"}; 
char KWcode[NKW1] = "xileweRWvbep";

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
	NewLine();
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

void GetName()
{
	char *c;
	c = Value;
	NewLine();
	if (!isalpha(Look)) {
		Expected("Name");
	}
	while (isalnum(Look)) {
		*c++ = toupper(Look);
		GetChar();
	}
	*c = '\0';
	SkipWhite();
}

//--------------------------------------------------------------
// Get a Number

int GetNum()
{
	NewLine();
	int Val;
	Val = 0;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look)) {
		Val = 10 * Val + Look - '0';
		GetChar();
	}
	SkipWhite();
	return Val;
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

void NewLine()
{
	while (Look == '\n') {
		GetChar();
		SkipWhite();
	}
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
	int i, j;
	for (i = 0; i < MaxEntry; i++) {
		for (j = 0; j < 8; j++) {
			ST[i][j] = '\0';
		}
		SType[i] = ' ';
	}
	LCount = 0;
	GetChar();
	Scan();
}	

//--------------------------------------------------------------
// Recognize an Addop

int IsAddop(char c)
{
	return c == '+' || c == '-';
}

//--------------------------------------------------------------
// Recognize a Mulop

int IsMulop(char c) 
{
	return c == '*' || c == '/';
}

//--------------------------------------------------------------
// Parse and Translate a Program

void Prog()
{
	MatchString("PROGRAM");
	Header();
	TopDecls();
	Main();
	Match(".");
}

//--------------------------------------------------------------
// Parse and Translate a Main Program

void Main()
{
	printf("FLAG\n");
	MatchString("BEGIN");
	printf("FLAG\n");
	Prolog();
	Block();
	MatchString("END");
	Epilog();
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment()
{
	char *Name;
	Name = Value;
	Match("=");
	BoolExpression();
	Store(Name);
}

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void Block()
{
	Scan();
	while (!(Token == 'e' || Token == 'l')) {
		switch (Token) {
			case 'i':
				DoIf();
				break;
			case 'w':
				DoWhile();
				break;
			case 'R':
				DoRead();
				break;
			case 'W':
				DoWrite();
				break;
			default:
				Assignment();
				break;
		}
		Scan();
	}
}

//--------------------------------------------------------------
// Process a Data Declaration

void Decl()
{
	GetName();
	Alloc(Value);
	while (Look == ',') {
		Match(",");
		GetName();
		Alloc(Value);
	}
}

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(Symbol N)
{
	if (InTable(N)) {
		snprintf(buf, MAX_BUF, "Duplicate Variable Name %s", N);
		Abort(buf);
	}
	
	AddEntry(N, 'v');
	printf("%s:%cDC ", N, TAB);
	if (Look == '=') {
		Match("=");
		if (Look == '-') {
			printf("%c", Look);
			Match("-");
		}
		printf("%d\n", GetNum());
	} else {
		printf("0\n");
	}
}

//--------------------------------------------------------------
// Look for Symbol in Table

int InTable(Symbol N)
{
	return Lookup(&ST, N, MaxEntry) != 0;
}

//--------------------------------------------------------------
// Add a New Entry to Symbol Table

void AddEntry(Symbol N, char T)
{
	int i;
	if (InTable(N)) {
		snprintf(buf, MAX_BUF, "Duplicate Identrifier %s", N);
		Abort(buf);
	}
	if (NEntry == MaxEntry) {
		Abort("Symbol Table Full");
	}
	for (i = 0; N[i] != '\0'; i++) {
		ST[NEntry][i] = N[i];
	}
	ST[NEntry][i + 1] = '\0';
	SType[NEntry] = T;
	NEntry++;
}

//--------------------------------------------------------------
// Parse and Translate Global Declarations

void TopDecls()
{
	Scan();
	while (Token != 'b') {
		switch (Token) {
			case 'v':
				Decl();
				break;
			default:
				snprintf(buf, MAX_BUF, "Unrecognized Keyword \"%c\"", Token);
				Abort(buf);
				break;
		}
		Scan();
	}
}

//--------------------------------------------------------------
// Write Header Info

void Header()
{
	printf("WARMST %cEQU $A01E\n", TAB);
	EmitLn("LIB TINYLIB");
}

//--------------------------------------------------------------
// Write the Prolog

void Prolog()
{
	PostLabel("MAIN");
}

//--------------------------------------------------------------
// Write the Epilog

void Epilog()
{
	EmitLn("DC WARMST");
	EmitLn("END MAIN");
}

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char *L)
{
	printf("%s:", L);
}

//--------------------------------------------------------------
// Clear the Primary Register

void Clear()
{
	EmitLn("CLR D0");
}

//--------------------------------------------------------------
// Negate the Primary Register

void Negate()
{
	EmitLn("NEG D0");
}


//--------------------------------------------------------------
// Load a Constant Value to Primary Register

void LoadConst(int n)
{
	Emit("MOVE #");
	printf("%d,D0\n", n);
}

//--------------------------------------------------------------
// Load a Variable to Primary Regsiter

void LoadVar(char *Name)
{
	if (!InTable(Name)) {
		Undefined(Name);
	}
	snprintf(buf, MAX_BUF, "MOVE %s(PC),D0", Name);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Push Primary onto Stack

void Push()
{
	EmitLn("MOVE D0,-(SP)");
}

//--------------------------------------------------------------
// Add Top of Stack to Primary

void PopAdd()
{
	EmitLn("ADD (SP)+,D0");
}

//--------------------------------------------------------------
// Subtract Primary from Top of Stack

void PopSub()
{
	EmitLn("SUB (SP)+,D0");
	EmitLn("NEG D0");
}

//--------------------------------------------------------------
// Multiply Top of Stack by Primary

void PopMul()
{
	EmitLn("MULS (SP)+,D0");
}

//--------------------------------------------------------------
// Divide Top of Stack by Primary

void PopDiv()
{
	EmitLn("MOVE (SP)+,D7");
	EmitLn("EXT.L D7");
	EmitLn("DIVS D0,D7");
	EmitLn("MOVE D7,D0");
}

//--------------------------------------------------------------
// Store Primary to Variable

void Store(char *Name)
{
	if (!InTable(Name)) {
		Undefined(Name);
	}
	snprintf(buf, MAX_BUF, "LEA %s(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

//--------------------------------------------------------------
// Report an Undefined Idenifier

void Undefined(char *n)
{
	snprintf(buf, MAX_BUF, "Undefined Identifer %s", n);
	Abort(buf);
}

//--------------------------------------------------------------
// Parse an Translate a Math Factor

void Factor() {
	if (Look == '(') {
		Match("(");
		BoolExpression();
		Match(")");
	} else if (isalpha(Look)) {
		GetName();
		LoadVar(Value);
	} else {
		LoadConst(GetNum());
	}
}

//--------------------------------------------------------------
// Parse and Translate a Negative Factor

void NegFactor()
{
	Match("-");
	if (isdigit(Look)) {
		LoadConst(-1 * GetNum());
	} else {
		Factor();
		Negate();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Leading Factor

void FirstFactor()
{
	switch (Look) {
		case '+':
			Match("+");
			Factor();
			break;
		case '-':
			NegFactor();
			break;
		default:
			Factor();
			break;
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply()
{
	Match("*");
	Factor();
	PopMul();
}

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide()
{
	Match("/");
	Factor();
	PopDiv();
}

//--------------------------------------------------------------
// Common Code Used by Term and FirstTerm

void Term1()
{
	NewLine();
	while (IsMulop(Look)) {
		Push();
		switch (Look) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
		}
		NewLine();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term()
{
	Factor();
	Term1();
}

//--------------------------------------------------------------
// Parse and Translate a Leading Term

void FirstTerm()
{
	FirstFactor();
	Term1();
}

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add()
{
	Match("+");
	Term();
	PopAdd();
}

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract()
{
	Match("-");
	Term();
	PopSub();
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression()
{
	NewLine();
	FirstTerm();
	while (IsAddop(Look)) {
		switch (Look) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
		}
		NewLine();
	}
}

//--------------------------------------------------------------
// Recognize a Boolean Orop

int IsOrOp(char c)
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
// Complement the Primary Register

void NotIt()
{
	EmitLn("NOT D0");
}

//--------------------------------------------------------------
// AND Top of Stack with Primary

void PopAnd()
{
	EmitLn("AND (SP)+,D0");
}

//--------------------------------------------------------------
// OR Top of Stack with Primary

void PopOr()
{
	EmitLn("OR (SP)+,D0");
}

//--------------------------------------------------------------
// XOR Top of Stack with Primary

void PopXor()
{
	EmitLn("EOR (SP)+,D0");
}

//--------------------------------------------------------------
// Compare Top of Stack with Primary

void PopCompare()
{
	EmitLn("CMP (SP)+,D0");
}

//--------------------------------------------------------------
// Set D0 If Compare was =

void SetEqual()
{
	EmitLn("SEQ D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Set D0 If Compare was !=

void SetNEqual()
{
	EmitLn("SNQ D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Set D0 If Compare was >

void SetGreater()
{
	EmitLn("SLQ D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Set D0 If Compare was <

void SetLess()
{
	EmitLn("SGQ D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Equals"

void Equals()
{
	Match("=");
	Expression();
	PopCompare();
	SetEqual();
}

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Not Equals"

void NotEqual()
{
	Match(">");
	Expression();
	PopCompare();
	SetNEqual();
}

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Less Than or Equal"

void LessOrEqual()
{
	Match("=");
	Expression();
	PopCompare();
	SetLessOrEqual();
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Less Than"

void Less()
{
	Match("<");
	switch (Look) {
		case '=':
			LessOrEqual();
			break;
		case '>':
			NotEqual();
			break;
		default:
			Expression();
			PopCompare();
			SetLess();
			break;
	}
}
		
//--------------------------------------------------------------
// Recognize and Translate a Relational "Greater Than"

void Greater()
{
	Match(">");
	if (Look == '=') {
		Match("=");
		Expression();
		PopCompare();
		SetGreaterOrEqual();
	} else {
		Expression();
		PopCompare();
		SetGreater();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation()
{
	Expression();
	if (IsRelop(Look)) {
		Push();
		switch (Look) {
			case '=':
				Equals();
				break;
			case '#':
				NotEqual();
				break;
			case '<':
				Less();
				break;
			case '>':
				Greater();
				break;
		}
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor with Leading NOT

void NotFactor()
{
	if (Look == '!') {
		Match("!");
		Relation();
		NotIt();
	} else {
		Relation();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Term

void BoolTerm()
{
	NewLine();
	NotFactor();
	while (Look == '&') {
		Push();
		Match("&");
		NotFactor();
		PopAnd();
		NewLine();
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr()
{
	Match("|");
	BoolTerm();
	PopOr();
}

//--------------------------------------------------------------
// Recognize and Translate an Exclusive OR

void BoolXor()
{
	Match("~");
	BoolTerm();
	PopXor();
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression()
{
	NewLine();
	BoolTerm();
	while (IsOrOp(Look)) {
		Push();
		switch (Look) {
			case '|':
				BoolOr();
				break;
			case '~':
				BoolXor();
				break;
		}
		NewLine();
	}
}

//--------------------------------------------------------------
// Branch Unconditional

void Branch(char *L)
{
	snprintf(buf, MAX_BUF, "BRA %s", L);
	EmitLn(buf);
}

//--------------------------------------------------------------
// Branch False

void BranchFalse(char *L)
{
	EmitLn("TST D0");
	snprintf(buf, MAX_BUF, "BEQ %s", L);
	EmitLn(buf);
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
// Recognize and Translate an IF Construct

void DoIf() 
{
	char *L1;
	char *L2;
	BoolExpression();
	L1 = NewLabel();
	L2 = L1;
	BranchFalse(L1);
	Block();
	if (Token == 'l') {
		L2 = NewLabel();
		Branch(L2);
		PostLabel(L1);
		Block();
	}
	PostLabel(L2);
	MatchString("ENDIF");
}

//--------------------------------------------------------------
// Parse and Translate a WHILE Statement

void DoWhile()
{
	char *L1;
	char *L2;
	L1 = NewLabel();
	L2 = NewLabel();
	PostLabel(L1);
	BoolExpression();
	BranchFalse(L2);
	Block();
	MatchString("ENDWHILE");
	Branch(L1);
	PostLabel(L2);
}

//--------------------------------------------------------------
// Table Lookup

int Lookup(TabPtr T, char *s, int n)
{
	int i, found;
	found = 0;
	i = n;
	while ((i > 0) && !found) {
		if (strcmp(s, (*T)[i - 1]) == 0) {
			found = 1;
		} else {
			i--;
		}
	}
	return i;
}
		
//--------------------------------------------------------------
// Get an Identifier and Scan it for Keywords

void Scan()
{
	GetName();
	Token = KWcode[Lookup(&KWlist, Value, NKW)];
}

//--------------------------------------------------------------
// Match a Specific Input String

void MatchString(char *x)
{
	if (strncmp(Value, x, strlen(x)) != 0) {
		snprintf(buf, MAX_BUF, "\"%s\"", x);
		Expected(buf);
	}
}

//--------------------------------------------------------------
// Set D0 If Compare was <=

void SetLessOrEqual()
{
	EmitLn("SGE D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Set D0 If Compare was >=

void SetGreaterOrEqual()
{
	EmitLn("SLE D0");
	EmitLn("EXT D0");
}

//--------------------------------------------------------------
// Read Variable to Primary Register

void ReadVar()
{
	EmitLn("BSR READ");
	Store(Value);
}

//--------------------------------------------------------------
// Write Variable to Primary Register

void WriteVar()
{
	EmitLn("BSR WRITE");
}

//--------------------------------------------------------------
// Process a Read Statement

void DoRead()
{
	Match("(");
	GetName();
	ReadVar();
	while (Look == ',') {
		Match(",");
		GetName();
		ReadVar();
	}
	Match(")");
}

//--------------------------------------------------------------
// Process a Write Statement

void DoWrite()
{
	Match("(");
	Expression();
	WriteVar();
	while (Look == ',') {
		Match(",");
		Expression();
		WriteVar();
	}
	Match(")");
}


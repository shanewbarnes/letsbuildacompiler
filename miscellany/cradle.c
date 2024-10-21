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
char TempChar = ' ';
char buf[MAX_BUF];
char Token;			// Encoded Token
char ValueArray[16];		// Unencoded Token
char *Value = ValueArray;
Symbol ST[MaxEntry];
char SType[MaxEntry];
int LCount;
int NEntry; 

//--------------------------------------------------------------
// Definition of Keywords and Token Types

Symbol KWlist[MaxEntry] = {"IF", "ELSE", "ENDIF", "WHILE", "ENDWHILE", "READ", "WRITE", "VAR", "END"}; 
char KWcode[NKW1] = "xileweRWve";

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
	SkipWhite();
	if (!isalpha(Look)) {
		Expected("Identifier");
	}
	Token = 'x';
	c = Value;
	do {
		*c++ = toupper(Look);
		GetChar();
	} while (isalnum(Look));
	*c = '\0';
}

//--------------------------------------------------------------
// Get a Number

void GetNum()
{
	char *c;
	SkipWhite();
	if (!isdigit(Look)) {
		Expected("Number");
	}
	Token = '#';
	c = Value;
	do {
		*c++ = Look;
		GetChar();
	} while (isdigit(Look));
	*c = '\0';
}

//--------------------------------------------------------------
// Get an Operator

void GetOp()
{
	SkipWhite();
	Token = Look;
	Value[0] = Look;
	Value[1] = '\0';
	GetChar();
}
		 
//--------------------------------------------------------------
// Get the Next Input Token

void Next()
{
	SkipWhite();
	if (isalpha(Look)) {
		GetName();
	} else if (isdigit(Look)) {
		GetNum();
	} else {
		GetOp();
	}
}

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c)
{
	return c == ' ' || c == TAB || c == '\n' || c == '{';
}

//--------------------------------------------------------------
// Skip Over Leading White Space

void SkipWhite()
{
	while (IsWhite(Look)) {
		if (Look == '{') {
			SkipComment();
		} else {
			GetChar();
		}
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
	LCount = 0;
	NEntry = 0;
	GetChar();
	Next();
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
// Parse and Translate an Assignment Statement

void Assignment()
{
	char Name[16];
	CheckTable(Value);
	snprintf(Name, 16, "%s", Value);
	Next();
	MatchString("=");
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
			case 'x':
				Assignment();
				break;
		}
		Semi();
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

void Alloc()
{
	Next();
	if (Token != 'x') {
		Expected("Variable Name");
	}
	CheckDup(Value);	
	AddEntry(Value, 'v');
	Allocate(Value, "0");
	Next();
}

//--------------------------------------------------------------
// Look for Symbol in Table

int InTable(Symbol N)
{
	return Lookup(&ST, N, NEntry) != 0;
}

//--------------------------------------------------------------
// Add a New Entry to Symbol Table

void AddEntry(Symbol N, char T)
{
	int i;
	CheckDup(N);
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
	while (Token == 'v') {
		Alloc();
		while (Token == ',') {
			Alloc();
		}
		Semi();
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
// Allocate Storage for a Static Variable

void Allocate(char *Name, char *Val)
{
	printf("%s:%cDC %s\n", Name, TAB, Val);
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

void LoadConst(char *Const)
{
	Emit("MOVE #");
	printf("%s,D0\n", Const);
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
// Report an Duplicate Idenifier

void Duplicate(char *n)
{
	snprintf(buf, MAX_BUF, "Duplicate Identifer %s", n);
	Abort(buf);
}

//--------------------------------------------------------------
// Check to Make Sure the Current Token is an Identifier

void CheckIdent()
{
	if (Token != 'x') {
		Expected("Identifier");
	}
}

//--------------------------------------------------------------
// Parse an Translate a Math Factor

void Factor() {
	if (Token == '(') {
		Next();
		BoolExpression();
		MatchString(")");
	} else {
		if (Token == 'x') {
			LoadVar(Value);
		} else if (Token == '#') {
			LoadConst(Value);
		} else {
			Expected("Math Factor");
		}
		Next();
	} 
}

//--------------------------------------------------------------
// Parse and Translate a Negative Factor

void NegFactor()
{
	Match("-");
	if (isdigit(Look)) {
		;
		//LoadConst(-1 * GetNum());
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
	Next();
	Factor();
	PopMul();
}

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide()
{
	Next();
	Factor();
	PopDiv();
}

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term()
{
	Factor();
	while (IsMulop(Token)) {
		Push();
		switch (Token) {
			case '*':
				Multiply();
				break;
			case '/':
				Divide();
				break;
		}
	}
}

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add()
{
	Next();
	Term();
	PopAdd();
}

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract()
{
	Next();
	Term();
	PopSub();
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression()
{
	if (IsAddop(Token)) {
		Clear();
	} else {
		Term();
	}
	while (IsAddop(Token)) {
		Push();
		switch (Token) {
			case '+':
				Add();
				break;
			case '-':
				Subtract();
				break;
		}
	}
}

//--------------------------------------------------------------
// Get Another Expression and Compare

void CompareExpression()
{
	Expression();
	PopCompare();
}

//--------------------------------------------------------------
// Get the Next Expression and Compare

void NextExpression()
{
	Next();
	CompareExpression();
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

void Equal()
{
	NextExpression();
	SetEqual();
}

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Not Equals"

void NotEqual()
{
	NextExpression();
	SetNEqual();
}

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Less Than or Equal"

void LessOrEqual()
{
	NextExpression();
	SetLessOrEqual();
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Less Than"

void Less()
{
	Next();
	switch (Token) {
		case '=':
			LessOrEqual();
			break;
		case '>':
			NotEqual();
			break;
		default:
			CompareExpression();
			SetLess();
			break;
	}
}
		
//--------------------------------------------------------------
// Recognize and Translate a Relational "Greater Than"

void Greater()
{
	Next();
	if (Token == '=') {
		NextExpression();
		SetGreaterOrEqual();
	} else {
		CompareExpression();
		SetGreater();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation()
{
	Expression();
	if (IsRelop(Token)) {
		Push();
		switch (Token) {
			case '=':
				Equal();
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
	if (Token == '!') {
		Next();
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
	NotFactor();
	while (Token == '&') {
		Push();
		Next();
		NotFactor();
		PopAnd();
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr()
{
	Next();
	BoolTerm();
	PopOr();
}

//--------------------------------------------------------------
// Recognize and Translate an Exclusive OR

void BoolXor()
{
	Next();
	BoolTerm();
	PopXor();
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression()
{
	BoolTerm();
	while (IsOrOp(Token)) {
		Push();
		switch (Token) {
			case '|':
				BoolOr();
				break;
			case '~':
				BoolXor();
				break;
		}
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
	Next();
	BoolExpression();
	L1 = NewLabel();
	L2 = L1;
	BranchFalse(L1);
	Block();
	if (Token == 'l') {
		Next();
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
	Next();
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
// Locate a Symbol in Table
// Returns the index of the entry. Zero if not present.

int Locate(Symbol N)
{
	return Lookup(&ST, N, NEntry);
}

//--------------------------------------------------------------
// Check to See if an Identifier is in the Symbol Table
// Report an error if it's not.

void CheckTable(Symbol N)
{
	if (!InTable(N)) {
		Undefined(N);
	}
}

//--------------------------------------------------------------
// Check the Symbol Table for Duplicate Idenifier
// Report an error if identifier is already in table.

void CheckDup(Symbol N)
{
	if (InTable(N)) {
		Duplicate(N);
	}
}

//--------------------------------------------------------------
// Get an Identifier and Scan it for Keywords

void Scan()
{
	if (Token == 'x') {
		Token = KWcode[Lookup(&KWlist, Value, NKW)];
	}
}

//--------------------------------------------------------------
// Match a Specific Input String

void MatchString(char *x)
{
	if (strncmp(Value, x, strlen(x)) != 0) {
		snprintf(buf, MAX_BUF, "\"%s\"", x);
		Expected(buf);
	}
	Next();
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

void ReadIt()
{
	EmitLn("BSR READ");
	Store(Value);
}

//--------------------------------------------------------------
// Write Variable to Primary Register

void WriteIt()
{
	EmitLn("BSR WRITE");
}

//--------------------------------------------------------------
// Read a Single Variable

void ReadVar()
{
	CheckIdent();
	CheckTable(Value);
	ReadIt(Value);
	Next();
}

//--------------------------------------------------------------
// Process a Read Statement

void DoRead()
{
	Next();
	MatchString("(");
	ReadVar();
	while (Token == ',') {
		Next();
		ReadVar();
	}
	MatchString(")");
}

//--------------------------------------------------------------
// Process a Write Statement

void DoWrite()
{
	Next();
	MatchString("(");
	Expression();
	WriteIt();
	while (Token == ',') {
		Next();
		Expression();
		WriteIt();
	}
	MatchString(")");
}

//--------------------------------------------------------------
// Match a Semicolon

void Semi()
{
	if (Token == ';') {
		Next();
	}
}

//--------------------------------------------------------------
// Skip A Comment Field

void SkipComment()
{
	while (Look != '}') {
		GetChar();
		if (Look == '{') {
			SkipComment();
		}
	}
	GetChar();
}

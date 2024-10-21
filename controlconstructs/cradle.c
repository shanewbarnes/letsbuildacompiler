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
	return c == '+' || c == '-';
}

//--------------------------------------------------------------
// Recognize and Translate an "Other"

void Other() 
{
	snprintf(buf, MAX_BUF, "%c", GetName());
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
	BoolExpression();
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
	BoolExpression();
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
	BoolExpression();
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
// Recognize and Transalate a Statement Block

void Block(char *L) 
{
	while (Look != 'e' && Look != 'l' && Look != 'u') {
		Fin();
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
				Assignment();
				break;
		}
		Fin();
	}
}

//--------------------------------------------------------------
// Recognize a Boolean Literal

int IsBoolean(char c)
{
	return toupper(c) == 'T' || toupper(c) == 'F';
}

//--------------------------------------------------------------
// Get a Boolean Literal

int GetBoolean()
{
	int c;
	if (!IsBoolean(Look)) {
		Expected("Boolean Literal");
	}
	c = toupper(Look) == 'T';
	GetChar();
	return c;
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Term

void BoolTerm()
{
	NotFactor();
	while (Look == '&') {
		EmitLn("MOVE D0,-(SP)");
		Match("&");
		NotFactor();
		EmitLn("AND (SP)+,D0");
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr()
{
	Match("|");
	BoolTerm();
	EmitLn("OR (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate an Exclusive OR

void BoolXor()
{
	Match("~");
	BoolTerm();
	EmitLn("EOR (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize a Boolean OrOp

int IsOrOp(char c)
{
	return c == '|' || c == '~';
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression()
{
	BoolTerm();
	while (IsOrOp(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
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
// Parse and Translate a Boolean Factor with NOT

void NotFactor()
{
	if (Look == '!') {
		Match("!");
		BoolFactor();
		EmitLn("EOR #-1,D0");
	} else {
		BoolFactor();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor

void BoolFactor()
{
	if (IsBoolean(Look)) {
		if (GetBoolean()) {
			EmitLn("MOVE #-1,D0");
		} else {
			EmitLn("CLR D0");
		}
	} else {
		Relation();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation()
{
	Expression();
	if (IsRelop(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
			case '=':
				Equals();
				break;
			case '#':
				NotEquals();
				break;
			case '<':
				Less();
				break;
			case '>':
				Greater();
				break;
		}
		EmitLn("TST D0");
	}
}

//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char c)
{
	return c == '=' || c == '#' || c == '<' || c == '>';
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Equals"

void Equals()
{
	Match("=");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SEQ D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Not Equals"

void NotEquals()
{
	Match("#");
	Expression();
	EmitLn("CMP(SP)+,D0");
	EmitLn("SNE D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Relational "Less Than"

void Less()
{
	Match("<");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SGE D0");
}

//--------------------------------------------------------------
// Recognize and Transalte a Relational "Greater Than"

void Greater()
{
	Match(">");
	Expression();
	EmitLn("CMP (SP)+,D0");
	EmitLn("SLE D0");
}

//--------------------------------------------------------------
// Parse and Translate an Identifier

void Ident()
{
	char Name = GetName();
	if (Look == '(') {
		Match("(");
		Match(")");
		snprintf(buf, MAX_BUF, "BSR %c", Name);
		EmitLn(buf);
	} else {
		snprintf(buf, MAX_BUF, "MOVE %c(PC),D0", Name);
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor()
{
	if (Look == '(') {
		Match("(");
		Expression();
		Match(")");
	} else if (isalpha(Look)) {
		Ident();
	} else {
		snprintf(buf, MAX_BUF, "MOVE #%c,D0", GetNum());
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate the First Math Factor

void SignedFactor()
{
	if (Look == '+') {
		GetChar();
	}
	if (Look == '-') {
		GetChar();
		if (isdigit(Look)) {
			snprintf(buf, MAX_BUF, "MOVE #-%d,D0", GetNum());
			EmitLn(buf);
		} else {
			Factor();
			EmitLn("NEG D0");
		}
	} else {
		Factor();
	}
}

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply()
{
	Match("*");
	Factor();
	EmitLn("MULS (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide()
{
	Match("/");
	Factor();
	EmitLn("MOVE (SP)+,D1");
	EmitLn("EXS.L D0");
	EmitLn("DIVS D1,D0");
}

//--------------------------------------------------------------
// Parse ad Translate a Math Term

void Term()
{
	SignedFactor();
	while (Look == '*' || Look == '/') {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
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
	Match("+");
	Term();
	EmitLn("ADD (SP)+,D0");
}

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract()
{
	Match("-");
	Term();
	EmitLn("SUB (SP)+,D0");
	EmitLn("NEG D0");
}

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression() 
{
	Term();
	while (IsAddop(Look)) {
		EmitLn("MOVE D0,-(SP)");
		switch (Look) {
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
// Skip a CR

void Fin() 
{
	if (Look == CR) {
		GetChar();
	}
}

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment() 
{
	char Name = GetName();
	Match("=");
	BoolExpression();
	snprintf(buf, MAX_BUF, "LEA %c(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

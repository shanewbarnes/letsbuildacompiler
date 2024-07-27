#include "cradle.h"

//--------------------------------------------------------------
// program KISS

//--------------------------------------------------------------
// Constant Declarations

const char TAB = '\t';
const char CR = '\n';

//--------------------------------------------------------------
// Variable Definitions

char Look;		// Lookahead Character
char Token;		// Encoded Token
char ValueArray[16];	// Unencoded Token
char *Value = ValueArray;
int LCount;		// Label Counter
char buf[MAX_BUF];

//--------------------------------------------------------------
// Variable Definitions

const Symbol KWlist[4] = {"IF", "ELSE", "ENDIF", "END"};
const char KWcode[5] = "xilee";

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
	if (Look != *x) {
		snprintf(tmp, MAX_BUF, "\"%s\"", x); 
		Expected(tmp);
	}
	GetChar();
	SkipWhite();
}

//--------------------------------------------------------------
// Table Lookup

int Lookup(TabPtr T, char *s, int n)
{
	int i;
	int found;

	found = 0;
	i = n;
	while (i > 0 && !found) {
		if (strcmp(s, (*T)[i - 1]) == 0) {
			found = 1;
		} else {
			i--;
		}
	}
	return i;
}

//--------------------------------------------------------------
// Get an Identifier

void GetName()
{
	char *c = Value;
	while (Look == CR) {
		Fin();
	}
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

void GetNum()
{
	char *n = Value;
	if (!isdigit(Look)) {
		Expected("Integer");
	}
	while (isdigit(Look)) {
		*n++ = Look;
		GetChar();
	}
	*n = '\0';
	SkipWhite();
	Token = '#';
}

//--------------------------------------------------------------
// Get an Identifier and Scan it for Keywords

void Scan()
{
	GetName();
	Token = KWcode[Lookup(&KWlist, Value, 4)];
}

//--------------------------------------------------------------
// Match a Specific Input String

void MatchString(char *x)
{
	char tmp[MAX_BUF];
	if (strncmp(Value, x, 16) != 0) {
		snprintf(tmp, MAX_BUF, "\"%s\"", x); 
		Expected(tmp);
	}
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
// Recognize White Space

int IsWhite(char c) 
{
	return c == ' ' || c == TAB; 
}

//--------------------------------------------------------------
// Skip Over Leading White Space 

void SkipWhite() {
	while (IsWhite(Look)) {
		GetChar();
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
// Parse and Translate an Identifier

void Ident()
{
	GetName();
	if (Look == '(') {
		Match("(");
		Match(")");
		snprintf(buf, MAX_BUF, "BSR %s", Value);
	} else {
		snprintf(buf, MAX_BUF, "MOVE %s(PC),D0", Value);
	}
	EmitLn(buf);
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
		GetNum();
		snprintf(buf, MAX_BUF, "MOVE #%s,D0", Value);
		EmitLn(buf);
	}
}

//--------------------------------------------------------------
// Parse and Translate the First Math Factor

void SignedFactor() 
{
	int s = Look == '-';
	if (IsAddop(Look)) {
		GetChar();
		SkipWhite();
	}
	Factor();
	if (s) {
		EmitLn("NEG D0");
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
// Completion of Term Processing (called by Term and FirstTerm)

void Term1()
{
	while (IsMulop(Look)) {
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
// Parse and Translate a Math Term

void Term()
{
	Factor();
	Term1();
}

//--------------------------------------------------------------
// Parse and Translate a Math Term with Possible Leading Sign

void FirstTerm()
{
	SignedFactor();
	Term1();
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
	FirstTerm();
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
// Parse and Translate a Boolean Condition

void Condition()
{
	EmitLn("Condition");
}

//--------------------------------------------------------------
// Recognize and Translate an IF Construct

void DoIf()
{
	char *L1;
	char *L2;
	Condition();
	L1 = NewLabel();
	L2 = L1;
	snprintf(buf, MAX_BUF, "BEQ %s", L1);
	EmitLn(buf);
	Block();
	if (Token == 'l') {
		L2 = NewLabel();
		snprintf(buf, MAX_BUF, "BRA %s", L2);
		EmitLn(buf);
		PostLabel(L1);
		Block();
	}
	PostLabel(L2);
	MatchString("ENDIF");
}	

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment() 
{
	char Name[NAME_SIZE];
	snprintf(Name, NAME_SIZE, "%s", Value);
	Match("=");
	Expression();
	snprintf(buf, MAX_BUF, "LEA %s(PC),A0", Name);
	EmitLn(buf);
	EmitLn("MOVE D0,(A0)");
}

//--------------------------------------------------------------
// Initialize

void Init()
{
	LCount = 0;
	GetChar();
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
// Skip a CR

void Fin()
{
	if (Look == CR) {
		GetChar();
	}
	SkipWhite();
}

//--------------------------------------------------------------
// Recognize and Translate a Statement Block

void Block()
{
	Scan();
	while (!(Token == 'e' || Token == 'l')) {
		switch (Token) {
			case 'i':
				DoIf();
				break;
			default:
				Assignment();
				break;
		}
		Scan();
	}
}

//--------------------------------------------------------------
// Parse and Translate a Program

void DoProgram()
{
	Block();
	MatchString("END");
	EmitLn("END");
}

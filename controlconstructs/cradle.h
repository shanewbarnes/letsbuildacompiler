#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100
#define LCOUNT_SIZE 2

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;
extern char buf[MAX_BUF];
extern int LCount;

//--------------------------------------------------------------
// Read New Character From Input Stream

void GetChar();

//--------------------------------------------------------------
// Report an Error

void Error(char *);

//--------------------------------------------------------------
// Report Error and Halt

void Abort(char *);

//--------------------------------------------------------------
// Report What Was Expected

void Expected(char *);

//--------------------------------------------------------------
// Match a Specific Input Character

void Match(char *); 

//--------------------------------------------------------------
// Get an Identifier

char GetName();

//--------------------------------------------------------------
// Get a Number

char GetNum();

//--------------------------------------------------------------
// Output a String with Tab

void Emit(char *);

//--------------------------------------------------------------
// Output a String with TAB and CRLF

void EmitLn(char *);

//--------------------------------------------------------------
// Initialize

void Init();

//--------------------------------------------------------------
// Recognize an Addop

int Addop(char);

//--------------------------------------------------------------
// Recognize and Translate an "Other"

void Other();

//--------------------------------------------------------------
// Parse and Translate a Program

void DoProgram();

//--------------------------------------------------------------
// Recognize and Translate a Statement Block

void Block();

//--------------------------------------------------------------
// Generate a Unique Label

char *NewLabel();

//--------------------------------------------------------------
// Post a Label to Output

void PostLabel();

//--------------------------------------------------------------
// Recognize and Translate an IF Construct

void DoIf();

//--------------------------------------------------------------
// Recognize and Translate an WHILE Construct

void DoWhile();

//--------------------------------------------------------------
// Recognize and Translate an LOOP Construct

void DoLoop();

//--------------------------------------------------------------
// Recognize and Translate an REPEAT Construct

void DoRepeat();

//--------------------------------------------------------------
// Recognize and Translate an FOR Construct

void DoFor();

//--------------------------------------------------------------
// Recognize and Translate an DO Construct

void DoDo();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Recognize a Boolean Literal

int IsBoolean(char);

//--------------------------------------------------------------
// Get a Boolean Literal

int GetBoolean();

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression();

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr();

//--------------------------------------------------------------
// Recognize and Translate an Exclusive Or

void BoolXor();

//--------------------------------------------------------------
// Recognize a Boolean OrOp

int IsOrop(char c);

//--------------------------------------------------------------
// Parse and Translate a Boolean Term

void BoolTerm();

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor with NOT

void NotFactor();

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor with NOT

void BoolFactor();

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation();

//--------------------------------------------------------------
// Recognize and Translate a Realtional "Equals"

void Equals();

//--------------------------------------------------------------
// Recognize and Translate a Realtional "Not Equals"

void NotEquals();

//--------------------------------------------------------------
// Recognize and Translate a Realtional "Less Than"

void Less();

//--------------------------------------------------------------
// Recognize and Translate a Realtional "Greater Than"

void Greater();

//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char);

//--------------------------------------------------------------
// Parse and Translate an Identifier

void Ident();

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor();

//--------------------------------------------------------------
// Parse and Translate the First Math Factor

void SignedFactor();

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply();

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide();

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term();

//--------------------------------------------------------------
// Recognzie and Translate an Add

void Add();

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Skip a CR

void Fin();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statment

void Assignment();

#endif

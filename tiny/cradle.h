#ifndef CRADLE_H
#define CRADLE_H

#define MAX_BUF 100
#define LCOUNT_SIZE 2
#define NKW 9
#define NKW1 10
#define MaxEntry 100

//--------------------------------------------------------------
// program Cradle

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Type Declarations

typedef char Symbol[8];
typedef Symbol SymTab[MaxEntry];
typedef const SymTab *TabPtr;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;		// Lookahead Character
extern char Token;		// Encoded Token
extern char ValueArray[16];	// Unencoded Token
extern char *Value;
extern char buf[MAX_BUF];

extern Symbol ST[MaxEntry];
extern char SType[MaxEntry];
extern int LCount;
extern int NEntry;

//--------------------------------------------------------------
// Definition of Keywords and Token Types

extern Symbol KWlist[MaxEntry];
extern char KWcode[NKW1];

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

void GetName();

//--------------------------------------------------------------
// Get a Number

int GetNum();

//--------------------------------------------------------------
// Recognize White Space

int IsWhite(char c);

//--------------------------------------------------------------
// Skip Over Leading White Space

void SkipWhite();

//--------------------------------------------------------------
// Skip Over an End-of-Line

void NewLine();

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
// Parse and Translate a Program

void Prog();

//--------------------------------------------------------------
// Look for Symbol in Table

int InTable(char *);

//--------------------------------------------------------------
// Parse and Translate a Main Program

void Main();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

//--------------------------------------------------------------
// Parse and Translate a Block of Statments

void Block();

//--------------------------------------------------------------
// Process a Data Declaration

void Decl();

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(char *);

//--------------------------------------------------------------
// Add a New Entry to Symbol Table

void AddEntry(Symbol, char);

//--------------------------------------------------------------
// Parse and Translate Global Declaration

void TopDecls();

//--------------------------------------------------------------
// Write Header Info

void Header();

//--------------------------------------------------------------
// Write the Prolog

void Prolog();

//--------------------------------------------------------------
// Write the Epilog

void Epilog();

//--------------------------------------------------------------
// Post a Label To Output

void PostLabel(char *L);

//--------------------------------------------------------------
// Clear the Primary Register

void Clear();

//--------------------------------------------------------------
// Negate the Primary Register

void Negate();

//--------------------------------------------------------------
// Load a Constant Value to Primary Register

void LoadConst(int);

//--------------------------------------------------------------
// Load a Variable to Primary Register

void LoadVar(char *);

//--------------------------------------------------------------
// Push Primary onto Stack

void Push();

//--------------------------------------------------------------
// Add Top of Stack to Primary

void PopAdd();

//--------------------------------------------------------------
// Subtract Primary from Top of Stack

void PopSub();

//--------------------------------------------------------------
// Multiply Top of Stack Primary

void PopMul();

//--------------------------------------------------------------
// Divide Top of Stack by Primary

void PopDiv();

//--------------------------------------------------------------
// Store Primary to Variable

void Store(char *);

//--------------------------------------------------------------
// Report an Undefined Identifer

void Undefined(char *);

//--------------------------------------------------------------
// Parse and Translate a Math Factor

void Factor();

//--------------------------------------------------------------
// Parse and Translate a Negative Factor

void NegFactor();

//--------------------------------------------------------------
// Parse and Translate a Leading Factor

void FirstFactor();

//--------------------------------------------------------------
// Recognize and Translate a Multiply

void Multiply();

//--------------------------------------------------------------
// Recognize and Translate a Divide

void Divide();

//--------------------------------------------------------------
// Common Code Used by Term and FirstTerm

void Term1();

//--------------------------------------------------------------
// Parse and Translate a Math Term

void Term();

//--------------------------------------------------------------
// Parse and Translate a Leading Term

void FirstTerm();

//--------------------------------------------------------------
// Recognize and Translate an Add

void Add();

//--------------------------------------------------------------
// Recognize and Translate a Subtract

void Subtract();

//--------------------------------------------------------------
// Parse and Translate an Expression

void Expression();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

//--------------------------------------------------------------
// Recognize a Boolean Orop

int IsOrOp(char c);
 
//--------------------------------------------------------------
// Recognize a Relop

int IsRelop(char c);

//--------------------------------------------------------------
// Complement the Primary Register

void NotIt();

//--------------------------------------------------------------
// AND Top of Stack with Primary

void PopAnd();

//--------------------------------------------------------------
// OR Top of Stack with Primary

void PopOr();

//--------------------------------------------------------------
// XOR Top of Stack with Primary

void PopXor();

//--------------------------------------------------------------
// Compare Top of Stack with Primary

void PopCompare();

//--------------------------------------------------------------
// Set D0 If Compare was =

void SetEqual();

//--------------------------------------------------------------
// Set D0 If Compare was !=

void SetNEqual();

//--------------------------------------------------------------
// Set D0 If Compare was >

void SetGreater();

//--------------------------------------------------------------
// Set D0 If Compare was <

void SetLess();

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Equals"

void Equals();

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Not Equals"

void NotEquals();

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Less Than"

void Less();

//--------------------------------------------------------------
// Recognzie and Translate a Relational "Greater Than"

void Greater();

//--------------------------------------------------------------
// Parse and Translate a Relation

void Relation();

//--------------------------------------------------------------
// Parse and Translate a Boolean Factor with Leading NOT

void NotFactor();

//--------------------------------------------------------------
// Parse and Translate a Boolean Term

void BoolTerm();

//--------------------------------------------------------------
// Recognize and Translate a Boolean OR

void BoolOr();

//--------------------------------------------------------------
// Recognize and Translate an Exclusive OR

void BoolXor();

//--------------------------------------------------------------
// Parse and Translate a Boolean Expression

void BoolExpression();

//--------------------------------------------------------------
// Branch Unconditional

void Branch(char *L);

//--------------------------------------------------------------
// Branch False

void BranchFalse(char *L);

//--------------------------------------------------------------
// Recognize and Translate an IF Construct

void DoIf();

//--------------------------------------------------------------
// Parse and Translate a WHILE Statement

void DoWhile();

//--------------------------------------------------------------
// Get an Identifier and Scan if for Keywords

void Scan();
	
//--------------------------------------------------------------
// Match a Specific Input String

void MatchString(char *);

//--------------------------------------------------------------
// Table Lookup

int Lookup(TabPtr, char *, int);

#endif


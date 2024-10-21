#ifndef TYPES_H
#define TYPES_H

#define MAX_BUF 100
#define ALPHA_SIZE 26

//--------------------------------------------------------------
// program Types

//--------------------------------------------------------------
// Constant Declarations

extern const char TAB;
extern const char CR;

//--------------------------------------------------------------
// Variable Declarations

extern char Look;
extern char ST[ALPHA_SIZE];
extern char buf[MAX_BUF];

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

long GetNum();

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
// Recognize an Mulop    
    
int IsMulop(char c);    
    
//--------------------------------------------------------------    
// Recognize a Boolean Orop    
    
int IsOrop(char c);    
    
//--------------------------------------------------------------    
// Recognize a Relop    
    
int IsRelop(char c);
    
//--------------------------------------------------------------    
// Recognize White Space    
    
int IsWhite(char c);
    
//--------------------------------------------------------------    
// Skip Over Leading White Space    

void SkipWhite();

//--------------------------------------------------------------
// Skip Over an End-of-Line

void Fin();

//--------------------------------------------------------------
// Dump the Symbol Table

void DumpTable();

//--------------------------------------------------------------
// Report Type of a Variable

char TypeOf(char);

//--------------------------------------------------------------
// Report if a Variable is in the Table

int InTable(char);

//--------------------------------------------------------------
// Check for a Duplicate Variable Name

void CheckDup(char);

//--------------------------------------------------------------
// Add Entry to Table

void AddEntry(char, char);

//--------------------------------------------------------------
// Allocate Storage for a Variable

void Alloc(char, char);

//--------------------------------------------------------------
// Parse and Translate a Data Declaration

void Decl();

//--------------------------------------------------------------
// Parse and Translate Global Declarations

void TopDecls();

//--------------------------------------------------------------
// Load a Variable to Primary Register

void LoadVar(char, char);

//--------------------------------------------------------------
// Generate a Move Instruction

void Move(char, char *, char *);

//--------------------------------------------------------------
// Recognize a Legal Variable Type

int IsVarType(char);

//--------------------------------------------------------------
// Get a Variable Type from the Symbol Table

char VarType(char);

//--------------------------------------------------------------
// Load a Variable to the Primary Register

char Load(char);

//--------------------------------------------------------------
// Store Primary to Variable

void StoreVar(char, char);

//--------------------------------------------------------------
// Store a Variable from the Primary Register

void Store(char, char);

//--------------------------------------------------------------
// Parse and Translate a Math Term

char Term();

//--------------------------------------------------------------
// Parse and Translate an Expression

char Expression();

//--------------------------------------------------------------
// Parse and Translate an Assignment Statement

void Assignment();

//--------------------------------------------------------------
// Parse and Translate a Block of Statements

void Block();

//--------------------------------------------------------------
// Convert a Data Item from One Type to Another

void Convert(char, char, char *);

//--------------------------------------------------------------
// Load a Constant to the Primary Register

char LoadNum(long);

//--------------------------------------------------------------
// Load a Constant to the Primary Register

void LoadConst(long, char);

//--------------------------------------------------------------
// Process a Term with leading Unary Operator

char Unop();

//--------------------------------------------------------------
// Push Primary onto Stack

void Push(char);

//--------------------------------------------------------------
// Recognize and Translate an Add

char Add(char);

//--------------------------------------------------------------
// Recognize and Translate a Subtract

char Subtract(char);

//--------------------------------------------------------------
// Pop Stack into Secondary Register

void Pop(char);

//--------------------------------------------------------------
// Promote the Size of a Register Value

char Promote(char, char, char *);

//--------------------------------------------------------------
// Force both Arguments to Same Type

char SameType(char, char);

//--------------------------------------------------------------
// Generate Code to Add Primary to the Stack

char PopAdd(char, char);

//--------------------------------------------------------------
// Generate Code to Subtract Primary from the Stack

char PopSub(char, char);

//--------------------------------------------------------------
// Add Top of Stack to Primary

void GenAdd(char);

//--------------------------------------------------------------
// Subtract Primary from Top of Stack

void GenSub(char);

//--------------------------------------------------------------
// Clear the Primary Register

void Clear();

//--------------------------------------------------------------
// Parse and Translate a Factor

char Factor();

//--------------------------------------------------------------
// Recognize and Translate a Multiply

char Multiply(char);

//--------------------------------------------------------------
// Recognize and Translate a Divide

char Divide(char);

//--------------------------------------------------------------
// Multiply Top of Stack by Primary (Word)

void GenMult();

//--------------------------------------------------------------
// Multiply Top of Stack by Primary (Long)

void GenLongMult();

//--------------------------------------------------------------
// Generate Code to Multiply Primary by Stack

char PopMul(char, char);

//--------------------------------------------------------------
// Generate Code to Divide Stack by the Primary

char PopDiv(char, char);

//--------------------------------------------------------------
// Divide Top of Stack by Primary (Word)

void GenDiv();

//--------------------------------------------------------------
// Divide Top of Stack by Primary (Long)

void GenLongDiv();

#endif

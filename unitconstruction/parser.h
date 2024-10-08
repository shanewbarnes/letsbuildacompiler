#ifndef PARSER_H 
#define PARSER_H

//--------------------------------------------------------------
// unit Parser

//--------------------------------------------------------------
// interface

//--------------------------------------------------------------
// Parse and Translate a Factor

void Factor();

//--------------------------------------------------------------
// Parse and Translate a Term with Optional Sign

void SignedTerm();

//--------------------------------------------------------------
// Parse and Translate a Expression

void Expression();

//--------------------------------------------------------------
// Parse and Translate an Addition Operation

void Add();

//--------------------------------------------------------------
// Parse and Translate an Subtraction Operation

void Subtract();

//--------------------------------------------------------------
// Parse and Translate a Term 

void Term();

//--------------------------------------------------------------
// Parse and Translate a Multiplication Operation

void Multiply();

//--------------------------------------------------------------
// Parse and Translate a Divide Operation

void Divide();

//--------------------------------------------------------------
// Parse and Translate an Assignment Operation

void Assignment();

//--------------------------------------------------------------
// Parse and Translate an Or Operation

void _Or();

//--------------------------------------------------------------
// Parse and Translate an Xor Operation

void _Xor();

//--------------------------------------------------------------
// Parse and Translate a Boolean And Operation

void _And();

//--------------------------------------------------------------
// Parse and Translate a Factor with Optional "Not"

void NotFactor();

#endif

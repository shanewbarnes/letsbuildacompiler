#ifndef ERRORS_H 
#define ERRORS_H

#define MAX_BUF 100

//--------------------------------------------------------------
// unit Errors

//--------------------------------------------------------------
// Write error Message and Halt

void Error(char *s);

//--------------------------------------------------------------
// Write "<something> Expected"

void Expected(char *s);

#endif

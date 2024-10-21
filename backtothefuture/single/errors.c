#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "errors.h"

//--------------------------------------------------------------
// unit Errors

//--------------------------------------------------------------
// Write error Message and Halt

void Error(char *s)
{
	printf("\n");
        printf("Error: %s.\n", s);
	exit(-1);
}

//--------------------------------------------------------------
// Write "<something> Expected"

void Expected(char *s)
{
	char tmp[MAX_BUF];
	snprintf(tmp, MAX_BUF, "%s Expected", s);
	Error(tmp);
}

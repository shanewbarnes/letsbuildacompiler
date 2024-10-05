#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "output.h"

//--------------------------------------------------------------
// unit Output

const char TAB = '\t';

//--------------------------------------------------------------
// Emit an Instruction

void Emit(char *s)
{
        printf("%c%s", TAB, s);
}

//--------------------------------------------------------------
// Emit an Instruction, Followed By a Newline

void EmitLn(char *s)
{
        Emit(s);
        printf("\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "scanner.h"
#include "error.h"
#include "codegen.h"
#include "parser.h"

//--------------------------------------------------------------
// unit Parser

//--------------------------------------------------------------
// Parse and Translate a Factor

void Factor()
{
	if (isdigit(Look)) {
		LoadConstant(GetNumber());
	} else if (isalpha(Look)) {
		LoadVariable(GetName());
	} else {
		snprintf(buf, MAX_BUF, "Unrecognized character %c", Look);
		Error(buf);
	}
}

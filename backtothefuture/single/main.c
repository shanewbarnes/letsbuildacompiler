#include <stdio.h>

#include "input.h"
#include "output.h"
#include "errors.h"
#include "scanner1.h"

int main() 
{
	Init();
	printf("%c", GetName());
	Match("=");
	printf("%c", GetNumber());
	Match("+");
	printf("%c\n", GetName());
}


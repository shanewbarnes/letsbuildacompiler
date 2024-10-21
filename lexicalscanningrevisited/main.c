#include "cradle.h"

int main() 
{
	Init();
	MatchString("PROGRAM");
	Header();
	TopDecls();
	MatchString("BEGIN");
	Prolog();
	Block();
	MatchString("END");
	Epilog();
}

#include "cradle.h"

int main() 
{
	Init();
	MatchString("PROGRAM");
	Semi();
	Header();
	TopDecls();
	MatchString("BEGIN");
	Prolog();
	Block();
	MatchString("END");
	Epilog();
}

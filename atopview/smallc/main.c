#include "cradle.h"

int main() 
{
	Init();
	while (Look != '\n') {
		GetClass();
		GetType();
		TopDecl();
	}
}

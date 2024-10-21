#include "cradle.h"

int main() 
{
	Init();
	Prog();
	if (Look != CR) {
		Abort("Unexpected data after \".\"");
	}
}

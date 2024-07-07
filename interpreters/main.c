#include <stdio.h>

#include "cradle.h"

int main() 
{
	Init();
	while (Look != '.') {
		switch (Look) {
			case '?':
				Input();
				break;
			case '!':
				Output();
				break;
			default:
				Assignment();
				break;
		}
		NewLine();
	}
}

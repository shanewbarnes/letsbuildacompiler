#include "cradle.h"

int main() 
{
	Init();
	do {
		Scan();
		switch (Token) {
			case 'x':
				printf("Ident ");
				break;
			case '#':
				printf("Number ");
				break;
			case 'i':
			case 'l':
			case 'e':
				printf("Keyword ");
				break;
			default:
				printf("Operator ");
				break;
		}
		printf(Value);
		printf("\n");
	} while (strcmp(Value, "END"));
}

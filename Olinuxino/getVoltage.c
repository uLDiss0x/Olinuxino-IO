#include "stdio.h"
#include "stdbool.h"

int main()
{
	short int n = 1;
	bool turnAllRelays = false;

	for(n = 1; n < 5; n++)
	{
		char* inputValue = "./mod-io -an1 0x58";
		printf("The voltage of input %d is %s", n, inputValue);
	}

	if (turnAllRelays == true) 
	{
		printf("./mod-io -relay 0x58 0x0F");
	}

	return 0;
}
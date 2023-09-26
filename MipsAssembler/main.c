/*
 * main.c
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */

#include "InstructionTable.h"
#include "Parse.h"
#include <stdio.h>

int main()
{
	Parse_ASSM("Asm.txt","Machine.txt");
	PrintSymbolTable();
	getchar();
	getchar();
	return 0;
}

/*
 * main.c
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */

#include "InstructionTable.h"
#include "Parse.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	char Str1[100] = {'\0'};
	char Str2[100] = {'\0'};
	printf("Enter Path of File 1 SRC:");
	scanf ("%s",Str1);
	printf("Enter Path of File 2 DEST:");
	scanf ("%s",Str2);
	Parse_ASSM(Str1 ,Str2);
	printf("=============================================\n");
	printf("=          Symbol Table                     =\n");
	printf("=============================================\n\n");
	PrintSymbolTable();
	printf("\n\n");
	printf("=============================================\n");
	printf("=             Output File                   =\n");
	printf("=============================================\n\n");
	FILE * Read_Output = fopen(Str2,"r");
	int c = fgetc(Read_Output);
	while (c != EOF) {
		printf("%c", c);
		c = fgetc(Read_Output);
	}
	getchar();
	getchar();
	return 0;
}

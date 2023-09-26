/*
 * Parse.h
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */

#ifndef PARSE_H_
#define PARSE_H_

#include "STD_TYPES.h"

#include "InstructionTable.h"

#define DATA_SECTION	0
#define TEXT_SECTION	1

typedef char SYMBOL;

typedef struct SymbolTable
{
	SYMBOL * Symbol;
	int32_t  Address;
	struct SymbolTable *Next;
	struct SymbolTable *prev;
}SymbolTable;

typedef struct
{
	SymbolTable *Head , *Tail;
	int No_Symbols ;
}SymbolDLL;

#define DATA_SECTION_START_ADDRESS 0x01000000
#define TEXT_SECTION_START_ADDRESS 0x00400000

bool Parse_ASSM(char * File_Name, char *File_Output);

void PrintSymbolTable();

#endif /* PARSE_H_ */

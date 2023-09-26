/*
 * InstructionTable.c
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */

#include "InstructionTable.h"

REGS MIPSRegisterSET[] = {
		"$0", "$at", "$v0", "$v1", "$a0", "$a1",
		"$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6",
		"$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
		"$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
};

DIRECTIVES *SupportedDirectives[] =
{
		".data",
		".text",//same for .code
		".code",//same fot .text
		".globl",
		".align",
		".byte",
		".word" ,
};

//Supported Instructions
INSTR * SupportedInstructions[] = {
		"bltz",
		"bgez",
		"j",
		"jal",
		"beq",
		"bne",
		"blez",
		"bgtz",
		"addi",
		"addiu",
		"slti",
		"sltiu",
		"andi",
		"ori",
		"xori",
		"lui",
		"mfc0",
		"mtc0",
		"bc1f",
		"bc1t",
		"mul",
		"lb",
		"lh",
		"lw",
		"lbu",
		"lhu",
		"sb",
		"sh",
		"sw",
		"lwc1",
		"swc1",
		"sll",
		"srl",
		"sra",
		"sllv",
		"srlv",
		"srav",
		"jr",
		"jalr",
		"syscall",
		"break",
		"mfhi",
		"mthi",
		"mflo",
		"mtlo",
		"mult",
		"multu",
		"div",
		"divu",
		"add",
		"addu",
		"sub",
		"subu",
		"and",
		"or",
		"xor",
		"nor",
		"slt",
		"sltu",
};

const uint8_t RegVal[] =
{
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	17,
	18,
	19,
	20,
	21,
	22,
	23,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
};
//opcode ,No of operands ,Function
const INSTR INSTR_ARRAY[NO_INSTRUCTIONS][4] =
{
   {0b000001 ,2,I_TYPE,0}, // bltz rs, label
   {0b000001 ,2,I_TYPE,0}, // bgez rs, label
   {0b000010 ,1,J_TYPE,0}, // j label
   {0b000011 ,1,J_TYPE,0}, // jal label
   {0b000100 ,3,I_TYPE,0}, // beq rs, rt, label
   {0b000101 ,3,I_TYPE,0}, // bne rs, rt, label
   {0b000110 ,2,I_TYPE,0}, // blez rs, label
   {0b000111 ,2,I_TYPE,0}, // bgtz rs, label
   {0b001000 ,3,I_TYPE,0}, // addi rt, rs, imm
   {0b001001 ,3,I_TYPE,0}, // addiu rt, rs, imm
   {0b001010 ,3,I_TYPE,0}, // slti rt, rs, imm
   {0b001011 ,3,I_TYPE,0}, // sltiu rt, rs, imm
   {0b001100 ,3,I_TYPE,0}, // andi rt, rs, imm
   {0b001101 ,3,I_TYPE,0}, // ori rt, rs, imm
   {0b001110 ,3,I_TYPE,0}, // xori rt, rs, imm
   {0b001111 ,2,I_TYPE,0}, // lui rt, imm
   {0b010000 ,2,I_TYPE,0}, // mfc0 rt, rd /
   {0b010000 ,2,I_TYPE,0}, // mtc0 rt, rd
   {0b010001 ,2,I_TYPE,0}, // bc1f label
   {0b010001 ,2,I_TYPE,0}, // bc1t label
   {0b011100 ,3,R_TYPE,2}, // mul rd, rs, rt
   {0b100000 ,2,I_TYPE,0}, // lb rt, imm(rs)
   {0b100001 ,2,I_TYPE,0}, // lh rt, imm(rs)
   {0b100011 ,2,I_TYPE,0}, // lw rt, imm(rs)
   {0b100100 ,2,I_TYPE,0}, // lbu rt, imm(rs)
   {0b100101 ,2,I_TYPE,0}, // lhu rt, imm(rs)
   {0b101000 ,2,I_TYPE,0}, // sb rt, imm(rs)
   {0b101001 ,2,I_TYPE,0}, // sh rt, imm(rs)
   {0b101011 ,2,I_TYPE,0}, // sw rt, imm(rs)
   {0b110001 ,2,I_TYPE,0}, // lwc1 ft, imm(rs)
   {0b111001 ,2,I_TYPE,0}, // swc1 ft, imm(rs)
   {0b000000 ,3,R_TYPE,0b000000}, // sll rd, rt, shamt
   {0b000000 ,3,R_TYPE,0b000010}, // srl rd, rt, shamt
   {0b000000 ,3,R_TYPE,0b000011}, // sra rd, rt, shamt
   {0b000000 ,3,R_TYPE,0b000100}, // sllv rd, rt, rs
   {0b000000 ,3,R_TYPE,0b000110}, // srlv rd, rt, rs
   {0b000000 ,3,R_TYPE,0b000111}, // srav rd, rt, rs
   {0b000000 ,1,R_TYPE,0b001000}, // jr rs
   {0b000000 ,1,R_TYPE,0b001001}, // jalr rs
   {0b000000 ,0,R_TYPE,0b001100}, // syscall
   {0b000000 ,0,R_TYPE,0b001101}, // break
   {0b000000 ,1,R_TYPE,0b010000}, // mfhi rd
   {0b000000 ,1,R_TYPE,0b010001}, // mthi rs
   {0b000000 ,1,R_TYPE,0b010010}, // mflo rd
   {0b000000 ,1,R_TYPE,0b010011}, // mtlo rs
   {0b000000 ,2,R_TYPE,0b011000}, // mult rs, rt
   {0b000000 ,2,R_TYPE,0b011001}, // multu rs, rt
   {0b000000 ,2,R_TYPE,0b011010}, // div rs, rt
   {0b000000 ,2,R_TYPE,0b011011}, // divu rs, rt
   {0b000000 ,3,R_TYPE,0b100000}, // add rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100001}, // addu rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100010}, // sub rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100011}, // subu rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100100}, // and rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100101}, // or rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100110}, // xor rd, rs, rt
   {0b000000 ,3,R_TYPE,0b100111}, // nor rd, rs, rt
   {0b000000 ,3,R_TYPE,0b101010}, // slt rd, rs, rt
   {0b000000 ,3,R_TYPE,0b101011}, // sltu rd, rs, rt
};

//Error Handling Method to check wether Instr Provided is Supported or Not
bool IS_INSTR_Available(const INSTR *Instr)
{
	bool Return_Type = TRUE;
	if(Instr != NULL)
	{
		uint32_t Instr_Counter = 0;
		for(Instr_Counter = 0 ; Instr_Counter<= NO_INSTRUCTIONS-1 ; Instr_Counter++)
		{
			if(SupportedInstructions[Instr_Counter] != NULL &&
					(strlen(SupportedInstructions[Instr_Counter]) == strlen(Instr))
					&& (strcmp(SupportedInstructions[Instr_Counter],Instr) == 0))
			{
				Return_Type = TRUE;
				break;
			}else{
				Return_Type = FALSE;
			}
		}
	}else{
		Return_Type = FALSE;
	}
	return Return_Type;
}

bool IndexRegTable(const REGS Register , int8_t *Index)
{
	bool Return_Type = TRUE;
	if (Register != NULL) {
		uint32_t Instr_Counter = 0;
		for (Instr_Counter = 0; Instr_Counter <= NO_INSTRUCTIONS - 1;
				Instr_Counter++) {
			if (MIPSRegisterSET[Instr_Counter] != NULL
					&& (strlen(MIPSRegisterSET[Instr_Counter])
							== strlen(Register))
					&& (strcmp(MIPSRegisterSET[Instr_Counter], Register) == 0)) {
				*Index = Instr_Counter;
				Return_Type = TRUE;
				break;
			} else {
				Return_Type = FALSE;
			}
		}
	} else {
		Return_Type = FALSE;
	}
	return Return_Type;
}

bool IndexInstructionTable(const INSTR *Instr , int8_t *Index)
{
	bool Return_Type = TRUE;
	if (Instr != NULL) {
		uint32_t Instr_Counter = 0;
		for (Instr_Counter = 0; Instr_Counter <= NO_INSTRUCTIONS - 1;
				Instr_Counter++) {
			if (SupportedInstructions[Instr_Counter] != NULL
					&& (strlen(SupportedInstructions[Instr_Counter])
							== strlen(Instr))
					&& (strcmp(SupportedInstructions[Instr_Counter], Instr) == 0)) {
				*Index = Instr_Counter;
				Return_Type = TRUE;
				break;
			} else {
				Return_Type = FALSE;
			}
		}
	} else {
		Return_Type = FALSE;
	}
	return Return_Type;
}

bool IS_Directive_Available(const DIRECTIVES *Instr)
{
	bool Return_Type = TRUE;
	if(Instr != NULL)
	{
		uint32_t Instr_Counter = 0;
		for(Instr_Counter = 0 ; Instr_Counter<= NO_DIRECTIVES - 1 ; Instr_Counter++)
		{
			if(SupportedDirectives[Instr_Counter] != NULL &&
					(strlen(SupportedDirectives[Instr_Counter]) == strlen(Instr))
					&& (strcmp(SupportedDirectives[Instr_Counter],Instr) == 0))
			{
				Return_Type = TRUE;
				break;
			}else{
				Return_Type = FALSE;
			}
		}
	}else{
		Return_Type = FALSE;
	}
	return Return_Type;
}

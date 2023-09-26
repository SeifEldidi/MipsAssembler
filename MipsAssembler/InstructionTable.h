/*
 * InstructionTable.h
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */

#ifndef INSTRUCTIONTABLE_H_
#define INSTRUCTIONTABLE_H_

#include <stdint.h>
#include <string.h>
#include "STD_TYPES.h"

//Register Definition File
/*
#define $0		0
#define $at     1
#define $v0		2
#define $v1		3
#define $a0		4
#define $a1     5
#define $a2		6
#define $a3		7
#define $t0		8
#define $t1     9
#define $t2		10
#define $t3		11
#define $t4		12
#define $t5     13
#define $t6		14
#define $t7		15
#define $s0		16
#define $s1     17
#define $s2		18
#define $s3		19
#define $s4		20
#define $s5     21
#define $s6		22
#define $s7		23
#define $t8		24
#define $t9		25
#define $k0		26
#define $k1		27
#define $gp		28
#define $sp		29
#define	$fp		30
#define $ra		31

//Instructions
#define bltz 	0
#define bgez 	1
#define j 		2
#define jal 	3	//label jump and link $ra = PC + 4, PC = JTA
#define beq 	4	//rs, rt, label branch if equal if ([rs] == [rt]) PC = BTA
#define bne 	5	//rs, rt, label branch if not equal if ([rs] != [rt]) PC = BTA
#define blez 	6	//rs, label branch if less than or equal to zero if ([rs] ≤ 0) PC = BTA
#define bgtz 	7	//rs, label branch if greater than zero if ([rs] > 0) PC = BTA
#define addi 	8	//rt, rs, imm add immediate [rt] = [rs] + SignImm
#define addiu 	9	//rt, rs, imm add immediate unsigned [rt] = [rs] + SignImm
#define slti 	10	//rt, rs, imm set less than immediate [rs] < SignImm ? [rt] = l : [rt] = 0
#define sltiu 	11	//rt, rs, imm set less than immediate unsigned [rs] < SignImm ? [rt] = l : [rt] = 0
#define andi 	12	//rt, rs, imm and immediate [rt] = [rs] & ZeroImm
#define ori 	13	//rt, rs, imm or immediate [rt] = [rs] | ZeroImm
#define xori 	14	//rt, rs, imm xor immediate [rt] = [rs] ^ ZeroImm
#define lui 	15	//rt, imm load upper immediate [rt] = {imm, 16'b0}
#define mfc0 	16	//rt, rd /
#define mtc0 	17	//rt, rd
#define bc1f 	18	//label/
#define bc1t 	19  //label
#define mul 	20	//rd, rs, rt multiply (32-bit result) [rd] = [rs] x [rt]
#define lb 		21	//rt, imm(rs) load byte [rt] = SignExt ([Address]7:0)
#define lh 		22	//rt, imm(rs) load halfword [rt] = SignExt ([Address]15:0)
#define lw 		23	//rt, imm(rs) load word [rt] = [Address]
#define lbu 	24	//rt, imm(rs) load byte unsigned [rt] = ZeroExt ([Address]7:0)
#define lhu 	25	//rt, imm(rs)

#define sb      26	//rt, imm(rs) store byte [Address]7:0 = [rt]7:0
#define sh      27	//rt, imm(rs) store halfword [Address]15:0 = [rt]15:0
#define sw      28	//rt, imm(rs) store word [Address] = [rt]
#define lwc1    29	//ft, imm(rs) load word to FP coprocessor 1 [ft] = [Address]
#define swc1    30	//ft, imm(rs) store word to FP coprocessor 1 [Address] = [ft]

#define sll     31  //rd, rt, shamt shift left logical [rd] = [rt] << shamt
#define srl     32  //rd, rt, shamt shift right logical [rd] = [rt] >> shamt
#define sra     33  //rd, rt, shamt shift right arithmetic [rd] = [rt] >>> shamt
#define sllv    34  //rd, rt, rs shift left logical variable [rd] = [rt] << [rs]4:0
#define srlv    35  //rd, rt, rs shift right logical variable [rd] = [rt] >> [rs]4:0
#define srav    36  //rd, rt, rs shift right arithmetic variable [rd] = [rt] >>> [rs]4:0
#define jr      37  //rs jump register PC = [rs]
#define jalr    38  //rs jump and link register $ra = PC + 4, PC = [rs]
#define syscall 39  //system call system call exception
#define break_Instr   40  //break break exception
#define mfhi    41  //rd move from hi [rd] = [hi]
#define mthi    42  //rs move to hi [hi] = [rs]
#define mflo    43  //rd move from lo [rd] = [lo]
#define mtlo    44  //rs move to lo [lo] = [rs]
#define mult    45  //rs, rt multiply {[hi], [lo]} = [rs] × [rt]
#define multu   46  //rs, rt multiply unsigned {[hi], [lo]} = [rs] × [rt]
#define div     48  //rs, rt divide [lo] = [rs]/[rt],[hi] = [rs]%[rt]
#define divu    49  //rs, rt divide unsigned [lo] = [rs]/[rt],[hi] = [rs]%[rt]

#define add     50  //rd, rs, rt add [rd] = [rs] + [rt]
#define addu    51  //rd, rs, rt add unsigned [rd] = [rs] + [rt]
#define sub     52  //rd, rs, rt subtract [rd] = [rs] – [rt]
#define subu    53  //rd, rs, rt subtract unsigned [rd] = [rs] – [rt]
#define and     54  //rd, rs, rt and [rd] = [rs] & [rt]
#define or      55  //rd, rs, rt or [rd] = [rs] | [rt]
#define xor     56  //rd, rs, rt xor [rd] = [rs] ^ [rt]
#define nor     57  //rd, rs, rt nor [rd] = ~([rs] | [rt])
#define slt     58  //rd, rs, rt set less than [rs] < [rt] ? [rd] = 1 : [rd] = 0
#define sltu    59  //rd, rs, rt
*/

#define R_TYPE 	0
#define I_TYPE  1
#define J_TYPE  2

#define NO_INSTRUCTIONS	60
#define NO_DIRECTIVES	7

typedef char*  REGS	;
typedef char   INSTR  ;
typedef char  DIRECTIVES ;

bool IndexInstructionTable(const INSTR *Instr , int8_t *Index);
bool IndexRegTable(const REGS Register , int8_t *Index);
bool IS_INSTR_Available(const INSTR *Instr);
bool IS_Directive_Available(const DIRECTIVES *Instr);

#endif /* INSTRUCTIONTABLE_H_ */

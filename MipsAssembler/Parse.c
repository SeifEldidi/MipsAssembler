/*
 * Parse.c
 *
 *  Created on: Sep 25, 2023
 *      Author: Seif pc
 */
#include "Parse.h"

static char * parseFirstToken(char *Line ,uint16_t Len ,uint16_t *Token_Len,char **Start);
static void  InsertSymbolTable(int32_t Address,uint8_t Section ,char *Token ,char * Val,int AsciLen,int32_t*Word,int WordLEn);
static bool isTokenLabel(char *Token ,uint16_t Token_len);
static bool FindSymbolTabel(char *Token , uint32_t *Address);
static void AddSymbolTabel();
static bool StrCmp(char *Str1,char *Str2);
static int16_t ConvertStringToInt(char *Token);
static bool LoadStoreInstr(char  *Token , char **Register ,uint16_t *Immediate);
static char * BinaryToHexString(int32_t Instr,int32_t Addr , int len,char *In,char *OP1 ,char *Op2 ,char *Op3);
static int    StringToint(char *Str);

extern const INSTR INSTR_ARRAY[NO_INSTRUCTIONS][4];
extern const uint8_t RegVal[];
SymbolDLL List ;

bool Parse_ASSM(char * File_Name, char *File_Output)
{
	bool Ret = TRUE;
	FILE *FILE_Buffer = NULL;
	FILE *FILE_Output  = NULL;
	uint8_t Flag_Data = 0;
	int Address = TEXT_SECTION_START_ADDRESS;

	FILE_Output = fopen(File_Output , "w");
	if(FILE_Output == NULL)
	{
		return FALSE;
	}

	if(File_Name != NULL)
	{
		char LineReader[256] ={'0'};
		char *Token = NULL;
		int16_t Len = 0;
		uint16_t TokLen = 0;
		char *Ptr = NULL;
		uint8_t Flag = TEXT_SECTION;
		//open Assembly File to Read
		FILE_Buffer = fopen(File_Name , "r");
		uint8_t ParseNumber = 0; //First Pass
		while(1)
		{
			if (fgets(LineReader, 256, FILE_Buffer) == NULL) {
				if(ParseNumber == 0)
				{
					Address = TEXT_SECTION_START_ADDRESS;
					rewind(FILE_Buffer);
					fgets(LineReader, 256, FILE_Buffer);
					ParseNumber = 1;
				}else{
					Ret = FALSE;
					break;
				}
			}
			Len = strlen(LineReader);
			if(ParseNumber == 0)
			{
				//Analyze Line Collected a line will contain a label or Inst or Directive
				Token = parseFirstToken(LineReader,Len,&TokLen,&Ptr);
				//Look for Labels
				if(*Token != '#' && *Token != '\n')
				{
					//Set Address
					if (strlen(Token) == strlen(".text")
							&& strcmp(Token, ".text") == 0) {
						Flag = TEXT_SECTION;
						Address = TEXT_SECTION_START_ADDRESS;
					} else if (strlen(Token) == strlen(".data")
							&& strcmp(Token, ".data") == 0) {
						Flag = DATA_SECTION;
						Address = DATA_SECTION_START_ADDRESS;
					}
					//Look for Labels
					bool Label_flag = isTokenLabel(Token,TokLen);
					// if Token is not a directive or Instr
					if( Label_flag )
					{
						if(*Token != '#' && *Token != '\n')
						{
							char * OldToken = Token;
							if(Flag == TEXT_SECTION)
							{
								*(OldToken + TokLen -1) = '\0';
								InsertSymbolTable(Address,TEXT_SECTION,OldToken,NULL,0,NULL,0);
								Token = parseFirstToken(Ptr ,Len,&TokLen,&Ptr);
								if (IS_INSTR_Available(Token))
									Address += 4;
								free(Token);
							}else if(Flag == DATA_SECTION)
							{
								uint16_t Prev = 0;
								char Tok_Char = '\0';
								char * AsciArr = NULL;
								int * WordArrayA = NULL;
								int AsciLen = 0;
								int WordLen = 0;
								*(OldToken + TokLen -1) = '\0';
								do
								{
									Token = parseFirstToken(Ptr ,Len,&TokLen,&Ptr);
									Tok_Char = *Token;
									if(strlen(Token) == strlen(".word")&& strcmp(Token, ".word") == 0)
									{
										if(WordArrayA != NULL)
											free(WordArrayA);
										Tok_Char = *Token;
										int len = 0;
										int WordArray[256] ={0};
										do
										{
											Token = parseFirstToken(Ptr ,Len,&TokLen,&Ptr);
											Tok_Char = *Token;
											//Convert String to Int
											if(Tok_Char != '\n' && Tok_Char !='\0')
											{
												Prev += 4;
												WordArray[len++] = StringToint(Token);
											}
										}while(Tok_Char != '\n' && Tok_Char !='\0');
										WordLen = len;
										WordArrayA = (int*)malloc(len * sizeof(int));
										memcpy(WordArrayA, WordArray, len *sizeof(int));
									}
									else if(strlen(Token) == strlen(".byte")&& strcmp(Token, ".byte") == 0)
									{
										free(Token);
										Prev += 1;
									}
									else if(strlen(Token) == strlen(".asciiz")&& strcmp(Token, ".asciiz") == 0)
									{
										char * Temp = Token;
										Token = parseFirstToken(Ptr ,Len,&TokLen,&Ptr);
										AsciLen = TokLen;
										AsciArr = Token;
										Tok_Char = *Token;
										free(Temp);
										Prev += TokLen;
									}
								}while(Tok_Char != '\n' && Tok_Char !='\0');
								InsertSymbolTable(Address,DATA_SECTION,OldToken,AsciArr,AsciLen,WordArrayA,WordLen);
								Address += Prev;
							}
						}
					}else{
						if(IS_INSTR_Available(Token))
							Address += 4;
						free(Token);
					}
				}
			}else if(ParseNumber == 1)
			{
				//Parse Instructions and Convert to Machine Since All Label Addresses are Known
				Token = parseFirstToken(LineReader,Len,&TokLen,&Ptr);
				if(StrCmp(Token , ".data"))
				{
					//Add Data Variables to Machine Code
					Flag_Data = 1;
					break;
				}
				if(*Token != '#' && *Token != '\n' && !IS_Directive_Available(Token))
				{
					bool Label_flag = isTokenLabel(Token,TokLen);
					if(Label_flag)
						Token = parseFirstToken(Ptr,Len,&TokLen,&Ptr);
					if(*Token != '#' && *Token != '\n')
					{
						if (IS_INSTR_Available(Token))
						{
							uint8_t Index = 0;
							uint8_t Op_Counter = 0;
							char * Operands[3] ={NULL} ;
							//Check Which Instruction is Encountered
							char * Instr = Token;
							IndexInstructionTable((const INSTR *)Token,(int8_t *)&Index);
							//Parse According To Number of operands
							int8_t No_Parses = INSTR_ARRAY[Index][1];
							while(No_Parses >0 )
							{
								//Parse Next Token
								Token = parseFirstToken(Ptr,Len,&TokLen,&Ptr);
								Operands[Op_Counter ++] = Token;
								No_Parses --;
								if((*(Token) == '\n' || *(Token) == '\0' )&&No_Parses>0)
								{
									Ret = FALSE;
									break;
								}
							}
							//Parse Next Token Trial
							Token = parseFirstToken(Ptr,Len,&TokLen,&Ptr);
							if(*Token != '\n' && *Token !='\0' && *Token != '#')
							{
								Ret = FALSE;
								break;
							}else{
								uint32_t Binary_Value = 0x00000000;
								//Decode INstruction To Assembly
								if(StrCmp(Instr , "add") || StrCmp(Instr , "sub") || StrCmp(Instr , "addu") ||StrCmp(Instr , "subu")
										||StrCmp(Instr , "and") || StrCmp(Instr , "or") || StrCmp(Instr , "xor") ||StrCmp(Instr , "nor") ||
										  StrCmp(Instr , "slt") || StrCmp(Instr , "sltu") || StrCmp(Instr , "sllv") || StrCmp(Instr , "srlv")
										  ||StrCmp(Instr , "srav"))
								{
									//R TYPE = 6bits OP CODE , 5BITS RS,5BITS RT,5BITS RD,5BITS SHAMT,6BITS FUNCT
									uint8_t ID1 = 0;
									uint8_t ID2 = 0;
									uint8_t ID3 = 0;
									bool Flag_1 = IndexRegTable((const REGS )Operands[0] ,(int8_t *)&ID1);
									bool Flag_2 = IndexRegTable((const REGS )Operands[1] ,(int8_t *) &ID2);
									bool Flag_3 = IndexRegTable((const REGS )Operands[2] ,(int8_t *)&ID3);
									if(Flag_1 && Flag_2 && Flag_3)
									{
										Binary_Value |= INSTR_ARRAY[Index][0] <<26;
										Binary_Value |= RegVal[ID1] <<11;
										Binary_Value |= RegVal[ID2] <<21;
										Binary_Value |= RegVal[ID3] <<16;
										Binary_Value |= INSTR_ARRAY[Index][3] <<0;
										char *String = BinaryToHexString(Binary_Value,Address,3,Instr,Operands[0],Operands[1],Operands[2]);
										fprintf(FILE_Output,String);
										free(String);
									}else{
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "sll") || StrCmp(Instr , "srl") ||StrCmp(Instr , "sra"))
								{
									//R TYPE = 6bits OP CODE , 5BITS RS,5BITS RT,5BITS RD,5BITS SHAMT,6BITS FUNCT
									uint8_t ID1 = 0;
									uint8_t ID2 = 0;
									bool Flag_1 = IndexRegTable((const REGS )Operands[0],(int8_t *)&ID1);
									bool Flag_2 = IndexRegTable((const REGS )Operands[1],(int8_t *)&ID2);
									int16_t SHAMT = ConvertStringToInt(Operands[2]);

									if (Flag_1 && Flag_2 ) {
										Binary_Value |= INSTR_ARRAY[Index][0]<< 26;
										Binary_Value |= RegVal[ID1] << 11;
										Binary_Value |= RegVal[ID2] << 16;
										Binary_Value |= SHAMT << 6;
										Binary_Value |= INSTR_ARRAY[Index][3]<< 0;
										char *String = BinaryToHexString(Binary_Value,Address,2,Instr,Operands[0],Operands[1],NULL);
										fprintf(FILE_Output, String);
										free(String);
									} else {
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "addi") || StrCmp(Instr , "addiu") ||StrCmp(Instr , "slti") ||
										StrCmp(Instr , "sltiu") || StrCmp(Instr , "andi")  ||StrCmp(Instr , "ori") ||
										StrCmp(Instr , "xori")    )
								{
									//I TYPE = 6bits OP CODE , 5BITS RS,5BITS RT,5BITS RD,16bits Imm
									uint8_t ID1 = 0;
									uint8_t ID2 = 0;
									bool Flag_1 = IndexRegTable((const REGS )Operands[0],(int8_t *)&ID1);
									bool Flag_2 = IndexRegTable((const REGS )Operands[1],(int8_t *)&ID2);
									int16_t SHAMT = ConvertStringToInt(Operands[2]);

									if (Flag_1 && Flag_2) {
										Binary_Value |= INSTR_ARRAY[Index][0]<< 26;
										Binary_Value |= RegVal[ID1] << 16;
										Binary_Value |= RegVal[ID2] << 21;
										Binary_Value |= SHAMT << 0;
										char *String = BinaryToHexString(Binary_Value,Address,3,Instr,Operands[0],Operands[1],Operands[2]);
										fprintf(FILE_Output, String);
										free(String);
									} else {
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "lb") || StrCmp(Instr , "lh")  ||StrCmp(Instr , "lw") ||
										 StrCmp(Instr , "lbu")|| StrCmp(Instr , "lhu") ||
										 StrCmp(Instr , "sb") || StrCmp(Instr , "sh")  ||StrCmp(Instr , "sw")
										 ||StrCmp(Instr , "swc1")  )
								{
									//I TYPE = 6bits OP CODE , 5BITS RS,5BITS RT,16bits Imm
									char *Temp = NULL;
									uint16_t Immediate;
									bool Flag_3 = LoadStoreInstr(Operands[1] ,&Temp,&Immediate);
									if(Flag_3 == TRUE)
									{
										uint8_t ID1 = 0;
										uint8_t ID2 = 0;
										bool Flag_1 = IndexRegTable((const REGS )Operands[0],(int8_t *)&ID1);
										bool Flag_2 = IndexRegTable((const REGS )Temp,(int8_t *)&ID2);

										if (Flag_1 && Flag_2) {
											Binary_Value |= INSTR_ARRAY[Index][0]<< 26;
											Binary_Value |= RegVal[ID1] << 16;
											Binary_Value |= RegVal[ID2] << 21;
											Binary_Value |= Immediate << 0;
											char *String =  BinaryToHexString(Binary_Value,Address,2,Instr,Operands[0],Operands[1],NULL);
											fprintf(FILE_Output, String);
											free(String);
										} else {
											Ret = FALSE;
											break;
										}
									}else{
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "j") || StrCmp(Instr , "jal") )
								{
									//J TYPE 6bits OPCODE , 26bit Immediate
									int32_t lAddress = 0x00000000;
									//1 Operand Label which is Equivalent to an address in the Symbol Table
									bool IN_SYMBOLTABLE = FindSymbolTabel((char *)Operands[0],(uint32_t *)&lAddress);
									if(IN_SYMBOLTABLE)
									{
										lAddress -= TEXT_SECTION_START_ADDRESS;
										lAddress /= 4;
										Binary_Value |= INSTR_ARRAY[Index][0]<< 26;
										Binary_Value |= (lAddress & 0x03FFFFFF << 0);
										char *String =  BinaryToHexString(Binary_Value,Address,1,Instr,Operands[0],NULL,NULL);
										fprintf(FILE_Output, String);
										free(String);
									}else{
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "jr") || StrCmp(Instr , "jalr"))
								{
									//R TYPE = 6bits OP CODE , 5BITS RS,5BITS RT,5BITS RD,5BITS SHAMT,6BITS FUNCT
									uint8_t ID1 = 0;
									bool Flag_1 = IndexRegTable((const REGS )Operands[0],(int8_t *)&ID1);
									if (Flag_1 ) {
										Binary_Value |= INSTR_ARRAY[Index][0]	<< 26;
										Binary_Value |= RegVal[ID1] << 21;
										char *String =  BinaryToHexString(Binary_Value,Address,1,Instr,Operands[0],NULL,NULL);
										fprintf(FILE_Output, String);
										free(String);
									} else {
										Ret = FALSE;
										break;
									}
								}else if(StrCmp(Instr , "bne") || StrCmp(Instr , "beq"))
								{
									//I TYPE = 6bits OP CODE , 5BITS RS, 5BITS RT,16bits Imm
									uint8_t ID1 = 0;
									uint8_t ID2 = 0;
									int32_t JumpAddress = 0x00000000;
									bool Flag_1 = IndexRegTable((const REGS )Operands[0],(int8_t *)&ID1);
									bool Flag_2 = IndexRegTable((const REGS )Operands[1],(int8_t *)&ID2);
									bool IN_SYMBOLTABLE = FindSymbolTabel((char *)Operands[2],(uint32_t *)&JumpAddress);
									if(Flag_1 && Flag_2 && IN_SYMBOLTABLE)
									{
										JumpAddress -= TEXT_SECTION_START_ADDRESS;
										//Caclulate PC Val
										JumpAddress  = (Address + 4 - TEXT_SECTION_START_ADDRESS) - JumpAddress;
										JumpAddress /= 4;
										//Machine Instruction
										Binary_Value |= INSTR_ARRAY[Index][0]	<< 26;
										Binary_Value |= RegVal[ID1] << 21;
										Binary_Value |= RegVal[ID2] << 16;
										Binary_Value |= (JumpAddress & 0xFFFF) << 0;
										//Send To Ouptut File
										char *String =  BinaryToHexString(Binary_Value,Address,3,Instr,Operands[0],Operands[1],Operands[2]);
										fprintf(FILE_Output, String);
										free(String);
									}else{
										Ret = FALSE;
										break;
									}
								}
							}
							Address += 4;
						}
						else {
							printf("0xXXXXXXXX : %s Instruction Not Supported ",
									Token);
						}
					}
				}else{
				}
			}
		}
	}else{
		Ret = FALSE;
	}
	//Add Symbol Table Data Section to Machine Code
	if(Flag_Data == 1)
	{
		AddSymbolTabel(FILE_Output);
	}
	fclose(FILE_Buffer);
	fclose(FILE_Output);
	return Ret;
}

static void  InsertSymbolTable(int32_t Address,uint8_t Section ,char *Token ,char * Val,int AsciLen,int32_t*Word,int WordLEn)
{
	if(Token != NULL)
	{
		SymbolTable *Node = (SymbolTable *)malloc(sizeof(SymbolTable));
		Node->Address = Address;
		Node->Symbol = Token;
		Node->AsciArr.Ptr = Val;
		Node->AsciArr.Len = AsciLen;
		Node->WordArr.Ptr = Word;
		Node->WordArr.Len = WordLEn;
		Node->Section = Section;
		Node->Next = NULL;
		Node->prev = NULL;

		if(List.No_Symbols== 0)
		{
			List.Head = Node;
			List.Tail = Node;
			List.No_Symbols++;
		}else{
			List.Tail->Next = Node;
			Node->prev = List.Tail;
			List.Tail = List.Tail->Next;
			List.No_Symbols++;
		}
	}
}

static void AddSymbolTabel(FILE *Out)
{
	SymbolTable *Head = List.Head;
	char Str[256] = {0};
	fprintf(Out, "\n\n#=========Data Section in Memory==========\n\n");
	while(Head != NULL)
	{
		if(Head->Section == DATA_SECTION)
		{
			//Symbol Could be a word or an asci val
			if(Head->WordArr.Ptr != NULL)
			{
				int32_t Len = 0;
				for(;Len <= Head->WordArr.Len -1 ;Len++)
				{
					sprintf(Str, "0x%08X:\t0x%08X\t\t #%s\n",Head->Address + 4*Len,Head->WordArr.Ptr[Len],Head->Symbol);
					fprintf(Out, Str);
				}
			}else if(Head->AsciArr.Ptr != NULL)
			{
				int32_t LLEN = 0;
				int32_t Len = 0;
				int32_t Bin = 0;
				int32_t Shift = 0;
				for(;Len <= (Head->AsciArr.Len)/4 - 1;Len++)
				{
					Bin = (Head->AsciArr.Ptr[Len<<2] <<0)| (Head->AsciArr.Ptr[((Len<<2) +1)] <<8)
								 |(Head->AsciArr.Ptr[((Len<<2) +2)] <<16)|(Head->AsciArr.Ptr[((Len<<2) +3)]<<24);
					sprintf(Str, "0x%08X:\t0x%08X\t\t #%s\n", Head->Address + Len ,Bin, Head->Symbol);
					fprintf(Out, Str);
				}
				Bin = 0;
				LLEN = Len;
				for(; Len <= ((Head->AsciArr.Len)%4) + LLEN - 1 ; Len++)
				{
					Bin |= Head->AsciArr.Ptr[Len] << 8*Shift;
					Shift++;
				}
				sprintf(Str, "0x%08X:\t0x%08X\t\t #%s\n", Head->Address + LLEN, Bin,Head->Symbol);
				fprintf(Out, Str);
			}
		}
		Head = Head->Next;
	}
}

static int    StringToint(char *Str)
{
	int32_t Ret  = 0;
	uint8_t  Flag = 0;
	uint32_t i = 0;
	if(*Str == '-')
		Flag = 1;
	while(*(Str + i) !='\0')
	{
		Ret = Ret*10 +(*(Str+i)-'0');
		i++;
	}
	return Flag == 1? Ret * -1 :Ret;
}

static char * BinaryToHexString(int32_t Instr,int32_t Addr , int len,char *In,char *OP1 ,char *Op2 ,char *Op3)
{
	char * Str = (char * )calloc(256 ,sizeof(char));
	if(len == 1)
		sprintf(Str, "0x%08X:\t0x%08X\t\t #%s %s\n",Addr,Instr,In,OP1);
	else if(len == 2)
		sprintf(Str, "0x%08X:\t0x%08X\t\t #%s %s,%s\n",Addr,Instr,In,OP1,Op2);
	else if(len == 3)
		sprintf(Str, "0x%08X:\t0x%08X\t\t #%s %s,%s,%s\n",Addr,Instr,In,OP1,Op2,Op3);
	return Str;
}

static bool LoadStoreInstr(char  *Token , char **Register ,uint16_t *Immediate)
{
	bool  ret = FALSE;
	uint8_t Size = 0;
	char * Temp = calloc(5,sizeof(char));
	uint32_t i = 0;
	*Immediate = 0;
	uint8_t Sign_F = 0;
	if (*(Token + i) == '-')
		Sign_F = 1;
	while(*(Token +i) !='(')
	{
		if(*(Token +i) >= '0' && *(Token +i) <='9')
		{
			*Immediate = *Immediate*10 + (*(Token+i) -'0');
		}else{
			return FALSE;
		}
		i++;
	}
	i++;
	if(Sign_F == 1)
		*Immediate *=-1;
	while(*(Token +i )!=')')
	{
		*(Temp + Size++) = *(Token +i);
		if(Size >3)
		{
			return FALSE;
		}
		i++;
	}
	ret = IndexRegTable((const REGS )Temp ,(int8_t *) &Size);
	if(ret == TRUE)
		*Register = Temp;
	else
		*Register = NULL;
	return ret;
}

static bool FindSymbolTabel(char *Token , uint32_t *Address)
{
	int8_t Len_Token = strlen(Token);
	SymbolTable * Head = List.Head;

	if (*(Token + Len_Token - 1) == ':')
		*(Token + Len_Token - 1) = '\0';

	while(Head != NULL)
	{
		if(strlen(Token) == strlen(Head->Symbol)&& strcmp(Token, Head->Symbol) == 0)
		{
			*Address = Head->Address;
			return TRUE;
		}
		Head = Head->Next;
	}
	return FALSE;
}

void PrintSymbolTable()
{
	SymbolTable * Head = List.Head;
	printf(" Adresss  \t\t Label \n");
	fflush(stdout);
	while(Head != NULL)
	{
		printf(" %08x \t\t %s \n",Head->Address ,Head->Symbol);
		fflush(stdout);
		Head = Head->Next;
	}
}

static int16_t ConvertStringToInt(char *Token)
{
	int16_t Ret = 0;
	int16_t i = 0;
	uint8_t Sign_F =0;
	if(*(Token + i) =='-')
		Sign_F = 1;

	while(*(Token +i) != '\0')
	{
		if(*(Token +i) >='0' && *(Token +i )<='9')
			Ret = Ret*10 +(*(Token +i ) - '0') ;
		i++;
	}
	if(Sign_F )
		Ret *=-1;
	return Ret;
}

static char * parseFirstToken(char *Line ,uint16_t Len ,uint16_t *Token_Len,char **Start)
{
	char * Token = NULL;
	if(Line != NULL)
	{
		uint16_t Counter = 0;
		uint16_t TokCounter = 0;
		//Skip Spaces at First
		Token = (char *) calloc(Len + 1, sizeof(char));
		while(*(Line +Counter)  == ' ' || *(Line +Counter)  == '\t' ||*(Line +Counter)  == ',' )
			Counter++;

		if(*(Line + Counter) =='\n' || *(Line + Counter) =='#')
		{
			*(Token + 0) = *(Line + Counter);
			*(Token + 1) ='\0';
			*Token_Len = 1;
		}else{
			//Skip Token
			while(*(Line + Counter) != ' ' && *(Line + Counter) != '\t'&&*(Line + Counter) != '\n'&&*(Line + Counter) != ',' &&*(Line+Counter ) !='\0')
			{
				if(*(Line +Counter) =='\"')
				{
					Counter ++;
				}else{
					if(*(Line + Counter ) =='\\')
					{
						if(*(Line + Counter + 1 ) == 'n' )
						{
							Counter += 2;
							*(Token + TokCounter) = '\n';
							TokCounter++;
						}else if(*(Line + Counter + 1) =='t')
						{
							Counter += 2;
							*(Token + TokCounter) = '\n';
							TokCounter++;
						}
					}else{
						*(Token + TokCounter) = *(Line + Counter);
						Counter++;
						TokCounter++;
					}
				}
			}
			*(Token + TokCounter) = '\0';
			*Start = Line + Counter;
			*Token_Len = TokCounter;
		}
	}else{
		Token = NULL;
	}
	return Token;
}

static bool StrCmp(char *Str1,char *Str2)
{
	return strlen(Str1) == strlen(Str2)&&( strcmp(Str2, Str1) == 0);
}

static bool isTokenLabel(char *Token ,uint16_t Token_len)
{
	bool isToken = FALSE;
	if(Token != NULL)
	{
		if(*(Token + Token_len - 1 )== ':')
		{
			*(Token + Token_len - 1 ) = '\0';
			isToken = TRUE;
		}
		else
			isToken = FALSE;
	}else{
		isToken = FALSE;
	}
	return isToken;
}



// p6.cpp

#include <stdexcept>
#include "basic_functions.h"
#include "p6.h"
#include "p5.h"
#include "p4.h"
#include "p3.h"
#include "p2.h"
#include "p1.h"

// useful constants

bool FOUR[8] = {0,0,1,0,0,0,0,0};
bool EIGHT[8] = {0,0,0,1,0,0,0,0};
bool SIXTEEN[8] = {0,0,0,0,1,0,0,0};
bool THIRTYTWO[8] = {0,0,0,0,0,1,0,0};

bool registry[16][8];
bool instructionMemory[256][8];
bool dataMemory[16][8];
bool PC[8];
bool IR[32];

// You should initialize the registry, PC, and other necessary data stores to
// zero when the program starts, by calling this function. The test program will
// call it, too.
void initialize()
{
	int i, j;
	
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 8; j++)
		{	
			registry[i][j] = 0;
			dataMemory[i][j] = 0;
		}
		
		if(i < 8)
		{
			PC[i] = 0;
		}
	}
	
	for(i = 0; i < 32; i++)
	{
		IR[i] = 0;
	}
	
	for(i = 0; i < 256; i++)
	{
		for(j = 0; j < 8; j++)
		{
			instructionMemory[i][j] = 0;
		}
	}
}
	

// Reading and writing registers.
bool* getRegistryValue(bool* output, bool registerNum[8])
{                                               
	output[0] = registry[BinToDec(registerNum)][0];
	output[1] = registry[BinToDec(registerNum)][1];
	output[2] = registry[BinToDec(registerNum)][2];
	output[3] = registry[BinToDec(registerNum)][3];
	output[4] = registry[BinToDec(registerNum)][4];
	output[5] = registry[BinToDec(registerNum)][5];
	output[6] = registry[BinToDec(registerNum)][6];
	output[7] = registry[BinToDec(registerNum)][7];
}
	
void setRegistryValue(bool registerNum[8], bool value[8])
{
	registry[BinToDec(registerNum)][0] =  value[0];
    registry[BinToDec(registerNum)][1] =  value[1];
    registry[BinToDec(registerNum)][2] =  value[2];
    registry[BinToDec(registerNum)][3] =  value[3];
    registry[BinToDec(registerNum)][4] =  value[4];
    registry[BinToDec(registerNum)][5] =  value[5];
	registry[BinToDec(registerNum)][6] =  value[6];
    registry[BinToDec(registerNum)][7] =  value[7];
}
	
	
// You may use any C++ provided features to implement this function.
int toDecimal(bool registerNum[8])
{
	return BinToDec(registerNum); // already did this in basic_functions.h
}

// PC = PC + 4.
void incrementPC()
{
	add(PC, PC, FOUR);
}	
	
// PC = PC + addressOffset; addressOffset must be multiple of 4.
void addPC(bool addressOffset[8])
{
	//TODO: throw exception if not divisible by 4
	add(PC,PC, addressOffset);
}
	

// Fetch 32 bits from instructionMemory[PC] to IR.
void instructionFetch()
{
	IR[0]  =  instructionMemory[BinToDec(PC)][0];
	IR[1]  =  instructionMemory[BinToDec(PC)][1];
	IR[2]  =  instructionMemory[BinToDec(PC)][2];
	IR[3]  =  instructionMemory[BinToDec(PC)][3];
	IR[4]  =  instructionMemory[BinToDec(PC)][4];
	IR[5]  =  instructionMemory[BinToDec(PC)][5];
	IR[6]  =  instructionMemory[BinToDec(PC)][6];
	IR[7]  =  instructionMemory[BinToDec(PC)][7];
	
	addPC(EIGHT); // PC += 8
	
	IR[8]  =  instructionMemory[BinToDec(PC)][0];
	IR[9]  =  instructionMemory[BinToDec(PC)][1];
	IR[10] =  instructionMemory[BinToDec(PC)][2];
	IR[11] =  instructionMemory[BinToDec(PC)][3];
	IR[12] =  instructionMemory[BinToDec(PC)][4];
	IR[13] =  instructionMemory[BinToDec(PC)][5];
	IR[14] =  instructionMemory[BinToDec(PC)][6];
	IR[15] =  instructionMemory[BinToDec(PC)][7];
	
	addPC(EIGHT); // PC += 8
	
	IR[16] =  instructionMemory[BinToDec(PC)][0];
	IR[17] =  instructionMemory[BinToDec(PC)][1];
	IR[18] =  instructionMemory[BinToDec(PC)][2];
	IR[19] =  instructionMemory[BinToDec(PC)][3];
	IR[20] =  instructionMemory[BinToDec(PC)][4];
	IR[21] =  instructionMemory[BinToDec(PC)][5];
	IR[22] =  instructionMemory[BinToDec(PC)][6];
	IR[23] =  instructionMemory[BinToDec(PC)][7];
	
	addPC(EIGHT); // PC += 8
	
	IR[24] =  instructionMemory[BinToDec(PC)][0];
	IR[25] =  instructionMemory[BinToDec(PC)][1];
	IR[26] =  instructionMemory[BinToDec(PC)][2];
	IR[27] =  instructionMemory[BinToDec(PC)][3];
	IR[28] =  instructionMemory[BinToDec(PC)][4];
	IR[29] =  instructionMemory[BinToDec(PC)][5];
	IR[30] =  instructionMemory[BinToDec(PC)][6];
	IR[31] =  instructionMemory[BinToDec(PC)][7];
}

// Parse a 32-bit instruction in IR to 4 or 3 parts.
//
// For R-format instructions, this will parse the instruction into name,
// reg_out, reg_in1, and reg_in2, and leave imm16 untouched.
//
// For I-format instructions, this will parse the instruction into name,
// reg_out, and imm16, and leave reg_in1 and reg_in2 untouched.
//
// Arguments:
// - name:    8bit for the name (op code) of the instruction.
// - reg_out: 8bit for the register that will be used to place the result.
// - reg_in1: 8bit for the register from which the 1st operand will be loaded.
// - reg_in2: 8bit for the register from which the 2nd operand will be loaded.
// - imm16:   16bit for the immediate number from the instruction.
void parseInstructionInIR(bool* name,
                          bool* reg_out,
                          bool* reg_in1,
                          bool* reg_in2,
                          bool* imm16)
{
	name[0]    =  IR[0];
	name[1]    =  IR[1];
	name[2]    =  IR[2];
	name[3]    =  IR[3];
	name[4]    =  IR[4];
	name[5]    =  IR[5];
	name[6]    =  IR[6];
	name[7]    =  IR[7];
	
	reg_out[0] =  IR[8];
	reg_out[1] =  IR[9];
	reg_out[2] =  IR[10];
	reg_out[3] =  IR[11];
	reg_out[4] =  IR[12];
	reg_out[5] =  IR[13];
	reg_out[6] =  IR[14];
	reg_out[7] =  IR[15];
	
	reg_in1[0] =  IR[16];
	reg_in1[1] =  IR[17];
	reg_in1[2] =  IR[18];
	reg_in1[3] =  IR[19];
	reg_in1[4] =  IR[20];
	reg_in1[5] =  IR[21];
	reg_in1[6] =  IR[22];
	reg_in1[7] =  IR[23];
	
	reg_in2[0] =  IR[24];
	reg_in2[1] =  IR[25];
	reg_in2[2] =  IR[26];
	reg_in2[3] =  IR[27];
	reg_in2[4] =  IR[28];
	reg_in2[5] =  IR[29];
	reg_in2[6] =  IR[30];
	reg_in2[7] =  IR[31];
}
	
	

// Load instructions from an input file stream.
// You may use any C++ provided features to implement this function.
void initializeInstructionMemory(std::ifstream& fp)
{
}
// Convert the name of an instruction (e.g. addu, lw) into its 8bit op code.
// You may use any C++ provided features to implement this function.
bool* getInstructionValue(bool* output, const std::string& instructionName)
{
}

// Read or write memory.
//
// When read is true and write is false, read the byte at the indicated memory
// location to the register indexed by registerNum.
//
// When write is true and read is false, write the content of the register
// indexed by registerNum to the indicated memory location.
//
// read and write should never be the same value.
//
// You may use if..else.. in mem() to choose from calling lw() or sw().
void mem(bool registerNum[8], bool memlocation[16], bool read, bool write)
{
	if(read == write)
	{
		throw(std::runtime_error("MEM error: read == write"));
	}
	else if(read)
	{
		lw(registerNum, memlocation);
	}
	else // write
	{
		sw(registerNum, memlocation);
	}
}
		
void lw(bool registerNum[8], bool memlocation[16])
{
	readMem(registerNum, memlocation);
}

void sw(bool registerNum[8], bool memlocation[16])
{
	writeMem(registerNum, memlocation);
}

// The following functions are helpers to mem(), lw(), and sw(). They will also
// make P7 easier.
//
// You may use if..else.. in readOrWriteMem() to choose from calling readMem()
// or writeMem().
void readOrWriteMem(bool value[8], bool memlocation[8], bool read, bool write)
{
	if(read)
	{
		readMem(value, memlocation);
	}
	else
	{
		writeMem(value, memlocation);
	}
}

void readMem(bool value[8], bool memlocation[16])
{
	registry[BinToDec(value)][0] = memlocation[0];
	registry[BinToDec(value)][1] = memlocation[1];
	registry[BinToDec(value)][2] = memlocation[2];
	registry[BinToDec(value)][3] = memlocation[3];
	registry[BinToDec(value)][4] = memlocation[4];
	registry[BinToDec(value)][5] = memlocation[5];
	registry[BinToDec(value)][6] = memlocation[6];
	registry[BinToDec(value)][7] = memlocation[7];
}

void writeMem(bool value[8], bool memlocation[16])
{
	memlocation[0] = registry[BinToDec(value)][0];
	memlocation[1] = registry[BinToDec(value)][1];
	memlocation[2] = registry[BinToDec(value)][2];
	memlocation[3] = registry[BinToDec(value)][3];
	memlocation[4] = registry[BinToDec(value)][4];
	memlocation[5] = registry[BinToDec(value)][5];
	memlocation[6] = registry[BinToDec(value)][6];
	memlocation[7] = registry[BinToDec(value)][7];
}
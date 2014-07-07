// p6.cpp

#include <stdexcept>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "basic_functions.h"
#include "test_utils.h"
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

//


std::vector<instruction> instruction_list;

bool registry[16][8];
int lockReg[16];
bool instructionMemory[256][8];
bool dataMemory[16][8];
bool PC[8];
bool IR[32];

bool ZeroedIR() // skip an empty instruction
{
	for(int i = 0; i < 32; i++)
	{
		if(IR[i])
			return false;
	}
	return true;
}

// helper for program loading
int RegFromString(std::string regstr)
{
	return atoi(regstr.c_str());
}

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
			lockReg[i] = -1;
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
	int reg = toDecimal(registerNum);
                                               
	output[0] = registry[reg][0];
	output[1] = registry[reg][1];
	output[2] = registry[reg][2];
	output[3] = registry[reg][3];
	output[4] = registry[reg][4];
	output[5] = registry[reg][5];
	output[6] = registry[reg][6];
	output[7] = registry[reg][7];
		
	return output;
}
	
void setRegistryValue(bool registerNum[8], bool value[8])
{
	int reg = toDecimal(registerNum);
	
	std::cout << "setRegistryValue(" << toDecimal(registerNum) << ") = " << toDecimal(value) << std::endl;
	
	if(reg > 0) // do not write to $0
	{	
		registry[reg][0] =  value[0];
		registry[reg][1] =  value[1];
		registry[reg][2] =  value[2];
		registry[reg][3] =  value[3];
		registry[reg][4] =  value[4];
		registry[reg][5] =  value[5];
		registry[reg][6] =  value[6];
		registry[reg][7] =  value[7];
	}	
}
	
	
// You may use any C++ provided features to implement this function.
int toDecimal(bool registerNum[8])
{
	int ans = 0;
	for(int i = 0; i < 7; i++)
	{
		if(registerNum[i])
		{
			ans += pow(2,i);
		}
	}
	return ans;
}

// PC = PC + 4.
void incrementPC()
{
	addu(PC, PC, FOUR);
}	
	
// PC = PC + addressOffset; addressOffset must be multiple of 4.
void addPC(bool addressOffset[8])
{
	int M = toDecimal(addressOffset);
	int n = int(M / 4);
	int r = M % 4;
	if(r)
	{
		std::cout << std::endl << "Invalid PC offset" << std::endl;
		return;//throw "Invalid address";
	}
	else
	{
		for(int i = 0; i < n; i++)
		{
			incrementPC();
		}
	}
}
	

// Fetch 32 bits from instructionMemory[PC] to IR.
int instructionFetch()
{
	int PCReg = toDecimal(PC);
		
	// OP
	IR[0]  =  instructionMemory[PCReg+3][0];
	IR[1]  =  instructionMemory[PCReg+3][1];
	IR[2]  =  instructionMemory[PCReg+3][2];
	IR[3]  =  instructionMemory[PCReg+3][3];
	IR[4]  =  instructionMemory[PCReg+3][4];
	IR[5]  =  instructionMemory[PCReg+3][5];
	IR[6]  =  instructionMemory[PCReg+3][6];
	IR[7]  =  instructionMemory[PCReg+3][7];
	
	// OUT REG
	IR[8]  =  instructionMemory[PCReg+2][0];
	IR[9]  =  instructionMemory[PCReg+2][1];
	IR[10] =  instructionMemory[PCReg+2][2];
	IR[11] =  instructionMemory[PCReg+2][3];
	IR[12] =  instructionMemory[PCReg+2][4];
	IR[13] =  instructionMemory[PCReg+2][5];
	IR[14] =  instructionMemory[PCReg+2][6];
	IR[15] =  instructionMemory[PCReg+2][7];
	
	// IN REG 1
	IR[16] =  instructionMemory[PCReg+1][0];
	IR[17] =  instructionMemory[PCReg+1][1];
	IR[18] =  instructionMemory[PCReg+1][2];
	IR[19] =  instructionMemory[PCReg+1][3];
	IR[20] =  instructionMemory[PCReg+1][4];
	IR[21] =  instructionMemory[PCReg+1][5];
	IR[22] =  instructionMemory[PCReg+1][6];
	IR[23] =  instructionMemory[PCReg+1][7];
	
	// IN REG 2
	IR[24] =  instructionMemory[PCReg][0];
	IR[25] =  instructionMemory[PCReg][1];
	IR[26] =  instructionMemory[PCReg][2];
	IR[27] =  instructionMemory[PCReg][3];
	IR[28] =  instructionMemory[PCReg][4];
	IR[29] =  instructionMemory[PCReg][5];
	IR[30] =  instructionMemory[PCReg][6];
	IR[31] =  instructionMemory[PCReg][7];
	
	incrementPC();
	
	if(ZeroedIR())
	{
		return -1;
	}
	else
	{
		//std::cout << "PCREG: " << PCReg << std::endl;
		return PCReg / 4;
	}
}

bool IsIFormatInstruction(bool op_code[8])
{
	switch(toDecimal(op_code))
	{
		case 30:
			return true;
			break;
		case 15:
			return true;
			break;
		default:
			return false;
			break;
	}
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
bool parseInstructionInIR(bool* name,
                          bool* reg_out,
                          bool* reg_in1,
                          bool* reg_in2,
                          bool* imm16)
{
	name[0]    =  IR[24];
	name[1]    =  IR[25];
	name[2]    =  IR[26];
	name[3]    =  IR[27];
	name[4]    =  IR[28];
	name[5]    =  IR[29];
	name[6]    =  IR[30];
	name[7]    =  IR[31];
	
	if(!IsIFormatInstruction(name))
	{
		reg_out[0] =  IR[16];
		reg_out[1] =  IR[17];
		reg_out[2] =  IR[18];
		reg_out[3] =  IR[19];
		reg_out[4] =  IR[20];
		reg_out[5] =  IR[21];
		reg_out[6] =  IR[22];
		reg_out[7] =  IR[23];
		
		reg_in1[0] =  IR[8]; 
		reg_in1[1] =  IR[9]; 
		reg_in1[2] =  IR[10];
		reg_in1[3] =  IR[11];
		reg_in1[4] =  IR[12];
		reg_in1[5] =  IR[13];
		reg_in1[6] =  IR[14];
		reg_in1[7] =  IR[15];
		
		reg_in2[0] =  IR[0];
		reg_in2[1] =  IR[1];
		reg_in2[2] =  IR[2];
		reg_in2[3] =  IR[3];
		reg_in2[4] =  IR[4];
		reg_in2[5] =  IR[5];
		reg_in2[6] =  IR[6];
		reg_in2[7] =  IR[7];
		
		return true;
	}
	else
	{
		imm16[0]  = IR[0];
		imm16[1]  = IR[1];
		imm16[2]  = IR[2];
		imm16[3]  = IR[3];
		imm16[4]  = IR[4];
		imm16[5]  = IR[5];
		imm16[6]  = IR[6];
		imm16[7]  = IR[7];
		imm16[8]  = IR[8];
		imm16[9]  = IR[9];
		imm16[10] = IR[10];
		imm16[11] = IR[11];
		imm16[12] = IR[12];
		imm16[13] = IR[13];
		imm16[14] = IR[14];
		imm16[15] = IR[15];
		
		reg_out[0] =  IR[16];
		reg_out[1] =  IR[17];
		reg_out[2] =  IR[18];
		reg_out[3] =  IR[19];
		reg_out[4] =  IR[20];
		reg_out[5] =  IR[21];
		reg_out[6] =  IR[22];
		reg_out[7] =  IR[23];
		
		return false;
	}
}
	
	

// Load instructions from an input file stream.
// You may use any C++ provided features to implement this function.
void initializeInstructionMemory(std::ifstream& fp)
{
	std::string line, instr, reg_dest, reg1, reg2;
	
	while(fp.good())
	{
		std::string temp;
		instruction i = {};
		
		std::getline(fp, line);		// get line from file
		int split = line.find(" "); // get 1st whitespace idx 
		i.INSTR = line.substr(0, split);
		line = line.substr(split + 1, line.length() - 1); //skip the whitespace
		
		split = line.find(",");
		i.DEST = line.substr(0, split);
		line = line.substr(split + 1, line.length() - 1);
		
		split = line.find(",");
		i.REG1 = line.substr(0, split);
		line = line.substr(split + 1, line.length() - 1);
		if(split > 0)
		{
			i.REG2 = line;
		}
		else
		{
			i.REG2 = "";
		}
		
		instruction_list.push_back(i);
	}
	
	// hack to cull the empty instruction appearing at the end
	instruction_list.pop_back();
	
	for(int i = 0; i < instruction_list.size(); i++)
	{	
		bool temp[8];
		if(instruction_list[i].REG2 != "") // if we aren't 'I' format
		{
			getInstructionValue(instructionMemory[4*i],instruction_list[i].INSTR);
			addu(instructionMemory[4*i + 1], to_bits(temp,RegFromString(instruction_list[i].DEST), 8), zero);
			addu(instructionMemory[4*i + 2], to_bits(temp,RegFromString(instruction_list[i].REG1), 8), zero);
			addu(instructionMemory[4*i + 3], to_bits(temp,RegFromString(instruction_list[i].REG2), 8), zero);
			
			//std::cout << std::endl << "R Instruction: " << instruction_list[i].INSTR << " " << RegFromString(instruction_list[i].DEST) << " " << RegFromString(instruction_list[i].REG1) << " " << RegFromString(instruction_list[i].REG2);
		}
		else
		{
			getInstructionValue(instructionMemory[4*i],instruction_list[i].INSTR);
			addu(instructionMemory[4*i + 1], to_bits(temp,RegFromString(instruction_list[i].DEST), 8), zero);
			addu(instructionMemory[4*i + 2], to_bits(temp,RegFromString(instruction_list[i].REG2), 8), zero);
			addu(instructionMemory[4*i + 3], to_bits(temp,RegFromString(instruction_list[i].REG1), 8), zero);
			
			
			//std::cout << std::endl << "I Instruction: " << instruction_list[i].INSTR << " " << RegFromString(instruction_list[i].DEST) << " " << RegFromString(instruction_list[i].REG1);
		}
	}
	
}
// Convert the name of an instruction (e.g. addu, lw) into its 8bit op code.
// You may use any C++ provided features to implement this function.
bool* getInstructionValue(bool* output, const std::string& instructionName)
{
	std::string instr = instructionName;
	std::transform(instr.begin(), instr.end(), instr.begin(), ::tolower);
	
	if(instr == "addu")
		return addu(output, OP_ADDU, zero);
	if(instr == "subu")
		return addu(output, OP_SUBU, zero);
	if(instr == "add")
		return addu(output, OP_ADD, zero);
	if(instr == "sub")
		return addu(output, OP_SUB, zero);
	if(instr == "neg")
		return addu(output, OP_NEG, zero);
	if(instr == "eq")
		return addu(output, OP_EQ, zero);
	if(instr == "lt")
		return addu(output, OP_LT, zero);
	if(instr == "gt")
		return addu(output, OP_GT, zero);
	if(instr == "and")
		return addu(output, OP_AND, zero);
	if(instr == "or")
		return addu(output, OP_OR, zero);
	if(instr == "sll")
		return addu(output, OP_SLL, zero);
	if(instr == "srl")
		return addu(output, OP_SRL, zero);
	if(instr == "multu")
		return addu(output, OP_MULTU, zero);
	if(instr == "mult")
		return addu(output, OP_MULT, zero);
	if(instr == "divu")
		return addu(output, OP_DIVU, zero);
	if(instr == "div")
		return addu(output, OP_DIV, zero);
	if(instr == "lw")
		return addu(output, OP_LW, zero);
	if(instr == "sw")
		return addu(output, OP_SW, zero);
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
	if(read && write)
	{
		throw(std::runtime_error("MEM error: read == write"));
	}
	else if(read)
	{
		lw(registerNum, memlocation);
	}
	else if(write)
	{
		sw(registerNum, memlocation);
	}
}
		
void lw(bool registerNum[8], bool memlocation[16])
{
	bool temp[8];
	
	std::cout << std::endl << "Reg(" << toDecimal(registerNum) << ") = " << toDecimal(dataMemory[toDecimal(memlocation)]) <<  std::endl;

	readMem(registerNum, memlocation);
}

void sw(bool registerNum[8], bool memlocation[16])
{
	bool temp[8];
	
	std::cout << std::endl << "Mem(" << toDecimal(memlocation) << ") = " << toDecimal(getRegistryValue(temp,registerNum)) <<  std::endl;
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
	registry[toDecimal(value)][0] = dataMemory[toDecimal(memlocation)][0];
	registry[toDecimal(value)][1] = dataMemory[toDecimal(memlocation)][1];
	registry[toDecimal(value)][2] = dataMemory[toDecimal(memlocation)][2];
	registry[toDecimal(value)][3] = dataMemory[toDecimal(memlocation)][3];
	registry[toDecimal(value)][4] = dataMemory[toDecimal(memlocation)][4];
	registry[toDecimal(value)][5] = dataMemory[toDecimal(memlocation)][5];
	registry[toDecimal(value)][6] = dataMemory[toDecimal(memlocation)][6];
	registry[toDecimal(value)][7] = dataMemory[toDecimal(memlocation)][7];
}

void writeMem(bool value[8], bool memlocation[16])
{
	dataMemory[toDecimal(memlocation)][0] = registry[toDecimal(value)][0];
	dataMemory[toDecimal(memlocation)][1] = registry[toDecimal(value)][1];
	dataMemory[toDecimal(memlocation)][2] = registry[toDecimal(value)][2];
	dataMemory[toDecimal(memlocation)][3] = registry[toDecimal(value)][3];
	dataMemory[toDecimal(memlocation)][4] = registry[toDecimal(value)][4];
	dataMemory[toDecimal(memlocation)][5] = registry[toDecimal(value)][5];
	dataMemory[toDecimal(memlocation)][6] = registry[toDecimal(value)][6];
	dataMemory[toDecimal(memlocation)][7] = registry[toDecimal(value)][7];
}
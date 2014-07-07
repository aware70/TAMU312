// Functions required by Problem 7.
//
// In your p7.cpp, you should include basic_functions.h, p1.h, and so on, if you
// want to use previously built functions.
//
// Note that you can only use functions you have built so far, unless explicitly
// indicated.

#ifndef P7_H
#define P7_H

// This will tick a clock cycle, in which you will mimic the execution of the 5
// stages: IF, ID, EX, MEM, and WB, using functions you have built before.
//
// Your implementation MUST clearly show these 5 stages.
//
// Do not forget to increment PC.
//
// Note that you will also need to make changes to support the "load immediate
// value" instruction (LI) whose op code is 00001010.
enum ExecutionStage
{
	EXE_IF = 1,
	EXE_ID = 2,
	EXE_EX = 3,
	EXE_MEM = 4,
	EXE_WB = 5,
	
	EXE_END = 6
};

struct LatentInstruction
{
	ExecutionStage stage;
	bool bWriteMem;
	bool bReadMem;
	bool bWriteReg;

	bool op_code[8]; // this instruction's op code
	bool rd[8];  // etc
	bool rs[8];	 // etc
	bool rt[8];	 // ...
	bool immi[16];
	bool out[16];
	
	bool arg1[8]; // values to store for ALU in execution stage
	bool arg2[8];
	
	bool bActive; // this instruction is "live." Used to find instruction to store temp variables within the context of alu
	bool iFormat;
	bool bStalled; // if we are stalled, avoid certain actions (like grabbing IR if we are repeating ID)
	
	int tag;

	LatentInstruction()
	{
		stage = EXE_IF;
		bActive = false;
		bWriteMem = false;
		bReadMem = false;
		bWriteReg = false;
		iFormat = false;
		bStalled = false;
		tag = -1;
	}
	
};


void WB(LatentInstruction& in);

void MEM(LatentInstruction& in);

void EX(LatentInstruction& in);

void ID(LatentInstruction& in);

void IF(LatentInstruction& in);

void tick();

bool* li(bool* output, bool* immmediate);

bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8], bool immediate[16]);



#endif // P7_H


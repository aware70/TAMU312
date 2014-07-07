// p7.cpp

#include <iostream>
#include <vector>
#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"
#include "p6.h"
#include "p7.h"

std::vector<LatentInstruction> pendingInstructions;

void WB(LatentInstruction& in)
{
	std::cout << std::endl << "WB: " << in.tag << std::endl << "OP_Code: " << toDecimal(in.op_code) << std::endl << "bWriteReg: " << in.bWriteReg << std::endl << "out: " << toDecimal(in.out) << std::endl << "rd: " << toDecimal(in.rd) << std::endl;

	if(in.bWriteReg)
	{
			setRegistryValue(in.rd, in.out); // WB
			lockReg[toDecimal(in.rd)] = -1; // free this register
	}
	in.stage = EXE_END; // send this instruction to the next stage for the next cycle
}

void MEM(LatentInstruction& in)
{
	std::cout << std::endl << "MEM: " << in.tag << std::endl << "OP_Code: " << toDecimal(in.op_code) << std::endl << "rd: " << toDecimal(in.rd) << std::endl << "bReadMem: " << in.bReadMem << std::endl << "bWriteMem: " << in.bWriteMem << std::endl;
	mem(in.rd, in.immi, in.bReadMem, in.bWriteMem); // mem read or write if neccessary
	in.stage = EXE_WB; // send this instruction to the next stage for the next cycle
}

void EX(LatentInstruction& in) // EX and MEM stage - Instruction is executed and result is stored in out, to be used in the next two stages
{
	std::cout << std::endl << "EX: " << in.tag << std::endl << "OP_Code: " << toDecimal(in.op_code) << std::endl << "rs val: " << toDecimal(in.arg1) << std::endl << "rt val: " << toDecimal(in.arg2) << std::endl;
	alu(in.op_code, in.out, in.arg1, in.arg2, in.immi); // executes instruction and sets read / write flags accordingly
	in.stage = EXE_MEM; // send this instruction to the next stage for the next cycle
}

void ID(LatentInstruction& in)
{
	bool RFormat;
	
	if(!in.bStalled)
	{
		RFormat = parseInstructionInIR(in.op_code, in.rd, in.rs, in.rt, in.immi); // Don't look in IR if we stalled last time!
	}
	else
	{
		RFormat = !in.iFormat;
	}

	if(RFormat) // decode IR for execution in the next cycle
	{
		in.iFormat = false;
		std::cout << std::endl << "ID: " << in.tag << std::endl << "Format: R" << std::endl << "OP_Code: " << toDecimal(in.op_code) << std::endl << "rs: " << toDecimal(in.rs) << std::endl << "rt: " << toDecimal(in.rt) << std::endl << "rd: " << toDecimal(in.rd) << std::endl;
		
		if(lockReg[toDecimal(in.rd)] != in.tag) // this instruction locked this register earlier, so don't prevent access
		{
			if(!equal(in.rd,zero) ) //do not attempt to write to zero.
			{
				if(lockReg[toDecimal(in.rd)] >= 0)
				{
					std::cout << "Attempt to access locked register " << toDecimal(in.rd) << ". Stalling" << std::endl;
					return;
				}
				else
				{
					std::cout << "Locking register " << toDecimal(in.rd) << std::endl;
					lockReg[toDecimal(in.rd)] = in.tag;
				}
			}
			else
			{
				std::cout << std::endl << "WARNING: Attempting to write to $0!" << std::endl;
			}
		}
		
		if(lockReg[toDecimal(in.rs)] >= 0)
		{
			std::cout << "Attempt to access locked register " << toDecimal(in.rs) << ". Stalling" << std::endl;
			in.bStalled = true;
			return;
		}
		else if(lockReg[toDecimal(in.rt)] >= 0)
		{
			std::cout << "Attempt to access locked register " << toDecimal(in.rt) << ". Stalling" << std::endl;
			in.bStalled = true;
			return;
		}
		else
		{
			getRegistryValue(in.arg1, in.rs); // retrieve registry values for use in next cycle
			getRegistryValue(in.arg2, in.rt);
		}
		
	}
	else
	{
		in.iFormat = true;
		//addu(in.arg1,zero,zero);
		//addu(in.arg2,zero,zero);
	
		std::cout << std::endl << "ID: " << in.tag << std::endl << "Format: I" << std::endl << "OP_Code: " << toDecimal(in.op_code) << std::endl << "rd: " << toDecimal(in.rd) << std::endl << "Immediate: " << toDecimal(in.immi) << std::endl;
	
		if(lockReg[toDecimal(in.rd)] != in.tag) // this instruction locked this register earlier, so don't prevent access
		{
			if(!equal(in.rd,zero) ) //do not attempt to write to zero
			{
				if(lockReg[toDecimal(in.rd)] >= 0)
				{
					std::cout << "Attempt to access locked register " << toDecimal(in.rd) << ". Stalling" << std::endl;
					in.bStalled = true;
					return;
				}
				else
				{
					std::cout << "Locking register " << toDecimal(in.rd) << std::endl;
					lockReg[toDecimal(in.rd)] = in.tag;
				}
			}
			else
			{
				std::cout << std::endl << "WARNING: Attempting to write to $0!" << std::endl;
			}
		}
	}	
	in.stage = EXE_EX; // send this instruction to the next stage for the next cycle
}

void IF(LatentInstruction& in)
{
	in.tag = instructionFetch();
	
	if(in.tag == -1)
	{
		//std::cout << std::endl << "Bad Instruction, removing" << std::endl;
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].tag == -1)
			{
				pendingInstructions.erase(pendingInstructions.begin() + i);
			}
		}
		return;
	}
	
	std::cout << std::endl << "IF: " << in.tag << " ";
	for(int i = 0; i < 32; i++)
	{
		std::cout << IR[31-i];
	}
	in.stage = EXE_ID; // send this instruction to the next stage for the next cycle
}

void tick()
{
	std::cout << std::endl << std::endl <<  "TICK BEGIN" <<  std::endl << std::endl;

	int max_instr = 5;
	int total_instr = 0;
	
	int max_cycles = 25;
	int cycle_count = 0;

	pendingInstructions.push_back(LatentInstruction());
	total_instr++;
	
	while(pendingInstructions.size() > 0 && cycle_count < max_cycles) // cycle
	{
		std::cout << std::endl << "CYCLE START" << std::endl;
		
		//std::cout << std::endl << "Instruction Stack: " << std::endl;
		//for(int i = 0; i < pendingInstructions.size(); i++)
		//{
		//		std::cout << pendingInstructions[i].tag << ": " << pendingInstructions[i].stage << std::endl; 
		//}
		
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].tag >= 0)
			{
				std::cout << std::endl << "Running instruction " << pendingInstructions[i].tag << " at stage " << pendingInstructions[i].stage << std::endl;
			}
			else
			{
				std::cout << std::endl << "Fetching next instruction" << std::endl;
			}
			
			switch(pendingInstructions[i].stage)
			{
				case EXE_IF:
					IF(pendingInstructions[i]);
					break;
				case EXE_ID:
					ID(pendingInstructions[i]);
					break;
				case EXE_EX:
					pendingInstructions[i].bActive = true; // active execution stage
					EX(pendingInstructions[i]);
					break;
				case EXE_MEM:
					pendingInstructions[i].bActive = false; // left execution
					MEM(pendingInstructions[i]);
					break;
				case EXE_WB:
					WB(pendingInstructions[i]);
					break;
				case EXE_END:
					pendingInstructions.erase(pendingInstructions.begin() + i);
					i -= 1; // set us back one				
					break;
				default:
					std::cout << "HIT DEFAULT CASE. FIX ME" << std::endl;
					break;
			}		
		}
		std::cout << std::endl << "CYCLE END" << std::endl;
		
		cycle_count++;
		
		/////// Get next instruction (up to five)
		if(total_instr < max_instr)
		{
			pendingInstructions.push_back(LatentInstruction());
			total_instr++;
		}
		///////
	}
	
	pendingInstructions.clear();
		
	std::cout << std::endl << std::endl << "TICK END Total Cycles: " << cycle_count <<  std::endl << std::endl;
}

bool* li(bool* output, bool immediate[16])
{
	addu(output, zero, immediate);
	
	//std::cout << std::endl << "li called: output = " << toDecimal(output) << " imm = " << toDecimal(immediate) << std::endl;
	
	return output;
}

bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8], bool immediate[16]) // reproduced to include immediate
{
	bool temp[8], temp2[8];
	bool temp3[8];
		
	addu(output,zero,zero);
	
//	mux(output, equal(op_code, OP_ADDU), addu(temp2,input1,input2),
//	mux(output, equal(op_code, OP_SUBU), subu(temp2,input1,input2),
//	mux(output, equal(op_code, OP_ADD), add(temp2,input1,input2),
//	mux(output, equal(op_code, OP_SUB), sub(temp2,input1,input2),
//	mux(output, equal(op_code, OP_NEG), negate_byte(temp2,input1),
//	mux(output, equal(op_code, OP_EQ), equal(temp2,input1,input2),
//	mux(output, equal(op_code, OP_LT), lessthan(temp2,input1,input2),
//	mux(output, equal(op_code, OP_GT), greaterthan(temp2,input1,input2),
//	mux(output, equal(op_code, OP_AND), and8(temp2,input1,input2),
//	mux(output, equal(op_code, OP_OR), or8(temp2,input1,input2), 
//	mux(output, equal(op_code, OP_SLL), shiftLeft(temp2,input1,input2),
//	mux(output, equal(op_code, OP_SRL), shiftRight(temp2, input1,input2),
//	mux(output, equal(op_code, OP_MULTU), multu(temp2,input1,input2), zero) ) ) ) ) ) ) ) ) ) ) ) );
//	mux(output, equal(op_code, OP_MULT), mult(temp2,input1,input2), zero) ) ) ) ) ) ) ) ) ) ) ) ) );
//	mux(output, equal(op_code, OP_DIV), div(temp2,input1,input2),
//	mux(output, equal(op_code, OP_DIVU), divu(temp2,input1,input2), zero))))))))))))))));
	
	if(equal(op_code, OP_ADDU))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return addu(output, input1, input2);
	}
	else if(equal(op_code, OP_SUBU))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return subu(output, input1, input2);
	}
	else if(equal(op_code, OP_ADD))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return add(output, input1, input2);
	}
	else if(equal(op_code, OP_SUB))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return sub(output, input1, input2);
	}
	else if(equal(op_code, OP_NEG))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return negate_byte(output, input1);
	}
	else if(equal(op_code, OP_EQ))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return equal(output, input1, input2);
	}
	else if(equal(op_code, OP_LT))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return lessthan(output, input1, input2);
	}
	else if(equal(op_code, OP_GT))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return greaterthan(output, input1, input2);
	}
	else if(equal(op_code, OP_AND))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return and8(output, input1, input2);
	}
	else if(equal(op_code, OP_OR))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return or8(output, input1, input2);
	}
	else if(equal(op_code, OP_SLL))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return shiftLeft(output, input1, input2);
	}
	else if(equal(op_code, OP_SRL))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return shiftRight(output, input1, input2);
	}
	else if(equal(op_code, OP_MULTU))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return multu(output, input1, input2);
	}
	else if(equal(op_code, OP_MULT))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return mult(output, input1, input2);
	}
	else if(equal(op_code, OP_DIVU))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return divu(output, input1, input2);
	}
	else if(equal(op_code, OP_DIV))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return div(output, input1, input2);
	}
	else if(equal(op_code, OP_LW))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{	
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bReadMem = true;
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return output; // don't care
	}
	else if(equal(op_code, OP_SW))
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{	
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bReadMem = false;
				pendingInstructions[i].bWriteMem = true;
				pendingInstructions[i].bWriteReg = false; 
			}
		}
		return output; // don't care
	}
	else if(equal(op_code, OP_LI));
	{
		for(int i = 0; i < pendingInstructions.size(); i++)
		{	
			if(pendingInstructions[i].bActive)
			{
				pendingInstructions[i].bWriteMem = false;
				pendingInstructions[i].bReadMem = false;
				pendingInstructions[i].bWriteReg = true;
			}
		}
		return li(output, immediate); // output the value to write
	}
	
	return output;
}

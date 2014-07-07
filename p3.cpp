#include "basic_functions.h"
#include <iostream>
#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"
#include "p6.h"
#include "p7.h"


using namespace std;

bool OP_ADDU[8]  = {0,0,0,0,1,0,0,0};
bool OP_SUBU[8]  = {1,0,0,0,1,0,0,0};
bool OP_ADD[8]   = {0,0,0,0,0,0,0,0};
bool OP_SUB[8]   = {1,0,0,0,0,0,0,0};
bool OP_NEG[8]   = {0,1,0,0,0,0,0,0};
bool OP_EQ[8]    = {1,1,0,0,0,0,0,0};
bool OP_LT[8]    = {0,0,1,0,0,0,0,0};
bool OP_GT[8]    = {1,0,1,0,0,0,0,0};
bool OP_AND[8]   = {0,1,1,0,0,0,0,0};
bool OP_OR[8]    = {1,1,1,0,0,0,0,0};
bool OP_SLL[8]   = {0,0,0,1,0,0,0,0};
bool OP_SRL[8]   = {1,0,0,1,0,0,0,0};
bool OP_MULTU[8] = {0,1,0,1,0,0,0,0};
bool OP_MULT[8]  = {1,1,0,1,0,0,0,0};
bool OP_DIVU[8]  = {0,0,1,1,0,0,0,0};
bool OP_DIV[8]   = {1,0,1,1,0,0,0,0};
bool OP_LW[8]	 = {0,1,1,1,0,0,0,0};
bool OP_SW[8]	 = {1,1,1,1,0,0,0,0};
bool OP_LI[8]	 = {0,1,1,1,1,0,0,0};

bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8])
{
	int i;
	bool temp[8], temp2[8];
	bool temp3[8];
	
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
		addu(output, input1, input2);
	}
	else if(equal(op_code, OP_SUBU))
	{
		subu(output, input1, input2);
	}
	else if(equal(op_code, OP_ADD))
	{
		add(output, input1, input2);
	}
	else if(equal(op_code, OP_SUB))
	{
		sub(output, input1, input2);
	}
	else if(equal(op_code, OP_NEG))
	{
		negate_byte(output, input1);
	}
	else if(equal(op_code, OP_EQ))
	{
		equal(output, input1, input2);
	}
	else if(equal(op_code, OP_LT))
	{
		lessthan(output, input1, input2);
	}
	else if(equal(op_code, OP_GT))
	{
		greaterthan(output, input1, input2);
	}
	else if(equal(op_code, OP_AND))
	{
		and8(output, input1, input2);
	}
	else if(equal(op_code, OP_OR))
	{
		or8(output, input1, input2);
	}
	else if(equal(op_code, OP_SLL))
	{
		shiftLeft(output, input1, input2);
	}
	else if(equal(op_code, OP_SRL))
	{
		shiftRight(output, input1, input2);
	}
	else if(equal(op_code, OP_MULTU))
	{
		multu(output, input1, input2);
	}
	else if(equal(op_code, OP_MULT))
	{
		mult(output, input1, input2);
	}
	else if(equal(op_code, OP_DIVU))
	{
		divu(output, input1, input2);
	}
	else if(equal(op_code, OP_DIV))
	{
		div(output, input1, input2);
	}

	return output;
}



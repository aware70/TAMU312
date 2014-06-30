#include "basic_functions.h"
#include <iostream>
#include "p1.h"
#include "p2.h"
#include "p3.h"
#include "p4.h"
#include "p5.h"

using namespace std;

bool OP_ADDU[8] = {0,0,0,0,1,0,0,0};
bool OP_SUBU[8] = {1,0,0,0,1,0,0,0};
bool OP_ADD[8]  = {0,0,0,0,0,0,0,0};
bool OP_SUB[8]  = {1,0,0,0,0,0,0,0};
bool OP_NEG[8]  = {0,1,0,0,0,0,0,0};
bool OP_EQ[8]   = {1,1,0,0,0,0,0,0};
bool OP_LT[8]   = {0,0,1,0,0,0,0,0};
bool OP_GT[8]   = {1,0,1,0,0,0,0,0};
bool OP_AND[8]  = {0,1,1,0,0,0,0,0};
bool OP_OR[8]   = {1,1,1,0,0,0,0,0};
bool OP_SLL[8]  = {0,0,0,1,0,0,0,0};
bool OP_SRL[8]  = {1,0,0,1,0,0,0,0};
bool OP_MULTU[8] = {0,1,0,1,0,0,0,0};
bool OP_MULT[8] = {1,1,0,1,0,0,0,0};
bool OP_DIVU[8] = {0,0,1,1,0,0,0,0};
bool OP_DIV[8]  = {1,0,1,1,0,0,0,0};

bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8])
{
	int i;
	bool temp[8], temp2[8];
	bool temp3[8];
	
	//shiftRight(temp3,input1, OP_EQ);
	//cout << endl;
	//for(i = 7; i >= 0; i--)
	//{
	//	cout << input1[i];
	//}
	//cout << endl;
	//for(i = 7; i >= 0; i--)
	//{
	//	cout << temp3[i];
	//}
	//cout << endl;
	
	mux(temp, equal(op_code, OP_ADDU), addu(temp2,input1,input2),
	mux(temp, equal(op_code, OP_SUBU), subu(temp2,input1,input2),
	mux(temp, equal(op_code, OP_ADD), add(temp2,input1,input2),
	mux(temp, equal(op_code, OP_SUB), sub(temp2,input1,input2),
	mux(temp, equal(op_code, OP_NEG), negate_byte(temp2,input1),
	mux(temp, equal(op_code, OP_EQ), equal(temp2,input1,input2),
	mux(temp, equal(op_code, OP_LT), lessthan(temp2,input1,input2),
	mux(temp, equal(op_code, OP_GT), greaterthan(temp2,input1,input2),
	mux(temp, equal(op_code, OP_AND), and8(temp2,input1,input2),
	mux(temp, equal(op_code, OP_OR), or8(temp2,input1,input2), 
	mux(temp, equal(op_code, OP_SLL), shiftLeft(temp2,input1,input2),
	mux(temp, equal(op_code, OP_SRL), shiftRight(temp2, input1,input2), 
	mux(temp, equal(op_code, OP_MULT), mult(temp2,input1,input2), zero) ) ) ) ) ) ) ) ) ) ) ) );
	//mux(temp, equal(op_code, OP_MULTU), multu(temp2,input1,input2),
	//mux(temp, equal(op_code, OP_DIV), div(temp2,input1,input2),
	//mux(temp, equal(op_code, OP_DIVU), divu(temp2,input1,input2), zero))))))))))))))));
	
	output[0] = temp[0];
	output[1] = temp[1];
	output[2] = temp[2];
	output[3] = temp[3];
	output[4] = temp[4];
	output[5] = temp[5];
	output[6] = temp[6];
	output[7] = temp[7];
	
	return output;
}


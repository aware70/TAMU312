#include "basic_functions.h"
#include "p1.h"
#include "p2.h"
#include "p3.h"

bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8])
{
	switch(BinToDec(op_code))
	{
		case OP_ADDU:
			return addu(output,input1,input2);
			break;
		case OP_SUBU:
			return subu(output,input1,input2);
			break;
		case OP_NEG:
			return negate_byte(output,input1);
			break;
		case OP_EQ:
			return equal(output,input1,input2);
			break;
		case OP_LT:
			return lessthan(output,input1,input2);
			break;
		case OP_GT:
			return greaterthan(output,input1,input2);
			break;
		case OP_AND:
			return and8(output,input1,input2);
			break;
		case OP_OR:
			return or8(output,input1,input2);
			break;
		case OP_SLL:
			return shiftLeft(output,input1);
			break;
		case OP_SRL:
			return shiftRight(output,input1);
			break;
		default:
			return zero;
			break;
	}
}


// CSCE 312 Programming Assignment
#include <stdexcept>
#include <iostream>

using namespace std;

static int functionCount;

// operation constants

bool zero[8] = {0,0,0,0,0,0,0,0};
bool one[8] = {1,0,0,0,0,0,0,0};

const unsigned int OP_ADDU = 16;
const unsigned int OP_SUBU = 17;
const unsigned int OP_ADD = 0;
const unsigned int OP_SUB = 1;
const unsigned int OP_NEG = 2;
const unsigned int OP_EQ = 3;
const unsigned int OP_LT = 4;
const unsigned int OP_GT = 5;
const unsigned int OP_AND = 6;
const unsigned int OP_OR = 7;
const unsigned int OP_SLL = 8;
const unsigned int OP_SRL = 9;

bool AND(bool a, bool b)
{
	functionCount++;
	return a && b;
}

bool OR(bool a, bool b)
{
	functionCount++;
	return a || b;
}

bool NOT(bool a)
{
	functionCount++;
	return !a;
}

bool XOR(bool a, bool b)
{
	return OR(AND(a,NOT(b)),AND(NOT(a),b));
}

bool MUX(bool operation, bool output_if_true, bool output_if_false)
{
	return OR(AND(output_if_true, NOT(operation)),AND(output_if_false,operation));
}

bool SUM(bool a, bool b)
{
	return XOR(a,b);
}

bool SUM(bool a, bool b, bool c)
{
	return SUM(SUM(a,b),c);
}

bool CarryOut(bool a, bool b, bool c)
{
	return OR(OR(AND(a,b),AND(a,c)),AND(b,c));
}

bool EQUAL(bool a, bool b)
{
	return NOT(SUM(a,b));
}

bool getValue(bool byte[8], int offset)
{
	return byte[offset];
}

void writeValue(bool* byte, int offset, bool value)
{
	byte[offset] = value;
}

void printValue(bool byte[8])
{
	for(int i = 7; i >= 0; i--)
	{
		cout << byte[i];
	}
	cout << endl;
}

int BinToDec(bool byte[8])
{
	int res = 0;
	for(int i = 0; i < 8; i++)
	{
		res += pow(2,i);
	}
	return res;
}

void printValueInDecimal(bool byte[8])
{
	cout << BinToDec(byte) << endl;
}

bool* mux(bool* output, bool operation, bool input1[8], bool input2[8])
{
	output[0] = MUX(operation, input1[0], input2[0]);
	output[1] = MUX(operation, input1[1], input2[1]);
	output[2] = MUX(operation, input1[2], input2[2]);
	output[3] = MUX(operation, input1[3], input2[3]);
	output[4] = MUX(operation, input1[4], input2[4]);
	output[5] = MUX(operation, input1[5], input2[5]);
	output[6] = MUX(operation, input1[6], input2[6]);
	output[7] = MUX(operation, input1[7], input2[7]);
	return output;
}

bool* addu(bool* output, bool input1[8], bool input2[8])
{
	output[0] = SUM(input1[0], input2[0], false);
	bool Carry = CarryOut(input1[0], input2[0], false);

	output[1] = SUM(input1[1], input2[1], Carry);
	Carry = CarryOut(input1[1], input2[1], Carry);

	output[2] = SUM(input1[2], input2[2], Carry);
	Carry = CarryOut(input1[2], input2[2], Carry);

	output[3] = SUM(input1[3], input2[3], Carry);
	Carry = CarryOut(input1[3], input2[3], Carry);

	output[4] = SUM(input1[4], input2[4], Carry);
	Carry = CarryOut(input1[4], input2[4], Carry);

	output[5] = SUM(input1[5], input2[5], Carry);
	Carry = CarryOut(input1[5], input2[5], Carry);

	output[6] = SUM(input1[6], input2[6], Carry);
	Carry = CarryOut(input1[6], input2[6], Carry);

	output[7] = SUM(input1[7], input2[7], Carry);
	Carry = CarryOut(input1[7], input2[7], Carry);

	output[8] = Carry;

	return output;
}

bool* negate(bool* output, bool input[8])
{
		output[0] = NOT(input[0]);
		output[1] = NOT(input[1]);
		output[2] = NOT(input[2]);
		output[3] = NOT(input[3]);
		output[4] = NOT(input[4]);
		output[5] = NOT(input[5]);
		output[6] = NOT(input[6]);
		output[7] = NOT(input[7]);

		bool one[8] = {1,0,0,0,0,0,0,0};
		bool temp[8];

		return addu(temp, output, one);
}

bool* subu(bool* output, bool input1[8], bool input2[8])
{
	return addu(output, input1, negate(output,input2));
}

bool equal(bool input1[8], bool input2[8])
{
	bool sum = SUM(input1[0],input2[0]);
	sum = SUM(sum, input1[1],input2[1]);
	sum = SUM(sum, input1[2],input2[2]);
	sum = SUM(sum, input1[3],input2[3]);
	sum = SUM(sum, input1[4],input2[4]);
	sum = SUM(sum, input1[5],input2[5]);
	sum = SUM(sum, input1[6],input2[6]);
	sum = SUM(sum, input1[7],input2[7]);
	return sum;
}

bool lessthan(bool input1[8], bool input2[8])
{
	bool* out;
	return AND(NOT(equal(input1,input2)),subu(out,input1,input2)[7]);
}

bool greaterthan(bool input1[8], bool input2[8])
{
	return lessthan(input2,input1);
}

bool* equal(bool* output, bool input1[8], bool input2[8])
{
	output[0] = EQUAL(input1[0],input2[0]);
	output[1] = EQUAL(input1[1],input2[1]);
	output[2] = EQUAL(input1[2],input2[2]);
	output[3] = EQUAL(input1[3],input2[3]);
	output[4] = EQUAL(input1[4],input2[4]);
	output[5] = EQUAL(input1[5],input2[5]);
	output[6] = EQUAL(input1[6],input2[6]);
	output[7] = EQUAL(input1[7],input2[7]);
	return output;
}

bool* lessthan(bool* output, bool input1[8], bool input2[8])
{
	output = mux(output,lessthan(input1, input2),one, zero);
	return output;
}

bool* greaterthan(bool* output, bool input1[8], bool input2[8])
{
	output = mux(output,greaterthan(input1, input2),one, zero);
	return output;
}

bool* and(bool* output, bool input1[8], bool input2[8])
{
	output[0] = AND(input1[0], input2[0]);
	output[1] = AND(input1[1], input2[1]);
	output[2] = AND(input1[2], input2[2]);
	output[3] = AND(input1[3], input2[3]);
	output[4] = AND(input1[4], input2[4]);
	output[5] = AND(input1[5], input2[5]);
	output[6] = AND(input1[6], input2[6]);
	output[7] = AND(input1[7], input2[7]);
	return output;
}

bool* or(bool* output, bool input1[8], bool input2[8])
{
	output[0] = OR(input1[0], input2[0]);
	output[1] = OR(input1[1], input2[1]);
	output[2] = OR(input1[2], input2[2]);
	output[3] = OR(input1[3], input2[3]);
	output[4] = OR(input1[4], input2[4]);
	output[5] = OR(input1[5], input2[5]);
	output[6] = OR(input1[6], input2[6]);
	output[7] = OR(input1[7], input2[7]);
	return output;
}

bool* shiftLeft(bool* output, bool input[8])
{
	output[0] = 0;
	output[1] = input[0];
	output[2] = input[1];
	output[3] = input[2];
	output[4] = input[3];
	output[5] = input[4];
	output[6] = input[5];
	output[7] = input[6];

	return output;
}

bool* shiftRight(bool* output, bool input[8])
{
	output[0] = input[1];
	output[1] = input[2];
	output[2] = input[3];
	output[3] = input[4];
	output[4] = input[5];
	output[5] = input[6];
	output[6] = input[7];
	output[7] = 0;

	return output;
}

void TestOverflow(bool Carry)
{
	if(Carry)
		throw runtime_error("Error: Overflow");
}

bool* add(bool* output, bool input1[8], bool input2[8])
{
	output[0] = SUM(input1[0], input2[0], 0);
	bool Carry = CarryOut(input1[0], input2[0], 0);

	output[1] = SUM(input1[1], input2[1], Carry);
	Carry = CarryOut(input1[1], input2[1], Carry);

	output[2] = SUM(input1[2], input2[2], Carry);
	Carry = CarryOut(input1[2], input2[2], Carry);

	output[3] = SUM(input1[3], input2[3], Carry);
	Carry = CarryOut(input1[3], input2[3], Carry);

	output[4] = SUM(input1[4], input2[4], Carry);
	Carry = CarryOut(input1[4], input2[4], Carry);

	output[5] = SUM(input1[5], input2[5], Carry);
	Carry = CarryOut(input1[5], input2[5], Carry);

	output[6] = SUM(input1[6], input2[6], Carry);
	Carry = CarryOut(input1[6], input2[6], Carry);

	output[7] = SUM(input1[7], input2[7], Carry);
	Carry = CarryOut(input1[7], input2[7], Carry);

	TestOverflow(Carry);

	return output;
}

bool* sub(bool* output, bool input1[8], bool input2[8])
{
	return add(output, input1, negate(output,input2));
}

bool* multu(bool* output, bool input1[8], bool input2[8])
{
	output[0] = 0;

	bool* a = input1;
	bool* b = input2;
	
	bool temp1[8];
	bool temp2[8];

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	addu(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	return output;
}

bool* multu(bool* output, bool input1[8], bool input2[8])
{
	output[0] = 0;

	bool* a = input1;
	bool* b = input2;
	
	bool temp1[8];
	bool temp2[8];

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	add(output, output, mux(temp1,greaterthan(b,zero), a, zero));
	a = shiftLeft(a, a);
	b = shiftRight(b, b);

	return output;
}

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
			return negate(output,input1);
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
			return and(output,input1,input2);
			break;
		case OP_OR:
			return or(output,input1,input2);
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


void printFunctionCount()
{
	cout << "FunctionCount: " << functionCount << endl;
}

void print(bool a)
{
	cout << "Result: " << a << endl;
}

void resetFunctionCount()
{
	functionCount = 0;
}

int main()
{
	try
	{
		resetFunctionCount();
		bool A = true;
		bool B = false;

		print(XOR(A,B));
		printFunctionCount();

		cout << "Press enter to exit..." << endl;
		cin.get();
		//	throw(runtime_error("ERROR: Invalid upper range!"));
		
		return 0;
	}
	
	catch(std::exception & ex)
	{
		cerr << ex.what() << endl;
		return 1;
	}
	
	catch(...)
	{
		cerr << "Unknown exception!" << endl;
		return 2;
	}
}
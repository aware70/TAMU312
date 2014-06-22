#include "basic_functions.h"
#include "p1.h"
#include "p2.h"

bool zero[8] = {0,0,0,0,0,0,0,0};
bool one[8] = {1,0,0,0,0,0,0,0};

bool* mux(bool* output, bool operation, bool input1[8], bool input2[8])
{
	output[0] = Mux(operation, input1[0], input2[0]);
	output[1] = Mux(operation, input1[1], input2[1]);
	output[2] = Mux(operation, input1[2], input2[2]);
	output[3] = Mux(operation, input1[3], input2[3]);
	output[4] = Mux(operation, input1[4], input2[4]);
	output[5] = Mux(operation, input1[5], input2[5]);
	output[6] = Mux(operation, input1[6], input2[6]);
	output[7] = Mux(operation, input1[7], input2[7]);
	return output;
}

bool* addu(bool* output, bool input1[8], bool input2[8])
{
	output[0] = Sum(input1[0], input2[0], false);
	bool Carry = CarryOut(input1[0], input2[0], false);

	output[1] = Sum(input1[1], input2[1], Carry);
	Carry = CarryOut(input1[1], input2[1], Carry);

	output[2] = Sum(input1[2], input2[2], Carry);
	Carry = CarryOut(input1[2], input2[2], Carry);

	output[3] = Sum(input1[3], input2[3], Carry);
	Carry = CarryOut(input1[3], input2[3], Carry);

	output[4] = Sum(input1[4], input2[4], Carry);
	Carry = CarryOut(input1[4], input2[4], Carry);

	output[5] = Sum(input1[5], input2[5], Carry);
	Carry = CarryOut(input1[5], input2[5], Carry);

	output[6] = Sum(input1[6], input2[6], Carry);
	Carry = CarryOut(input1[6], input2[6], Carry);

	output[7] = Sum(input1[7], input2[7], Carry);
	Carry = CarryOut(input1[7], input2[7], Carry);

	output[8] = Carry;

	return output;
}

bool* negate(bool* output, bool input[8])
{
		output[0] = Not(input[0]);
		output[1] = Not(input[1]);
		output[2] = Not(input[2]);
		output[3] = Not(input[3]);
		output[4] = Not(input[4]);
		output[5] = Not(input[5]);
		output[6] = Not(input[6]);
		output[7] = Not(input[7]);

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
	bool sum = Sum(input1[0],input2[0]);
	sum = Sum(sum, input1[1],input2[1]);
	sum = Sum(sum, input1[2],input2[2]);
	sum = Sum(sum, input1[3],input2[3]);
	sum = Sum(sum, input1[4],input2[4]);
	sum = Sum(sum, input1[5],input2[5]);
	sum = Sum(sum, input1[6],input2[6]);
	sum = Sum(sum, input1[7],input2[7]);
	return sum;
}

bool lessthan(bool input1[8], bool input2[8])
{
	bool* out;
	return And(Not(equal(input1,input2)),subu(out,input1,input2)[7]);
}

bool greaterthan(bool input1[8], bool input2[8])
{
	return lessthan(input2,input1);
}

bool* equal(bool* output, bool input1[8], bool input2[8])
{
	output[0] = Equal(input1[0],input2[0]);
	output[1] = Equal(input1[1],input2[1]);
	output[2] = Equal(input1[2],input2[2]);
	output[3] = Equal(input1[3],input2[3]);
	output[4] = Equal(input1[4],input2[4]);
	output[5] = Equal(input1[5],input2[5]);
	output[6] = Equal(input1[6],input2[6]);
	output[7] = Equal(input1[7],input2[7]);
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

bool* and8(bool* output, bool input1[8], bool input2[8])
{
	output[0] = And(input1[0], input2[0]);
	output[1] = And(input1[1], input2[1]);
	output[2] = And(input1[2], input2[2]);
	output[3] = And(input1[3], input2[3]);
	output[4] = And(input1[4], input2[4]);
	output[5] = And(input1[5], input2[5]);
	output[6] = And(input1[6], input2[6]);
	output[7] = And(input1[7], input2[7]);
	return output;
}

bool* or8(bool* output, bool input1[8], bool input2[8])
{
	output[0] = Or(input1[0], input2[0]);
	output[1] = Or(input1[1], input2[1]);
	output[2] = Or(input1[2], input2[2]);
	output[3] = Or(input1[3], input2[3]);
	output[4] = Or(input1[4], input2[4]);
	output[5] = Or(input1[5], input2[5]);
	output[6] = Or(input1[6], input2[6]);
	output[7] = Or(input1[7], input2[7]);
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


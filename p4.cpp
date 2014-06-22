#include <stdexcept>
#include "basic_functions.h"
#include "p1.h"
#include "p2.h"
#include "p4.h"

using namespace std;

void TestOverflow(bool Carry)
{
	if(Carry)
		throw runtime_error("Error: Overflow");
}

bool* add(bool* output, bool input1[8], bool input2[8])
{
	output[0] = Sum(input1[0], input2[0], 0);
	bool Carry = CarryOut(input1[0], input2[0], 0);

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

	TestOverflow(Carry);

	return output;
}

bool* sub(bool* output, bool input1[8], bool input2[8])
{
	return add(output, input1, negate(output,input2));
}


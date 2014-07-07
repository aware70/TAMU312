#include <stdexcept>
#include "basic_functions.h"
#include "p1.h"
#include "p2.h"
#include "p4.h"

using namespace std;

void TestOverflow(bool Carry)
{
	//if(Carry)
		//throw "Overflow!";
}

bool* add(bool* output, bool input1[8], bool input2[8])
{
	bool temp[16];
	
	temp[0] = Sum(input1[0], input2[0]);
	bool Carry = CarryOut(input1[0], input2[0]);

	temp[1] = Sum(input1[1], input2[1], Carry);
	Carry = CarryOut(input1[1], input2[1], Carry);

	temp[2] = Sum(input1[2], input2[2], Carry);
	Carry = CarryOut(input1[2], input2[2], Carry);

	temp[3] = Sum(input1[3], input2[3], Carry);
	Carry = CarryOut(input1[3], input2[3], Carry);

	temp[4] = Sum(input1[4], input2[4], Carry);
	Carry = CarryOut(input1[4], input2[4], Carry);

	temp[5] = Sum(input1[5], input2[5], Carry);
	Carry = CarryOut(input1[5], input2[5], Carry);

	temp[6] = Sum(input1[6], input2[6], Carry);
	Carry = CarryOut(input1[6], input2[6], Carry);

	temp[7] = Sum(input1[7], input2[7], Carry);
	Carry = CarryOut(input1[7], input2[7], Carry);
	
	output[0] = temp[0];
	output[1] = temp[1];
	output[2] = temp[2];
	output[3] = temp[3];
	output[4] = temp[4];
	output[5] = temp[5];
	output[6] = temp[6];
    output[7] = temp[7];

	if(Carry)
	{
		//throw std::exception();
	}

	return output;
}

bool* sub(bool* output, bool input1[8], bool input2[8])
{
	bool temp[8];
	
	return add(output, input1, negate_byte(temp,input2));
}


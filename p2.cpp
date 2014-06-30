#include "basic_functions.h"
#include "p1.h"
#include "p2.h"
#include "test_utils.h"

bool zero[8] = {0,0,0,0,0,0,0,0};
bool one[8] = {1,0,0,0,0,0,0,0};
bool two[8] = {0,1,0,0,0,0,0,0};
bool three[8] = {1,1,0,0,0,0,0,0};
bool four[8] = {0,0,1,0,0,0,0,0};
bool five[8] = {1,0,1,0,0,0,0,0};
bool six[8] = {0,1,1,0,0,0,0,0};
bool seven[8] = {1,1,1,0,0,0,0,0};
bool eight[8] = {0,0,0,1,0,0,0,0};

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
	output[0] = Sum(input1[0], input2[0]);
	bool Carry = CarryOut(input1[0], input2[0]);

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
	//Carry = CarryOut(input1[7], input2[7], Carry);

	//output[8] = Carry;

	return output;
}

bool* negate_byte(bool* output, bool input[8])
{
		bool temp[8];

		temp[0] = Not(input[0]);
		temp[1] = Not(input[1]);
		temp[2] = Not(input[2]);
		temp[3] = Not(input[3]);
		temp[4] = Not(input[4]);
		temp[5] = Not(input[5]);
		temp[6] = Not(input[6]);
		temp[7] = Not(input[7]);
				
		addu(output, temp, one);
		
		return output;
}

bool* subu(bool* output, bool input1[8], bool input2[8])
{
	bool temp[8];
	negate_byte(temp,input2);
	return addu(output, input1, temp);
}

bool equal(bool input1[8], bool input2[8])
{	
	bool ans[8];
	ans[0] = Not(Sum(input1[0],input2[0]));
	ans[1] = Not(Sum(input1[1],input2[1]));
	ans[2] = Not(Sum(input1[2],input2[2]));
	ans[3] = Not(Sum(input1[3],input2[3]));
	ans[4] = Not(Sum(input1[4],input2[4]));
	ans[5] = Not(Sum(input1[5],input2[5]));
	ans[6] = Not(Sum(input1[6],input2[6]));
	ans[7] = Not(Sum(input1[7],input2[7]));
	return And(And(And(And(And(And(And(ans[0],ans[1]),ans[2]),ans[3]),ans[4]),ans[5]),ans[6]),ans[7]);
}

bool lessthan(bool input1[8], bool input2[8])
{
	bool isNeg[7];
	bool NE = Not(equal(input1,input2));
	subu(isNeg, input1,input2);
	
	return And(isNeg[7], NE);
	//return true;
}

bool greaterthan(bool input1[8], bool input2[8])
{
	bool NE = Not(equal(input1,input2));

	return NE && lessthan(input2,input1);
}

bool* equal(bool* output, bool input1[8], bool input2[8])
{
	//output[0] = Equal(input1[0],input2[0]);
	//output[1] = Equal(input1[1],input2[1]);
	//output[2] = Equal(input1[2],input2[2]);
	//output[3] = Equal(input1[3],input2[3]);
	//output[4] = Equal(input1[4],input2[4]);
	//output[5] = Equal(input1[5],input2[5]);
	//output[6] = Equal(input1[6],input2[6]);
	//output[7] = Equal(input1[7],input2[7]);
	output[0] = equal(input1, input2);
	
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

bool* shiftLeft(bool* output, bool input[8], bool shift[8])
{
	bool temp1[8], temp2[8], temp3[8];

	mux(temp2, equal(shift,zero), input,
	mux(temp2, equal(shift,one), shiftLeft(temp1, input),
	mux(temp2, equal(shift,two), shiftLeft(temp1,shiftLeft(temp3,input)),
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,input))),
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,input)))),
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,input))))),
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,input)))))),	
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,input))))))),
	mux(temp2, equal(shift,three), shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,shiftLeft(temp1,shiftLeft(temp3,input)))))))), zero ) ) ) ) ) ) ) ) );
	
	output[0] = temp2[0];
	output[1] = temp2[1];
	output[2] = temp2[2];
	output[3] = temp2[3];
	output[4] = temp2[4];
	output[5] = temp2[5];
	output[6] = temp2[6];
	output[7] = temp2[7];
	

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

bool* shiftRight(bool* output, bool input[8], bool shift[8])
{
	bool temp1[8], temp2[8], temp3[8];

	mux(temp2, equal(shift,zero), input,
	mux(temp2, equal(shift,one), shiftRight(temp1, input),
	mux(temp2, equal(shift,two), shiftRight(temp1,shiftRight(temp3,input)),
	mux(temp2, equal(shift,three), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,input))),
	mux(temp2, equal(shift,four), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,input)))),
	mux(temp2, equal(shift,five), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,input))))),
	mux(temp2, equal(shift,six), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,input)))))),	
	mux(temp2, equal(shift,seven), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,input))))))),
	mux(temp2, equal(shift,eight), shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,shiftRight(temp1,shiftRight(temp3,input)))))))), zero ) ) ) ) ) ) ) ) );
	
	output[0] = temp2[0];
	output[1] = temp2[1];
	output[2] = temp2[2];
	output[3] = temp2[3];
	output[4] = temp2[4];
	output[5] = temp2[5];
	output[6] = temp2[6];
	output[7] = temp2[7];
	
}

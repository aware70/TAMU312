#include "basic_functions.h"
#include <iostream>
#include "p1.h"
#include "p2.h"
#include "p4.h"
#include "p5.h"

bool* multu(bool* output, bool input1[8], bool input2[8])
{
	bool temp1[16];
	bool temp2[8];
	bool temp3[16];
	bool temp4[16];
	bool temp5[8];
	bool temp6[16];
	bool temp[16];
	
	
	for(int i = 0; i < 8; i++)     // Take out the trash!
	{                              // Take out the trash!
		temp1[i] = input1[i];      // Take out the trash!
		temp1[i+8] = input1[7];            // sign extend
	}                              // Take out the trash!
	                               // Take out the trash!
	addu(temp2, zero, zero);       // Take out the trash!
	addu(temp2, input2, zero);     // Take out the trash!
	addu(temp, zero, zero);        // Take out the trash!
	
//	std::cout << std::endl << "Input1:" << std::endl;
//	for(int j = 0; j < 16; j++)
//	{
//		std::cout << temp1[15-j];
//	}
//	std::cout << std::endl << "Input2:" << std::endl;
//	for(int j = 0; j < 8; j++)
//	{
//		std::cout << temp2[7-j];
//	}
	
	for(int i = 0; i < 8; i++)
	{	
		if( Equal(temp2[0], 1) )
		{
//			std::cout << std::endl << "Adding ";
//			std::cout << std::endl;
//			for(int j = 0; j < 16; j++)
//			{
//				std::cout << temp1[15-j];
//			}
//			std::cout << std::endl;
//			for(int j = 0; j < 16; j++)
//			{
//				std::cout << temp[15-j];
//			}
			addu(temp6, temp, zero); // annoying hack because assigning vars to themselves (like +=) doesn't work >:(
			addu(temp, temp6, temp1);
//			std::cout << std::endl << "Result:" << std::endl;
//			for(int j = 0; j < 16; j++)
//			{
//				std::cout << temp[15-j];
//			}	
		}
		addu(temp4, temp1, zero);
		shiftLeft(temp1, temp4); 
		addu(temp5, temp2, zero);
		shiftRight(temp2, temp5);
	}
	
//	std::cout << std::endl << "FINAL RESULT:" << std::endl;
//	for(int j = 0; j < 16; j++)
//	{
//		std::cout << temp[15-j];
//	}	
//	std::cout << std::endl;
   
	output[0] = temp[0];
	output[1] = temp[1];
	output[2] = temp[2];
	output[3] = temp[3];
	output[4] = temp[4];
	output[5] = temp[5];
	output[6] = temp[6];
	output[7] = temp[7];
	output[8] = temp[8];
	output[9] = temp[9];
	output[10] = temp[10];
	output[11] = temp[11];
	output[12] = temp[12];
	output[13] = temp[13];
	output[14] = temp[14];
	output[15] = temp[15];
	
	return output;
}

bool* mult(bool* output, bool input1[16], bool input2[8])
{
	bool temp1[16];
	bool temp2[8];
	bool temp3[16];
	
	for(int i = 0; i < 8; i++)
	{
		temp1[i] = input1[i];
		temp1[i+8] = input1[7]; // sign extend
	}
	
	addu(temp2, input2, zero);

	if(temp2[7]) // if multiplier is negative
	{
		negate_byte(temp1, temp1);
		negate_byte(temp2, temp2);
	}
	
	multu(output, temp1, temp2);
		
	return output;
}

bool* divu(bool* output, bool input1[8], bool input2[8])
{
	bool R[8];
	bool Q[8];
	bool temp1[8];
	bool temp2[8];
	bool temp3[8];

	
	addu(temp1, input1, zero);
	addu(temp2, input2, zero);
	
	for(int i = 0; i < 8; i++)
	{
		R[i] = 0;
		Q[i] = 0;
	}
	
	
	for(int i = 7; i >= 0; i--)
	{
		shiftLeft(temp3, R);
		addu(R, temp3, zero);
		R[0] = temp1[i];
		if(Or(greaterthan(R, input2),equal(R,input2)))
		{
			subu(temp3,R,input2);
			addu(R, temp3, zero);
			Q[i] = 1;
		}
	}
	
	for(int i = 0; i < 8; i++)
	{
		output[i] = Q[i];
		output[i+8] = R[i];
	}
	
	return output;
}

bool* div(bool* output, bool input1[8], bool input2[8])
{
	bool temp1[16];
	bool temp2[8];
	bool temp3[16];
	
	for(int i = 0; i < 8; i++)
	{
		temp1[i] = input1[i];
		temp1[i+8] = input1[7]; // sign extend
	}
	
	addu(temp2, input2, zero);

	if(temp2[7]) // if multiplier is negative
	{
		negate_byte(temp1, temp1);
		negate_byte(temp2, temp2);
	}
	
	divu(output, temp1, temp2);
		
	return output;
}


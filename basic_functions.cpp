#include <cmath>
#include <iostream>
#include "basic_functions.h"

using namespace std;

int functionCount;

bool And(bool a, bool b)
{
  functionCount++;
  return a && b;
}

bool Or(bool a, bool b)
{
  functionCount++;
  return a || b;
}

bool Not(bool a)
{
  functionCount++;
  return !a;
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


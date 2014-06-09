// CSCE 312 Programming Assignment
#include <stdexcept>
#include <iostream>

using namespace std;

static int functionCount;

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
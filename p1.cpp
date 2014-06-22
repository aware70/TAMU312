#include "basic_functions.h"
#include "p1.h"

bool XOR(bool a, bool b)
{
	return Or(And(a,Not(b)),And(Not(a),b));
}

bool Mux(bool operation, bool output_if_true, bool output_if_false)
{
	return Or(And(output_if_true, Not(operation)),And(output_if_false,operation));
}

bool Sum(bool a, bool b)
{
	return XOR(a,b);
}

bool Sum(bool a, bool b, bool c)
{
	return Sum(Sum(a,b),c);
}

bool CarryOut(bool a, bool b, bool c)
{
	return Or(Or(And(a,b),And(a,c)),And(b,c));
}

bool Equal(bool a, bool b)
{
	return Not(Sum(a,b));
}


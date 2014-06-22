#include "basic_functions.h"
#include "p1.h"
#include "p2.h"
#include "p4.h"
#include "p5.h"

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

bool* mult(bool* output, bool input1[8], bool input2[8])
{
  // TODO
  return output;
}

bool* divu(bool* output, bool input1[8], bool input2[8])
{
  // TODO
  return output;
}

bool* div(bool* output, bool input1[8], bool input2[8])
{
  // TODO
  return output;
}


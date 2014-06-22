// Functions required by Problem 3.
//
// In your p3.cpp, you should include basic_functions.h, p1.h, and so on, if you
// want to use previously built functions.
//
// Note that you can only use functions you have built so far.

#ifndef P3_H
#define P3_H

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

// An ALU. An pointer to output should be returned as the return value.
bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8]);

#endif // P3_H


// Functions required by Problem 3.
//
// In your p3.cpp, you should include basic_functions.h, p1.h, and so on, if you
// want to use previously built functions.
//
// Note that you can only use functions you have built so far.

#ifndef P3_H
#define P3_H

extern bool OP_ADDU[8];
extern bool OP_SUBU[8];
extern bool OP_ADD[8];  
extern bool OP_SUB[8];  
extern bool OP_NEG[8];  
extern bool OP_EQ[8];   
extern bool OP_LT[8];   
extern bool OP_GT[8];   
extern bool OP_AND[8];  
extern bool OP_OR[8];   
extern bool OP_SLL[8];  
extern bool OP_SRL[8];  
extern bool OP_MULTU[8];
extern bool OP_MULT[8]; 
extern bool OP_DIVU[8]; 
extern bool OP_DIV[8]; 
extern bool OP_LW[8];
extern bool OP_SW[8];
extern bool OP_LI[8];

// An ALU. An pointer to output should be returned as the return value.
bool* alu(bool op_code[8], bool* output, bool input1[8], bool input2[8]);

#endif // P3_H


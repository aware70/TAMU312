This is my shot at the pipeline, which I'm rather proud of. I sort of cheated by caching instructions in a vector of a struct I created. However, temporary actual data (like
the contents of a register) is only stored in that struct between the ID and EXE stages (which I think I could get rid of, but the assignment suggested I do that so what the
heck.


The hazards in the test program are handled by stalling.

li $4, 11
li $5, 13
addu $6, $4, $5
sw $6, 10

A step by step walkthrough of how my program handles this:

Tick 1

Cycle 1

IF: li $4, 11  // fetch the 32 bit binary number representing this from instruction memory into IR

Cycle 2

ID: li $4, 11 // grab the contents of IR and use it to prepare arguments to pass to the ALU (op_code, input1, input2, etc.) This is and R format instruction, so LOCK the
	      // destination register of this instruction to THIS instruction. If this register is already locked, STALL this instruction at this stage.

IF: li $5, 13 // fetch the 32 bit binary number representing this from instruction memory into IR

Cycle 3

EX: li $4, 11// perform ALU operation, set mem and reg read/write flags in this instruction and cache the result with this instruction. In this case, we perform use the equivalent of addi out, $0, 11

ID: li $5, 13 // grab the contents of IR and use it to prepare arguments to pass to the ALU (op_code, input1, input2, etc.) This is and I format instruction, so LOCK the
	      // destination register of this instruction to THIS instruction. If this register is already locked, STALL this instruction at this stage.

IF: addu $6, $4, $5 // fetch the 32 bit binary number representing this from instruction memory into IR

Cycle 4

MEM: li $4, 11 // perform any read/write (but not both!) operations here. None in this case.

EX: li $5, 13 // perform ALU operation, set mem and reg read/write flags in this instruction and cache the result with this instruction. In this case, we perform use the equivalent of addi out, $0, 13

ID: addu $6, $4, $5 // grab the contents of IR and use it to prepare arguments to pass to the ALU (op_code, input1, input2, etc.) This is an R format instruction.
	            // Lock the destination register to THIS instruction. If this register is already locked, STALL this instruction at this stage. 
		    // Otherwise, check the two source registers of this instruction. If they are locked, a previous instruction will need to finish writing them before we may use them.
		    // STALL if either source is locked. Notice that at this point we have reserved our destination (preventing SW from executing until we are finished and from locking
		    // us out (because the destination is locked to this instruction specifically)) even though we have STALLED because both $4 and $5 are locked. 


IF: sw $6, 10 // fetch the 32 bit binary number representing this from instruction memory into IR

Cycle 5

WB: li $4, 11 // if this instruction's bWriteReg flag is set, write the result of the ALU operation to this instruction's destination register and unlock it

MEM: li $5, 13 // perform any read/write (but not both!) operations here. None in this case.

STALL ID: addu $6, $4, $5 // because we have stalled, we don't parse IR. Instead we check our source registries, one of which ($4) is still locked.

ID: sw $6, 10 // grab the contents of IR and use it to prepare arguments to pass to the ALU (op_code, input1, input2, etc.) This is an R format instruction.
	      // Lock the destination register to THIS instruction. If this register is already locked, STALL this instruction at this stage. 
              // Otherwise, check the two source registers of this instruction. If they are locked, a previous instruction will need to finish writing them before we may use them.
	      // STALL if either source is locked. Notice that at this point we have reserved our destination (preventing SW from executing until we are finished and from locking
	      // us out (because the destination is locked to this instruction specifically)) even though we have STALLED because both $4 and $5 are locked.

Cycle 6

WB: li $5, 13 // if this instruction's bWriteReg flag is set, write the result of the ALU operation to this instruction's destination register and unlock it

STALL ID: addu $6, $4, $5 // because we have stalled, we don't parse IR. Instead we check our source registries, one of which ($4) is still locked.

STALL ID: sw $6, 10 // because we have stalled, we don't parse IR. Instead we check our destination registry, which ($6) is still locked.

Cycle 7

ID: addu $6, $4, $5 // because we have stalled, we don't parse IR. Instead we check our source registries, which are no longer locked and cache them for use in EX. We now proceed to EX.

STALL ID: sw $6, 10 // because we have stalled, we don't parse IR. Instead we check our destination registry, which ($6) is still locked.

Cycle 8

EXE: addu $6, $4, $5 // the ALU takes the cached values from the source registries, performs addu on them, and caches the result. Also sets bRegWrite = 1 for this instruction

STALL ID: sw $6, 10 // because we have stalled, we don't parse IR. Instead we check our destination registry, which ($6) is still locked.

Cycle 9

MEM: addu $6, $4, $5 // perform any read/write (but not both!) operations here. None in this case.

STALL ID: sw $6, 10 // because we have stalled, we don't parse IR. Instead we check our destination registry, which ($6) is still locked.

Cycle 10

WB: addu $6, $4, $5 // the cached ALU result is written to $6, and $6 is unlocked.

STALL ID: sw $6, 10 // because we have stalled, we don't parse IR. Instead we check our destination registry, which is no longer locked locked. We now proceed to EX.

Cycle 11

EX: sw $6, 10 // the ALU sets bWriteReg = 0, bReadMem = 0, bWriteMem = 1 for this instruction. The contents of $6 is placed in the ALU's output.

Cycle 12

MEM: sw $6, 10 // the cached output from the ALU is stored in memory location 10

Cycle 13

WB:  sw $6, 10 // no registry is written to (bRegWrite = 0)

	    

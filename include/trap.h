#ifndef _TRAP_H_
#define _TRAP_H_

#ifndef __ASSEMBLER__

#include <types.h>

struct Trapframe {
	/* Saved main processor registers. */
	unsigned long regs[32];

	/* Saved special registers. */
	unsigned long cp0_status;
	unsigned long hi;
	unsigned long lo;
	unsigned long cp0_badvaddr;
	unsigned long cp0_cause;
	unsigned long cp0_epc;
};

void print_tf(struct Trapframe *tf);

#endif /* !__ASSEMBLER__ */

/*
 * Stack layout for all exceptions
 */

#define TF_REG0 0						//$0
#define TF_REG1 ((TF_REG0) + 4)			//at
#define TF_REG2 ((TF_REG1) + 4)			//v0
#define TF_REG3 ((TF_REG2) + 4)			//v1
#define TF_REG4 ((TF_REG3) + 4)			//a0
#define TF_REG5 ((TF_REG4) + 4)			//a1
#define TF_REG6 ((TF_REG5) + 4)			//a2
#define TF_REG7 ((TF_REG6) + 4)			//a3
#define TF_REG8 ((TF_REG7) + 4)			//t0
#define TF_REG9 ((TF_REG8) + 4)
#define TF_REG10 ((TF_REG9) + 4)
#define TF_REG11 ((TF_REG10) + 4)
#define TF_REG12 ((TF_REG11) + 4)
#define TF_REG13 ((TF_REG12) + 4)
#define TF_REG14 ((TF_REG13) + 4)
#define TF_REG15 ((TF_REG14) + 4)		//t7
#define TF_REG16 ((TF_REG15) + 4)		//s0
#define TF_REG17 ((TF_REG16) + 4)
#define TF_REG18 ((TF_REG17) + 4)
#define TF_REG19 ((TF_REG18) + 4)
#define TF_REG20 ((TF_REG19) + 4)
#define TF_REG21 ((TF_REG20) + 4)
#define TF_REG22 ((TF_REG21) + 4)
#define TF_REG23 ((TF_REG22) + 4)		//s7
#define TF_REG24 ((TF_REG23) + 4)		//t8
#define TF_REG25 ((TF_REG24) + 4)		//t9
/*
 * $26 (k0) and $27 (k1) not saved
 */
#define TF_REG26 ((TF_REG25) + 4)
#define TF_REG27 ((TF_REG26) + 4)
#define TF_REG28 ((TF_REG27) + 4)		//gp
#define TF_REG29 ((TF_REG28) + 4)		//sp
#define TF_REG30 ((TF_REG29) + 4)		//fp
#define TF_REG31 ((TF_REG30) + 4)		//ra

#define TF_STATUS ((TF_REG31) + 4)

#define TF_HI ((TF_STATUS) + 4)
#define TF_LO ((TF_HI) + 4)

#define TF_BADVADDR ((TF_LO) + 4)
#define TF_CAUSE ((TF_BADVADDR) + 4)
#define TF_EPC ((TF_CAUSE) + 4)
/*
 * Size of stack frame, word/double word alignment
 */
#define TF_SIZE ((TF_EPC) + 4)
#endif /* _TRAP_H_ */

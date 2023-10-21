#define LATHECPP_FPGA

#include <neorv32.h>
#include "fpgaLatheReg.h"
#include "fpgaLatheBits.h"

#define EXT extern
#include "fpga.h"

#if defined(FPGA_INCLUDE)	// <-

typedef volatile struct __attribute__((packed,aligned(4))) S_CFS
{
 uint32_t rsv;
 uint32_t ctl;
 uint32_t data;
 uint32_t op;
} T_CFS, *P_CFS;

#define CFS ((T_CFS *) (NEORV32_CFS_BASE))

void ld(int op, int val);
uint32_t rd(int op);

void nopW(void);
void nopR(void);

#endif	/* FPGA_INCLUDE */ // ->
#ifdef LATHECPP_FPGA

inline void nopW(void)
{
 asm volatile ("	li	t1, 4");
 asm volatile ("1:	addi	t1, t1, -1");
 asm volatile ("	bgtz	t1, 1b");
}

inline void nopR(void)
{
 asm volatile ("	li	t1, 5");
 asm volatile ("1:	addi	t1, t1, -1");
 asm volatile ("	bgtz	t1, 1b");
}

void ld(int op, int val)
{
 CFS->data = val;
 CFS->op = op;
 nopW();
}

uint32_t rd(int op)
{
 CFS->op= 0x100 | op;
 nopR();
 return CFS->data;
}

#endif  /* LATHECPP_FPGA */
#define LATHECPP_FPGA

#include <neorv32.h>
// #include "fpgaLatheReg.h"
// #include "fpgaLatheBits.h"

#define EXT extern
#include "Lathe.h"
#include "fpga.h"
#include "dbgSerial.h"

#if defined(FPGA_INCLUDE)	// <-

typedef volatile struct __attribute__((packed,aligned(4))) S_CFS
{
 uint32_t rsv;			/* 0 */
 uint32_t ctl;			/* 1 */
 uint32_t data;			/* 2 */
 uint32_t op;			/* 3 */
 uint32_t millis;		/* 4 */
 uint32_t zMpg;			/* 5 */
 uint32_t xMpg;			/* 6 */
 uint32_t inPin;		/* 7 */
 uint32_t dbg;			/* 8 */
} T_CFS, *P_CFS;

#define MPG_MASK  0x7f
#define MPG_DIR   0x80
#define MPG_EMPTY 0x100

#define CFS ((T_CFS *) (NEORV32_CFS_BASE))

static inline uint32_t millis(void)
{
 return CFS->millis;
}

void ld(int op, int val);
uint32_t rd(int op);

void nopW(void);
void nopR(void);

#endif	/* FPGA_INCLUDE */ // ->
#if defined(LATHECPP_FPGA)

extern inline uint32_t millis(void);

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

#include "fpgaLatheStr.h"

void ld(const int op, const int val)
{
 if (dbg1) {
  dbgPutStr("ld");
  dbgPutHexByte(op);
  dbgPutSpace();
  const char *p = &fpgaOpStr[op].c0;
  dbgPutC(*p++);
  dbgPutC(*p++);
  dbgPutC(*p++);
  dbgPutC(*p);

  dbgPutHex(val, 4);
  dbgPutInt(val);
  dbgNewLine(); }

 CFS->data = val;
 CFS->op = op;
 nopW();
}

uint32_t rd(const int op)
{
 CFS->op= 0x100 | op;
 nopR();
 return CFS->data;
}

#endif  /* LATHECPP_FPGA */

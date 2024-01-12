#if !defined(FPGA_INC)	// <-
#define FPGA_INC

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

#endif  /* FPGA_INC */	// ->

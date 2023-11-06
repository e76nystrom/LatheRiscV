#if !defined(FPGA_INC)	// <-
#define FPGA_INC

typedef volatile struct __attribute__((packed,aligned(4))) S_CFS
{
 uint32_t rsv;
 uint32_t ctl;
 uint32_t data;
 uint32_t op;
 uint32_t millis;
} T_CFS, *P_CFS;

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

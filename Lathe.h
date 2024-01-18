#if !defined(LATHE_INC)	// <-
#define LATHE_INC

#define dbg0 1			/* Lathe.c */
#define dbg1 1			/* fpga.c */
#define dbg2 1			/* remCmd.c */
#define dbg3 1			/* axisctl.c */
#define dbg3L 1			/* axisctl.c loc */
#define dbgM 1			/* dbgMsg() */
#define dbgM0 1           	/* dbgMsg() debug prints */

typedef struct S_CH2
{
 char c0;
 char c1;
} T_CH2, *P_CH2;

#define printf neorv32_uart0_printf

int main(void);

#endif  /* LATHE_INC */	// ->

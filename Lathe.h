#if !defined(LATHE_INC)	// <-
#define LATHE_INC

#define dbg0 0			/* Lathe.c */
#define dbg1 0			/* fpga.c */
#define dbg2 0			/* remCmd.c */
#define dbg3 0			/* axisctl.c */
#define dbgM 1			/* dbgMsg() */
#define dbgM0 0           	/* dbgMsg() debug prints */

typedef struct S_CH2
{
 char c0;
 char c1;
} T_CH2, *P_CH2;

#define printf neorv32_uart0_printf

int main(void);

#endif  /* LATHE_INC */	// ->

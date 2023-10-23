#if !defined(LATHE_INC)	// <-
#define LATHE_INC

typedef struct S_CH2
{
 char c0;
 char c1;
} T_CH2, *P_CH2;

#define printf neorv32_uart0_printf

int main(void);

#endif  /* LATHE_INC */	// ->

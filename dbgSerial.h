#if !defined(DBGSERIAL_INC)	// <-
#define DBGSERIAL_INC

void dbgPutC(char c);
void dbgNewLine(void);
void dbgPutStr(const char *p);
void dbgPutDigit(char ch);
void dbgPutHex(unsigned int val, int size);


#endif  /* DBGSERIAL_INC */	// ->

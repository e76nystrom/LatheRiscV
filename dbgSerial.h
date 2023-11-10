#if !defined(DBGSERIAL_INC)	// <-
#define DBGSERIAL_INC

void dbgPutC(char c);
void dbgPutSpace(void);
void dbgNewLine(void);
void dbgPutStr(const char *p);
void dbgPutDigit(char ch);
void dbgPutHexByte(char ch);
void dbgPutHex(unsigned int val, int size);

char* dbgFmtNum(char *buf, int val);

#endif  /* DBGSERIAL_INC */	// ->

#if !defined(DBG_SERIAL_INC)	// <-
#define DBG_SERIAL_INC

void dbgPutC(char c);
void dbgPutSpace(void);
void dbgNewLine(void);
void dbgPutStr(const char *p);
void dbgPutDigit(char ch);
void dbgPutHexByte(char ch);
void dbgPutHex(unsigned int val, int size);

char* dbgFmtNum(char *buf, int val);
void dbgPutInt(int val);

#endif  /* DBG_SERIAL_INC */	// ->

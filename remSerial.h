#if !defined(REM_SERIAL_INC)	// <-
#define REM_SERIAL_INC

#define REM_TX_SIZE 100
#define REM_RX_SIZE 128

typedef struct
{
 int tx_ena;
 int tx_fil;
 int tx_emp;
 int tx_count;
 char tx_buffer[REM_TX_SIZE];

 int tx_save;
 int rx_fil;
 int rx_emp;
 int rx_count;
 char rx_buffer[REM_RX_SIZE];

 int state;
 int remCmdRcv;
} T_REMCTL, *P_REMCTL;

EXT T_REMCTL remCtl;

typedef struct s_serVar
{
 char dbgBuffer;

 char lineStart;
 char lineLen;
 char eolFlag;
} T_SER_VAR, *P_SER_VAR;

EXT T_SER_VAR serial;

void remSerialSetup(void);

void remPoll(void);
void remSend(void);
void remSendStart(void);
void remRecv(void);

int  remCount(void);
void remRxSkip(int n);
void remFill(const char *p, int n);

void remPut(char ch);
void remPutDigit(char ch);
void remPutHexByte(char ch);
// ReSharper disable once CppConstParameterInDeclaration
void remPutStrLen(const unsigned char *p, const int size);
void remPutStr(const char *p);
// ReSharper disable once CppConstParameterInDeclaration
void remPutHex(const unsigned char *p, const int size);

int  remGet(void);
char remGetHexEcho(int *val);
unsigned char remGetHexByte(void);
char remGetHex(int *val);
#if 0
char regGetStr(char *buf, int bufLen);
#endif

uint32_t remRxReady(void);
int remRxRead(void);
uint32_t remTxEmpty(void);
uint32_t remTxFull(void);
void remTxSend(char ch);
void remRxIntEna(void);
void remRxIntDis(void);
void remRxIntClr(void);
void remTxIntEna(void);
void remTxIntDis(void);
void remTxIntClr(void);

#endif  /* REM_SERIAL_INC */	// ->

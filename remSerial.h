#if !defined(REMSERIAL_INC)	// <-
#define REMSERIAL_INC
#include <neorv32_uart.h>

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
void dbgPutC(char c);
void dbgPutStr(const char *p);

int  remCount(void);
void remRxSkip(int n);
void remFill(const char *p, int n);

void remPut(char ch);
void remPutStrLen(const unsigned char *p, int size);
void remPutStr(const char *p);
void remSndHex(const unsigned char *p, int size);

int  remGet(void);
char remGetHex(int *val);
char remGetHexEcho(int *val);
char regGetStr(char *buf, int bufLen);

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

#endif  /* REMSERIAL_INC */	// ->

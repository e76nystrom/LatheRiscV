#define LATHECPP_REM_SERIAL

#include <neorv32.h>
#include <stdbool.h>
#include <string.h>
#define EXT extern
#include <remSerial.h>

#if defined(REM_SERIAL_INCLUDE)	// <-
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

#endif	/* REM_SERIAL_INCLUDE */ // ->
#ifdef LATHECPP_REM_SERIAL

inline uint32_t remRxReady()
{
 return (NEORV32_UART1->CTRL & (1 << UART_CTRL_RX_NEMPTY));
}

inline int remRxRead()
{
 return (char) (NEORV32_UART1->DATA >> UART_DATA_RTX_LSB);
}

inline uint32_t remTxEmpty()
{
 return (NEORV32_UART1->CTRL & (1 << UART_CTRL_TX_EMPTY));
}

inline uint32_t remTxFull()
{
 return (NEORV32_UART1->CTRL & (1 << UART_CTRL_TX_FULL));
}

inline void remTxSend(char ch)
{
 NEORV32_UART1->DATA = (uint32_t) (ch << UART_DATA_RTX_LSB);
}

inline void remRxIntEna()
{
 neorv32_cpu_csr_set(CSR_MIE, 1 << UART1_RX_FIRQ_ENABLE);
}

inline void remRxIntDis()
{
 neorv32_cpu_csr_clr(CSR_MIE, 1 << UART1_RX_FIRQ_ENABLE);
}

inline void remRxIntClr()
{
 neorv32_cpu_csr_clr(CSR_MIP, 1 <<  UART1_RX_FIRQ_PENDING);
}

inline void remTxIntEna()
{
 neorv32_cpu_csr_set(CSR_MIE, 1 << UART1_TX_FIRQ_ENABLE);
}

inline void remTxIntDis()
{
 neorv32_cpu_csr_set(CSR_MIE, 1 << UART1_TX_FIRQ_ENABLE);
}

inline void remTxIntClr()
{
 neorv32_cpu_csr_clr(CSR_MIP, 1 <<  UART1_TX_FIRQ_PENDING);
}

#if 0
static void remRxIsr(void) __attribute__ ((interrupt ("machine")));

#pragma GCC optimize ("align-functions=4")

/*extern "C"*/
void remRxIsr(void)
{
 if (remRxReady())		/* if received character */
 {
  char ch = remRxRead();	/* read character */
  if (remCtl.state == 0)	/* if waiting for start */
  {
   if (ch == 1)			/* if start of message received */
   {
    remCtl.state = 1;		/* set to start receiving */
   }
  }
  else				/* if receiving data */
  {
   if (remCtl.rx_count < REM_RX_SIZE) /* if room in buffer */
   {
    int fill = remCtl.rx_fil;	/* temp copy of fill pointer */
    remCtl.rx_buffer[fill++] = ch; /* put character in buffer */
    if (fill >= REM_RX_SIZE)	/* if past end of buffer */
     fill = 0;			/* reset to zero */
    remCtl.rx_fil = fill;		/* save fill pointer */
    remCtl.rx_count++;		/* update count */

    if (ch == '\r')		/* if end of command */
    {
     remCtl.state = 0;		/* set to waiting for start */
     remCtl.remCmdRcv = 1;
    }
   }
  }
 }
 remRxIntClr();
}

static void remTxIsr(void) __attribute__ ((interrupt ("machine")));

/*extern "C"*/
void remTxIsr(void)
{
 if (remTxEmpty())		/* if transmit empty */
 {
  if (remCtl.tx_count != 0)	/* if anything in buffer */
  {
   int emp = remCtl.tx_emp;	/* temp copy of empty pointer */
   remTxSend(remCtl.tx_buffer[emp++]); /* send character */
   if (emp >= REM_TX_SIZE)	/* if at buffer end */
    emp = 0;			/* reset to start */
   remCtl.tx_emp = emp;		/* save empty pointer */
   --remCtl.tx_count;		/* count it off */
  }
  else				/* if nothing to send */
   remTxIntDis();		/* disable transmit interrupt */
 }
 remTxIntClr();
}

#endif

void remSerialSetup()
{
 memset((void *) &remCtl, 0, sizeof(remCtl));

#if 0
 neorv32_rte_handler_install(UART1_RX_RTE_ID, &remRxIsr);
 neorv32_cpu_csr_set(CSR_MIE, 1 << UART1_RX_FIRQ_ENABLE);

 neorv32_rte_handler_install(UART1_TX_RTE_ID, &remTxIsr);
 neorv32_cpu_csr_set(CSR_MIE, 1 << UART1_TX_FIRQ_ENABLE);
#endif
}

void dbgPutC(char c)
{
 neorv32_uart_t *uart = NEORV32_UART0;
 if ((uart->CTRL & (1 << UART_CTRL_TX_FULL)) == 0)
  uart->DATA = (uint32_t) c << UART_DATA_RTX_LSB;
}

void dbgPutStr(const char *p)
{
 
 while (1)
 {
  char c = *p++;
  if (c == 0)
   break;
  if ((NEORV32_UART0->CTRL & (1 << UART_CTRL_TX_FULL)) == 0)
   NEORV32_UART0->DATA = (uint32_t) c << UART_DATA_RTX_LSB;
 }
}

void remRecv(void)
{
// while (remRxReady() != 0)	/* if received character */
 while (neorv32_uart_char_received(NEORV32_UART1))
 {
  char ch = remRxRead();	/* read character */
  if (remCtl.state == 0)	/* if waiting for start */
  {
   if (ch == 1)			/* if start of message received */
   {
    dbgPutC('<');
    remCtl.state = 1;		/* set to start receiving */
   }
   else
    dbgPutC('-');
  }
  else				/* if receiving data */
  {
   if (remCtl.rx_count < REM_RX_SIZE) /* if room in buffer */
   {
    int fill = remCtl.rx_fil;	/* temp copy of fill pointer */
    remCtl.rx_buffer[fill++] = ch; /* put character in buffer */
    if (fill >= REM_RX_SIZE)	/* if past end of buffer */
     fill = 0;			/* reset to zero */
    remCtl.rx_fil = fill;	/* save fill pointer */
    remCtl.rx_count++;		/* update count */

    if (ch == '\r')		/* if end of command */
    {
     dbgPutC('>');
     dbgPutStr("\n\r");
     remCtl.state = 0;		/* set to waiting for start */
     remCtl.remCmdRcv = 1;
    }
    else
     dbgPutC(ch);
   }
  }
 }
}

void remSendStart(void)
{
 remCtl.tx_ena = 1;
}

void remSend(void)
{
 if (remCtl.tx_ena)		/* if transmit enabled */
 {
  while (remTxFull() == 0)	/* if transmit not full */
  {
   if (remCtl.tx_count != 0)	/* if anything in buffer */
   {
    int emp = remCtl.tx_emp;	/* temp copy of empty pointer */
    remTxSend(remCtl.tx_buffer[emp++]); /* send character */
    if (emp >= REM_TX_SIZE)	/* if at buffer end */
     emp = 0;			/* reset to start */
    remCtl.tx_emp = emp;	/* save empty pointer */
    --remCtl.tx_count;		/* count it off */
   }
   else				/* if buffer empty */
   {
    remCtl.tx_ena = 0;          /* disable transmitter */
    break;                      /* exit loop */
   }
  }
 }
}

void remPoll()
{
 remRecv();
 remSend();
}

int remCount()
{
 return(remCtl.tx_count);
}

void remRxSkip(int n)
{
 int emp = remCtl.rx_emp;
 emp += n;
 if (emp >= REM_RX_SIZE)
  emp -= REM_RX_SIZE;
 remCtl.rx_emp = emp;
 remCtl.rx_count -= n;
}

void remFill(const char *str, int n)
{
 int fill = remCtl.tx_save;
 while (--n >= 0)
 {
  remCtl.tx_buffer[fill++] = *str++; /* put character in buffer */
  if (fill >= REM_TX_SIZE)	/* if past end of buffer */
   fill = 0;			/* reset to zero */
 }
}

void remPut(char ch)
{
 if (remCtl.tx_count < REM_TX_SIZE) /* if room for data */
 {
  int fill = remCtl.tx_fil;	 /* temp copy of fill pointer */
  remCtl.tx_buffer[fill++] = ch; /* put character in buffer */
  if (fill >= REM_TX_SIZE)	/* if past end of buffer */
   fill = 0;			/* reset to zero */
  remCtl.tx_fil = fill;		/* save fill pointer */

//  remTxIntDis();
  remCtl.tx_count++;		/* update count */
//  remTxIntEna();		/* enable transmit interrupt */
 }
}

void remPutStrLen(const unsigned char *p, int size)
{
 if (remCtl.tx_count + size < REM_TX_SIZE) /* if room for data */
 {
  int fill = remCtl.tx_fil;	 /* temp copy of fill pointer */
  char *dst = &remCtl.tx_buffer[fill]; /* initialize pointer */
  int s = size;
  while (--s >= 0)		/* while not done */
  {
   *dst++ = *p++;		/* put character in buffer */
   fill += 1;			/* update fill pointer */
   if (fill >= REM_TX_SIZE)	/* if past end of buffer */
   {
    dst = remCtl.tx_buffer;
    fill = 0;			/* reset to zero */
   }
  }
  remCtl.tx_fil = fill;		/* save fill pointer */
  remCtl.tx_count += size;	/* update size */
 }
}

void remPutStr(const char *p)
{
 while (true)
 {
  char ch = *p++;
  if (ch == 0)
   break;
  remPut(ch);
  if (ch == '\n')
   remPut('\r');
 }
}

void remSndHex(const unsigned char *p, int size)
{
 char tmp;
 char ch;
 int zeros = 0;

 p += size;
 while (size != 0)
 {
  --size;
  p--;
  tmp = *p;
  ch = tmp;
  ch >>= 4;
  ch &= 0xf;
  if ((ch != 0)
  ||  zeros)
  {
   zeros = 1;
   if (ch < 10)
    ch += '0';
   else
    ch += 'a' - 10;
   remPut(ch);
  }

  tmp &= 0xf;
  if ((tmp != 0)
  ||  zeros)
  {
   zeros = 1;
   if (tmp < 10)
    tmp += '0';
   else
    tmp += 'a' - 10;
   remPut(tmp);
  }
 }
 if (zeros == 0)
  remPut('0');
}

int remGet()
{
 if (remCtl.rx_count != 0)	/* if anything in buffer */
 {
  int emp = remCtl.rx_emp;	/* temp copy of empty pointer */
  unsigned char ch = remCtl.rx_buffer[emp++]; /* send character */
  if (emp >= REM_RX_SIZE)	/* if at buffer end */
   emp = 0;			/* reset to start */
  remCtl.rx_emp = emp;		/* save empty pointer */

//  remRxIntDis();
  --remCtl.rx_count;		/* count it off */
//  remRxIntEna();
  return(ch);
 }
 return(-1);			/* nothing in buffer */
}

char remGetHexEcho(int *val)
{
 char ch;
 bool neg = false;
 int tmpVal = 0;
 int count = 0;

 while (count <= 8)
 {
  int tmp = remGet();
  if (tmp > 0)
  {
   ch = (char) tmp;
   if ((ch >= '0') &&
       (ch <= '9'))
   {
    remPut(ch);
    ch -= '0';
    count++;
   }
   else if ((ch >= 'a') &&
            (ch <= 'f'))
   {
    remPut(ch);
    ch -= 'a' - 10;
    count++;
   }
   else if (ch == '-')
   {
    neg = true;
    remPut(ch);
   }
   else if (ch == ' ')
   {
    remPut(ch);
    break;
   }
   else if (ch == '\r')
    break;
   else
    continue;
   tmpVal <<= 4;
   tmpVal += ch;
  }
  else
   return(0);
 }
 if (neg)
  tmpVal = -tmpVal;
 *val = tmpVal;
 return(count != 0);
}

char remGetHex(int *val)
{
 char ch;
 bool neg = false;
 int tmpVal = 0;
 int count = 0;

 while (count <= 8)
 {
  int tmp = remGet();
  if (tmp > 0)
  {
   ch = (char) tmp;
   if ((ch >= '0') &&
       (ch <= '9'))
   {
    ch -= '0';
    count++;
   }
   else if ((ch >= 'a') &&
	    (ch <= 'f'))
   {
    ch -= 'a' - 10;
    count++;
   }
   else if (ch == '-')
    neg = true;
   else if ((ch == ' ') || (ch == '|') || (ch == '\r'))
    break;
   else
    continue;
   tmpVal <<= 4;
   tmpVal += ch;
  }
  else
   return(0);
 }
 if (neg)
  tmpVal = -tmpVal;
 *val = tmpVal;
 return(count != 0);
}

char remGetStr(char *buf, int bufLen)
{
 int len = 0;
 char *p;
 char ch;
 p = buf;
 while (true)
 {
  int tmp = remGet();
  if (tmp > 0)
  {
   ch = (char) tmp;
   if ((ch == ' ') || (ch == '\n'))
   {
    if (len < bufLen)
     *p++ = 0;
    break;
   }
   else
   {
    if (len < bufLen)
    {
     *p++ = ch;
     len++;
    }
   }
  }
  else
  {
   len = 0;
   break;
  }
 }
 return(len);
}

#endif	/* LATHE_CPP_REM_SERIAL */

#define LATHECPP_DBG_SERIAL

#include <neorv32.h>
#define EXT extern
#include <dbgSerial.h>

#if defined(DBG_SERIAL_INCLUDE)	// <-

void dbgPutC(char c);
void dbgPutSpace(void);
void dbgNewLine(void);
void dbgPutStr(const char *p);
void dbgPutDigit(char ch);
void dbgPutHexByte(char ch);
void dbgPutHex(unsigned int val, int size);

char* dbgFmtNum(char *buf, int val);

#endif	/* DBG_SERIAL_INCLUDE */ // ->
#if defined(LATHECPP_DBG_SERIAL)

void dbgPutC(const char c)
{
 neorv32_uart_t *uart = NEORV32_UART0;
 while ((uart->CTRL & 1 << UART_CTRL_TX_FULL) != 0)
  ;
 uart->DATA = (uint32_t) c << UART_DATA_RTX_LSB;
}

void dbgPutSpace(void)
{
 dbgPutC(' ');
}

void dbgNewLine(void)
{
 dbgPutC('\n');
 dbgPutC('\r');
}

void dbgPutStr(const char *p)
{
 
 while (1)
 {
  const char c = *p++;
  if (c == 0)
   break;
  if (c == '\n')
   dbgPutC('\r');
  while ((NEORV32_UART0->CTRL & 1 << UART_CTRL_TX_FULL) != 0)
   ;
   NEORV32_UART0->DATA = (uint32_t) c << UART_DATA_RTX_LSB;
 }
}

void dbgPutDigit(char ch)
{
 ch &= 0xf;
 if (ch < 10)
  ch += '0';
 else
  ch += 'a' - 10;
 dbgPutC(ch);
}

void dbgPutHexByte(const char ch)
{
 const char tmp = ch >> 4;
 dbgPutDigit(tmp);
 dbgPutDigit(ch);
}

void dbgPutHex(unsigned int val, int size)
{
 const unsigned char *p = (unsigned char *) &val;
 p += size;
 while (size != 0)
 {
  size -= 1;
  p -= 1;
  const unsigned char tmp = *p;
  unsigned char ch = tmp;
  ch >>= 4;
  dbgPutDigit(ch);
  dbgPutDigit(tmp);
 }
}

#define FMT_SCALE 4

char* dbgFmtNum(char *buf, int val)
{
 char tmpBuf[16];
 char *p = tmpBuf;
 char *p1 = buf;

 int neg = 0;
 if (val < 0)
 {
  neg = 1;
  val = -val;
 }

 int i = 0;
 while (val != 0)
 {
  *p++ = val % 10 + '0';
  val /= 10;
  i += 1;
 }

 if (neg)
  *p1++ = '-';

 if (i <= FMT_SCALE)
 {
  *p1++ = '0';
 }
 else
 {
  while (i > FMT_SCALE)
  {
   *p1++ = *--p;
   i -= 1;
  }
 }

 *p1++ = '.';

 int j = FMT_SCALE - i;
 while (j > 0)
 {
  *p1++ = '0';
  j -= 1;
 }
 
 while (i > 0)
 {
  *p1++ = *--p;
  i -= 1;
 }

 // ReSharper disable once CppDFAUnusedValue
 *p1++ = 0;
 return buf;
}

#endif	/* LATHECPP_DBG_SERIAL */

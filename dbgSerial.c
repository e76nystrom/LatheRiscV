#define LATHECPP_DBG_SERIAL

#include <neorv32.h>
#define EXT extern
#include <dbgSerial.h>

#if defined(DBG_SERIAL_INCLUDE)	// <-

void dbgPutC(char c);
void dbgNewLine(void);
void dbgPutStr(const char *p);
void dbgPutDigit(char ch);
void dbgPutHex(unsigned int val, int size);


#endif	/* DBG_SERIAL_INCLUDE */ // ->
#if defined(LATHECPP_DBG_SERIAL)

void dbgPutC(char c)
{
 neorv32_uart_t *uart = NEORV32_UART0;
 while ((uart->CTRL & (1 << UART_CTRL_TX_FULL)) != 0)
  ;
  uart->DATA = (uint32_t) c << UART_DATA_RTX_LSB;
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
  char c = *p++;
  if (c == 0)
   break;
  if (c == '\n')
   dbgPutC('\r');
  while ((NEORV32_UART0->CTRL & (1 << UART_CTRL_TX_FULL)) != 0)
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

void dbgPutHex(unsigned int val, int size)
{
 unsigned char tmp;
 unsigned char ch;

 unsigned char *p = (unsigned char *) &val;
 p += size;
 while (size != 0)
 {
  size -= 1;
  p -= 1;
  tmp = *p;
  ch = tmp;
  ch >>= 4;
  dbgPutDigit(ch);
  dbgPutDigit(tmp);
 }
}

#endif	/* LATHECPP_DBG_SERIAL */

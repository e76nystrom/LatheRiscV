#include <neorv32.h>
#include "fpgaLatheReg.h"
#include "fpgaLatheBits.h"
#include "ctlstates.h"

#define EXT
#include "Lathe.h"
#include "axisCtl.h"
#include "remSerial.h"
#include "remCmd.h"
//#include "fpga.h"

#define DEBUG

#if defined(DEBUG)

#define DBG_BAUD_RATE 57600
#define REM_BAUD_RATE 19200

#endif

int spiEna = 0;

#if defined(LATHE_INCLUDE)	// <-

#define printf neorv32_uart0_printf

int main(void);

#endif	/* LATHE_INCLUDE */ // ->

neorv32_uart_t *uart0;
neorv32_uart_t *uart1;

int main()
{
 uart0 = (neorv32_uart_t *) NEORV32_UART0_BASE;
 uart1 = (neorv32_uart_t *) NEORV32_UART0_BASE;
 
 neorv32_rte_setup();			/* setup vectors */

 neorv32_uart0_setup(DBG_BAUD_RATE, 0); /* init uart */
 neorv32_uart0_puts("dbg starting 1\n");

 neorv32_uart1_setup(REM_BAUD_RATE, 0); /* init uart */
 neorv32_uart1_puts("rem starting\n");

 remSerialSetup();
 initAccelTable();
 runInit();
 initAxisCtl();

 int tmp = 0x12345678;
 remSndHex((const unsigned char *) &tmp, 4);
 remSendStart();

 neorv32_cpu_csr_set(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE);

 if (spiEna)
 {
  int prsc = 0;
  int cDiv = 3;
  int clkPhase = 0;
  int clkPolarity = 0;
  int irqMask = 0;
  neorv32_spi_setup(prsc, cDiv, clkPhase, clkPolarity, irqMask);
 }
 
 neorv32_gpio_port_set(0);

 while (true)
 {
  remPoll();
  if (remCtl.remCmdRcv)
  {
   remCtl.remCmdRcv = 0;
   remCmd();
  }
  axisCtl();
  runProcess();
 }
}
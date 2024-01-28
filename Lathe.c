#include <stdbool.h>
#include <neorv32.h>
#include "fpgaLatheReg.h"
// #include "fpgaLatheBits.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "ctlstates.h"

#define EXT
#include "Lathe.h"
// #include "riscvCmdList.h"
#include "fpgaLatheBits.h"
#include "axisCtl.h"
#include "dbgSerial.h"
#include <ctlbits.h>
#include <riscvStruct.h>

#include "remSerial.h"
#include "remCmd.h"
#include "fpga.h"

#define DEBUG

#if defined(DEBUG)

#define DBG_BAUD_RATE 57600
#define REM_BAUD_RATE 19200

#endif

//#define INPUT_DIAG

int spiEna = 0;

#if defined(LATHE_INCLUDE)	// <-

#define dbg0 1			/* Lathe.c */
#define dbg1 1			/* fpga.c */
#define dbg2 1			/* remCmd.c */
#define dbg3 1			/* axisctl.c */
#define dbg3L 0			/* axisctl.c loc */
#define dbg3I 0			/* axisctl.c index clock */
#define dbgM 1			/* dbgMsg() */
#define dbgM0 1           	/* dbgMsg() debug prints */

typedef struct S_CH2
{
 char c0;
 char c1;
} T_CH2, *P_CH2;

#define printf neorv32_uart0_printf

int main(void);

#endif	/* LATHE_INCLUDE */ // ->

neorv32_uart_t *uart0;
neorv32_uart_t *uart1;

int main(void)
{
 neorv32_gpio_port_set(0);
 CFS->ctl = 0;

 uart0 = (neorv32_uart_t *) NEORV32_UART0_BASE;
 uart1 = (neorv32_uart_t *) NEORV32_UART0_BASE;
 
 neorv32_rte_setup();			/* setup vectors */

 neorv32_uart0_setup(DBG_BAUD_RATE, 0); /* init uart */
 neorv32_uart0_puts("\ndbg starting " __DATE__ " " __TIME__"\n");

 neorv32_uart1_setup(REM_BAUD_RATE, 0); /* init uart */
 neorv32_uart1_puts("\nrem starting\n");

#if defined(INPUT_TEST)
 CFS->ctl = RISCV_IN_TEST | RISCV_DATA;	/* ***testing inputs*** */
 dbgPutStr("\n***testing inputs***\n");
#endif

 ld(F_Ld_Dsp_Reg, 0xabcd);

 neorv32_gpio_port_set(1);

 remSerialSetup();
 initAccelTable();
 runInit();
 remCmdInit();
 initAxisCtl();

 // int tmp = 0x12345678;
 // remSndHex((const unsigned char *) &tmp, 4);
 remSendStart();

 neorv32_cpu_csr_set(CSR_MSTATUS, 1 << CSR_MSTATUS_MIE);

 if (spiEna)
 {
  const int prsc = 0;
  const int cDiv = 3;
  const int clkPhase = 0;
  const int clkPolarity = 0;
  const int irqMask = 0;
  neorv32_spi_setup(prsc, cDiv, clkPhase, clkPolarity, irqMask);
 }

#if defined(INPUT_DIAG)

#include "fpgaLatheReg.h"
 
#if 1
 ld(F_Ld_Cfg_Ctl, CFG_ZHOME_INV | CFG_ZMINUS_INV |
   CFG_ZPLUS_INV | CFG_PROBE_INV);
#endif

#if 1
 ld(F_Ld_Cfg_Ctl, CFG_XHOME_INV | CFG_XMINUS_INV |
   CFG_XPLUS_INV | CFG_PROBE_INV);
#endif

 if (dbg0) {
  int mask = 1;
  for (int i = 0; i < INPUTS_SIZE; i++)
  {
   CFS->inPin = mask;
   dbgPutStr("mask   ");
   dbgInPin(mask);

   dbgPutStr("inputs ");
   dbgInPin(rd(F_Rd_Inputs));

   dbgPutStr("z axis ");
   dbgAxisStatus(&zAxis);

   dbgPutStr("x axis ");
   dbgAxisStatus(&xAxis);
   mask <<= 1;
  } }

#endif

#if 0
 char buf[16];
 dbgPutStr(dbgFmtNum(buf, 1));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, -1));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 10));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 100));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 1000));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 10000));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 123456789));
 dbgNewLine();
 dbgPutStr(dbgFmtNum(buf, 0x7fffffff));
 dbgNewLine();
 #endif

#if 0
 printf("dbgQue %x sizeof %x\n", (uint32_t) &dbgQue,
	(uint32_t) sizeof(dbgQue));
 printf("DEBUG_DATA_SIZE %x siezof(T_DEBUG_DATA) %x\n",
	DEBUG_DATA_SIZE, sizeof(T_DEBUG_DATA));
 printf("runQue %x\n", (uint32_t) &runQue);
#endif

 if (dbg0) {
  dbgPutStr("zMpg");
  for (int i = 1; i < 32; i++)
  {
   int const tmp = CFS->zMpg;
   dbgPutHex(tmp, 2);
   if (tmp & 0x100)
    break;
  }
  dbgNewLine();

  dbgPutStr("xMpg");
  for (int i = 1; i < 32; i++)
  {
   int const tmp = CFS->xMpg;
   dbgPutHex(tmp, 2);
   if (tmp & 0x100)
    break;
  }
  dbgNewLine(); }
 
 neorv32_gpio_port_set(0);
 
 rVar.rJogPause = DISABLE_JOG;
 
 // ReSharper disable once CppDFAEndlessLoop
 while (true)
 {
  remPoll();
  if (remCtl.remCmdRcv)
  {
   remCtl.remCmdRcv = 0;
   CFS->dbg = 1;
   remCmd();
  }
  CFS->dbg = 2;
  axisCtl();
  CFS->dbg = 4;
  runProcess();
  CFS->dbg = 8;
 }
}

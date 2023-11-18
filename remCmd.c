#define LATHECPP_REM_CMD

#include <neorv32.h>
#include <string.h>
#include <stdbool.h>
#include "ctlStates.h"
#include "ctlbits.h"
#define EXT extern
#include "lathe.h"
#include "remCmd.h"
#include "axisCtl.h"
#include "dbgSerial.h"
#include "remSerial.h"
#include "riscvStruct.h"
#include "fpga.h"
#include "fpgaLatheReg.h"
#include "fpgaLatheBits.h"

#if defined(REM_CMD_INCLUDE)	// <-

typedef struct S_RUN_DATA
{
 enum RISCV_CMD parm;
 int val1;
 int val2;
} T_RUN_DATA, *P_RUN_DATA;

#define RUN_DATA_SIZE 32

typedef struct S_RUN_QUE
{
 int count;
 int fil;
 int emp;
 T_RUN_DATA data[RUN_DATA_SIZE];
} T_RUN_QUE, *P_RUN_QUE;

EXT T_RUN_QUE runQue;

typedef struct S_RUN_CTL
{
 enum RISCV_RUN_WAIT wait;
} T_RUN_CTL, *P_RUN_CTL;

EXT T_RUN_CTL runCtl;

typedef struct S_DEBUG_DATA
{
 int dbg;
 int32_t val;
 uint32_t time;
} T_DEBUG_DATA, *P_DEBUG_DATA;

#define DEBUG_DATA_SIZE 48
// 2-code 1-space 1-neg 8-val 1-space
// #define MAX_DBG_SIZE (2 + 1 + 1 + 8 + 1)
// 2-code 4-val
#define MAX_DBG_SIZE (1 + 4)

typedef struct S_DEBUG_QUE
{
 int count;
 int fil;
 int emp;
 T_DEBUG_DATA data[DEBUG_DATA_SIZE];
 int rsv[16];
} T_DEBUG_QUE, *P_DEBUG_QUE;

EXT T_DEBUG_QUE dbgQue;

void remCmd(void);

void runInit(void);
void runProcess(void);

void rspPut(char ch);
void rspPutDigit(char ch);
void rspPutByte(char ch);

void rspReplDigit(unsigned char ch);
void rspReplByte(unsigned char ch);
void rspReplHex(unsigned char ch);

void rspPutHex(unsigned int val, int size);

void saveAccel(int type, int val);
void saveData(int type, int val);

void clrDbgBuf(void);
void dbgMsg(int dbg, int val);

#endif	/* REM_CMD_INCLUDE */ // ->
#if defined(LATHECPP_REM_CMD)

#include "dMessageStr.h"

void dbgMsg(const int dbg, const int val)
{
 if (dbgQue.count < DEBUG_DATA_SIZE) /* if buffer not full */
 {
  dbgQue.count++;		/* update message count */

  const P_DEBUG_DATA p = &dbgQue.data[dbgQue.fil]; /* get place to put msg */

  p->dbg = dbg;			/* save message type */
  p->val = val;			/* save value */
  p->time = millis();		/* save time */

  dbgQue.fil++;			/* update fill pointer */
  if (dbgQue.fil >= DEBUG_DATA_SIZE) /* if at end */
   dbgQue.fil = 0;		/* set to zero */
 }

 dbgPutStr("dbgMsg ");
 dbgPutHexByte(dbgQue.count);
 dbgPutSpace();
 const char *p = (char *) &dMessageStr[dbg];
 dbgPutC(*p++);
 dbgPutC(*p++);
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgPutSpace();
 dbgPutHexByte(dbg);
 dbgPutSpace();
 dbgPutHex(val, 4);
 dbgNewLine();
}

void clrDbgBuf(void)
{
 memset(&dbgQue.data, 0, sizeof(dbgQue.data));
 dbgQue.count = 0;
 dbgQue.fil = 0;
 dbgQue.emp = 0;
}

//#include "riscvParm.h"

#define RSP_BUF_SIZE 128
typedef struct S_RSP_CTL
{
 int count;
 unsigned char *p;
 unsigned char *p0;
 unsigned char buf[RSP_BUF_SIZE];
} T_RSP_CTL;

T_RSP_CTL rspCtl;

void rspInit(void)
{
 rspCtl.count = 0;
 rspCtl.p = rspCtl.buf;
 rspCtl.p0 = &rspCtl.buf[1];
}

void rspPut(const char ch)
{
 if (rspCtl.count < RSP_BUF_SIZE)
 {
  rspCtl.count += 1;
  *rspCtl.p++ = ch;
 }
}

void rspPutDigit(char ch)
{
 if (rspCtl.count < RSP_BUF_SIZE)
 {
  rspCtl.count += 1;
  ch &= 0xf;
  if (ch < 10)
   ch += '0';
  else
   ch += 'a' - 10;
  *rspCtl.p++ = ch;
 }
}

void rspPutByte(char ch)
{
 const char tmp = ch;
 ch >>= 4;
 rspPutDigit(ch);
 rspPutDigit(tmp);
}

void rspReplDigit(unsigned char ch)
{
 if (rspCtl.count < RSP_BUF_SIZE)
 {
  ch &= 0xf;
  if (ch < 10)
   ch += '0';
  else
   ch += 'a' - 10;
  *rspCtl.p0++ = ch;
 }
}

void rspReplByte(unsigned char ch)
{
 const unsigned char tmp = ch;
 ch >>= 4;
 rspReplDigit(ch);
 rspReplDigit(tmp);
}

void rspReplHex(const unsigned char ch)
{
 unsigned char tmp = ch;
 tmp >>= 4;
 rspReplDigit(tmp);
 rspReplDigit(ch);
}

void rspPutHex(unsigned int val, int size)
{
 int zeros = 0;

 rspPut(' ');
 const unsigned char *p = (unsigned char *) &val;
 p += size;
 while (size != 0)
 {
  --size;
  p--;
  unsigned char tmp = *p;
  unsigned char ch = tmp;
  ch >>= 4;
  if (ch != 0
  ||  zeros)
  {
   zeros = 1;
   rspPutDigit(ch);
  }

  tmp &= 0xf;
  if (tmp != 0
  ||  zeros)
  {
   zeros = 1;
   rspPutDigit(tmp);
  }
 }
 if (zeros == 0)
  rspPut('0');
}

#include "riscvCmdStr.h"

int dbg;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
void remCmd(void)
{
 int parm;
 int val1;
 int val2;

 rspInit();

 rspPut('-');
 rspPut(0);			/* reserve space for count */
 rspPut(0);
 rspPut(0);			/* reserve space for que count */
 rspPut(0);

 remRxSkip(2);

 dbg = 0;
 while (true)
 {
  P_RUN_DATA data = 0;
  
  // char tmp1 = remCtl.rx_emp;
  // char tmp2 = remCtl.rx_buffer[remCtl.rx_emp];

  if (remGetHex(&parm) == 0)	/* read parameter */
   break;

  if (parm < R_READ_ALL)
  {
   // dbgPutHexByte(tmp1);
   // dbgPutSpace();
   // dbgPutC(tmp2);
   // dbgPutSpace();
   // dbgPutHexByte((remCtl.rx_emp - tmp1) & (REM_RX_SIZE-1));
   // dbgPutSpace();
   
   dbg += 1;
   dbgPutC('*');
   dbgPutHexByte(dbg);
   dbgPutSpace();

   dbgPutHexByte(parm);
   dbgPutSpace();
   if ((parm & 0xff) < R_MAX_CMD)
   {
    const char *p = (char *) &riscvCmdStr[parm];
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   else
   {
    dbgPutStr("**err**");
   }
   dbgNewLine();
  // dbgPutHexByte(parm);
  // dbgPutSpace();
  }

  int n = 0;
  switch (parm)
  {
  case R_NONE:
  case R_STOP:
   break;
   
  case R_STOP_Z:
   axisStop(&zAxis);
   break;
   
  case R_STOP_X:
   axisStop(&xAxis);
   break;

  case R_SETUP:
   dbgPutStr("setup\n");
   rVar.rMvStatus = 0;
   runInit();
   break;

  case R_RESUME:
   dbgPutStr("resume\n");
   if (runCtl.wait == RW_PAUSE)
    runCtl.wait = RW_NONE;

   if (rVar.rJogPause & DISABLE_JOG)
    rVar.rJogPause &= ~(PAUSE_ENA_X_JOG | PAUSE_ENA_Z_JOG);

   rVar.rMvStatus &= ~(MV_PAUSE | MV_MEASURE | MV_READ_X | MV_READ_Z);
   break;

  case R_DONE:
   rVar.rMvStatus &= ~(MV_DONE);
   rVar.rJogPause = 0;
   break;
   
  case R_SET_LOC_X:
   CFS->ctl = RISCV_DATA;
   remGetHex(&val1);
   setLoc(&xAxis, val1);
   dbgMsg(D_XLOC, val1);
   break;

  case R_SET_LOC_Z:
   CFS->ctl = RISCV_DATA;
   ld(F_Ld_Cfg_Ctl, CFG_DRO_STEP | CFG_ZDRO_INV | CFG_XDRO_INV);
   remGetHex(&val1);
   setLoc(&zAxis, val1);
   dbgMsg(D_ZLOC, val1);
   break;

  case R_STEPS_Z:
   remGetHex(&val1);
   zAxis.stepsInch = val1;
   break;

  case R_STEPS_X:
   remGetHex(&val1);
   xAxis.stepsInch = val1;
   break;

  case R_PAUSE:
   dbgPutStr("***pause\n");
  case R_OP_START:
  case R_OP_DONE:
  case R_START_SPIN:
  case R_STOP_SPIN:
  {
   if (runQue.count < RUN_DATA_SIZE)
   {
    data = &runQue.data[runQue.fil];
    data->parm = parm;
   }
  }
  break;

  case R_SET_ACCEL:
   n = 2;
   remGetHex(&val1);
   remGetHex(&val2);
   saveAccel(val1, val2);
   break;

  case R_SET_DATA:
  {
   n = 2;
   remGetHex(&val1);
   remGetHex(&val2);
    T_DATA_UNION val;
     val.t_int = val2;
     setRiscvVar(val1, val);
   }
   break;

  case R_GET_DATA:
  {
   remGetHex(&val1);
   T_DATA_UNION val;
   getRiscvVar(val1, &val);
   rspPutByte(parm);
   rspPutHex(val.t_int32_t, riscvSize[parm]);
  }
  break;

  case R_JOG_Z:
   remGetHex(&val1);
   jogMove(&zAxis, val1);
   break;

  case R_JOG_X:
   remGetHex(&val1);
   jogMove(&xAxis, val1);
   break;

  case R_MOVE_Z:
  case R_MOVE_X:
  case R_MOVE_REL_X:
  case R_MOVE_REL_Z:
  case R_SET_ACCEL_Q:
  {
   remGetHex(&val1);
   remGetHex(&val2);
   if (runQue.count < RUN_DATA_SIZE)
   {
    n = 2;
    data = &runQue.data[runQue.fil];
    data->parm = parm;
    data->val1 = val1;
    data->val2 = val2;
   }
  }
  break;

  case R_SAVE_Z:
  case R_SAVE_X:
  case R_HOFS_Z:
  case R_HOFS_X:
  case R_PASS:
  {
   remGetHex(&val1);
   if (runQue.count < RUN_DATA_SIZE)
   {
    n = 1;
    data = &runQue.data[runQue.fil];
    data->parm = parm;
    data->val1 = val1;
   }
  }
  break;

  case R_READ_ALL:
   rspPutByte(parm);
   rspPutHex(zAxis.curLoc, sizeof(zAxis.curLoc));
   rspPutHex(xAxis.curLoc, sizeof(xAxis.curLoc));
   rspPutHex(indexData.rpm, 2);
   rspPutHex(rVar.rCurPass, 2);
   rspPutHex(zAxis.dro, sizeof(zAxis.dro));
   rspPutHex(xAxis.dro, sizeof(xAxis.dro));
   rspPutHex(rVar.rMvStatus, 2);
   // rspPutHex(RUN_DATA_SIZE - runQue.count, 1);
   rspPutHex(dbgQue.count, 1);
   dbg = -1;
   break;

  case R_READ_DBG:
  {
   remGetHex(&val1);
   rspPutByte(parm);
   do
   {
    if (dbgQue.count == 0 ||	/* if  no data */
        RSP_BUF_SIZE - rspCtl.count < MAX_DBG_SIZE) /* of no space */
     break;

    val1 -= 1;
    dbgQue.count -= 1;		/* count off a message */

    const P_DEBUG_DATA p = &dbgQue.data[dbgQue.emp]; /* get pointer to data */
    dbgQue.emp++;		 /* update empty pointer */
    if (dbgQue.emp >= DEBUG_DATA_SIZE) /* if past end */
     dbgQue.emp = 0;		/* point back to beginning */

    rspPut(p->dbg);
    const char *p1 = (char *) &p->val;
    for (int i = 0; i < sizeof(p->val); i++)
     rspPut(*p1++);

   } while (val1 > 0);		/* while more requested */

   if (rspCtl.count > 7)
   {
    dbgPutStr("dbgMsg ");
    dbgPutHexByte(dbgQue.count);
    dbgPutSpace();
    dbgPutHexByte(rspCtl.count);
    dbgNewLine();
   }
  }
  dbg = -1;
  break;

  default:
   break;
  } /* end switch (parm) */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
  if (data != 0)
  {
   dbgPutHexByte(runQue.count);
   dbgPutSpace();
   dbgPutHexByte(runQue.fil);
   dbgPutStr(" queued");
   
   if (n >= 1)
   {
    dbgPutSpace();
    dbgPutHex(data->val1, 4);
   }

   if (n >= 2)
   {
    dbgPutSpace();
    dbgPutHex(data->val2, 4);
   }

   runQue.fil += 1;
   if (runQue.fil >= RUN_DATA_SIZE)
    runQue.fil = 0;
   runQue.count += 1;
   dbgNewLine();
  }
#pragma clang diagnostic pop

  // runProcess();
 } /* end while(true) */

 if (dbg > 0)
 {
  dbgPutStr("&@&@ ");
  dbgPutHexByte(dbg);
  dbgNewLine();
 }

 const int free = RUN_DATA_SIZE - runQue.count;

 // dbgNewLine();
 // dbgPutStr("que ");
 // dbgPutHex(free, 1);
 // dbgNewLine();

 rspPut('*');
 rspReplHex(rspCtl.count - 3);
 rspReplHex(free);

 remPutStrLen(rspCtl.buf, rspCtl.count);
 remSendStart();

 while (true)
 {
  const int tmp = remGet();
  if (tmp < 0)
  {
   /* printf("end of buffer\n"); */
   break;
  }
  if (tmp == '\r')
   break;
  /* printf("extra char %d\n", tmp); */
  /* flushBuf(); */
 }

 runProcess();
} /* end remCmd() */
#pragma clang diagnostic pop

void runInit(void)
{
 memset(&runQue, 0, sizeof(runQue));
 memset(&runCtl, 0, sizeof(runCtl));
 runCtl.wait = RW_NONE;
}

void spindleCheck(void)
{
 if (indexData.clocks != 0)
 {
  if (indexData.clocks != indexData.lastClocks)
  {
   indexData.lastClocks = indexData.clocks;
   int delta = (int) indexData.clocks - (int) indexData.lastClocks;
   if (delta < 0)
    delta = -delta;
   const int percent = delta * 1000 / (int) indexData.clocks;
   if (percent < 10)
    runCtl.wait = RW_NONE;
  }
 }
}

#include "riscvRunWaitStr.h"

int lastWait;

void runProcess(void)
{
 const int wait = runCtl.wait;

 if (wait != lastWait)
 {
  lastWait = wait;
  dbgMsg(D_MSTA, wait);

  dbgPutStr("wait ");
  dbgPutHexByte(runCtl.wait);
  dbgPutSpace();
  const char *p = (char *) &riscvRunWaitStr[wait];
  dbgPutC(*p++);
  dbgPutC(*p);
  dbgNewLine();
 }

 if (wait != RW_NONE)
 {
  switch (runCtl.wait)
  {
   case RW_PAUSE:
    return;
    // break;

   case RW_SPIN_START:
    spindleCheck();
    break;

   case RW_SPIN_STOP:
    dbgPutStr("spindle stop\n");
    runCtl.wait = RW_NONE;
    break;

   case RW_WAIT_Z:
   case RW_WAIT_X:
    break;

   case RW_NONE:
   default:
    dbgPutStr("invalid wait\n");
    runCtl.wait = RW_NONE;
    break;
  }
 }
 else
 {
  if (runQue.count != 0)
  {
   if (runQue.emp < 0 || runQue.emp > RUN_DATA_SIZE)
   {
    dbgPutStr("runQue.emp ");
    dbgPutHex(runQue.emp, 4);
    dbgNewLine();
   }
   const P_RUN_DATA data = &runQue.data[runQue.emp];
   runQue.emp += 1;
   if (runQue.emp >= RUN_DATA_SIZE)
    runQue.emp = 0;
   runQue.count -= 1;

   const int parm = data->parm;
    
   dbgPutStr("q ");
   dbgPutHexByte(parm);
   dbgPutSpace();
   if (parm < R_MAX_CMD)
   {
    const char *p = (char *) &riscvCmdStr[parm];
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   else
   {
    dbgPutStr("out of range");
   }
   dbgPutSpace();
   dbgPutHexByte(runQue.count);
   dbgNewLine();

   if (data->parm != R_SET_ACCEL_Q)
    dbgMsg(D_MCMD, data->parm);

   switch (data->parm)
   {
   case R_OP_START:
    dbgMsg(D_DONE, PARM_START);
    rVar.rMvStatus &= ~MV_DONE;
    rVar.rMvStatus |= MV_ACTIVE;
    rVar.rJogPause = DISABLE_JOG;
    zAxis.mpgState = MPG_DISABLED;
    xAxis.mpgState = MPG_DISABLED;
    break;

   case R_OP_DONE:
    dbgMsg(D_DONE, PARM_DONE);
    rVar.rMvStatus &= ~MV_ACTIVE;
    rVar.rMvStatus |= MV_DONE;
    break;

   case R_PAUSE:
    dbgPutStr("pause\n");
    // rVar.cmdPaused = 1;

    rVar.rJogPause = DISABLE_JOG | data->val1;

    if (data->val1 & PAUSE_READ_X)
     rVar.rMvStatus |= MV_READ_X;

    if (data->val1 & PAUSE_READ_Z)
     rVar.rMvStatus |= MV_READ_Z;

    runCtl.wait = RW_PAUSE;
    rVar.rMvStatus |= MV_PAUSE;
    break;

   case R_START_SPIN:
    runCtl.wait = RW_SPIN_START;
    break;

   case R_STOP_SPIN:
    runCtl.wait = RW_SPIN_STOP;
    break;

   case R_PASS:
   {
    const int pass = data->val1;
    dbgMsg(D_PASS, pass);
    printf("\npass %d %d\n\n", pass >> 8 & 0xff, pass & 0xff);
    rVar.rCurPass = data->val1;
   }
   break;

   case R_SET_ACCEL_Q:
    saveAccel(data->val1, data->val2);
    break;
   
   case R_SAVE_Z:
    zAxis.savedLoc = data->val1;
    break;
     
   case R_SAVE_X:
    xAxis.savedLoc = data->val1;
    break;
     
   case R_HOFS_Z:
    zAxis.homeOffset = data->val1;
    break;
     
   case R_HOFS_X:
    xAxis.homeOffset = data->val1;
    break;
     
   case R_MOVE_Z:
    move(&zAxis, data->val1, data->val2);
    break;

   case R_MOVE_X:
    move(&xAxis, data->val1, data->val2);
    break;

   case R_MOVE_REL_Z:
    moveRel(&zAxis, data->val1, data->val2);
    break;

   case R_MOVE_REL_X:
    moveRel(&xAxis, data->val1, data->val2);
    break;

   default:
    break;
   }
  }
 }
}

#include "axisAccelTypeStr.h"

void saveAccel(const int type, const int val)
{
 T_INT_BYTE tmp;
 tmp.iVal = type;

 dbgPutStr("saveAccel ");
 dbgPutHex(type, 2);
 dbgPutSpace();
 const char *p = (char *) &axisAccelTypeStr[tmp.bVal[1]];
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgPutSpace();
 dbgPutHex(val, 4);
 dbgNewLine();

 const P_ACCEL_DATA accel = accelData[tmp.bVal[1]];
 switch (tmp.bVal[0])
 {
 case RP_INITIAL_SUM:
  accel->initialSum = val;
  break;
 case RP_INCR1:
  accel->incr1 = val;
  break;
 case RP_INCR2:
  accel->incr2 = val;
  break;
 case RP_ACCEL_VAL:
  accel->accelVal = val;
  break;
 case RP_ACCEL_COUNT:
  accel->accelCount = val;
  break;
 case RP_FREQ_DIV:
  accel->freqDiv = val;
  break;
 default:
  break;
 }
}

void saveData(const int type, const int val)
{
 switch (type)
 {
 case RD_Z_BACKLASH:
  zAxis.backlashSteps = val;
  break;

 case RD_X_BACKLASH:
  xAxis.backlashSteps = val;
  break;

 default:
  break;
 }
}

#endif	/* LATHECPP_REM_CMD */

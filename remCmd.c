#include <string.h>
#define LATHECPP_REM_CMD

#include <neorv32.h>
#include <stdbool.h>
#include "ctlStates.h"
#include "ctlbits.h"
#define EXT extern
//#include "lathe.h"
#include "axisCtl.h"
#include "remSerial.h"
#include "remCmd.h"
#include "riscvParmList.h"
#include "riscvStruct.h"

#if defined(REM_CMD_INCLUDE)	// <-

typedef struct S_RUN_DATA
{
 enum SEL_RISCV_TYPE parm;
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
 enum RISCV_RUN_WAIT_TYPE wait;
 int pass;
 int mvStatus;
} T_RUN_CTL, *P_RUN_CTL;

EXT T_RUN_CTL runCtl;

typedef struct S_DEBUG_DATA
{
 enum D_MESSAGE dbg;
 int32_t val;
 uint32_t time;
} T_DEBUG_DATA, *P_DEBUG_DATA;

#define DEBUG_DATA_SIZE 16

typedef struct S_DEBUG_QUE
{
 int count;
 int fil;
 int emp;
 T_DEBUG_DATA data[DEBUG_DATA_SIZE];
} T_DEBUG_QUE, *P_DEBUG_QUE;

EXT T_DEBUG_QUE dbgQue;

void remCmd(void);

void runInit(void);
void runProcess(void);

void rspPut(char ch);
void rspPutDigit(char ch);
void rspPutByte(char ch);
void rspPutCount(void);
void rspPutHex(unsigned int val, int size);

void saveAccel(int type, int val);
void saveData(int type, int val);

void dbgNewLine(void);
void dbgPutDigit(char ch);
void dbgPutHex(unsigned int val, int size);

#endif	/* REM_CMD_INCLUDE */ // ->
#ifdef LATHECPP_REM_CMD

#include "riscvParm.h"

#define RSP_BUF_SIZE 128
typedef struct S_RSP_CTL
{
 int count;
 unsigned char *p;
 unsigned char buf[RSP_BUF_SIZE];
} T_RSP_CTL;

T_RSP_CTL rspCtl;

void rspInit(void)
{
 rspCtl.count = 0;
 rspCtl.p = rspCtl.buf;
}

void rspPut(char ch)
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
 char tmp = ch;
 ch >>= 4;
 rspPutDigit(ch);
 rspPutDigit(tmp);
}

void rspPutCount(void)
{
 char tmp;
 char ch;
 tmp = rspCtl.count - 3;
 ch = tmp;
 ch >>= 4;
 rspCtl.p = &rspCtl.buf[1];
 rspPutDigit(ch);
 rspPutDigit(tmp);
 rspCtl.count -= 2;
}

void rspPutHex(unsigned int val, int size)
{
 unsigned char tmp;
 unsigned char ch;
 int zeros = 0;
 
 rspPut(' ');
 unsigned char *p = (unsigned char *) &val;
 p += size;
 while (size != 0)
 {
  --size;
  p--;
  tmp = *p;
  ch = tmp;
  ch >>= 4;
  if ((ch != 0)
  ||  zeros)
  {
   zeros = 1;
   rspPutDigit(ch);
  }

  tmp &= 0xf;
  if ((tmp != 0)
  ||  zeros)
  {
   zeros = 1;
   rspPutDigit(tmp);
  }
 }
 if (zeros == 0)
  rspPut('0');
}

void dbgNewLine(void)
{
 dbgPutC('\n');
 dbgPutC('\r');
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

void remCmd(void)
{
 int parm;
 int val1;
 int val2;
 P_RUN_DATA data;

 rspInit();

 rspPut('-');
 rspPut(0);			/* reserve space for count */
 rspPut(0);

 remRxSkip(2);

 while (true)
 {
  data = 0;
  if (remGetHex(&parm) == 0)	/* read parameter */
   break;
  dbgPutHex(parm, 1);
  dbgPutC(' ');
  switch (parm)
  {
  case R_NONE:
  case R_OP_START:
  case R_OP_DONE:
  case R_STOP:
  case R_STOP_Z:
  case R_STOP_X:
   break;
   
  case R_RESUME:
   if (runCtl.wait == RW_PAUSE)
    runCtl.wait = RW_NONE;
   runCtl.mvStatus &= ~(MV_PAUSE | MV_MEASURE | MV_READ_X | MV_READ_Z);
   break;
  
  case R_SET_LOC_X:
   remGetHex(&val1);
   setLoc(&xAxis, val1);
   break;
   
  case R_SET_LOC_Z:
   remGetHex(&val1);
   setLoc(&zAxis, val1);
   break;

  case R_PAUSE:
  case R_START_SPIN:
  case R_STOP_SPIN:
  case R_WAIT_X:
  case R_WAIT_Z:
  {
   if (runQue.count < RUN_DATA_SIZE)
   {
    data = &runQue.data[runQue.fil];
    data->parm = parm;
   }
  }
  break;

  case R_SET_ACCEL:
   remGetHex(&val1);
   remGetHex(&val2);
   saveAccel(val1, val2);
   break;

  case R_SET_DATA:
  {
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
   rspPutHex(val.t_int32_t, riscvParm[parm]);
  }
  break;

  case R_MOVE_X:
  case R_MOVE_Z:
  case R_MOVE_REL_X:
  case R_MOVE_REL_Z:
  {
   remGetHex(&val1);
   remGetHex(&val2);
   if (runQue.count < RUN_DATA_SIZE)
   {
    data = &runQue.data[runQue.fil];
    data->parm = parm;
    data->val1 = val1;
    data->val2 = val2;
   }
  } 
  break;

  case R_PASS:
  {
   remGetHex(&val1);
   if (runQue.count < RUN_DATA_SIZE)
   {
    data = &runQue.data[runQue.fil];
    data->parm = parm;
    data->val1 = val1;
   }
  }

  case R_READ_ALL:
   rspPutByte(parm);
   rspPutHex(zAxis.curLoc, sizeof(zAxis.curLoc));
   rspPutHex(xAxis.curLoc, sizeof(xAxis.curLoc));
   rspPutHex(indexData.rpm, 2);
   rspPutHex(runCtl.pass, 2);
   rspPutHex(zAxis.dro, sizeof(zAxis.dro));
   rspPutHex(xAxis.dro, sizeof(xAxis.dro));
   rspPutHex(runCtl.mvStatus, 2);
   rspPutHex(RUN_DATA_SIZE - runQue.count, 1);
   rspPutHex(dbgQue.count, 1);
   break;

  default:
   break;
  }

  if (data != 0)
  {
   runQue.fil += 1;
   if (runQue.fil >= RUN_DATA_SIZE)
    runQue.fil = 0;
   runQue.count += 1;
  }
  runProcess();
 }
 dbgNewLine();
 int free = RUN_DATA_SIZE - runQue.count;

 dbgPutStr("que ");
 dbgPutHex(free, 1);
 dbgNewLine();

 // rspPutHex(free, 1);
 rspPut('*');
 rspPutCount();
 remPutStrLen((const unsigned char *) rspCtl.buf, rspCtl.count);
 remSendStart();

 while (true)
 {
  int tmp = remGet();
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
}

void runInit(void)
{
 memset((void *) &runQue, 0, sizeof(runQue));
 memset((void *) &runCtl, 0, sizeof(runCtl));
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
   int percent = delta * 1000 / (int) indexData.clocks;
   if (percent < 10)
    runCtl.wait = RW_NONE;
  }
 }
}

void runProcess(void)
{
 if (runCtl.wait != RW_NONE)
 {
  switch (runCtl.wait)
  {
   case RW_PAUSE:
    return;
    // break;

   case RW_SPIN_START:
   case RW_SPIN_STOP:
    spindleCheck();
    // runCtl.wait = RW_NONE;
    break;

   case RW_WAIT_X:
    if (xAxis.state == RS_IDLE)
     runCtl.wait = RW_NONE;
    break;

   case RW_WAIT_Z:
    if (zAxis.state == RS_IDLE)
     runCtl.wait = RW_NONE;
    break;

   case RW_NONE:
   default:
    runCtl.wait = RW_NONE;
    break;
  }
 }
 else
 {
  if (runQue.count != 0)
  {
   P_RUN_DATA data = &runQue.data[runQue.emp];
   runQue.emp += 1;
   if (runQue.emp >= RUN_DATA_SIZE)
    runQue.emp = 0;
   runQue.count -= 1;

   switch (data->parm)
   {
    case R_OP_START:
     runCtl.mvStatus &= ~MV_DONE;
     runCtl.mvStatus |= MV_ACTIVE;
     break;

    case R_OP_DONE:
     runCtl.mvStatus &= ~MV_ACTIVE;
     runCtl.mvStatus |= MV_DONE;
     break;

    case R_PAUSE:
     runCtl.wait = RW_PAUSE;
     runCtl.mvStatus |= MV_PAUSE;
     break;
    case R_START_SPIN:
     runCtl.wait = RW_SPIN_START;
     break;
    case R_STOP_SPIN:
     runCtl.wait = RW_SPIN_STOP;
     break;

    case R_WAIT_Z:
     runCtl.wait = RW_WAIT_Z;
     break;
    case R_WAIT_X:
     runCtl.wait = RW_WAIT_X;
     break;

    case R_PASS:
     runCtl.pass = data->val1;
     break;

    case R_MOVE_Z:
     moveZ(data->val1, data->val2);
     break;
    case R_MOVE_X:
     moveX(data->val1, data->val2);
     break;
    case R_MOVE_REL_Z:
     moveRelZ(data->val1, data->val2);
     break;
    case R_MOVE_REL_X:
     moveRelX(data->val1, data->val2);
     break;

    default:
     break;
   }
  }
 }
}

void saveAccel(int type, int val)
{
 dbgPutStr("saveAccel ");
 dbgPutHex(type, 2);
 dbgPutC(' ');
 dbgPutHex(val, 4);
 dbgNewLine();
 T_INT_BYTE tmp;
 tmp.iVal = type;
 P_ACCEL_DATA accel = accelData[tmp.bVal[1]];
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

void saveData(int type, int val)
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

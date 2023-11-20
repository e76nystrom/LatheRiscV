#define LATHECPP_AXIS_CTL

#include <string.h>
#include <stdbool.h>

#include <neorv32.h>
#include "ctlStates.h"
#include "ctlbits.h"
#include "fpgaLatheReg.h"
#include "fpgaLatheBits.h"

#define EXT extern
#include "lathe.h"
#include "axisCtl.h"
#include "fpga.h"
#include "dbgSerial.h"
//#include "remSerial.h"
#include "remCmd.h"
#include "riscvStruct.h"

#if defined(AXIS_CTL_INCLUDE)	// <-

#define R_DIR_POS 1
#define R_DIR_NEG 0

#define R_DIR_INV R_DIR_POS

#define MPG_SLOW 20
#define MPG_STEPS_COUNT 14
#define CLKS_MSEC (50000000 / 1000)
#define MPG_WAIT_DELAY 50

typedef struct S_ACCEL_DATA
{
 int initialSum;
 int incr1;
 int incr2;
 int accelVal;
 int accelCount;
 int freqDiv;
} T_ACCEL_DATA, *P_ACCEL_DATA;

EXT T_ACCEL_DATA zTurn;
EXT T_ACCEL_DATA zTaper;
EXT T_ACCEL_DATA zMove;
EXT T_ACCEL_DATA zJog;
EXT T_ACCEL_DATA zSlow;

EXT T_ACCEL_DATA xTurn;
EXT T_ACCEL_DATA xTaper;
EXT T_ACCEL_DATA xMove;
EXT T_ACCEL_DATA xJog;
EXT T_ACCEL_DATA xSlow;

EXT P_ACCEL_DATA accelData[RP_MAX];

typedef union U_INT_BYTE
{
 int iVal;
 unsigned char bVal[4];
} T_INT_BYTE;

typedef struct S_AXIS_CONSTANT
{
 char name;			/* axis name */
 int axisID;			/* axis identifier */
 struct S_AXIS_CTL *other;	/* pointer to other axis */
 int base;			/* command base for fpga */
 int accelOffset;		/* offset into accel table */
 int clkShift;			/* clk register shift */
 int statDone;			/* axis done flag */
 int statEna;			/* axis enable flag */
 int waitState;			/* axis wait state */
 int dbgBase;			/* base axis dbg types */
 int jogFlag;			/* jog enable flag */
 uint32_t *mpgData;		/* pointer to jog fifo */
 int *mpgJogInc;		/* mpg jog increment */
} T_AXIS_CONSTANT;

typedef struct S_AXIS_CTL
{
 enum RISCV_AXIS_STATE_TYPE state; /* current wait state */
 enum RISCV_AXIS_STATE_TYPE lastState; /* last state */
 enum MPG_STATE mpgState;	/* mpg state */
 enum MPG_STATE lastMpgState;	/* last mpg state */
 int mpgInvert;			/* invert direction of mpg */
 int cmd;			/* current command flags */
 int dist;			/* distance to move */
 int dir;			/* current direction */
 int ctlFlag;			/* axisCtl flags */

 int curDist;			/* backlash distance */
 int curLoc;			/* current location */
 int expLoc;			/* expected location */
 int stepsInch;			/* steps per inch */
 int homeOffset;		/* home offset */
 int savedLoc;			/* save location for taper */
 int dro;			/* dro reading */

 int endLoc;
 int endDist;
 int endDro;

 int ignore;			/* ignore after first error */

 uint32_t dirWaitStart;		/* dir change timer start */
 int backlashSteps;		/* backlash steps */
 int mpgAxisCtl;			/* axis control settings in mpg mode */

 T_AXIS_CONSTANT c;		/* axis constant data */
} T_AXIS_CTL, *P_AXIS_CTL;

extern T_AXIS_CTL zAxis;
extern T_AXIS_CTL xAxis;

typedef struct S_INDEX_DATA
{
 uint32_t clocks;
 uint32_t lastClocks;
 uint32_t lastTime;
 uint32_t rpm;
} T_INDEX_DATA;

EXT T_INDEX_DATA indexData;
EXT uint32_t lastJogPause;
EXT int clockSelVal;

#define FPGA_FREQUENCY 50000000
#define INDEX_INTERVAL 100

void initAccelTable(void);
void initAxisCtl(void);
void axisCtl(void);
void axisStateCheck(P_AXIS_CTL axis);
void axisCheck(P_AXIS_CTL axis, int status);
void move(P_AXIS_CTL axis, int cmd, int loc);
void jogMove(P_AXIS_CTL axis, int dist);
void jogMpg(P_AXIS_CTL axis);
void moveRel(P_AXIS_CTL axis, int dist, int cmd);
void moveBacklash(P_AXIS_CTL axis);
void setLoc(P_AXIS_CTL axis, int loc);
void axisStop(P_AXIS_CTL a);
void axisMove(P_AXIS_CTL a);
void clockLoad(P_AXIS_CTL axis, int clkSel);
void axisLoad(P_AXIS_CTL a, int index);

char *fmtLoc(char *buf, P_AXIS_CTL axis, int loc);
char *fmtDist(char *buf, P_AXIS_CTL axis, int dist);

#endif	/* AXIS_CTL_INCLUDE */ // ->
#if defined(LATHECPP_AXIS_CTL)

char *fmtLoc(char *buf, const P_AXIS_CTL axis, int loc)
{
 loc -= axis->homeOffset;
 loc *= 10000;
 loc /= axis->stepsInch;
 return dbgFmtNum(buf, loc);
}

char *fmtDist(char *buf, const P_AXIS_CTL axis, int dist)
{
 dist *= 10000;
 dist /= axis->stepsInch;
 return dbgFmtNum(buf, dist);
}

void initAccelTable(void)
{
 accelData[RP_Z_TURN]  = &zTurn;
 accelData[RP_Z_TAPER] = &zTaper;
 accelData[RP_Z_MOVE]  = &zMove;
 accelData[RP_Z_JOG]   = &zJog;
 accelData[RP_Z_SLOW]  = &zSlow;

 accelData[RP_X_TURN]  = &xTurn;
 accelData[RP_X_TAPER] = &xTaper;
 accelData[RP_X_MOVE]  = &xMove;
 accelData[RP_X_JOG]   = &xJog;
 accelData[RP_X_SLOW]  = &xSlow;
}

T_AXIS_CTL zAxis;
T_AXIS_CTL xAxis;

T_AXIS_CONSTANT zInitData =
{
 .name        = 'z',
 .axisID      = RA_Z_AXIS,
 .other       = &xAxis,
 .base        = F_ZAxis_Base,
 .accelOffset = RP_Z_BASE,
 .clkShift    = Z_FREQ_SHIFT,
 .statDone    = Z_AXIS_DONE,
 .statEna     = Z_AXIS_ENA,
 .waitState   = RW_WAIT_Z,
 .dbgBase     = D_ZBASE,
 .jogFlag     = PAUSE_ENA_Z_JOG,
 .mpgData     = (uint32_t *) &CFS->zMpg,
 .mpgJogInc   = &rVar.rZJogInc,
};

T_AXIS_CONSTANT xInitData =
{
 .name        = 'x',
 .axisID      = RA_X_AXIS,
 .other       = &zAxis,
 .base        = F_XAxis_Base,
 .accelOffset = RP_X_BASE,
 .clkShift    = X_FREQ_SHIFT,
 .statDone    = X_AXIS_DONE,
 .statEna     = X_AXIS_ENA,
 .waitState   = RW_WAIT_X,
 .jogFlag     = PAUSE_ENA_X_JOG,
 .dbgBase     = D_XBASE,
 .mpgData     = (uint32_t *) &CFS->xMpg,
 .mpgJogInc   = &rVar.rXJogInc,
};

void initAxisCtl(void)
{
 printf("InitAxisCtl\n");
 memset(&zAxis, 0, sizeof(zAxis));
 memset(&xAxis, 0, sizeof(xAxis));

 memcpy(&zAxis.c, &zInitData, sizeof(T_AXIS_CONSTANT));
 printf("z %d %d %d\n", zAxis.c.axisID, zAxis.c.clkShift, zAxis.c.dbgBase);

 memcpy(&xAxis.c, &xInitData, sizeof(T_AXIS_CONSTANT));
 printf("x %d %d %d\n", xAxis.c.axisID, xAxis.c.clkShift, xAxis.c.dbgBase);

 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, 0);
 const uint32_t zCtl = rd(F_ZAxis_Base + F_Rd_Axis_Ctl);

 ld(F_XAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_XAxis_Base + F_Ld_Axis_Ctl, 0);
 const uint32_t xCtl = rd(F_XAxis_Base + F_Rd_Axis_Ctl);
 printf("zCtl %x xCtl %x\n", zCtl, xCtl);

 const uint32_t zStat = rd(F_ZAxis_Base + F_Rd_Axis_Status);
 const uint32_t xStat = rd(F_XAxis_Base + F_Rd_Axis_Status);
 printf("zStat %x xStat %x\n", zStat, xStat);
}

uint32_t delta;
uint32_t lastMvStatus;
uint32_t lastStatus;

#include "statusRegStr.h"
#include "mvStatusBitsStr.h"
#include "pauseBitsStr.h"

void axisCtl(void)
{
 const uint32_t t = millis();
 delta = t - indexData.lastTime;
 if (delta > INDEX_INTERVAL)
 {
  // dbgPutC('.');
  // dbgPutHex(delta, 4);
  indexData.lastTime = t;
  uint32_t clocks = rd(F_Rd_Idx_Clks);
  indexData.clocks = clocks;
  if (clocks != 0)
  {
   clocks /= 60;
   indexData.rpm = FPGA_FREQUENCY / clocks;
  }
  else
   indexData.rpm = 0;
 }

 if (rVar.rMvStatus != lastMvStatus)
 {
  dbgPutStr("mvStatus ");
  dbgPutHex(rVar.rMvStatus, 2);
  int mask = 1;
  lastMvStatus = rVar.rMvStatus;
  for (int i = 0; i < R_MV_MAX; i++)
  {
   if (rVar.rMvStatus & mask)
   {
    const char *p = (char *) &mvStatusBitsStr[i];
    dbgPutSpace();
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   mask <<= 1;
  }
  dbgNewLine();
 }

 if (rVar.rJogPause != lastJogPause)
 {
  dbgPutStr("jogPause ");
  dbgPutHexByte(rVar.rJogPause);
  int mask = 1;
  lastJogPause = rVar.rJogPause;
  for (int i = 0; i < R_PAUSE_MAX; i++)
  {
   if (rVar.rJogPause & mask)
   {
    const char *p = (char *) &pauseBitsStr[i];
    dbgPutSpace();
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   mask <<= 1;
  }
  dbgNewLine();
 }

 const int status = rd(F_Rd_Status);
 if (status != lastStatus)
 {
  lastStatus = status;
  dbgPutStr("status ");
  dbgPutHex(status, 2);
  // ReSharper disable once CppRedundantParentheses
  int mask = 1 << (STATUS_SIZE - 1);
  int i = STATUS_SIZE - 1;
  while (true)
  {
   if (status & mask)
   {
    const char *p = (char *) &statusRegStr[i];
    dbgPutSpace();
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   mask >>= 1;
   if (mask == 0)
    break;
   i -= 1;
  }
  dbgNewLine();
 }
 
 axisCheck(&zAxis, status);
 axisCheck(&xAxis, status);
}

void axisStateCheck(const P_AXIS_CTL axis)
{
 if (axis->state != axis->lastState)
 {
  axis->lastState = axis->state;
  dbgMsg(axis->c.dbgBase + D_ST, axis->state);
 }
}

void axisCheck(const P_AXIS_CTL axis, const int status)
{
 const int base = axis->c.base;

 axis->dro = (int) rd(base + F_Sync_Base + F_Rd_Dro);
 axis->curLoc = (int) rd(base + F_Sync_Base + F_Rd_Loc);

 if (axis->state != RS_IDLE)
 {
  // int value = (int) rd(base + F_Rd_Axis_Ctl);
  const int doneStat = axis->state == RS_WAIT_TAPER ?
                        axis->c.other->c.statDone : axis->c.statDone;

  if (status & doneStat)
  {
   dbgPutC(axis->c.name);
   dbgPutStr(" axis done\n");

   if (axis->state == RS_WAIT_BACKLASH)
   {
    axis->ctlFlag &= ~CTL_BACKLASH;
    axisMove(axis);
    axis->state = RS_WAIT;
   }
   else if (axis->state == RS_WAIT ||
	    axis->state == RS_WAIT_TAPER)
   {
    axis->endDist = (int) rd(base + F_Sync_Base + F_Rd_Dist);
    axis->endLoc =  (int) rd(base + F_Sync_Base + F_Rd_Loc);
    axis->endDro =  (int) rd(base + F_Sync_Base + F_Rd_Dro);
    
    dbgMsg(axis->c.dbgBase + D_EXP, axis->expLoc);
    dbgMsg(axis->c.dbgBase + D_LOC, axis->endLoc);
    dbgMsg(axis->c.dbgBase + D_DRO, axis->endDro);

    char locBuf[16];
    char droBuf[16];
    fmtLoc(locBuf, axis, axis->endLoc);
    fmtLoc(droBuf, axis, axis->endDro);
    printf("dist %d loc %s dro %s\n", axis->endDist, locBuf, droBuf);
    printf("delta %d\n", axis->endLoc - axis->expLoc);

    const int xPos = rd(base + F_Sync_Base + F_Rd_XPos);
    dbgMsg(axis->c.dbgBase + D_X, xPos);
    const int yPos = rd(base + F_Sync_Base + F_Rd_YPos);
    dbgMsg(axis->c.dbgBase + D_Y, yPos);
    printf("xPos %d yPos %d\n", xPos, yPos);
    
    dbgPutHex(status, 2);
    dbgPutSpace();
    dbgPutC(axis->c.name);
    dbgPutStr(" axis idle\n\n");

    clockLoad(axis, CLK_NONE);
    ld(base + F_Ld_Axis_Ctl, 0);
   
    axis->ctlFlag = 0;
    axis->state = RS_IDLE;
    if (runCtl.wait == axis->c.waitState)
     runCtl.wait = RW_NONE;
   }
  }
 }
#if 1
 else
 {
  jogMpg(axis);
 }
#endif
 
 axisStateCheck(axis);

 if (axis->state != RS_IDLE &&
     (status & axis->c.statEna) == 0)
 {
  if (axis->ignore == 0)
  {
   axis->ignore = 1;
   printf("waiting no enable\n");
  }
 }
 else
  axis->ignore = 0;
}

void move(const P_AXIS_CTL axis, const int cmd, const int loc)
{
 dbgMsg(axis->c.dbgBase + D_MOV, loc);

 char buf[16];
 axis->expLoc = loc;
 const int pos = rd(axis->c.base + F_Sync_Base + F_Rd_Loc);
 dbgMsg(axis->c.dbgBase + D_CUR, pos);

 dbgPutC(axis->c.name);
 dbgPutStr(" move ");
 dbgPutHex(cmd, 2);
 dbgPutStr(" pos ");
 dbgPutStr(fmtLoc(buf, axis, pos));
 dbgPutStr(" loc ");
 dbgPutStr(fmtLoc(buf, axis, loc));
 dbgNewLine();

 const int dist = loc - pos;
 moveRel(axis, dist, cmd);
}

void jogMove(const P_AXIS_CTL axis, const int dist)
{
 dbgPutC(axis->c.name);
 dbgPutStr(" jog move\n");
 if (axis->state == RS_IDLE)
  moveRel(axis, dist, DIST_MODE | CMD_JOG);
 else
  ld(axis->c.base + F_Sync_Base + F_Ld_Dist, dist);
}

#include "mpgStateStr.h"

void jogMpg(const P_AXIS_CTL axis)
{
 switch (axis->mpgState)
 {
 case MPG_DISABLED:
  if (rVar.rJogPause == 0 ||
      (rVar.rJogPause & axis->c.jogFlag) != 0)
  {
   if (accelData[axis->c.accelOffset + A_JOG]->initialSum != 0)
   {
    axis->dist = 0;
    axis->curDist = 0;

    axisLoad(axis, axis->c.accelOffset + A_JOG);
    axis->mpgAxisCtl = CTL_DIST_MODE | CTL_START;
    if (axis->dir == R_DIR_POS)
     axis->mpgAxisCtl |= CTL_DIR;
    ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl);
    clockLoad(axis, CLK_FREQ);

    axis->mpgState = MPG_CHECK_QUE;
   }
  }
  break;
  
 case MPG_CHECK_QUE:
 {
  if (rVar.rJogPause & DISABLE_JOG &&
      (rVar.rJogPause & axis->c.jogFlag) == 0)
  {
   ld(axis->c.base + F_Ld_Axis_Ctl, 0);
   axis->mpgState = MPG_DISABLED;
   break;
  }

  int mpgDelta = *axis->c.mpgData; /* read data from fifo */

  if ((mpgDelta & MPG_EMPTY) != 0) /* if no data */
   return;			/* exit */

  dbgPutHexByte(mpgDelta);
  dbgPutSpace();
  
  int dir = (mpgDelta & MPG_DIR) != 0 ? R_DIR_POS : R_DIR_NEG; /* get dir */
  dir ^= axis->mpgInvert;	/* invert direction if needed */
  mpgDelta &= MPG_MASK;		/* mask off direction bit */

  if (dir != axis->dir)		/* if direction change */
  {
   axis->dir = dir;		/* save direction */
   axis->dirWaitStart = millis(); /* save start time */
   const int axisStat = rd(axis->c.base + F_Rd_Status); /* read axis status */
   if ((axisStat & AX_DIST_ZERO) == 0) /* if axis active */
   {
    const int dist = rd(axis->c.base + F_Sync_Base + F_Rd_Accel_Steps);
    ld(axis->c.base + F_Sync_Base + F_Ld_Dist, dist); /* set min dist to go */
   }
   axis->mpgState = MPG_DIR_CHANGE_WAIT; /* wait for stop */
  }

  int dist = *axis->c.mpgJogInc;
  if (dist == 0)		/* if continuous */
  {
   if (mpgDelta < MPG_SLOW)	/* if fast jog */
    dist = MPG_STEPS_COUNT;
   else				/* if slow */
    dist = 1;

   // const int ctr = mpgDelta * CLKS_MSEC / dist;
   // ld(axis->c.base + F_Ld_Freq, ctr);
  }
  else				/* if incremental */
  {
   // const int ctr = MPG_SLOW * 1000 * CLKS_MSEC / MPG_STEPS_COUNT;
   // ld(axis->c.base + F_Ld_Freq, ctr);
  }

  ld(axis->c.base + F_Sync_Base + F_Ld_Dist, dist); /* set min dist to go */
 }
 break;

 case MPG_DIR_CHANGE_WAIT:
 {
  const int axisStat = rd(axis->c.base + F_Rd_Status); /* read axis status */
  if ((axisStat & AX_DIST_ZERO) == 0 ||	/* if axis active */
      millis() - axis->dirWaitStart > MPG_WAIT_DELAY) /* or delay done */
  {
   if (axis->dir == R_DIR_POS)
    axis->mpgAxisCtl |= CTL_DIR;
   else
    axis->mpgAxisCtl &= ~CTL_DIR;

   ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl);

   if (axis->backlashSteps != 0)
   {
    axis->mpgAxisCtl |= CTL_BACKLASH;
    ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl);
    axis->mpgAxisCtl &= ~CTL_BACKLASH;

    ld(axis->c.base + F_Sync_Base + F_Ld_Dist, axis->backlashSteps);
    axis->mpgState = MPG_WAIT_BACKLASH;
   }
   else
    axis->mpgState = MPG_CHECK_QUE;
  }
 }
  break;

 case MPG_WAIT_BACKLASH:
 {
  const int axisStat = rd(axis->c.base + F_Rd_Axis_Status); /* read axis status */
  if ((axisStat & AX_DIST_ZERO) != 0) /* if done */
  {
   ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl); /* clear backlash */

   while ((*axis->c.mpgData & MPG_EMPTY) == 0) /* while fifo not empty */
    ;

   axis->mpgState = MPG_CHECK_QUE; /* return to check queue state */
  }
  break;
 }

 default:
  break;
 } /* end switch */

 if (axis->mpgState != axis->lastMpgState)
 {
  axis->lastMpgState = axis->mpgState;
  dbgPutC(axis->c.name);
  dbgPutStr(" mpgState ");
  dbgPutHexByte(axis->mpgState);
  dbgPutSpace();
  const char *p = (char *) &mpgStateStr[axis->mpgState];
  dbgPutC(*p++);
  dbgPutC(*p);
  dbgNewLine();
 }
}

void moveRel(const P_AXIS_CTL axis, const int dist, const int cmd)
{
 dbgMsg(axis->c.dbgBase + D_MVCM, cmd);
 dbgMsg(axis->c.dbgBase + D_DST, dist);

 char buf[16];
 dbgPutC(axis->c.name);
 dbgPutStr(" moveRel ");
 dbgPutHex(cmd, 2);
 dbgPutSpace();
 dbgPutStr(fmtLoc(buf, axis, axis->expLoc));
 dbgPutSpace();
 dbgPutHex(dist, 4);
 dbgPutSpace();
 dbgPutStr(fmtDist(buf, axis, dist));
 dbgNewLine();
  
 if (axis->state == RS_IDLE)
 {
  axis->ctlFlag = 0;			/* clear control flag */
  axis->cmd = cmd;
  if (dist != 0)			/* if distance non zero */
  {
   int dirChange;
   if (dist > 0)			/* if moving positive */
   {
    axis->dist = dist;			/* save distance */
    dirChange = axis->dir != R_DIR_POS; /* dir chg */
    axis->dir = R_DIR_POS;		/* move positive direction */
    axis->ctlFlag |= CTL_DIR;		/* set direction positive */
   }
   else
   {
    axis->dist = -dist;			/* make positive */
    dirChange = axis->dir != R_DIR_NEG; /* dir chg */
    axis->dir = R_DIR_NEG;		/* set move direction to negative */
   }

   if (dirChange &&			/* if direction change */
       axis->backlashSteps != 0)	/* and backlash present */
   {
    moveBacklash(axis);
    axis->state = RS_WAIT_BACKLASH;	/* set to wait for backlash */
   }
   else					/* if no backlash */
   {
    axisMove(axis);
    axis->state = RS_WAIT;		/* set to start move */
    runCtl.wait = axis->c.waitState;
    dbgPutC(axis->c.name);
    dbgPutStr(" axis wait\n");
   }
  }
 }
 axisStateCheck(axis);
}

void moveBacklash(const P_AXIS_CTL axis)
{
 axis->curDist = axis->backlashSteps;
 axisLoad(axis, axis->c.accelOffset + A_JOG);
 axis->ctlFlag |= CTL_BACKLASH;	/* set direction positive */

 const int tmp = axis->ctlFlag | CTL_START;
 dbgMsg(axis->c.dbgBase + D_ACTL, tmp);
 ld(axis->c.base + F_Ld_Axis_Ctl, tmp);
}

void setLoc(const P_AXIS_CTL axis, const int loc)
{
 const int base = axis->c.base;
 
 ld(base + F_Sync_Base + F_Ld_Loc, loc);
 ld(base + F_Sync_Base + F_Ld_Dro, loc);
 ld(base + F_Ld_Axis_Ctl, CTL_SET_LOC | CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

void axisStop(const P_AXIS_CTL a)
{
 dbgPutC(a->c.name);
 dbgPutStr(" axisStop\n");

 ld(a->c.base + F_Ld_Axis_Ctl, 0);
 clockLoad(a, CLK_NONE);
 a->state = RS_IDLE;
}

void axisMove(const P_AXIS_CTL a)
{
 dbgPutC(a->c.name);
 dbgPutStr(" axisMove\n");

 int accelIndex = -1;
 int clkSel = CLK_FREQ;
 switch (a->cmd & CMD_MSK)
 {
 case CMD_SYN:
  accelIndex = A_TURN;
  clkSel = CLK_CH;
  if (a->cmd & SYN_START)
   a->ctlFlag |= CTL_WAIT_SYNC;
  
  if (a->cmd & SYN_TAPER)
  {
   const P_AXIS_CTL aT = a->c.other;	/* get other axis */
   aT->curLoc = (int) rd(aT->c.base + F_Sync_Base + F_Rd_Loc);
   dbgMsg(aT->c.dbgBase + D_MOV, aT->savedLoc);
   dbgMsg(aT->c.dbgBase + D_CUR, aT->curLoc);
   aT->expLoc = aT->savedLoc;

   int slvDist = aT->savedLoc - aT->curLoc;
   dbgMsg(aT->c.dbgBase + D_DST, slvDist);

   int slvCtl = CTL_SLAVE;
   if (slvDist > 0)
    slvCtl |= CTL_DIR;
   else
    slvDist = -slvDist;
   aT->curDist = slvDist;
   axisLoad(aT, aT->c.accelOffset + A_TAPER);
   dbgMsg(aT->c.dbgBase + D_ACTL, slvCtl);
   ld(aT->c.base + F_Ld_Axis_Ctl, slvCtl);
   clockLoad(aT, CLK_SLV_CH);

   aT->state = RS_WAIT_TAPER;
  }
  break;

 case CMD_JOG:
  if (a->cmd & DIST_MODE)
   a->ctlFlag |= CTL_DIST_MODE;
  accelIndex = A_JOG;
  break;

 case CMD_MOV:
 case CMD_MAX:
  accelIndex = A_MOVE;
  break;

  // case CMD_SPEED:
  //  break;

 case JOG_SLOW:
  accelIndex = A_SLOW;
  break;

 default:
  break;
 }

 if (accelIndex >= 0)
 {
  axisLoad(a, a->c.accelOffset + accelIndex);
  clockLoad(a, clkSel);
  const int tmp = a->ctlFlag | CTL_START;
  dbgMsg(a->c.dbgBase + D_ACTL, tmp);
  ld(a->c.base + F_Ld_Axis_Ctl, tmp);
 }
}

void clockLoad(const P_AXIS_CTL axis, const int clkSel)
{
 const int shift = axis->c.clkShift;
 clockSelVal &= ~(CLK_MASK << shift);
 clockSelVal |= clkSel << shift;
 ld(F_Ld_Clk_Ctl, clockSelVal);
}

extern T_CH2 axisAccelTypeStr[];

void axisLoad(const P_AXIS_CTL a, const int index)
{
 dbgPutC(a->c.name);
 dbgPutStr(" axisLoad ");
 const char *p = (char *) &axisAccelTypeStr[index];
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgPutSpace();
 dbgPutHexByte(index);
 dbgNewLine();

 const int base = a->c.base;
 const P_ACCEL_DATA aData = accelData[index];
 if (aData->freqDiv != 0)
  ld(base + F_Ld_Freq, aData->freqDiv);

 const int bSyn = a->c.base + F_Sync_Base;
 ld(bSyn + F_Ld_D, aData->initialSum);
 ld(bSyn + F_Ld_Incr1, aData->incr1);
 ld(bSyn + F_Ld_Incr2, aData->incr2);

 ld(bSyn + F_Ld_Accel_Val, aData->accelVal);
 ld(bSyn + F_Ld_Accel_Count, aData->accelCount);

 int dist = a->curDist;
 if (dist != 0)
 {
  a->curDist = 0;
 }
 else
 {
  dist = a->dist;
 }
 ld(base + F_Sync_Base + F_Ld_Dist, dist);
 ld(base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

#endif  /* LATHECPP_AXIS_CTL */

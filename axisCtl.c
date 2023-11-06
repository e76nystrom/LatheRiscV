#include <string.h>
#define LATHECPP_AXIS_CTL

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
#include "remSerial.h"
#include "remCmd.h"
#include "riscvStruct.h"

#if defined(AXIS_CTL_INCLUDE)	// <-

#include <stdbool.h>

#define R_DIR_POS 1
#define R_DIR_NEG 0

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
} T_AXIS_CONSTANT;

typedef struct S_AXIS_CTL
{
 enum RISCV_AXIS_STATE_TYPE state; /* current wait state */
 enum RISCV_AXIS_STATE_TYPE lastState; /* last state */
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

 int backlashSteps;		/* backlash steps */
 
 T_AXIS_CONSTANT c;		/* axis constant data */
#if 0
 char name;			/* axis name */
 int axisID;			/* axis identifier */
 struct S_AXIS_CTL *other;	/* pointer to other axis */
 int base;			/* command base for fpga */
 int accelOffset;		/* offset into accel table */
 int clkShift;			/* clk register shift */
 int statDone;			/* axis done flag */
 int statEna;			/* axis enable flag */
 int waitState;			/* axis wait state */
#endif
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

#define FPGA_FREQUENCY 50000000
#define INDEX_INTERVAL 100

void initAccelTable(void);
void initAxisCtl(void);
void axisStateCheck(P_AXIS_CTL axis);
void axisCheck(P_AXIS_CTL axis, int status);
void axisCtl(void);
void axisLoad(P_AXIS_CTL a, int index);
void axisMove(P_AXIS_CTL a);
void moveZ(int cmd, int loc);
void moveX(int cmd, int loc);
void moveRelZ(int cmd, int dist);
void moveRelX(int cmd, int dist);
void moveRel(P_AXIS_CTL axis, int cmd, int dist);
void setLoc(P_AXIS_CTL axis, int loc);

char *fmtLoc(char *buf, P_AXIS_CTL axis, int loc);
char *fmtDist(char *buf, P_AXIS_CTL axis, int dist);

#endif	/* AXIS_CTL_INCLUDE */ // ->
#if defined(LATHECPP_AXIS_CTL)

char *fmtLoc(char *buf, P_AXIS_CTL axis, int loc)
{
 loc -= axis->homeOffset;
 loc *= 10000;
 loc /= axis->stepsInch;
 return(dbgFmtNum(buf, loc));
}

char *fmtDist(char *buf, P_AXIS_CTL axis, int dist)
{
 dist *= 10000;
 dist /= axis->stepsInch;
 return(dbgFmtNum(buf, dist));
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
 .dbgBase     = D_ZBASE
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
 .dbgBase     = D_XBASE
};

void initAxisCtl(void)
{
 printf("InitAxisCtl\n");
 memset((void *) &zAxis, 0, sizeof(zAxis));
 memset((void *) &xAxis, 0, sizeof(xAxis));

 memcpy((void *) &zAxis.c, (void *) &zInitData, sizeof(T_AXIS_CONSTANT));
 printf("z %d %d %d\n", zAxis.c.axisID, zAxis.c.clkShift, zAxis.c.dbgBase);

 memcpy((void *) &xAxis.c, (void *) &xInitData, sizeof(T_AXIS_CONSTANT));
 printf("x %d %d %d\n", xAxis.c.axisID, xAxis.c.clkShift, xAxis.c.dbgBase);

 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, 0);
 uint32_t zCtl = rd(F_ZAxis_Base + F_Rd_Axis_Ctl);

 ld(F_XAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_XAxis_Base + F_Ld_Axis_Ctl, 0);
 uint32_t xCtl = rd(F_XAxis_Base + F_Rd_Axis_Ctl);
 printf("zCtl %x xCtl %x\n", zCtl, xCtl);

 uint32_t zStat = rd(F_ZAxis_Base + F_Rd_Axis_Status);
 uint32_t xStat = rd(F_XAxis_Base + F_Rd_Axis_Status);
 printf("zStat %x xStat %x\n", zStat, xStat);
}

uint32_t delta;
uint32_t lastMvStatus;
uint32_t lastStatus;

#include "statusRegStr.h"

T_CH2 mvDbg[] =
{
 {'P', 'A'},
 {'R', 'X'},
 {'R', 'Z'},
 {'A', 'C'},
 {'D', 'N'},
 {'X', 'L'},
 {'Z', 'L'},
 {'X', 'A'},
 {'X', 'H'},
 {'Z', 'A'},
 {'Z', 'H'},
 {'M', 'S'},
 {'E', 'S'}
};

void axisCtl(void)
{
 uint32_t t = millis();
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
  for (int i = 0; i < 12; i++)
  {
   if (rVar.rMvStatus & mask)
   {
    char *p = (char *) &mvDbg[i];
    dbgPutC(' ');
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   mask <<= 1;
  }
  dbgNewLine();
 }

 int status = (int) rd(F_Rd_Status);
 if (status != lastStatus)
 {
  lastStatus = status;
  dbgPutStr("status ");
  dbgPutHex(status, 2);
  int mask = 1 << (STATUS_SIZE-1);
  int i = (STATUS_SIZE-1);
  while (true)
  {
   if (status & mask)
   {
    char *p = (char *) &statusRegStr[i];
    dbgPutC(' ');
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

void axisStateCheck(P_AXIS_CTL axis)
{
 if (axis->state != axis->lastState)
 {
  axis->lastState = axis->state;
  dbgMsg(axis->c.dbgBase + D_ST, axis->state);
 }
}

void axisCheck(P_AXIS_CTL axis, int status)
{
 int base = axis->c.base;

 axis->dro = (int) rd(base + F_Sync_Base + F_Rd_Dro);

 if (axis->state != RS_IDLE)
 {
  // int value = (int) rd(base + F_Rd_Axis_Ctl);
  axis->curLoc = (int) rd(base + F_Sync_Base + F_Rd_Loc);

  int doneStat = (axis->state == RS_WAIT_TAPER ?
		  axis->c.other->c.statDone : axis->c.statDone);

  if (status & doneStat)
  {
   dbgPutC(axis->c.name);
   dbgPutStr(" axis done\n");

   ld(base + F_Ld_Axis_Ctl , CTL_INIT);
   ld(base + F_Ld_Axis_Ctl, 0);
   
   if (axis->state == RS_WAIT_BACKLASH)
   {
    axis->ctlFlag &= ~CTL_BACKLASH;
    axisMove(axis);
    axis->state = RS_WAIT;
   }
   else if ((axis->state == RS_WAIT) ||
	    (axis->state == RS_WAIT_TAPER))
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
    // int axisStatus = (int) rd(base + F_Rd_Axis_Status);

    dbgPutHex(status, 2);
    dbgPutC(' ');
    dbgPutC(axis->c.name);
    dbgPutStr(" axis idle\n\n");

    axis->ctlFlag = 0;
    axis->state = RS_IDLE;
    if (runCtl.wait == axis->c.waitState)
     runCtl.wait = RW_NONE;
   }
  }
 }

 axisStateCheck(axis);

 if ((axis->state != RS_IDLE) &&
     ((status & axis->c.statEna) == 0))
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

#include "accelTypeStr.h"

void axisLoad(P_AXIS_CTL a, int index)
{
 dbgPutC(a->c.name);
 dbgPutStr(" axisLoad ");
 char *p = (char *) &accelTypeStr[index];
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgPutC(' ');
 dbgPutHex(index, 1);
 dbgNewLine();
 
 int base = a->c.base;
 P_ACCEL_DATA aData = accelData[index];
 if (aData->freqDiv != 0)
  ld(base + F_Ld_Freq, aData->freqDiv);

 int bSyn = a->c.base + F_Sync_Base;
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

#define A_TURN  (RP_Z_TURN  - RP_Z_BASE)
#define A_TAPER (RP_Z_TAPER - RP_Z_BASE)
#define A_MOVE  (RP_Z_MOVE  - RP_Z_BASE)
#define A_JOG   (RP_Z_JOG   - RP_Z_BASE)
#define A_SLOW  (RP_Z_SLOW  - RP_Z_BASE)

void axisMove(P_AXIS_CTL a)
{
 dbgPutC(a->c.name);
 dbgPutStr(" axisMove\n");

 int accelIndex = -1;
 int clkSel = CLK_FREQ << a->c.clkShift;
 switch (a->cmd & CMD_MSK)
 {
 case CMD_SYN:
  accelIndex = A_TURN;
  clkSel = CLK_CH << a->c.clkShift;
  if (a->cmd & SYN_START)
   a->ctlFlag = CTL_WAIT_SYNC;
  if (a->cmd & SYN_TAPER)
  {
   P_AXIS_CTL aT = a->c.other;	/* get other axis */
   int slvCtl = CTL_SLAVE;
   int slvDist = aT->savedLoc - aT->curLoc;
   if (slvDist > 0)
    slvCtl |= CTL_DIR;
   else
    slvDist = -slvDist;
   aT->curDist = slvDist;
   axisLoad(aT, aT->c.accelOffset + A_TAPER);
   aT->curLoc = (int) rd(aT->c.base + F_Sync_Base + F_Rd_Loc);
   ld(aT->c.base + F_Ld_Axis_Ctl, slvCtl);
   clkSel |= CLK_SLV_CH << aT->c.clkShift;
  }
  break;

 case CMD_JOG:
  accelIndex = A_JOG;
  break;

 case CMD_MOV:
 case CMD_MAX:
  accelIndex = A_MOVE;
  break;

  // case CMD_SPEED:
  //  break;

 case JOGSLOW:
  accelIndex = A_SLOW;
  break;

 default:
  break;
 }

 if (accelIndex >= 0)
 {
  axisLoad(a, a->c.accelOffset + accelIndex);
  ld(F_Ld_Clk_Ctl, clkSel);
  ld(a->c.base + F_Ld_Axis_Ctl, a->ctlFlag | CTL_START);
 }
}

void moveZ(int cmd, int loc)
{
 dbgMsg(zAxis.c.dbgBase + D_MOV, loc);

 char buf[16];
 zAxis.expLoc = loc;
 int pos = (int) rd(F_ZAxis_Base + F_Sync_Base + F_Rd_Loc);
 dbgMsg(zAxis.c.dbgBase + D_LOC, pos);

 dbgPutStr("z move ");
 dbgPutHex(cmd, 2);
 dbgPutStr(" pos ");
 dbgPutStr(fmtLoc(buf, &zAxis, pos));
 dbgPutStr(" loc ");
 dbgPutStr(fmtLoc(buf, &zAxis, loc));
 dbgNewLine();

 int dist = loc - pos;
 moveRel(&zAxis, cmd, dist);
}

void moveX(int cmd, int loc)
{
 dbgMsg(xAxis.c.dbgBase + D_MOV, loc);

 char buf[16];
 xAxis.expLoc = loc;
 int pos = (int) rd(F_XAxis_Base + F_Sync_Base + F_Rd_Loc);
 dbgMsg(xAxis.c.dbgBase + D_LOC, pos);

 dbgPutStr("x move ");
 dbgPutHex(cmd, 2);
 dbgPutStr(" pos ");
 dbgPutStr(fmtLoc(buf, &xAxis, pos));
 dbgPutStr(" loc ");
 dbgPutStr(fmtLoc(buf, &xAxis, loc));
 dbgNewLine();

 int dist = loc - pos;
 moveRel(&xAxis, cmd, dist);
}

void moveRel(P_AXIS_CTL axis, int cmd, int dist)
{
 dbgMsg(axis->c.dbgBase + D_DST, dist);

 char buf[16];
 dbgPutC(axis->c.name);
 dbgPutStr(" moveRel ");
 dbgPutHex(cmd, 2);
 dbgPutC(' ');
 dbgPutStr(fmtLoc(buf, axis, axis->expLoc));
 dbgPutC(' ');
 dbgPutHex(dist, 4);
 dbgPutC(' ');
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
    dirChange = (axis->dir != R_DIR_POS); /* dir chg */
    axis->dir = R_DIR_POS;		/* move positive direction */
    axis->ctlFlag |= CTL_DIR;		/* set direction positive */
   }
   else
   {
    axis->dist = -dist;			/* make positive */
    dirChange = (axis->dir != R_DIR_NEG); /* dir chg */
    axis->dir = R_DIR_NEG;		/* set move direction to negative */
   }

   if (dirChange &&			/* if direction change */
       (axis->backlashSteps != 0))	/* and backlash present */
   {
    axis->curDist = axis->backlashSteps;
    axisLoad(axis, axis->c.accelOffset + A_JOG);
    axis->ctlFlag |= CTL_BACKLASH;	/* set direction positive */

    ld(axis->c.base + F_Ld_Axis_Ctl, axis->ctlFlag | CTL_START);

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

void moveRelZ(int cmd, int dist)
{
 moveRel(&zAxis, cmd, dist);
}

void moveRelX(int cmd, int dist)
{
 moveRel(&xAxis, cmd, dist);
}

void setLoc(P_AXIS_CTL axis, int loc)
{
 int base = axis->c.base;
 
 ld(base + F_Sync_Base + F_Ld_Loc, loc);
 ld(base + F_Sync_Base + F_Ld_Dro, loc);
 ld(base + F_Ld_Axis_Ctl, CTL_SET_LOC | CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

#endif  /* LATHECPP_AXIS_CTL */

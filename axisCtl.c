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
//#include "remCmd.h"
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

typedef struct S_AXIS_CTL
{
 enum RISCV_AXIS_STATE_TYPE state; /* current wait state */
 int cmd;			/* current command flags */
 int dist;			/* distance to move */
 int dir;			/* current direction */
 int ctlFlag;			/* axisCtl flags */

 int curDist;			/* backlash distance */
 int curLoc;			/* current location */
 int expLoc;			/* expected location */
 int dro;			/* dro reading */

 int ignore;			/* ignore after first error */

 int backlashSteps;		/* backlash steps */
 
 char name;			/* axis name */
 int axisID;			/* axis identifier */
 struct S_AXIS_CTL *other;	/* pointer to other axis */
 int base;			/* command base for fpga */
 int accelOffset;		/* offset into accel table */
 int clkShift;			/* clk register shift */
 int statDone;			/* axis done flag */
 int statEna;			/* axis enable flag */
} T_AXIS_CTL, *P_AXIS_CTL;

EXT T_AXIS_CTL zAxis;
EXT T_AXIS_CTL xAxis;

typedef struct S_INDEX_DATA
{
 uint32_t clocks;
 uint32_t lastClocks;
 uint32_t lastTime;
 uint32_t rpm;
} T_INDEX_DATA;

EXT T_INDEX_DATA indexData;

#define FPGA_FREQUENCY 50000000
#define INDEX_INTERVAL FPGA_FREQUENCY

void initAccelTable(void);
void initAxisCtl(void);
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

#endif	/* AXIS_CTL_INCLUDE */ // ->
#if defined(LATHECPP_AXIS_CTL)

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

void initAxisCtl(void)
{
 printf("InitAxisCtl\n");
 memset((void *) &zAxis, 0, sizeof(zAxis));
 memset((void *) &xAxis, 0, sizeof(xAxis));

 zAxis.name        = 'z';
 zAxis.axisID      = RA_Z_AXIS;
 zAxis.other       = &xAxis;
 zAxis.base        = F_ZAxis_Base;
 zAxis.accelOffset = RP_Z_BASE;
 zAxis.clkShift    = Z_FREQ_SHIFT;
 zAxis.statDone    = Z_AXIS_DONE;
 zAxis.statEna     = Z_AXIS_ENA;
 printf("z %d %d\n", zAxis.axisID, zAxis.clkShift);

 zAxis.name        = 'x';
 xAxis.axisID      = RA_X_AXIS;
 xAxis.other       = &zAxis;
 xAxis.base        = F_XAxis_Base;
 xAxis.accelOffset = RP_X_BASE;
 xAxis.clkShift    = X_FREQ_SHIFT;
 xAxis.statDone    = Z_AXIS_DONE;
 xAxis.statEna     = Z_AXIS_ENA;
 printf("x %d %d\n", xAxis.axisID, xAxis.clkShift);
}

uint32_t delta;
uint32_t lastMvStatus;

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
 uint32_t t = NEORV32_MTIME->TIME_LO;
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
 axisCheck(&zAxis, status);
 axisCheck(&xAxis, status);
}

void axisCheck(P_AXIS_CTL axis, int status)
{
 int base = axis->base;

 axis->dro = (int) rd(base + F_Sync_Base + F_Rd_Dro);

 if (axis->state != RS_IDLE)
 {
//  int value = (int) rd(base + F_Rd_Axis_Ctl);
  axis->curLoc = (int) rd(base + F_Sync_Base + F_Rd_Loc);
  
//  int dist = (int) rd(base + F_Sync_Base + F_Rd_Dist);

  if (status & axis->statDone)
  {
//   int axisStatus = (int) rd(base + F_Rd_Axis_Status);

   ld(base + F_Ld_Axis_Ctl , CTL_INIT);
   ld(base + F_Ld_Axis_Ctl, 0);
   if (axis->state == RS_WAIT_BACKLASH)
   {
    axisMove(axis);
    axis->state = RS_WAIT;
   }
   else if (axis->state == RS_WAIT)
   {
    axis->ctlFlag = 0;
    axis->state = RS_IDLE;
   }
  }

  if ((status & Z_AXIS_ENA) == 0)
  {
   if (axis->ignore == 0)
   {
    axis->ignore = 1;
    printf("waiting no enable\n");
   }
  }
 }
 else
  axis->ignore = 0;
}

void axisLoad(P_AXIS_CTL a, int index)
{
 dbgPutC(a->name);
 dbgPutStr(" axisLoad ");
 dbgPutHex(index, 1);
 dbgNewLine();
 
 int base = a->base;
 P_ACCEL_DATA aData = accelData[index];
 if (aData->freqDiv != 0)
  ld(base + F_Ld_Freq, aData->freqDiv);

 int bSyn = a->base + F_Sync_Base;
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
 dbgPutC(a->name);
 dbgPutStr(" axisMove\n");

 int accelIndex = -1;
 int clkSel = CLK_FREQ;
 switch (a->cmd & CMD_MSK)
 {
 case CMD_SYN:
  accelIndex = A_TURN;
  clkSel = CLK_CH;
  if (a->cmd & SYN_START)
   a->ctlFlag = CTL_WAIT_SYNC;
  if (a->cmd & SYN_TAPER)
  {
  }
  break;

 case CMD_JOG:accelIndex = A_JOG;
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
  axisLoad(a, accelIndex);
  ld(a->base + F_Ld_Axis_Ctl, a->ctlFlag | CTL_START);
  ld(F_Ld_Clk_Ctl, clkSel << a->clkShift);
 }
}

void moveZ(int cmd, int loc)
{
 int pos = (int) rd(F_ZAxis_Base + F_Sync_Base + F_Rd_Loc);
 int dist = loc - pos;
 moveRel(&zAxis, cmd, dist);
}

void moveX(int cmd, int loc)
{
 int pos = (int) rd(F_XAxis_Base + F_Sync_Base + F_Rd_Loc);
 int dist = loc - pos;
 moveRel(&xAxis, cmd, dist);
}

void moveRel(P_AXIS_CTL axis, int cmd, int dist)
{
 if (axis->state == RS_IDLE)
 {
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
    axisLoad(axis, axis->accelOffset + A_JOG);
    axis->ctlFlag |= CTL_BACKLASH;	/* set direction positive */

    ld(axis->base + F_Ld_Axis_Ctl, axis->ctlFlag | CTL_START);

    axis->ctlFlag &= ~CTL_BACKLASH;	/* set direction positive */
    axis->state = RS_WAIT_BACKLASH;	/* set to wait for backlash */
   }
   else					/* if no backlash */
   {
    axisMove(axis);
    axis->state = RS_WAIT;		/* set to start move */
   }
  }
 }
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
 int base = axis->base;
 
 ld(base + F_Sync_Base + F_Ld_Loc, loc);
 ld(base + F_Ld_Axis_Ctl, CTL_SET_LOC | CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

#endif  /* LATHECPP_AXIS_CTL */

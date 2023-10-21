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
#include "remCmd.h"
#include "remSerial.h"

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

void initAccelTable(void);

typedef union U_INT_BYTE
{
 int iVal;
 unsigned char bVal[4];
} T_INT_BYTE;

typedef struct S_AXIS_CTL
{
 enum RISCV_AXIS_STATE_TYPE state; /* current wait state */
 int cmd;			/* current command flags */
 int curDist;			/* backlash distance */
 int dist;			/* distance to move */
 int curLoc;			/* current location */
 int expLoc;			/* expected location */
 int dir;			/* current direction */
 int dro;			/* dro reading */
 int ctlFlags;			/* axisCtl flags */
 int ignore;

 int backlashSteps;		/* backlash steps */
 
 int axisID;			/* axis identifier */
 struct S_AXIS_CTL *other;	/* pointer to other axis */
 int base;			/* command base for fpga */
 int accelOffset;		/* offset into accel table */
 int freqShift;			/* frequency shift */
} T_AXIS_CTL, *P_AXIS_CTL;

EXT T_AXIS_CTL zAxis;
EXT T_AXIS_CTL xAxis;

typedef struct S_INDEX_DATA
{
 uint32_t clocks;
 uint32_t lastClocks;
 uint32_t lastTime;
 int rpm;
} T_INDEX_DATA;

EXT T_INDEX_DATA indexData;

#define FPGA_FREQUENCY 50000000
#define INDEX_INTERVAL FPGA_FREQUENCY

void initAxisCtl(void);
void axisCtl(void);
void axisLoad(P_AXIS_CTL a, P_ACCEL_DATA accel);
void axisMove(P_AXIS_CTL a);
void moveZ(int cmd, int loc);
void moveX(int cmd, int loc);
void moveRelZ(int cmd, int dist);
void moveRelX(int cmd, int dist);
void setLoc(P_AXIS_CTL axis, int loc);

#endif	/* AXIS_CTL_INCLUDE */ // ->
#ifdef LATHECPP_AXIS_CTL

void initAxisCtl(void)
{
 printf("InitAxisCtl\n");
 memset((void *) &zAxis, 0, sizeof(zAxis));
 memset((void *) &xAxis, 0, sizeof(xAxis));

 zAxis.axisID = RA_Z_AXIS;
 zAxis.other = &xAxis;
 zAxis.base  = F_ZAxis_Base;
 zAxis.accelOffset = RP_Z_BASE;
 zAxis.freqShift = Z_FREQ_SHIFT;
 printf("z %d %d\n", zAxis.axisID, zAxis.freqShift);

 xAxis.axisID = RA_X_AXIS;
 xAxis.other = &zAxis;
 xAxis.base  = F_XAxis_Base;
 xAxis.accelOffset = RP_X_BASE;
 xAxis.freqShift = X_FREQ_SHIFT;
 printf("x %d %d\n", xAxis.axisID, xAxis.freqShift);
}

uint32_t delta;

void axisCtl(void)
{
 uint32_t t = NEORV32_MTIME->TIME_LO;
 delta = t - indexData.lastTime;
 if (delta > INDEX_INTERVAL)
 {
  dbgPutC('.');
  // dbgPutHex(delta, 4);
  indexData.lastTime = t;
  indexData.clocks = (int) rd(F_Rd_Idx_Clks);
  if (indexData.clocks != 0)
   indexData.rpm = FPGA_FREQUENCY / ((indexData.clocks + 1) * 60);
  else
   indexData.rpm = 0;
 }

 int status = (int) rd(F_Rd_Status);

 int base = F_ZAxis_Base;
 zAxis.dro = (int) rd(base + F_Sync_Base + F_Rd_Dro);

 if (zAxis.state != RS_IDLE)
 {
//  int value = (int) rd(base + F_Rd_Axis_Ctl);
  zAxis.curLoc = (int) rd(base + F_Sync_Base + F_Rd_Loc);
  
//  int dist = (int) rd(base + F_Sync_Base + F_Rd_Dist);

  if (status & Z_AXIS_DONE)
  {
//   int axisStatus = (int) rd(base + F_Rd_Axis_Status);

   ld(base + F_Ld_Axis_Ctl , CTL_INIT);
   ld(base + F_Ld_Axis_Ctl, 0);
   if (zAxis.state == RS_WAIT_BACKLASH)
   {
    axisMove(&zAxis);
    zAxis.state = RS_WAIT;
   }
   else if (zAxis.state == RS_WAIT)
   {
    zAxis.ctlFlags = 0;
    zAxis.state = RS_IDLE;
   }
  }

  if ((status & Z_AXIS_ENA) == 0)
  {
   if (zAxis.ignore == 0)
   {
    zAxis.ignore = 1;
    printf("z waiting no enable\n");
   }
  }
 }
 else
  zAxis.ignore = 0;
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

void axisLoad(P_AXIS_CTL a, P_ACCEL_DATA aData)
{
 int base = a->base;
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
 int index = -1;
 int clkSel = CLK_FREQ;
 switch (a->cmd & CMD_MSK)
 {
 case CMD_SYN:
  index = A_TURN;
  clkSel = CLK_CH;
  break;

 case CMD_JOG:
  index = A_JOG;
  break;

 case CMD_MOV:
 case CMD_MAX:
  index = A_MOVE;
  break;

  // case CMD_SPEED:
  //  break;

 case JOGSLOW:
  index = A_SLOW;
  break;

 default:
  break;
 }

 if (index >= 0)
 {
  axisLoad(a, accelData[a->accelOffset + index]);
  ld(a->base + F_Ld_Axis_Ctl, a->ctlFlags | CTL_START);
  int tmp = clkSel << a->freqShift;
  ld(F_Ld_Freq, tmp);
 }
}

void moveZ(int cmd, int loc)
{
 int pos = (int) rd(F_ZAxis_Base + F_Sync_Base + F_Rd_Loc);
 int dist = loc - pos;
 moveRelZ(cmd, dist);
}

void moveX(int cmd, int loc)
{
}

void moveRelZ(int cmd, int dist)
{
 if (zAxis.state == RS_IDLE)
 {
  zAxis.cmd = cmd;
  if (dist != 0)			/* if distance non zero */
  {
   int dirChange;
   if (dist > 0)			/* if moving positive */
   {
    zAxis.dist = dist;			/* save distance */
    dirChange = (zAxis.dir != R_DIR_POS); /* dir chg */
    zAxis.dir = R_DIR_POS;		/* move positive direction */
    zAxis.ctlFlags |= CTL_DIR;		/* set direction positive */
   }
   else
   {
    zAxis.dist = -dist;			/* make positive */
    dirChange = (zAxis.dir != R_DIR_NEG); /* dir chg */
    zAxis.dir = R_DIR_NEG;		/* set move direction to negative */
   }

   if (dirChange &&			/* if direction change */
       (zAxis.backlashSteps != 0))	/* and backlash present */
   {
    zAxis.curDist = zAxis.backlashSteps;
    axisLoad(&zAxis, &zJog);
    zAxis.ctlFlags |= CTL_BACKLASH;	/* set direction positive */

    ld(zAxis.base + F_Ld_Axis_Ctl, zAxis.ctlFlags | CTL_START);

    zAxis.ctlFlags &= ~CTL_BACKLASH;	/* set direction positive */
    zAxis.state = RS_WAIT_BACKLASH;	/* set to wait for backlash */
   }
   else					/* if no backlash */
   {
    axisMove(&zAxis);
    zAxis.state = RS_WAIT;		/* set to start move */
   }
  }
 }
}

void moveRelX(int cmd, int dist)
{
}

void setLoc(P_AXIS_CTL axis, int loc)
{
 int base = axis->base;
 
 ld(base + F_Sync_Base + F_Ld_Loc, loc);
 ld(base + F_Ld_Axis_Ctl, CTL_SET_LOC | CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

#endif  /* LATHECPP_AXIS_CTL */

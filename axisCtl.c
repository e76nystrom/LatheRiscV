#define LATHECPP_AXIS_CTL

#include <string.h>

#include <neorv32.h>
#include "ctlstates.h"
#include "ctlbits.h"
#include "fpgaLatheReg.h"
#include "fpgaLatheBits.h"

#define EXT extern
#include "Lathe.h"
#include "axisCtl.h"
#include "remCmd.h"
#include "fpga.h"
#include "dbgSerial.h"
//#include "remSerial.h"

#if defined(AXIS_CTL_INCLUDE)	// <-
//#include <stdbool.h>
#include <riscvStruct.h>
#include <ctlstates.h>

#define INPUT_TEST

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
 int base;			/* axis command base for fpga */
 int accelOffset;		/* offset into accel table */
 int clkShift;			/* clk register shift */
 int statDone;			/* axis done flag */
 int statEna;			/* axis enable flag */
 int waitState;			/* axis wait state */
 int dbgBase;			/* base axis dbg types */
 int jogFlag;			/* jog enable flag */
 uint32_t *mpgData;		/* pointer to jog fifo */
 int homedBit;
 int homeActiveBit;
#if defined(INPUT_TEST)
 int inMask;
 int inMinus;
 int inPlus;
 int inHome;
#endif
} T_AXIS_CONSTANT;

typedef struct S_AXIS_CTL
{
 enum RISCV_AXIS_STATE state;	/* current wait state */
 enum RISCV_AXIS_STATE lastState; /* last state */
 enum MPG_STATE mpgState;	/* mpg state */
 enum MPG_STATE lastMpgState;	/* last mpg state */
 enum H_STATES homeState;
 enum H_STATES lastHomeState;
 int mpgInvert;			/* invert direction of mpg */
 int cmd;			/* current command flags */
 int dist;			/* distance to move */
 int dir;			/* current direction */
 int ctlFlag;			/* axisCtl flags */

 int curDist;			/* backlash distance */
 int curLoc;			/* current location */
 int expLoc;			/* expected location */
 int dro;			/* dro reading */

 int endLoc;
 int endDist;
 int endDro;

 int ignore;			/* ignore after first error */

 uint32_t dirWaitStart;		/* dir change timer start */
 int backlashSteps;		/* backlash steps */
 int mpgAxisCtl;		/* axis dtl mpg mode */
 int inputStatus;		/* input signal status */
 uint32_t lastLoc;
 T_AXIS_VAR v;
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

char *fmtLoc(char *buf, P_AXIS_CTL axis, int loc);
char *fmtDist(char *buf, P_AXIS_CTL axis, int dist);
void dbgPrtLoc(const char *str, const P_AXIS_CTL axis, int loc);
void dbgPrtDist(const char *str, const P_AXIS_CTL axis, int dist);

void dbgAxisInit(P_AXIS_CTL axis);

void dbgStatus(int status);
void dbgAxisCtl(char name, int val);
void dbgMvStatus(int val);
void dbgJogPause(int val);
void dbgAxisStatus(const char *str, const P_AXIS_CTL axis);
void dbgInPin(const char *str, const int val);
void dbgAxisState(const P_AXIS_CTL axis);
void dbgMoveCmd(const int cmd);
void dbgSelPrt(int index, const char *label, const char *p);

void initAccelTable(void);

void initAxisCtl(void);

void axisCtl(void);
void axisStateCheck(P_AXIS_CTL axis);
void axisCheck(P_AXIS_CTL axis, int status);
void homeCtl(P_AXIS_CTL axis);
void move(P_AXIS_CTL axis, int cmd, int loc);
void jogMove(P_AXIS_CTL axis, int dist);
void jogMpg(P_AXIS_CTL axis);
void moveRel(P_AXIS_CTL axis, int dist, int cmd);
void moveBacklash(P_AXIS_CTL axis);
void setLoc(P_AXIS_CTL axis, int loc);
void axisStop(P_AXIS_CTL axis);
void encScalerSetup(void);
void axisMove(P_AXIS_CTL axis);
void axisHome(P_AXIS_CTL axis, int homeCmd);
void clockLoad(P_AXIS_CTL axis, int clkSel);
void axisLoad(P_AXIS_CTL axis, int index);

#endif	/* AXIS_CTL_INCLUDE */ // ->
#if defined(LATHECPP_AXIS_CTL)

uint32_t delta;
uint32_t lastMvStatus;
uint32_t lastStatus;

char *fmtLoc(char *buf, const P_AXIS_CTL axis, int loc)
{
 loc -= axis->v.homeOffset;
 loc *= 10000;
 loc /= axis->v.stepsInch;
 return dbgFmtNum(buf, loc);
}

char *fmtDist(char *buf, const P_AXIS_CTL axis, int dist)
{
 dist *= 10000;
 dist /= axis->v.stepsInch;
 return dbgFmtNum(buf, dist);
}

void dbgPrtLoc(const char *str, const P_AXIS_CTL axis, int loc)
{
 dbgPutStr(str);
 dbgPutInt(loc);
 char buf[16];
 dbgPutStr(fmtLoc(buf, axis, loc));
}

void dbgPrtDist(const char *str, const P_AXIS_CTL axis, int dist)
{
 dbgPutStr(str);
 dbgPutInt(dist);
 char buf[16];
 dbgPutStr(fmtDist(buf, axis, dist));
}

void dbgAxisInit(P_AXIS_CTL axis)
{
 dbgPutC(axis->c.name);
 dbgPutHexByte(axis->c.axisID);
 dbgPutHexByte(axis->c.clkShift);
 dbgPutHexByte(axis->c.dbgBase);
 dbgNewLine();
}

#include "statusRegStr.h"

void dbgStatus(const int status)
{
 dbgPutStr("status");
 dbgPutHex(status, 2);
 // ReSharper disable once CppRedundantParentheses
 int mask = 1 << (STATUS_SIZE-1);
 for (int i = STATUS_SIZE-1; i >= 0; i--)
 {
  if (status & mask)
  {
   const char *p = &statusRegStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask >>= 1;
 }
 dbgNewLine();
}

#include "axisCtlRegStr.h"

void dbgAxisCtl(const char name, const int val)
{
 dbgPutC(name);
 dbgPutStr(" axis ctl reg");
 dbgPutHex(val, 2);
 int mask = 1 << (AXIS_CTL_REG_SIZE-1);
 for (int i = AXIS_CTL_REG_SIZE-1; i > 0; i--)
 {
  if (val & mask)
  {
   const char *p = &axisCtlRegStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask >>= 1;
 }
 dbgNewLine();
}

#include "mvStatusBitsStr.h"

void dbgMvStatus(const int val)
{
 dbgPutStr("mvStatus");
 dbgPutHex(rVar.rMvStatus, 2);
 int mask = 1;
 for (int i = 0; i < R_MV_MAX; i++)
 {
  if (rVar.rMvStatus & mask)
  {
   const char *p = &mvStatusBitsStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask <<= 1;
 }
 dbgNewLine();
}

#include "pauseBitsStr.h"

void dbgJogPause(const int val)
{
 dbgPutStr("jogPause");
 dbgPutHexByte(rVar.rJogPause);
 int mask = 1;
 for (int i = 0; i < R_PAUSE_MAX; i++)
 {
  if (val & mask)
  {
   const char *p = &pauseBitsStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask <<= 1;
 }
 dbgNewLine();
}

#include "axisStatusRegStr.h"

void dbgAxisStatus(const char *str, const P_AXIS_CTL axis)
{
 dbgPutC(axis->c.name);
 dbgPutSpace();
 dbgPutStr(str);
 const int axisStatus = rd(axis->c.base + F_Rd_Axis_Status);
 dbgPutHexByte(axisStatus);
 int mask = 1;
 for (int i = 0; i < AXIS_STATUS_REG_SIZE; i++)
 {
  if (axisStatus & mask)
  {
   const char *p = &axisStatusRegStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask <<= 1;
 }
 dbgNewLine();
}

#include "inputsRegStr.h"

void dbgInPin(const char *str, const int val)
{
 dbgPutStr(str);
 dbgPutStr(" input pins");
 dbgPutHex(CFS->inPin, 2);
 dbgPutHex(val, 2);
 int mask = 1;
 for (int i = 0; i < INPUTS_REG_SIZE; i++)
 {
  if (val & mask)
  {
   const char *p = &inputsRegStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);
  }
  mask <<= 1;
 }
 dbgNewLine();
}

#include "riscvAxisStateStr.h"

void dbgAxisState(const P_AXIS_CTL axis)
{
 dbgPutC(axis->c.name);
 dbgPutStr(" axis state");
 const int state = axis->state;
 dbgPutHexByte(state);
 dbgPutSpace();
 const char *p = &riscvAxisStateStr[state].c0;
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgNewLine(); 
}

#include "moveCmdStr.h"
#include "moveBitStr.h"

void dbgMoveCmd(const int cmd)
{
 dbgPutHex(cmd, 2);

 const char *p = &moveCmdStr[cmd & CMD_MSK].c0;
 dbgPutStr(" ");
 dbgPutC(*p++);
 dbgPutC(*p);   

 int mask = 1 << (M_BIT_MAX-1);
 for (int i = M_BIT_MAX-1; i > M_RSV_2; i--)
 {
  if (mask & cmd)
  {
   p = (char *) &moveBitStr[i].c0;
   dbgPutSpace();
   dbgPutC(*p++);
   dbgPutC(*p);   
  }
  mask >>= 1;
 }
}

void dbgSelPrt(int index, const char *label, const char *p)
{
 dbgPutStr(label);
 dbgPutHexByte(index);
 dbgPutC(' ');
 dbgPutC(*p++);
 dbgPutC(*p);
 dbgNewLine();
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

T_AXIS_CONSTANT zInitData =
{
 .name          = 'z',
 .axisID        = RA_Z_AXIS,
 .other         = &xAxis,
 .base          = F_ZAxis_Base,
 .accelOffset   = RP_Z_BASE,
 .clkShift      = Z_FREQ_SHIFT,
 .statDone      = Z_AXIS_DONE,
 .statEna       = Z_AXIS_ENA,
 .waitState     = RW_WAIT_Z,
 .dbgBase       = D_ZBASE,
 .jogFlag       = PAUSE_ENA_Z_JOG,
 .mpgData       = (uint32_t *) &CFS->zMpg,
 .homedBit      = MV_Z_HOME,
 .homeActiveBit = MV_Z_HOME_ACTIVE,
#if defined(INPUT_TEST)
 .inMask        = ~(IN_PROBE | IN_ZHOME | IN_ZPLUS | IN_ZMINUS),
 .inMinus       = IN_ZMINUS,
 .inPlus        = IN_ZPLUS,
 .inHome        = IN_ZHOME,
#endif
};

T_AXIS_CONSTANT xInitData =
{
 .name          = 'x',
 .axisID        = RA_X_AXIS,
 .other         = &zAxis,
 .base          = F_XAxis_Base,
 .accelOffset   = RP_X_BASE,
 .clkShift      = X_FREQ_SHIFT,
 .statDone      = X_AXIS_DONE,
 .statEna       = X_AXIS_ENA,
 .waitState     = RW_WAIT_X,
 .dbgBase       = D_XBASE,
 .jogFlag       = PAUSE_ENA_X_JOG,
 .mpgData       = (uint32_t *) &CFS->xMpg,
 .homedBit      = MV_X_HOME,
 .homeActiveBit = MV_X_HOME_ACTIVE,
#if defined(INPUT_TEST)
 .inMask        = ~(IN_PROBE | IN_XHOME | IN_XPLUS | IN_XMINUS),
 .inMinus       = IN_XMINUS,
 .inPlus        = IN_XPLUS,
 .inHome        = IN_XHOME,
#endif
};

T_AXIS_CTL zAxis;
T_AXIS_CTL xAxis;

void initAxisCtl(void)
{
 if (dbg3) {
  dbgPutStr("initAxisCtl\n"); }
 memset(&zAxis, 0, sizeof(zAxis));
 memset(&xAxis, 0, sizeof(xAxis));

 memcpy(&zAxis.c, &zInitData, sizeof(T_AXIS_CONSTANT));
 if (dbg3) {
  dbgAxisInit(&zAxis); }

 memcpy(&xAxis.c, &xInitData, sizeof(T_AXIS_CONSTANT));
 if (dbg3) {
  dbgAxisInit(&xAxis); }

 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_ZAxis_Base + F_Ld_Axis_Ctl, 0);
 const uint32_t zCtl = rd(F_ZAxis_Base + F_Rd_Axis_Ctl);

 ld(F_XAxis_Base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(F_XAxis_Base + F_Ld_Axis_Ctl, 0);
 const uint32_t xCtl = rd(F_XAxis_Base + F_Rd_Axis_Ctl);

 if (dbg3) {
  dbgPutStr("zCtl");
  dbgPutHex(zCtl, 2);
  dbgPutStr(" xCtl");
  dbgPutHex(xCtl, 2);
  dbgNewLine(); 

  dbgAxisStatus("init", &zAxis);
  dbgAxisStatus("init", &xAxis); }
}

void axisCtl(void)
{
 const uint32_t t = millis();
 delta = t - indexData.lastTime;
 if (delta > INDEX_INTERVAL)
 {
  // dbgPutC('.');
  // dbgPutHex(delta, 4);
  indexData.lastTime = t;
  uint32_t clocks = rd(F_Index_Base + F_Rd_Index_Clks);
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
  lastMvStatus = rVar.rMvStatus;
  if (dbg3) {
   dbgMvStatus(rVar.rMvStatus); }
 }

 if (rVar.rJogPause != lastJogPause)
 {
  lastJogPause = rVar.rJogPause;
  if (dbg3) {
   dbgJogPause(rVar.rJogPause); }
 }

 const int status = rd(F_Rd_Status);
 if (status != lastStatus)
 {
  lastStatus = status;
  if (dbg3)
   dbgStatus(status);
 }
 
 axisCheck(&zAxis, status);
 axisCheck(&xAxis, status);
}

void axisStateCheck(const P_AXIS_CTL axis)
{
 if (axis->state != axis->lastState)
 {
  axis->lastState = axis->state;
  if (dbg3) {
   dbgAxisState(axis); }
 }
}

int inPin;

#define WAIT_TAPER_RUNOUT ((1 << RS_WAIT_TAPER) | (1 << RS_WAIT_RUNOUT))
#define WAIT_MASK ((1 << RS_WAIT) | (1 << RS_WAIT_TAPER) | \
		   (1 << RS_WAIT_RUNOUT))

void axisCheck(const P_AXIS_CTL axis, const int status)
{
 const int base = axis->c.base;

 axis->dro = (int) rd(base + F_Sync_Base + F_Rd_Dro);
 const int curLoc = (int) rd(base + F_Sync_Base + F_Rd_Loc);
 if (dbg3L) {
  if (curLoc != axis->curLoc)
  {
   const uint32_t t = millis();
   if ((t - axis->lastLoc) >= 100)
   {
    axis->lastLoc = t;
    dbgPutC(axis->c.name);
    dbgPrtLoc(" loc", axis, curLoc);
    dbgNewLine();
   }
  } }
 axis->curLoc = curLoc;

#if defined(INPUT_TEST)

 const int cur = axis->curLoc;
 int inPinTmp = inPin & axis->c.inMask;

 if (cur <= axis->v.testLimMin)
  inPinTmp |= axis->c.inMinus;
 if (cur >=  axis->v.testLimMax)
  inPinTmp |= axis->c.inPlus;

 if ((axis->cmd & (FIND_HOME | CLEAR_HOME)) != 0 &&
     cur >= axis->v.testHomeMin &&
     cur <= axis->v.testHomeMax)
  inPinTmp |= axis->c.inHome;

 if ((axis->cmd & FIND_PROBE) != 0  &&
     cur <= axis->v.testProbe)
  inPinTmp |= CTL_PROBE;

 if (inPinTmp != inPin)
 {
  inPin = inPinTmp;
  CFS->inPin = inPinTmp;

  if (dbg3) {
   dbgPutC(axis->c.name);
   dbgPrtLoc(" loc", axis, cur);
   dbgNewLine();
   dbgInPin("set", inPin);
   dbgInPin("rd ", rd(F_Rd_Inputs));  }
 }
 
#endif

 if (axis->state != RS_IDLE)
 {
  // int value = (int) rd(base + F_Rd_Axis_Ctl);
  const int state = axis->state;
  const int stateMask = (1 << state);
  const int doneStat = (((stateMask & WAIT_TAPER_RUNOUT) != 0) ? \
			axis->c.other->c.statDone : axis->c.statDone);

  if (status & doneStat)
  {
   if (dbg3) {
    dbgAxisStatus("axis done", axis);
    dbgPrtLoc("loc", axis, curLoc);
    dbgNewLine(); }

   if (state == RS_WAIT_BACKLASH)
   {
    axis->ctlFlag &= ~CTL_BACKLASH;
    axisMove(axis);
    axis->state = RS_WAIT;
    axisStateCheck(axis);
   }
   else if ((stateMask & WAIT_MASK) != 0)
   {
    if (rVar.rRunoutDist != 0)
     ld(F_RunOut_Base + F_Ld_RunOut_Ctl, 0);
    
    // axis->endDist = (int) rd(base + F_Sync_Base + F_Rd_Dist);
    axis->endLoc =  (int) rd(base + F_Sync_Base + F_Rd_Loc);
    axis->endDro =  (int) rd(base + F_Sync_Base + F_Rd_Dro);
    // if (dbg3) {
    //  dbgPrtLoc("encLoc", axis, axis->endLoc);
    //  dbgNewLine(); }
    
    if ((axis->ctlFlag & CTL_CH_DIRECT) != 0)
     ld(F_Ld_Sync_Ctl, 0);

    clockLoad(axis, CLK_NONE);
    ld(base + F_Ld_Axis_Ctl, 0);

    if (dbgM) {
     dbgMsg(axis->c.dbgBase + D_EXP, axis->expLoc);
     dbgMsg(axis->c.dbgBase + D_LOC, axis->endLoc);
     dbgMsg(axis->c.dbgBase + D_DRO, axis->endDro);
    }

    if (dbg3) {
     if (axis->c.name == 'z')
     {
      const int turnCount = rd(F_Index_Base + F_Rd_Turn_Count);
      dbgPutStr("turn count");
      dbgPutInt(turnCount);
      dbgNewLine();
     }

     dbgPrtDist("dist", axis, axis->dist);
     dbgPrtLoc(" loc", axis, axis->endLoc);
     dbgPrtLoc(" dro", axis, axis->endDro);
     dbgPrtLoc(" exp", axis, axis->expLoc);
     dbgNewLine(); }

    const int deltaLoc = axis->endLoc - axis->expLoc;
    if (deltaLoc != 0)
    {
     dbgPrtDist("delta", axis, deltaLoc);
     dbgNewLine();
    }
    
    const int xPos = rd(base + F_Sync_Base + F_Rd_XPos);
    if (xPos != 0)
    {
     const int yPos = rd(base + F_Sync_Base + F_Rd_YPos);

     if (dbgM) {
     dbgMsg(axis->c.dbgBase + D_X, xPos);
     dbgMsg(axis->c.dbgBase + D_Y, yPos); }

     if (dbg3) {
      dbgPutStr("xPos");
      dbgPutInt(xPos);
      dbgPutStr(" yPos");
      dbgPutInt(yPos);
      dbgNewLine(); }
    }
    
    axis->ctlFlag = 0;
    axis->state = RS_IDLE;
    if (runCtl.wait == axis->c.waitState)
     runCtl.wait = RW_NONE;

    axisStateCheck(axis);
    if (dbg3) {
     dbgAxisStatus("axis idle", axis);
     dbgNewLine(); }
   }
  }
 }
 else
 {
  jogMpg(axis);
  homeCtl(axis);
 }
 
 if (axis->state != RS_IDLE &&
     (status & axis->c.statEna) == 0)
 {
  if ((axis->ignore == 0) &&
      ((axis->cmd & (SYN_START | ENA_RUNOUT)) == 0))
  {
   axis->ignore = 1;
   dbgPutC(axis->c.name);
   dbgPutStr(" waiting no enable\n");
  }
 }
 else
  axis->ignore = 0;
}

#include "hStatesStr.h"

void homeCtl(const P_AXIS_CTL axis)
{
 const int inputStatus = rd(F_Rd_Inputs);

 if (axis->homeState != axis->lastHomeState)
 {
  axis->lastHomeState = axis->homeState;
  dbgMsg(D_HST, axis->homeState);

  dbgPutC(axis->c.name);
  dbgPutStr(" axis home state");
  dbgPutHexByte(axis->homeState);

  const char *p = &hStatesStr[axis->homeState].c0;
  dbgPutSpace();
  dbgPutC(*p++);
  dbgPutC(*p);

  dbgPutStr(" status");
  dbgPutHexByte(axis->v.homeStatus);

  dbgPutStr(" inputs");
  const int tmp = axis->inputStatus;
  dbgPutHex(tmp, 2);
  int mask = 1;
  for (int i = 0; i < INPUTS_REG_SIZE; i++)
  {
   if (inputStatus & mask)
   {
    p = &inputsRegStr[i].c0;
    dbgPutSpace();
    dbgPutC(*p++);
    dbgPutC(*p);
   }
   mask <<= 1;
  }
  dbgNewLine();
 }

 if (axis->homeState != H_IDLE)
 {
  switch (axis->homeState)
  {
  case H_IDLE:			/* 0x00 idle */
   break;

  case H_HOME:			/* 0x01 found home switch */
   if ((inputStatus & axis->c.inHome) != 0) /* if home switch set */
   {
    moveRel(axis, -axis->v.homeFindFwd, CMD_JOG | CLEAR_HOME); /* move off switch */
    axis->homeState = H_OFF_HOME; /* off home state */
   }
   else
   {				/* if did not find switch */
    axis->v.homeStatus = HOME_FAIL; /* set failure status */
    axis->homeState = H_IDLE;	/* return to idle state */
   }
   break;

  case H_OFF_HOME:		/* 0x02 move off home switch */
   if ((inputStatus & axis->c.inHome) == 0) /* if home switch open */
   {
    moveRel(axis, axis->v.homeBackoff, CMD_JOG); /* move to backoff dist */
    axis->homeState = H_BACKOFF;	/* backoff state */
   }
   else
   {
    axis->v.homeStatus = HOME_FAIL; /* set failure status */
    axis->homeState = H_IDLE;	/* return to idle state */
   }
   break;

  case H_BACKOFF:		/* 0x03 wait for backoff complete */
   if ((inputStatus & axis->c.inHome) == 0) /* if clear of switch */
   {
    moveRel(axis, axis->v.homeSlow, JOG_SLOW | FIND_HOME); /* move back slowly */
    axis->homeState = H_SLOW;	/* advance to wait */
   }
   else
   {				/* if did not find switch */
    axis->v.homeStatus = HOME_FAIL; /* set failure status */
    axis->homeState = H_IDLE;	/* return to idle state */
   }
   break;

  case H_SLOW:			/* 0x04 wait to find switch */
  {
   int tmp = rVar.rMvStatus;
   tmp &= ~axis->c.homeActiveBit; /* home complete */
   if ((inputStatus & axis->c.inHome) != 0) /* if successful */
   {
    axis->v.homeStatus = HOME_SUCCESS; /* set flag */

    tmp |= axis->c.homedBit;	/* indicate homed */
    axis->curLoc = 0;		/* set position to zero */
    axis->dro = 0;		/* set dro position to zero */
   }
   else				/* if failure */
   {
    axis->v.homeStatus = HOME_FAIL; /* set failed flag */
   }
   rVar.rMvStatus = tmp;
   axis->homeState = H_IDLE;	/* back to idle state */
  }
  break;

  default:
   axis->homeState = H_IDLE;	/* invalid return to idle */
   break;
  } /* switch */

  // ReSharper disable once CppDFAConstantConditions
  // if (axis->homeState == H_IDLE)
   // ReSharper disable once CppDFAUnreachableCode
  //  rVar.rJogPause &= ~DISABLE_JOG;

 }  /* if idle */
}

void move(const P_AXIS_CTL axis, const int cmd, const int loc)
{
 axis->expLoc = loc;
 const int pos = rd(axis->c.base + F_Sync_Base + F_Rd_Loc);

 if (dbg3) {
  dbgNewLine();
  dbgPutC(axis->c.name);
  dbgPutStr(" move");
  dbgMoveCmd(cmd);
  dbgPrtLoc(" pos", axis, pos);
  dbgPrtLoc(" loc", axis, loc);
  dbgNewLine(); }

 if (dbgM) {
  dbgMsg(axis->c.dbgBase + D_MOV, loc);
  dbgMsg(axis->c.dbgBase + D_CUR, pos); }

 const int dist = loc - pos;
 moveRel(axis, dist, cmd);
}

void jogMove(const P_AXIS_CTL axis, const int dist)
{
 if (dbg3) {
  dbgPutC(axis->c.name);
  dbgPutStr(" jog move\n"); }
 
 if (axis->state == RS_IDLE)
 {
  rVar.rJogPause = DISABLE_JOG;
  axis->mpgState = MPG_DISABLED;
  moveRel(axis, dist, DIST_MODE | CMD_JOG);
 }
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
    if (dbg3) {
     dbgAxisCtl(axis->c.name, axis->mpgAxisCtl); }
    clockLoad(axis, CLK_FREQ);

    axis->mpgState = MPG_CHECK_QUE;
    if (dbg3) {
     dbgJogPause(rVar.rJogPause); }
   }
  }
  break;
  
 case MPG_CHECK_QUE:
 {
  if (rVar.rJogPause & DISABLE_JOG &&
      (rVar.rJogPause & axis->c.jogFlag) == 0)
  {
   ld(axis->c.base + F_Ld_Axis_Ctl, CTL_INIT);
   ld(axis->c.base + F_Ld_Axis_Ctl, 0);
   axis->mpgState = MPG_DISABLED;
   if (dbg3) {
    dbgJogPause(rVar.rJogPause); }
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

  int dist = axis->v.jogInc;
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

  ld(axis->c.base + F_Sync_Base + F_Ld_Dist, dist); /* set dist to go */
 }
 break;

 case MPG_DIR_CHANGE_WAIT:
 {
  const int axisStat = rd(axis->c.base + F_Rd_Status); /* read status */
  if ((axisStat & AX_DIST_ZERO) == 0 ||	/* if axis active */
      millis() - axis->dirWaitStart > MPG_WAIT_DELAY) /* or delay done */
  {
   if (axis->dir == R_DIR_POS)
    axis->mpgAxisCtl |= CTL_DIR;
   else
    axis->mpgAxisCtl &= ~CTL_DIR;

   ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl);
   if (dbg3) {
    dbgAxisCtl(axis->c.name, axis->mpgAxisCtl); }

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
  const int axisStat = rd(axis->c.base + F_Rd_Axis_Status); /* read status */
  if ((axisStat & AX_DIST_ZERO) != 0) /* if done */
  {
   ld(axis->c.base + F_Ld_Axis_Ctl, axis->mpgAxisCtl); /* clear backlash */
   if (dbg3) {
    dbgAxisCtl(axis->c.name, axis->mpgAxisCtl); }

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
  if (dbg3) {
   dbgPutC(axis->c.name);
   dbgPutStr(" mpgState");
   dbgPutHexByte(axis->mpgState);
   dbgPutSpace();
   const char *p = (char *) &mpgStateStr[axis->mpgState];
   dbgPutC(*p++);
   dbgPutC(*p);
   dbgNewLine(); }
 }
}

void moveRel(const P_AXIS_CTL axis, const int dist, const int cmd)
{
 if (dbg3) {
  dbgNewLine();
  dbgPutC(axis->c.name);
  dbgPutStr(" moveRel");
  dbgMoveCmd(cmd);
  dbgPrtDist(" dist", axis, dist);
  dbgPrtLoc(" exp", axis, axis->expLoc);
  dbgNewLine(); }
  
 if (dbgM) {
 dbgMsg(axis->c.dbgBase + D_MVCM, cmd);
 dbgMsg(axis->c.dbgBase + D_DST, dist); }

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
   }
  }
  else				/* distance zero */
  {
   dbgPutC(axis->c.name);
   dbgPutStr(" axis distance zero\n\n");
  }
 }
 else				/* axis not idle */
 {
  dbgAxisState(axis);
  dbgPutStr("*** ");
  dbgPutC(axis->c.name);
  dbgPutStr(" axis not idle\n\n");
 }

 axisStateCheck(axis);
}

void moveBacklash(const P_AXIS_CTL axis)
{
 axis->curDist = axis->backlashSteps;
 axisLoad(axis, axis->c.accelOffset + A_JOG);
 axis->ctlFlag |= CTL_BACKLASH;	/* set direction positive */

 const int tmp = axis->ctlFlag | CTL_START;
 ld(axis->c.base + F_Ld_Axis_Ctl, tmp);

 if (dbgM) {
  dbgMsg(axis->c.dbgBase + D_ACTL, tmp); }
}

void setLoc(const P_AXIS_CTL axis, const int loc)
{
 const int base = axis->c.base;
 
 ld(base + F_Sync_Base + F_Ld_Loc, loc);
 ld(base + F_Sync_Base + F_Ld_Dro, loc);
 ld(base + F_Ld_Axis_Ctl, CTL_SET_LOC | CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

void axisStop(const P_AXIS_CTL axis)
{
 if (dbg3) {
  dbgPutC(axis->c.name);
  dbgPutStr(" axisStop\n"); }

 rVar.rJogPause = 0;
 ld(axis->c.base + F_Ld_Axis_Ctl, 0);
 clockLoad(axis, CLK_NONE);
 axis->state = RS_IDLE;
}

void encScalerSetup()
{
 ld(F_Enc_Base + F_Ld_Enc_Prescale, rVar.rSynEncPreScaler);
 ld(F_Enc_Base + F_Ld_Enc_Cycle, rVar.rSynEncCycle);
 ld(F_Enc_Base + F_Ld_Int_Cycle, rVar.rSynOutCycle);

 ld(F_Ld_Sync_Ctl, SYN_ENC_INIT);
 ld(F_Ld_Sync_Ctl, 0);
 ld(F_Ld_Sync_Ctl, SYN_ENC_ENA | (ENC_CLK_CH << ENC_CLK_SHIFT));
}

#include "selTurnStr.h"
#include "selThreadStr.h"
#include "selRunoutStr.h"

void axisMove(const P_AXIS_CTL axis)
{
 if (dbg3) {
  dbgPutC(axis->c.name);
  dbgPutStr(" axisMove\n"); }

 int accelIndex = -1;
 int clkSel = CLK_FREQ;
 
 const int cmd = axis->cmd;
 const int swCmd = cmd & CMD_MSK;
 if (dbg3) {
  dbgSelPrt(swCmd, "moveCmd", &moveCmdStr[swCmd].c0); }
 switch (swCmd)
 {
 case CMD_SYN:
  accelIndex = A_TURN;
  clkSel = CLK_CH;
  
  if ((cmd & (SYN_START | SYN_LEFT)) != 0)
   axis->ctlFlag |= CTL_WAIT_SYNC;
  
  if (cmd & SYN_TAPER)
  {
   const P_AXIS_CTL aT = axis->c.other;	/* get other axis */

   aT->curLoc = (int) rd(aT->c.base + F_Sync_Base + F_Rd_Loc);
   aT->expLoc = aT->v.savedLoc;

   if (dbgM) {
    dbgMsg(aT->c.dbgBase + D_MOV, aT->v.savedLoc);
    dbgMsg(aT->c.dbgBase + D_CUR, aT->curLoc); }

   int slvDist = aT->v.savedLoc - aT->curLoc;
   if (dbgM) {
    dbgMsg(aT->c.dbgBase + D_DST, slvDist); }

   int slvCtl = CTL_SLAVE;
   if (slvDist > 0)
    slvCtl |= CTL_DIR;
   else
    slvDist = -slvDist;
   aT->curDist = slvDist;

   axisLoad(aT, aT->c.accelOffset + A_TAPER);

   ld(aT->c.base + F_Ld_Axis_Ctl, slvCtl);

   if (dbg3) {
    dbgAxisCtl(axis->c.name, slvCtl); }

   if (dbgM) {
    dbgMsg(aT->c.dbgBase + D_ACTL, slvCtl); }

   clockLoad(aT, CLK_SLV_CH);

   aT->state = RS_WAIT_TAPER;
  }

  if ((cmd & ENA_THREAD) == 0)	/* if turning */
  {
   const int syn = rVar.rTurnSync;
   if (dbg3) {
    dbgSelPrt(syn, "turnSyn", &selTurnStr[syn].c0); }
   switch (syn)
   {
   case SEL_TU_STEP:
    break;

   case SEL_TU_ENC:
    break;

   case SEL_TU_SYN:
    encScalerSetup();
    axis->ctlFlag |= CTL_CH_DIRECT;
    clkSel = CLK_INT_CLK;
    accelIndex = -1;
    break;

   default:
    break;
   }
  }
  else				/* if threading */
  {
   const int syn = rVar.rThreadSync;
   if (dbg3) {
    dbgSelPrt(syn, "threadSyn", &selThreadStr[syn].c0); }
   switch (syn)
   {
   case SEL_TH_STEP:
    break;

   case SEL_TH_ENC:
    break;

   case SEL_TH_SYN:
    encScalerSetup();
    axis->ctlFlag |= CTL_CH_DIRECT;
    clkSel = CLK_INT_CLK;
    accelIndex = -1;
    break;

   default:
    break;
   }
  }

  if ((cmd & ENA_RUNOUT) != 0)	/* if runout enabled */
  {
   const int syn = rVar.rRunoutSync;
   if (syn != 0)
   {
    if (dbg3) {
     dbgSelPrt(syn, "runoutSyn", &selRunoutStr[syn].c0); }

    const P_AXIS_CTL aT = axis->c.other;	/* get other axis */
    aT->curLoc = (int) rd(aT->c.base + F_Sync_Base + F_Rd_Loc);

    if (dbgM) {
     dbgMsg(aT->c.dbgBase + D_CUR, aT->curLoc); }

    switch (syn)
    {
    case SEL_RU_STEP:
     break;

    case SEL_RU_ENC:
     break;

    case SEL_RU_SYN:
    {
     encScalerSetup();

     int runoutLimit = rVar.rRunoutDist;
     if ((cmd & SYN_LEFT) == 0)
      runoutLimit = axis->dist - runoutLimit;

     ld(F_RunOut_Base + F_Ld_Run_Limit, runoutLimit);
     ld(F_RunOut_Base + F_Ld_RunOut_Ctl, RUN_OUT_INIT);
     ld(F_RunOut_Base + F_Ld_RunOut_Ctl, 0);

     const int runoutFlag = ((cmd & SYN_LEFT) == 0 ?
			     (RUN_OUT_ENA | RUN_OUT_DIR) : RUN_OUT_ENA);
     ld(F_RunOut_Base + F_Ld_RunOut_Ctl, runoutFlag);

     int xAxisFlag = CTL_SLAVE | CTL_CH_DIRECT;

     int depth = rVar.rRunoutDepth;
     aT->dist = depth;

     if (dbg3) {
      dbgPrtDist("depth", aT, depth);
      dbgPrtLoc(" loc", aT, aT->curLoc);
      dbgNewLine(); }

     aT->expLoc = aT->curLoc + depth;
     if (depth > 0)
      xAxisFlag |= CTL_DIR;
     else
      depth = -depth;

     if (dbgM) {
      dbgMsg(axis->c.dbgBase + D_EXP, axis->expLoc);
      dbgMsg(aT->c.dbgBase + D_DST, depth); }

     ld(aT->c.base + F_Sync_Base + F_Ld_Dist, depth);

     ld(aT->c.base + F_Ld_Axis_Ctl, CTL_INIT);
     ld(aT->c.base + F_Ld_Axis_Ctl, 0);
	 
     ld(aT->c.base + F_Ld_Axis_Ctl, xAxisFlag);

     if (dbg3) {
      dbgAxisCtl(aT->c.name, xAxisFlag); }

     if (dbgM) {
      dbgMsg(aT->c.dbgBase + D_ACTL, xAxisFlag); }

     clockLoad(aT, CLK_INT_CLK);

     aT->cmd = axis->cmd;
     aT->state = RS_WAIT_RUNOUT;
    }
    break;

    default:
     break;
    }
   }
  }
  break;

 case CMD_JOG:
  if (cmd & DIST_MODE)
   axis->ctlFlag |= CTL_DIST_MODE;
  
  if (cmd & CLEAR_HOME)
   axis->ctlFlag |= CTL_HOME;
  if (cmd & FIND_HOME)
   axis->ctlFlag |= CTL_HOME | CTL_HOME_POL;

  accelIndex = A_JOG;
  break;

 case CMD_MOV:
 case CMD_MAX:
  accelIndex = A_MOVE;
  break;

  // case CMD_SPEED:
  //  break;

 case JOG_SLOW:
  if (cmd & FIND_HOME)
   axis->ctlFlag |= CTL_HOME | CTL_HOME_POL;

  if (cmd & FIND_PROBE)
   axis->ctlFlag |= CTL_PROBE;

  accelIndex = A_SLOW;
  break;

 default:
  break;
 }

 clockLoad(axis, clkSel);

 if (accelIndex >= 0)
 {
  axisLoad(axis, axis->c.accelOffset + accelIndex);
 }
 else
 {
  const int base = axis->c.base;
  ld(base + F_Sync_Base + F_Ld_Dist, axis->dist);
  ld(base + F_Ld_Axis_Ctl, CTL_INIT);
  ld(base + F_Ld_Axis_Ctl, 0);
 }

 const int flag = axis->ctlFlag | CTL_START;

 ld(axis->c.base + F_Ld_Axis_Ctl, flag);

 if (dbg3) {
  dbgAxisCtl(axis->c.name, flag); }

 if (dbgM) {
  dbgMsg(axis->c.dbgBase + D_ACTL, flag); }

 if (dbg3) {
  dbgAxisStatus("axisMove start", axis);
  dbgStatus(rd(F_Rd_Status));
  dbgInPin("rd", rd(F_Rd_Inputs)); }
}

void axisHome(const P_AXIS_CTL axis, const int homeCmd)
{
 if (dbg3) {
 dbgPutC(axis->c.name);
 dbgPutStr(" axisHome\n"); }

 rVar.rJogPause |= DISABLE_JOG;	/* disable jogging */
 if (dbg3) {
  dbgJogPause(rVar.rJogPause);
  dbgMvStatus(rVar.rMvStatus); }

 jogMpg(axis);			/* call mpg poll routine to disable */

 if (dbg3) {
  dbgStatus(rd(F_Rd_Status)); }
 
 if (axis->state == RS_IDLE)	/* if axis idle */
 {
  const int inputStatus = rd(F_Rd_Inputs);
  axis->v.homeStatus = HOME_ACTIVE;
  int dist;
  int flag;
  if (homeCmd == HOME_FWD)	/* if forward homing*/
  {
   if ((inputStatus & axis->c.inHome) == 0) /* if home switch open */
   {
    dist = axis->v.homeFindFwd;
    flag = CMD_JOG | FIND_HOME;
    axis->homeState = H_HOME;	/* find home forward */
   }
   else
   {
    dist = -axis->v.homeFindFwd;
    flag = CMD_JOG | CLEAR_HOME;
    axis->homeState = H_OFF_HOME; /* move off home switch */
   }
  }
  else				/* if reverse homing */
  {
   if ((inputStatus & axis->c.inHome) == 0) /* if home switch hope */
   {
    dist = axis->v.homeFindRev;
    flag = CMD_JOG | FIND_HOME;
    axis->homeState = H_HOME;	/* find home */
   }
   else
   {
    dist = -axis->v.homeFindFwd;
    flag = CMD_JOG | CLEAR_HOME;
    axis->homeState = H_OFF_HOME; /* move off home switch */
   }
  }
  moveRel(axis, dist, flag);
  int mvStatus = rVar.rMvStatus;
  mvStatus &= ~axis->c.homedBit;  /* set not homed */
  mvStatus |= axis->c.homeActiveBit; /* set home active */
  rVar.rMvStatus = mvStatus;
  if (dbg3) {
   dbgMvStatus(rVar.rMvStatus); }
 }
}

#include "fpgaClkSelStr.h"

void clockLoad(const P_AXIS_CTL axis, const int clkSel)
{
 if (dbg3) {
  dbgPutC(axis->c.name);
  dbgSelPrt(clkSel, " clockLoad", &fpgaClkSelStr[clkSel].c0); }

 const int shift = axis->c.clkShift;
 clockSelVal &= ~(CLK_MASK << shift);
 clockSelVal |= clkSel << shift;
 ld(F_Ld_Clk_Ctl, clockSelVal);
}

struct S_AXIS_ACCEL_TYPE
{
 char c0;
 char c1;
};

extern struct S_AXIS_ACCEL_TYPE axisAccelTypeStr[];

void axisLoad(const P_AXIS_CTL axis, const int index)
{
 if (dbg3) {  
  dbgPutC(axis->c.name);
  dbgSelPrt(index, " axisLoad", &axisAccelTypeStr[index].c0); }

 const int base = axis->c.base;
 const P_ACCEL_DATA aData = accelData[index];
 if (aData->freqDiv != 0)
  ld(base + F_Ld_Freq, aData->freqDiv);

 const int bSyn = axis->c.base + F_Sync_Base;
 ld(bSyn + F_Ld_D, aData->initialSum);
 ld(bSyn + F_Ld_Incr1, aData->incr1);
 ld(bSyn + F_Ld_Incr2, aData->incr2);

 ld(bSyn + F_Ld_Accel_Val, aData->accelVal);
 ld(bSyn + F_Ld_Accel_Count, aData->accelCount);

 int dist = axis->curDist;
 if (dist != 0)
 {
  axis->curDist = 0;
 }
 else
 {
  dist = axis->dist;
 }
 ld(base + F_Sync_Base + F_Ld_Dist, dist);
 ld(base + F_Ld_Axis_Ctl, CTL_INIT);
 ld(base + F_Ld_Axis_Ctl, 0);
}

#endif  /* LATHECPP_AXIS_CTL */

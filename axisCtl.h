#if !defined(AXIS_CTL_INC)	// <-
#define AXIS_CTL_INC
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
 union
 {
  int accelCount;
  int accelMax;
 };
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

EXT T_ACCEL_DATA spRun;
EXT T_ACCEL_DATA spJog;

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
void spLoad(const int index);

#endif  /* AXIS_CTL_INC */	// ->

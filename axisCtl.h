#if !defined(AXIS_CTL_INC)	// <-
#define AXIS_CTL_INC

#define R_DIR_POS 1
#define R_DIR_NEG 0

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

#endif  /* AXIS_CTL_INC */	// ->

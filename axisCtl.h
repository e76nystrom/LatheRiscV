#if !defined(AXISCTL_INC)	// <-
#define AXISCTL_INC

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
void axisCtl(void);
void axisStateCheck(P_AXIS_CTL axis);
void axisCheck(P_AXIS_CTL axis, int status);
void move(P_AXIS_CTL axis, int cmd, int loc);
void jogMove(P_AXIS_CTL axis, int dist);
void moveRel(P_AXIS_CTL axis, int dist, int cmd);
void setLoc(P_AXIS_CTL axis, int loc);
void axisStop(P_AXIS_CTL a);
void axisMove(P_AXIS_CTL a);
void axisLoad(P_AXIS_CTL a, int index);

char *fmtLoc(char *buf, P_AXIS_CTL axis, int loc);
char *fmtDist(char *buf, P_AXIS_CTL axis, int dist);

#endif  /* AXISCTL_INC */	// ->

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

#endif  /* AXISCTL_INC */	// ->

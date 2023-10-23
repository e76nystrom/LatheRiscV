#if !defined(REMCMD_INC)	// <-
#define REMCMD_INC

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

#endif  /* REMCMD_INC */	// ->

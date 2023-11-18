#if !defined(REM_CMD_INC)	// <-
#define REM_CMD_INC

typedef struct S_RUN_DATA
{
 enum RISCV_CMD parm;
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
 enum RISCV_RUN_WAIT wait;
} T_RUN_CTL, *P_RUN_CTL;

EXT T_RUN_CTL runCtl;

typedef struct S_DEBUG_DATA
{
 int dbg;
 int32_t val;
 uint32_t time;
} T_DEBUG_DATA, *P_DEBUG_DATA;

#define DEBUG_DATA_SIZE 48
// 2-code 1-space 1-neg 8-val 1-space
// #define MAX_DBG_SIZE (2 + 1 + 1 + 8 + 1)
// 2-code 4-val
#define MAX_DBG_SIZE (1 + 4)

typedef struct S_DEBUG_QUE
{
 int count;
 int fil;
 int emp;
 T_DEBUG_DATA data[DEBUG_DATA_SIZE];
 int rsv[16];
} T_DEBUG_QUE, *P_DEBUG_QUE;

EXT T_DEBUG_QUE dbgQue;

void remCmd(void);

void runInit(void);
void runProcess(void);

void rspPut(char ch);
void rspPutDigit(char ch);
void rspPutByte(char ch);

void rspReplDigit(unsigned char ch);
void rspReplByte(unsigned char ch);
void rspReplHex(unsigned char ch);

void rspPutHex(unsigned int val, int size);

void saveAccel(int type, int val);
void saveData(int type, int val);

void clrDbgBuf(void);
void dbgMsg(int dbg, int val);

#endif  /* REM_CMD_INC */	// ->

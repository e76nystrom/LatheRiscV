#if !defined(RISCV_STRUCT)
#define RISCV_STRUCT
// c1File

#include <stdint.h>

#if !defined(__DATA_UNION__)
#define __DATA_UNION__

#define uint_t unsigned int

typedef union uDataUnion
{
 float    t_float;
 int      t_int;
 uint_t   t_uint_t;
 int32_t  t_int32_t;
 uint32_t t_uint32_t;
 int16_t  t_int16_t;
 uint16_t t_uint16_t;
 char     t_char;
} T_DATA_UNION, *P_DATA_UNION;

#endif  /* __DATA_UNION__ */

void setRiscvVar(int parm, T_DATA_UNION val);
void getRiscvVar(int parm, P_DATA_UNION val);

typedef struct sRiscvVar
{
 uint32_t rMvStatus;    /* 0x00 R_MV_STATUS      move status */
 int rJogPause;         /* 0x01 R_JOG_PAUSE      jog pause */
 int rCurPass;          /* 0x02 R_CUR_PASS       current pass */
 int rCfgVal;           /* 0x03 R_CFG_VAL        fpga configuration value */
 int rPRpm;             /* 0x04 R_P_RPM          spindle rpm */
 int rPwmDiv;           /* 0x05 R_PWM_DIV        pwm divider */
 int rPwmCtr;           /* 0x06 R_PWM_CTR        pwm counter max */
 int rSynEncPreScaler;  /* 0x07 R_SYN_ENC_PRE_SCALER encoder prescaler */
 int rSynEncCycle;      /* 0x08 R_SYN_ENC_CYCLE  encoder cycle len */
 int rSynOutCycle;      /* 0x09 R_SYN_OUT_CYCLE  output cycle len */
 int rTurnSync;         /* 0x0a R_TURN_SYNC      turn sync */
 int rThreadSync;       /* 0x0b R_THREAD_SYNC    thread sync */
 int rRunoutSync;       /* 0x0c R_RUNOUT_SYNC    runout sync */
 int rThreadFlags;      /* 0x0d R_THREAD_FLAGS   threading flags */
 int rRunoutLimit;      /* 0x0e R_RUNOUT_LIMIT   runout limit */
} T_RISCV_VAR, *P_RISCV_VAR;

typedef struct
{
 int stepsInch;
 int savedLoc;
 int homeOffset;
 int loc;
 int dro;
 int jogInc;
 int homeStatus;
 int homeFindFwd;
 int homeFindRev;
 int homeBackoff;
 int homeSlow;
 int testLimMin;
 int testLimMax;
 int testHomeMin;
 int testHomeMax;
 int testProbe;
}
T_AXIS_VAR, *P_AXIS_VAR;

extern unsigned char riscvSize[];
extern T_RISCV_VAR rVar;

#endif /* RISCV_STRUCT */

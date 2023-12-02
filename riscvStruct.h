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
 int rPwmCtr;           /* 0x06 R_PWM_CTR        pwm counter maxy */
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

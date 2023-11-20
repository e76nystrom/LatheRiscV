#include <stdint.h>
#define NO_REM_MACROS
#include "riscvParm.h"
#include "riscvStruct.h"

T_RISCV_VAR rVar;

#define FLT (0x80)
#define SIZE_MASK (0x7)

unsigned char riscvSize[] =
{
 sizeof(rVar.rMvStatus),                /* 0x00 move status */
 sizeof(rVar.rJogPause),                /* 0x01 jog pause */
 sizeof(rVar.rCurPass),                 /* 0x02 current pass */
 sizeof(rVar.rCfgVal),                  /* 0x03 fpga configuration value */
 sizeof(rVar.rPRpm),                    /* 0x04 spindle rpm */
 sizeof(rVar.rPwmDiv),                  /* 0x05 pwm divider */
 sizeof(rVar.rPwmCtr),                  /* 0x06 pwm counter maxy */
 sizeof(rVar.rPXLoc),                   /* 0x07  */
 sizeof(rVar.rPZLoc),                   /* 0x08  */
 sizeof(rVar.rPXDro),                   /* 0x09  */
 sizeof(rVar.rPZDro),                   /* 0x0a  */
 sizeof(rVar.rXJogInc),                 /* 0x0b  */
 sizeof(rVar.rZJogInc),                 /* 0x0c  */
};

void setRiscvVar(const int parm, const T_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00 move status */
  rVar.rMvStatus = val.t_uint32_t;
  break;

 case R_JOG_PAUSE:               /*  1 0x01 jog pause */
  rVar.rJogPause = val.t_int;
  break;

 case R_CUR_PASS:                /*  2 0x02 current pass */
  rVar.rCurPass = val.t_int;
  break;

 case R_CFG_VAL:                 /*  3 0x03 fpga configuration value */
  rVar.rCfgVal = val.t_int;
  break;

 case R_P_RPM:                   /*  4 0x04 spindle rpm */
  rVar.rPRpm = val.t_int;
  break;

 case R_PWM_DIV:                 /*  5 0x05 pwm divider */
  rVar.rPwmDiv = val.t_int;
  break;

 case R_PWM_CTR:                 /*  6 0x06 pwm counter maxy */
  rVar.rPwmCtr = val.t_int;
  break;

 case R_P_X_LOC:                 /*  7 0x07  */
  rVar.rPXLoc = val.t_int;
  break;

 case R_P_Z_LOC:                 /*  8 0x08  */
  rVar.rPZLoc = val.t_int;
  break;

 case R_P_X_DRO:                 /*  9 0x09  */
  rVar.rPXDro = val.t_int;
  break;

 case R_P_Z_DRO:                 /* 10 0x0a  */
  rVar.rPZDro = val.t_int;
  break;

 case R_X_JOG_INC:               /* 11 0x0b  */
  rVar.rXJogInc = val.t_int;
  break;

 case R_Z_JOG_INC:               /* 12 0x0c  */
  rVar.rZJogInc = val.t_int;
  break;

 }
}

void getRiscvVar(const int parm, const P_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00 move status */
  val->t_uint32_t = rVar.rMvStatus;
  break;

 case R_JOG_PAUSE:               /*  1 0x01 jog pause */
  val->t_int = rVar.rJogPause;
  break;

 case R_CUR_PASS:                /*  2 0x02 current pass */
  val->t_int = rVar.rCurPass;
  break;

 case R_CFG_VAL:                 /*  3 0x03 fpga configuration value */
  val->t_int = rVar.rCfgVal;
  break;

 case R_P_RPM:                   /*  4 0x04 spindle rpm */
  val->t_int = rVar.rPRpm;
  break;

 case R_PWM_DIV:                 /*  5 0x05 pwm divider */
  val->t_int = rVar.rPwmDiv;
  break;

 case R_PWM_CTR:                 /*  6 0x06 pwm counter maxy */
  val->t_int = rVar.rPwmCtr;
  break;

 case R_P_X_LOC:                 /*  7 0x07  */
  val->t_int = rVar.rPXLoc;
  break;

 case R_P_Z_LOC:                 /*  8 0x08  */
  val->t_int = rVar.rPZLoc;
  break;

 case R_P_X_DRO:                 /*  9 0x09  */
  val->t_int = rVar.rPXDro;
  break;

 case R_P_Z_DRO:                 /* 10 0x0a  */
  val->t_int = rVar.rPZDro;
  break;

 case R_X_JOG_INC:               /* 11 0x0b  */
  val->t_int = rVar.rXJogInc;
  break;

 case R_Z_JOG_INC:               /* 12 0x0c  */
  val->t_int = rVar.rZJogInc;
  break;

 }
}

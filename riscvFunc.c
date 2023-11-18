#include <stdint.h>
#define NO_REM_MACROS
#include "riscvParm.h"
#include "riscvStruct.h"

T_RISCV_VAR rVar;

#define FLT (0x80)
#define SIZE_MASK (0x7)

unsigned char riscvSize[] =
{
 sizeof(rVar.rMvStatus),                /* 0x00  */
 sizeof(rVar.rJogPause),                /* 0x01  */
 sizeof(rVar.rCurPass),                 /* 0x02  */
 sizeof(rVar.rPRpm),                    /* 0x03  */
 sizeof(rVar.rPXLoc),                   /* 0x04  */
 sizeof(rVar.rPZLoc),                   /* 0x05  */
 sizeof(rVar.rPXDro),                   /* 0x06  */
 sizeof(rVar.rPZDro),                   /* 0x07  */
 sizeof(rVar.rXJogInc),                 /* 0x08  */
 sizeof(rVar.rZJogInc),                 /* 0x09  */
};

void setRiscvVar(const int parm, const T_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00  */
  rVar.rMvStatus = val.t_uint32_t;
  break;

 case R_JOG_PAUSE:               /*  1 0x01  */
  rVar.rJogPause = val.t_int;
  break;

 case R_CUR_PASS:                /*  2 0x02  */
  rVar.rCurPass = val.t_int;
  break;

 case R_P_RPM:                   /*  3 0x03  */
  rVar.rPRpm = val.t_int;
  break;

 case R_P_X_LOC:                 /*  4 0x04  */
  rVar.rPXLoc = val.t_int;
  break;

 case R_P_Z_LOC:                 /*  5 0x05  */
  rVar.rPZLoc = val.t_int;
  break;

 case R_P_X_DRO:                 /*  6 0x06  */
  rVar.rPXDro = val.t_int;
  break;

 case R_P_Z_DRO:                 /*  7 0x07  */
  rVar.rPZDro = val.t_int;
  break;

 case R_X_JOG_INC:               /*  8 0x08  */
  rVar.rXJogInc = val.t_int;
  break;

 case R_Z_JOG_INC:               /*  9 0x09  */
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

 case R_MV_STATUS:               /*  0 0x00  */
  val->t_uint32_t = rVar.rMvStatus;
  break;

 case R_JOG_PAUSE:               /*  1 0x01  */
  val->t_int = rVar.rJogPause;
  break;

 case R_CUR_PASS:                /*  2 0x02  */
  val->t_int = rVar.rCurPass;
  break;

 case R_P_RPM:                   /*  3 0x03  */
  val->t_int = rVar.rPRpm;
  break;

 case R_P_X_LOC:                 /*  4 0x04  */
  val->t_int = rVar.rPXLoc;
  break;

 case R_P_Z_LOC:                 /*  5 0x05  */
  val->t_int = rVar.rPZLoc;
  break;

 case R_P_X_DRO:                 /*  6 0x06  */
  val->t_int = rVar.rPXDro;
  break;

 case R_P_Z_DRO:                 /*  7 0x07  */
  val->t_int = rVar.rPZDro;
  break;

 case R_X_JOG_INC:               /*  8 0x08  */
  val->t_int = rVar.rXJogInc;
  break;

 case R_Z_JOG_INC:               /*  9 0x09  */
  val->t_int = rVar.rZJogInc;
  break;

 }
}

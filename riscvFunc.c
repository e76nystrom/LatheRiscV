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
 sizeof(rVar.rCurPass),                 /* 0x01  */
 sizeof(rVar.rParmRpm),                 /* 0x02  */
 sizeof(rVar.rParmXLoc),                /* 0x03  */
 sizeof(rVar.rParmZLoc),                /* 0x04  */
 sizeof(rVar.rParmXDro),                /* 0x05  */
 sizeof(rVar.rParmZDro),                /* 0x06  */
};

void setRiscvVar(int parm, T_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00  */
  rVar.rMvStatus = val.t_uint32_t;
  break;

 case R_CUR_PASS:                /*  1 0x01  */
  rVar.rCurPass = val.t_int;
  break;

 case R_PARM_RPM:                /*  2 0x02  */
  rVar.rParmRpm = val.t_int;
  break;

 case R_PARM_X_LOC:              /*  3 0x03  */
  rVar.rParmXLoc = val.t_int;
  break;

 case R_PARM_Z_LOC:              /*  4 0x04  */
  rVar.rParmZLoc = val.t_int;
  break;

 case R_PARM_X_DRO:              /*  5 0x05  */
  rVar.rParmXDro = val.t_int;
  break;

 case R_PARM_Z_DRO:              /*  6 0x06  */
  rVar.rParmZDro = val.t_int;
  break;

 }
}

void getRiscvVar(int parm, P_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00  */
  val->t_uint32_t = rVar.rMvStatus;
  break;

 case R_CUR_PASS:                /*  1 0x01  */
  val->t_int = rVar.rCurPass;
  break;

 case R_PARM_RPM:                /*  2 0x02  */
  val->t_int = rVar.rParmRpm;
  break;

 case R_PARM_X_LOC:              /*  3 0x03  */
  val->t_int = rVar.rParmXLoc;
  break;

 case R_PARM_Z_LOC:              /*  4 0x04  */
  val->t_int = rVar.rParmZLoc;
  break;

 case R_PARM_X_DRO:              /*  5 0x05  */
  val->t_int = rVar.rParmXDro;
  break;

 case R_PARM_Z_DRO:              /*  6 0x06  */
  val->t_int = rVar.rParmZDro;
  break;

 }
}

#include <stdint.h>
#define NO_REM_MACROS
#include "riscvParmList.h"

#include "riscvStruct.h"

T_RISCV_VAR rVar;

void setRiscvVar(int parm, T_DATA_UNION val);
void getRiscvVar(int parm, P_DATA_UNION val);

void setRiscvVar(int parm, T_DATA_UNION val)
{
 switch(parm)
 {
 case R_PARM_RPM:                /*  0 0x00  */
  rVar.rParmRpm = val.t_int;
  break;

 case R_PARM_X_LOC:              /*  1 0x01  */
  rVar.rParmXLoc = val.t_int;
  break;

 case R_PARM_Z_LOC:              /*  2 0x02  */
  rVar.rParmZLoc = val.t_int;
  break;

 case R_PARM_X_DRO:              /*  3 0x03  */
  rVar.rParmXDro = val.t_int;
  break;

 case R_PARM_Z_DRO:              /*  4 0x04  */
  rVar.rParmZDro = val.t_int;
  break;

 };
}

void getRiscvVar(int parm, P_DATA_UNION val)
{
 switch(parm)
 {
 case R_PARM_RPM:                /*  0 0x00  */
  val->t_int = rVar.rParmRpm;
  break;

 case R_PARM_X_LOC:              /*  1 0x01  */
  val->t_int = rVar.rParmXLoc;
  break;

 case R_PARM_Z_LOC:              /*  2 0x02  */
  val->t_int = rVar.rParmZLoc;
  break;

 case R_PARM_X_DRO:              /*  3 0x03  */
  val->t_int = rVar.rParmXDro;
  break;

 case R_PARM_Z_DRO:              /*  4 0x04  */
  val->t_int = rVar.rParmZDro;
  break;

 };
}

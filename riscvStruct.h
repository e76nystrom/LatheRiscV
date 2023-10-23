#if !defined(RISCV_STRUCT)
#define RISCV_STRUCT

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
 uint32_t rMvStatus;    /* 0x00 R_MV_STATUS       */
 int rCurPass;          /* 0x01 R_CUR_PASS        */
 int rParmRpm;          /* 0x02 R_PARM_RPM        */
 int rParmXLoc;         /* 0x03 R_PARM_X_LOC      */
 int rParmZLoc;         /* 0x04 R_PARM_Z_LOC      */
 int rParmXDro;         /* 0x05 R_PARM_X_DRO      */
 int rParmZDro;         /* 0x06 R_PARM_Z_DRO      */
} T_RISCV_VAR, *P_RISCV_VAR;

extern unsigned char riscvSize[];
extern T_RISCV_VAR rVar;

#endif /* RISCV_STRUCT */

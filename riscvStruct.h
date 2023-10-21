#if !defined(RISCV_STRUCT)
#define RISCV_STRUCT

#include <stdint.h>

#if !defined(__DATA_UNION__)
#define __DATA_UNION__

typedef union uDataUnion
{
 float        t_float;
 int          t_int;
 unsigned int t_unsigned_int;
 int32_t      t_int32_t;
 int16_t      t_int16_t;
 uint16_t     t_uint16_t;
 char         t_char;
} T_DATA_UNION, *P_DATA_UNION;

#endif  /* __DATA_UNION__ */

void setRiscvVar(int parm, T_DATA_UNION val);
void getRiscvVar(int parm, P_DATA_UNION val);

typedef struct sRiscvVar
{
 int rParmRpm;                  /* 0x00  */
 int rParmXLoc;                 /* 0x01  */
 int rParmZLoc;                 /* 0x02  */
 int rParmXDro;                 /* 0x03  */
 int rParmZDro;                 /* 0x04  */
} T_RISCV_VAR, *P_RISCV_VAR;

extern unsigned char riscvSize[];
extern T_RISCV_VAR rVar;

#endif /* RISCV_STRUCT */

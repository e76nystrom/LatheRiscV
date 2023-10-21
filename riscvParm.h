/* defines */

#define FLT (0x80)
#define SIZE_MASK (0x7)

unsigned char riscvParm[] =
{
 sizeof(rVar.rParmRpm),                 /* 0x00  */
 sizeof(rVar.rParmXLoc),                /* 0x01  */
 sizeof(rVar.rParmZLoc),                /* 0x02  */
 sizeof(rVar.rParmXDro),                /* 0x03  */
 sizeof(rVar.rParmZDro),                /* 0x04  */
};

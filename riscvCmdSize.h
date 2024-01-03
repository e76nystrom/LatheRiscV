#if !defined(RISCV_CMD_SIZE)
#define RISCV_CMD_SIZE

int riscvCmdSize[] =
{
 0,     /*  R_NONE           0x00  0 'NO' no operation */
 0,     /*  R_READ_ALL       0x01  1 'RA' read all status */
 1,     /*  R_READ_DBG       0x02  2 'RD' read debug info */
 0,     /*  R_SETUP          0x03  3 'SU' setup */
 0,     /*  R_RESUME         0x04  4 'RE' resume */
 0,     /*  R_STOP           0x05  5 'SP' stop */
 0,     /*  R_STOP_X         0x06  6 'SX' stop x */
 0,     /*  R_STOP_Z         0x07  7 'SZ' stop z */
 0,     /*  R_DONE           0x08  8 'DN' done */
 0,     /*  R_SEND_DONE      0x09  9 'ND' send data done */
 0,     /*  R_STR_SPIN       0x0a 10 'S+' start spindle */
 0,     /*  R_STP_SPIN       0x0b 11 'S-' stop spindle */
 0,     /*  R_UPD_SPIN       0x0c 12 'US' update spindle speed */
 1,     /*  R_SET_LOC_X      0x0d 13 'LX' set x loc */
 1,     /*  R_SET_LOC_Z      0x0e 14 'LZ' set z loc */
 2,     /*  R_SET_ACCEL      0x0f 15 'SA' set accel parm */
 2,     /*  R_SET_DATA       0x10 16 'SD' set data */
 1,     /*  R_GET_DATA       0x11 17 'GD' set data */
 1,     /*  R_JOG_Z          0x12 18 'JZ' jog move z */
 1,     /*  R_JOG_X          0x13 19 'JX' jog move x */
 1,     /*  R_HOME_Z         0x14 20 'HZ' home z */
 1,     /*  R_HOME_X         0x15 21 'HX' home x */
 0,     /*  R_OP_START       0x16 22 'OS' start first queued command */
 0,     /*  R_OP_DONE        0x17 23 'OD' done */
 0,     /*  R_PAUSE          0x18 24 'PA' pause */
 0,     /*  R_ENC_SCL_STR    0x19 25 'ES' encoder scale start */
 0,     /*  R_STR_SPIN_Q     0x1a 26 'Q+' start spindle */
 0,     /*  R_STP_SPIN_Q     0x1b 27 'Q-' stop spindle */
 1,     /*  R_PASS           0x1c 28 'PS' pass */
 2,     /*  R_SET_ACCEL_Q    0x1d 29 'SQ' set accel parm queued */
 2,     /*  R_SET_DATA_Q     0x1e 30 'SD' set data */
 2,     /*  R_MOVE_Z         0x1f 31 'MZ' move z */
 2,     /*  R_MOVE_X         0x20 32 'MX' move x */
 2,     /*  R_MOVE_REL_Z     0x21 33 'RZ' move rel z */
 2,     /*  R_MOVE_REL_X     0x22 34 'RX' move rel x */
};

#endif  /* RISCV_CMD_SIZE */

#if !defined(RISCV_CMD_LIST)
#define RISCV_CMD_LIST

// cFile

enum RISCV_CMD
{
 R_NONE,                        /* 0x00 'NO' no operation */
 R_READ_ALL,                    /* 0x01 'RA' read all status */
 R_READ_DBG,                    /* 0x02 'RD' read debug info */
 R_SETUP,                       /* 0x03 'SU' setup */
 R_RESUME,                      /* 0x04 'RE' resume */
 R_STOP,                        /* 0x05 'SP' stop */
 R_STOP_X,                      /* 0x06 'SX' stop x */
 R_STOP_Z,                      /* 0x07 'SZ' stop z */
 R_DONE,                        /* 0x08 'DN' done */
 R_SEND_DONE,                   /* 0x09 'ND' send data done */
 R_STR_SPIN,                    /* 0x0a 'S+' start spindle */
 R_STP_SPIN,                    /* 0x0b 'S-' stop spindle */
 R_UPD_SPIN,                    /* 0x0c 'US' update spindle speed */
 R_SET_LOC_X,                   /* 0x0d 'LX' set x loc */
 R_SET_LOC_Z,                   /* 0x0e 'LZ' set z loc */
 R_SET_ACCEL,                   /* 0x0f 'SA' set accel parm */
 R_SET_DATA,                    /* 0x10 'SD' set data */
 R_GET_DATA,                    /* 0x11 'GD' set data */
 R_JOG_Z,                       /* 0x12 'JZ' jog move z */
 R_JOG_X,                       /* 0x13 'JX' jog move x */
 R_HOME_Z,                      /* 0x14 'MZ' home z */
 R_HOME_X,                      /* 0x15 'MX' home x */
 R_OP_START,                    /* 0x16 'OS' start first queued command */
 R_OP_DONE,                     /* 0x17 'OD' done */
 R_PAUSE,                       /* 0x18 'PA' pause */
 R_STR_SPIN_Q,                  /* 0x19 'Q+' start spindle */
 R_STP_SPIN_Q,                  /* 0x1a 'Q-' stop spindle */
 R_PASS,                        /* 0x1b 'PS' pass */
 R_SET_ACCEL_Q,                 /* 0x1c 'SQ' set accel parm queued */
 R_SET_DATA_Q,                  /* 0x1d 'SD' set data */
 R_MOVE_Z,                      /* 0x1e 'MZ' move z */
 R_MOVE_X,                      /* 0x1f 'MX' move x */
 R_MOVE_REL_Z,                  /* 0x20 'HZ' move rel z */
 R_MOVE_REL_X,                  /* 0x21 'HX' move rel x */
 R_MAX_CMD,                     /* 0x22 'MX' maximum command */
};

#endif  /* RISCV_CMD_LIST */

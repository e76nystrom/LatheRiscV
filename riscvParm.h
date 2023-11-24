#if !defined(RISCV_PARM_INC)
#define RISCV_PARM_INC
// cFile

/* defines */

#define FLT (0x80)
#define SIZE_MASK (0x7)

enum RISCV_PARM_DEF
{
 R_MV_STATUS,                   /* 0x00 move status */
 R_JOG_PAUSE,                   /* 0x01 jog pause */
 R_CUR_PASS,                    /* 0x02 current pass */
 R_CFG_VAL,                     /* 0x03 fpga configuration value */
 R_P_RPM,                       /* 0x04 spindle rpm */
 R_PWM_DIV,                     /* 0x05 pwm divider */
 R_PWM_CTR,                     /* 0x06 pwm counter maxy */
 R_Z_STEPS_INCH,                /* 0x07 steps per inch */
 R_Z_SAVED_LOC,                 /* 0x08 saved for que op */
 R_Z_HOME_OFFSET,               /* 0x09 home offset */
 R_Z_LOC,                       /* 0x0a location */
 R_Z_DRO,                       /* 0x0b dro */
 R_Z_JOG_INC,                   /* 0x0c jog increment */
 R_Z_HOME_STATUS,               /* 0x0d home status */
 R_Z_HOME_FIND_FWD,             /* 0x0e max homing distance */
 R_Z_HOME_FIND_REV,             /* 0x0f max rev homing distance */
 R_Z_HOME_BACKOFF,              /* 0x10 home backoff dist */
 R_Z_HOME_SLOW,                 /* 0x11 home backoff dist */
 R_X_STEPS_INCH,                /* 0x12 steps per inch */
 R_X_SAVED_LOC,                 /* 0x13 saved for que op */
 R_X_HOME_OFFSET,               /* 0x14 home offset */
 R_X_LOC,                       /* 0x15 location */
 R_X_DRO,                       /* 0x16 dro */
 R_X_JOG_INC,                   /* 0x17 jog increment */
 R_X_HOME_STATUS,               /* 0x18 home status */
 R_X_HOME_FIND_FWD,             /* 0x19 max homing distance */
 R_X_HOME_FIND_REV,             /* 0x1a max rev homing distance */
 R_X_HOME_BACKOFF,              /* 0x1b home backoff dist */
 R_X_HOME_SLOW,                 /* 0x1c home backoff dist */
};

#endif  /* RISCV_PARM_INC */

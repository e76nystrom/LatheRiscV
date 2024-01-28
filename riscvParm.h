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
 R_SP_RPM,                      /* 0x04 spindle rpm */
 R_PWM_DIV,                     /* 0x05 pwm divider */
 R_PWM_CTR,                     /* 0x06 pwm counter max */
 R_SYN_ENC_PRE_SCALER,          /* 0x07 encoder prescaler */
 R_SYN_ENC_CYCLE,               /* 0x08 encoder cycle len */
 R_SYN_OUT_CYCLE,               /* 0x09 output cycle len */
 R_STEP_DRV,                    /* 0x0a stepper drive */
 R_ENC_PER_REV,                 /* 0x0b spindle encoder count */
 R_SP_STEP_MULT,                /* 0x0c spindle step multiplier */
 R_TURN_SYNC,                   /* 0x0d turn sync */
 R_THREAD_SYNC,                 /* 0x0e thread sync */
 R_RUNOUT_SYNC,                 /* 0x0f runout sync */
 R_THREAD_FLAGS,                /* 0x10 threading flags */
 R_RUNOUT_DIST,                 /* 0x11 runout distance */
 R_RUNOUT_DEPTH,                /* 0x12 runout depth */
 R_Z_STEPS_INCH,                /* 0x13 steps per inch */
 R_Z_SAVED_LOC,                 /* 0x14 saved for que op */
 R_Z_HOME_OFFSET,               /* 0x15 home offset */
 R_Z_LOC,                       /* 0x16 location */
 R_Z_DRO,                       /* 0x17 dro */
 R_Z_JOG_INC,                   /* 0x18 jog increment */
 R_Z_HOME_STATUS,               /* 0x19 home status */
 R_Z_HOME_FIND_FWD,             /* 0x1a max homing distance */
 R_Z_HOME_FIND_REV,             /* 0x1b max rev homing distance */
 R_Z_HOME_BACKOFF,              /* 0x1c home backoff dist */
 R_Z_HOME_SLOW,                 /* 0x1d home backoff dist */
 R_Z_TEST_LIM_MIN,              /* 0x1e test minimum limit */
 R_Z_TEST_LIM_MAX,              /* 0x1f test maximum limit */
 R_Z_TEST_HOME_MIN,             /* 0x20 test home minimum */
 R_Z_TEST_HOME_MAX,             /* 0x21 test home maximum */
 R_Z_TEST_PROBE,                /* 0x22 test probe position */
 R_X_STEPS_INCH,                /* 0x23 steps per inch */
 R_X_SAVED_LOC,                 /* 0x24 saved for que op */
 R_X_HOME_OFFSET,               /* 0x25 home offset */
 R_X_LOC,                       /* 0x26 location */
 R_X_DRO,                       /* 0x27 dro */
 R_X_JOG_INC,                   /* 0x28 jog increment */
 R_X_HOME_STATUS,               /* 0x29 home status */
 R_X_HOME_FIND_FWD,             /* 0x2a max homing distance */
 R_X_HOME_FIND_REV,             /* 0x2b max rev homing distance */
 R_X_HOME_BACKOFF,              /* 0x2c home backoff dist */
 R_X_HOME_SLOW,                 /* 0x2d home backoff dist */
 R_X_TEST_LIM_MIN,              /* 0x2e test minimum limit */
 R_X_TEST_LIM_MAX,              /* 0x2f test maximum limit */
 R_X_TEST_HOME_MIN,             /* 0x30 test home minimum */
 R_X_TEST_HOME_MAX,             /* 0x31 test home maximum */
 R_X_TEST_PROBE,                /* 0x32 test probe position */
};

#endif  /* RISCV_PARM_INC */

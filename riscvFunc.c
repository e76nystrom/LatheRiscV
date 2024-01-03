// c2File

#include <stdint.h>
#define NO_REM_MACROS
#include "riscvParm.h"
#include "riscvStruct.h"
#if !defined(EXT)
#define EXT extern
#endif
T_RISCV_VAR rVar;

#include "axisCtl.h"

#define FLT (0x80)
#define SIZE_MASK (0x7)

unsigned char riscvSize[] =
{
 sizeof(rVar.rMvStatus),                /* 0x00 move status */
 sizeof(rVar.rJogPause),                /* 0x01 jog pause */
 sizeof(rVar.rCurPass),                 /* 0x02 current pass */
 sizeof(rVar.rCfgVal),                  /* 0x03 fpga configuration value */
 sizeof(rVar.rPRpm),                    /* 0x04 spindle rpm */
 sizeof(rVar.rPwmDiv),                  /* 0x05 pwm divider */
 sizeof(rVar.rPwmCtr),                  /* 0x06 pwm counter max */
 sizeof(rVar.rSynEncPreScaler),         /* 0x07 encoder prescaler */
 sizeof(rVar.rSynEncCycle),             /* 0x08 encoder cycle len */
 sizeof(rVar.rSynOutCycle),             /* 0x09 output cycle len */
 sizeof(rVar.rTurnSync),                /* 0x0a turn sync */
 sizeof(rVar.rThreadSync),              /* 0x0b thread sync */
 sizeof(rVar.rRunoutSync),              /* 0x0c runout sync */
 sizeof(rVar.rThreadFlags),             /* 0x0d threading flags */
 sizeof(rVar.rRunoutLimit),             /* 0x0e runout limit */
 sizeof(zAxis.v.stepsInch),             /* 0x0f steps per inch */
 sizeof(zAxis.v.savedLoc),              /* 0x10 saved for que op */
 sizeof(zAxis.v.homeOffset),            /* 0x11 home offset */
 sizeof(zAxis.v.loc),                   /* 0x12 location */
 sizeof(zAxis.v.dro),                   /* 0x13 dro */
 sizeof(zAxis.v.jogInc),                /* 0x14 jog increment */
 sizeof(zAxis.v.homeStatus),            /* 0x15 home status */
 sizeof(zAxis.v.homeFindFwd),           /* 0x16 max homing distance */
 sizeof(zAxis.v.homeFindRev),           /* 0x17 max rev homing distance */
 sizeof(zAxis.v.homeBackoff),           /* 0x18 home backoff dist */
 sizeof(zAxis.v.homeSlow),              /* 0x19 home backoff dist */
 sizeof(zAxis.v.testLimMin),            /* 0x1a test minimum limit */
 sizeof(zAxis.v.testLimMax),            /* 0x1b test maximum limit */
 sizeof(zAxis.v.testHomeMin),           /* 0x1c test home minimum */
 sizeof(zAxis.v.testHomeMax),           /* 0x1d test home maximum */
 sizeof(zAxis.v.testProbe),             /* 0x1e test probe position */
 sizeof(xAxis.v.stepsInch),             /* 0x1f steps per inch */
 sizeof(xAxis.v.savedLoc),              /* 0x20 saved for que op */
 sizeof(xAxis.v.homeOffset),            /* 0x21 home offset */
 sizeof(xAxis.v.loc),                   /* 0x22 location */
 sizeof(xAxis.v.dro),                   /* 0x23 dro */
 sizeof(xAxis.v.jogInc),                /* 0x24 jog increment */
 sizeof(xAxis.v.homeStatus),            /* 0x25 home status */
 sizeof(xAxis.v.homeFindFwd),           /* 0x26 max homing distance */
 sizeof(xAxis.v.homeFindRev),           /* 0x27 max rev homing distance */
 sizeof(xAxis.v.homeBackoff),           /* 0x28 home backoff dist */
 sizeof(xAxis.v.homeSlow),              /* 0x29 home backoff dist */
 sizeof(xAxis.v.testLimMin),            /* 0x2a test minimum limit */
 sizeof(xAxis.v.testLimMax),            /* 0x2b test maximum limit */
 sizeof(xAxis.v.testHomeMin),           /* 0x2c test home minimum */
 sizeof(xAxis.v.testHomeMax),           /* 0x2d test home maximum */
 sizeof(xAxis.v.testProbe),             /* 0x2e test probe position */
};

T_AXIS_VAR zAxisVar;
T_AXIS_VAR xAxisVar;

void setRiscvVar(const int parm, const T_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00 move status */
  rVar.rMvStatus = val.t_uint32_t;
  break;

 case R_JOG_PAUSE:               /*  1 0x01 jog pause */
  rVar.rJogPause = val.t_int;
  break;

 case R_CUR_PASS:                /*  2 0x02 current pass */
  rVar.rCurPass = val.t_int;
  break;

 case R_CFG_VAL:                 /*  3 0x03 fpga configuration value */
  rVar.rCfgVal = val.t_int;
  break;

 case R_P_RPM:                   /*  4 0x04 spindle rpm */
  rVar.rPRpm = val.t_int;
  break;

 case R_PWM_DIV:                 /*  5 0x05 pwm divider */
  rVar.rPwmDiv = val.t_int;
  break;

 case R_PWM_CTR:                 /*  6 0x06 pwm counter max */
  rVar.rPwmCtr = val.t_int;
  break;

 case R_SYN_ENC_PRE_SCALER:      /*  7 0x07 encoder prescaler */
  rVar.rSynEncPreScaler = val.t_int;
  break;

 case R_SYN_ENC_CYCLE:           /*  8 0x08 encoder cycle len */
  rVar.rSynEncCycle = val.t_int;
  break;

 case R_SYN_OUT_CYCLE:           /*  9 0x09 output cycle len */
  rVar.rSynOutCycle = val.t_int;
  break;

 case R_TURN_SYNC:               /* 10 0x0a turn sync */
  rVar.rTurnSync = val.t_int;
  break;

 case R_THREAD_SYNC:             /* 11 0x0b thread sync */
  rVar.rThreadSync = val.t_int;
  break;

 case R_RUNOUT_SYNC:             /* 12 0x0c runout sync */
  rVar.rRunoutSync = val.t_int;
  break;

 case R_THREAD_FLAGS:            /* 13 0x0d threading flags */
  rVar.rThreadFlags = val.t_int;
  break;

 case R_RUNOUT_LIMIT:            /* 14 0x0e runout limit */
  rVar.rRunoutLimit = val.t_int;
  break;

 case R_Z_STEPS_INCH:            /* 15 0x0f steps per inch */
  zAxis.v.stepsInch = val.t_int;
  break;

 case R_Z_SAVED_LOC:             /* 16 0x10 saved for que op */
  zAxis.v.savedLoc = val.t_int;
  break;

 case R_Z_HOME_OFFSET:           /* 17 0x11 home offset */
  zAxis.v.homeOffset = val.t_int;
  break;

 case R_Z_LOC:                   /* 18 0x12 location */
  zAxis.v.loc = val.t_int;
  break;

 case R_Z_DRO:                   /* 19 0x13 dro */
  zAxis.v.dro = val.t_int;
  break;

 case R_Z_JOG_INC:               /* 20 0x14 jog increment */
  zAxis.v.jogInc = val.t_int;
  break;

 case R_Z_HOME_STATUS:           /* 21 0x15 home status */
  zAxis.v.homeStatus = val.t_int;
  break;

 case R_Z_HOME_FIND_FWD:         /* 22 0x16 max homing distance */
  zAxis.v.homeFindFwd = val.t_int;
  break;

 case R_Z_HOME_FIND_REV:         /* 23 0x17 max rev homing distance */
  zAxis.v.homeFindRev = val.t_int;
  break;

 case R_Z_HOME_BACKOFF:          /* 24 0x18 home backoff dist */
  zAxis.v.homeBackoff = val.t_int;
  break;

 case R_Z_HOME_SLOW:             /* 25 0x19 home backoff dist */
  zAxis.v.homeSlow = val.t_int;
  break;

 case R_Z_TEST_LIM_MIN:          /* 26 0x1a test minimum limit */
  zAxis.v.testLimMin = val.t_int;
  break;

 case R_Z_TEST_LIM_MAX:          /* 27 0x1b test maximum limit */
  zAxis.v.testLimMax = val.t_int;
  break;

 case R_Z_TEST_HOME_MIN:         /* 28 0x1c test home minimum */
  zAxis.v.testHomeMin = val.t_int;
  break;

 case R_Z_TEST_HOME_MAX:         /* 29 0x1d test home maximum */
  zAxis.v.testHomeMax = val.t_int;
  break;

 case R_Z_TEST_PROBE:            /* 30 0x1e test probe position */
  zAxis.v.testProbe = val.t_int;
  break;

 case R_X_STEPS_INCH:            /* 31 0x1f steps per inch */
  xAxis.v.stepsInch = val.t_int;
  break;

 case R_X_SAVED_LOC:             /* 32 0x20 saved for que op */
  xAxis.v.savedLoc = val.t_int;
  break;

 case R_X_HOME_OFFSET:           /* 33 0x21 home offset */
  xAxis.v.homeOffset = val.t_int;
  break;

 case R_X_LOC:                   /* 34 0x22 location */
  xAxis.v.loc = val.t_int;
  break;

 case R_X_DRO:                   /* 35 0x23 dro */
  xAxis.v.dro = val.t_int;
  break;

 case R_X_JOG_INC:               /* 36 0x24 jog increment */
  xAxis.v.jogInc = val.t_int;
  break;

 case R_X_HOME_STATUS:           /* 37 0x25 home status */
  xAxis.v.homeStatus = val.t_int;
  break;

 case R_X_HOME_FIND_FWD:         /* 38 0x26 max homing distance */
  xAxis.v.homeFindFwd = val.t_int;
  break;

 case R_X_HOME_FIND_REV:         /* 39 0x27 max rev homing distance */
  xAxis.v.homeFindRev = val.t_int;
  break;

 case R_X_HOME_BACKOFF:          /* 40 0x28 home backoff dist */
  xAxis.v.homeBackoff = val.t_int;
  break;

 case R_X_HOME_SLOW:             /* 41 0x29 home backoff dist */
  xAxis.v.homeSlow = val.t_int;
  break;

 case R_X_TEST_LIM_MIN:          /* 42 0x2a test minimum limit */
  xAxis.v.testLimMin = val.t_int;
  break;

 case R_X_TEST_LIM_MAX:          /* 43 0x2b test maximum limit */
  xAxis.v.testLimMax = val.t_int;
  break;

 case R_X_TEST_HOME_MIN:         /* 44 0x2c test home minimum */
  xAxis.v.testHomeMin = val.t_int;
  break;

 case R_X_TEST_HOME_MAX:         /* 45 0x2d test home maximum */
  xAxis.v.testHomeMax = val.t_int;
  break;

 case R_X_TEST_PROBE:            /* 46 0x2e test probe position */
  xAxis.v.testProbe = val.t_int;
  break;

 }
}

void getRiscvVar(const int parm, const P_DATA_UNION val)
{
 switch(parm)
 {
 default:
  break;

 case R_MV_STATUS:               /*  0 0x00 move status */
  val->t_uint32_t = rVar.rMvStatus;
  break;

 case R_JOG_PAUSE:               /*  1 0x01 jog pause */
  val->t_int = rVar.rJogPause;
  break;

 case R_CUR_PASS:                /*  2 0x02 current pass */
  val->t_int = rVar.rCurPass;
  break;

 case R_CFG_VAL:                 /*  3 0x03 fpga configuration value */
  val->t_int = rVar.rCfgVal;
  break;

 case R_P_RPM:                   /*  4 0x04 spindle rpm */
  val->t_int = rVar.rPRpm;
  break;

 case R_PWM_DIV:                 /*  5 0x05 pwm divider */
  val->t_int = rVar.rPwmDiv;
  break;

 case R_PWM_CTR:                 /*  6 0x06 pwm counter max */
  val->t_int = rVar.rPwmCtr;
  break;

 case R_SYN_ENC_PRE_SCALER:      /*  7 0x07 encoder prescaler */
  val->t_int = rVar.rSynEncPreScaler;
  break;

 case R_SYN_ENC_CYCLE:           /*  8 0x08 encoder cycle len */
  val->t_int = rVar.rSynEncCycle;
  break;

 case R_SYN_OUT_CYCLE:           /*  9 0x09 output cycle len */
  val->t_int = rVar.rSynOutCycle;
  break;

 case R_TURN_SYNC:               /* 10 0x0a turn sync */
  val->t_int = rVar.rTurnSync;
  break;

 case R_THREAD_SYNC:             /* 11 0x0b thread sync */
  val->t_int = rVar.rThreadSync;
  break;

 case R_RUNOUT_SYNC:             /* 12 0x0c runout sync */
  val->t_int = rVar.rRunoutSync;
  break;

 case R_THREAD_FLAGS:            /* 13 0x0d threading flags */
  val->t_int = rVar.rThreadFlags;
  break;

 case R_RUNOUT_LIMIT:            /* 14 0x0e runout limit */
  val->t_int = rVar.rRunoutLimit;
  break;

 case R_Z_STEPS_INCH:            /* 15 0x0f steps per inch */
  val->t_int = zAxis.v.stepsInch;
  break;

 case R_Z_SAVED_LOC:             /* 16 0x10 saved for que op */
  val->t_int = zAxis.v.savedLoc;
  break;

 case R_Z_HOME_OFFSET:           /* 17 0x11 home offset */
  val->t_int = zAxis.v.homeOffset;
  break;

 case R_Z_LOC:                   /* 18 0x12 location */
  val->t_int = zAxis.v.loc;
  break;

 case R_Z_DRO:                   /* 19 0x13 dro */
  val->t_int = zAxis.v.dro;
  break;

 case R_Z_JOG_INC:               /* 20 0x14 jog increment */
  val->t_int = zAxis.v.jogInc;
  break;

 case R_Z_HOME_STATUS:           /* 21 0x15 home status */
  val->t_int = zAxis.v.homeStatus;
  break;

 case R_Z_HOME_FIND_FWD:         /* 22 0x16 max homing distance */
  val->t_int = zAxis.v.homeFindFwd;
  break;

 case R_Z_HOME_FIND_REV:         /* 23 0x17 max rev homing distance */
  val->t_int = zAxis.v.homeFindRev;
  break;

 case R_Z_HOME_BACKOFF:          /* 24 0x18 home backoff dist */
  val->t_int = zAxis.v.homeBackoff;
  break;

 case R_Z_HOME_SLOW:             /* 25 0x19 home backoff dist */
  val->t_int = zAxis.v.homeSlow;
  break;

 case R_Z_TEST_LIM_MIN:          /* 26 0x1a test minimum limit */
  val->t_int = zAxis.v.testLimMin;
  break;

 case R_Z_TEST_LIM_MAX:          /* 27 0x1b test maximum limit */
  val->t_int = zAxis.v.testLimMax;
  break;

 case R_Z_TEST_HOME_MIN:         /* 28 0x1c test home minimum */
  val->t_int = zAxis.v.testHomeMin;
  break;

 case R_Z_TEST_HOME_MAX:         /* 29 0x1d test home maximum */
  val->t_int = zAxis.v.testHomeMax;
  break;

 case R_Z_TEST_PROBE:            /* 30 0x1e test probe position */
  val->t_int = zAxis.v.testProbe;
  break;

 case R_X_STEPS_INCH:            /* 31 0x1f steps per inch */
  val->t_int = xAxis.v.stepsInch;
  break;

 case R_X_SAVED_LOC:             /* 32 0x20 saved for que op */
  val->t_int = xAxis.v.savedLoc;
  break;

 case R_X_HOME_OFFSET:           /* 33 0x21 home offset */
  val->t_int = xAxis.v.homeOffset;
  break;

 case R_X_LOC:                   /* 34 0x22 location */
  val->t_int = xAxis.v.loc;
  break;

 case R_X_DRO:                   /* 35 0x23 dro */
  val->t_int = xAxis.v.dro;
  break;

 case R_X_JOG_INC:               /* 36 0x24 jog increment */
  val->t_int = xAxis.v.jogInc;
  break;

 case R_X_HOME_STATUS:           /* 37 0x25 home status */
  val->t_int = xAxis.v.homeStatus;
  break;

 case R_X_HOME_FIND_FWD:         /* 38 0x26 max homing distance */
  val->t_int = xAxis.v.homeFindFwd;
  break;

 case R_X_HOME_FIND_REV:         /* 39 0x27 max rev homing distance */
  val->t_int = xAxis.v.homeFindRev;
  break;

 case R_X_HOME_BACKOFF:          /* 40 0x28 home backoff dist */
  val->t_int = xAxis.v.homeBackoff;
  break;

 case R_X_HOME_SLOW:             /* 41 0x29 home backoff dist */
  val->t_int = xAxis.v.homeSlow;
  break;

 case R_X_TEST_LIM_MIN:          /* 42 0x2a test minimum limit */
  val->t_int = xAxis.v.testLimMin;
  break;

 case R_X_TEST_LIM_MAX:          /* 43 0x2b test maximum limit */
  val->t_int = xAxis.v.testLimMax;
  break;

 case R_X_TEST_HOME_MIN:         /* 44 0x2c test home minimum */
  val->t_int = xAxis.v.testHomeMin;
  break;

 case R_X_TEST_HOME_MAX:         /* 45 0x2d test home maximum */
  val->t_int = xAxis.v.testHomeMax;
  break;

 case R_X_TEST_PROBE:            /* 46 0x2e test probe position */
  val->t_int = xAxis.v.testProbe;
  break;

 }
}

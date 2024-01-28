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
 sizeof(rVar.rSpRpm),                   /* 0x04 spindle rpm */
 sizeof(rVar.rPwmDiv),                  /* 0x05 pwm divider */
 sizeof(rVar.rPwmCtr),                  /* 0x06 pwm counter max */
 sizeof(rVar.rSynEncPreScaler),         /* 0x07 encoder prescaler */
 sizeof(rVar.rSynEncCycle),             /* 0x08 encoder cycle len */
 sizeof(rVar.rSynOutCycle),             /* 0x09 output cycle len */
 sizeof(rVar.rStepDrv),                 /* 0x0a stepper drive */
 sizeof(rVar.rEncPerRev),               /* 0x0b spindle encoder count */
 sizeof(rVar.rSpStepMult),              /* 0x0c spindle step multiplier */
 sizeof(rVar.rTurnSync),                /* 0x0d turn sync */
 sizeof(rVar.rThreadSync),              /* 0x0e thread sync */
 sizeof(rVar.rRunoutSync),              /* 0x0f runout sync */
 sizeof(rVar.rThreadFlags),             /* 0x10 threading flags */
 sizeof(rVar.rRunoutDist),              /* 0x11 runout distance */
 sizeof(rVar.rRunoutDepth),             /* 0x12 runout depth */
 sizeof(zAxis.v.stepsInch),             /* 0x13 steps per inch */
 sizeof(zAxis.v.savedLoc),              /* 0x14 saved for que op */
 sizeof(zAxis.v.homeOffset),            /* 0x15 home offset */
 sizeof(zAxis.v.loc),                   /* 0x16 location */
 sizeof(zAxis.v.dro),                   /* 0x17 dro */
 sizeof(zAxis.v.jogInc),                /* 0x18 jog increment */
 sizeof(zAxis.v.homeStatus),            /* 0x19 home status */
 sizeof(zAxis.v.homeFindFwd),           /* 0x1a max homing distance */
 sizeof(zAxis.v.homeFindRev),           /* 0x1b max rev homing distance */
 sizeof(zAxis.v.homeBackoff),           /* 0x1c home backoff dist */
 sizeof(zAxis.v.homeSlow),              /* 0x1d home backoff dist */
 sizeof(zAxis.v.testLimMin),            /* 0x1e test minimum limit */
 sizeof(zAxis.v.testLimMax),            /* 0x1f test maximum limit */
 sizeof(zAxis.v.testHomeMin),           /* 0x20 test home minimum */
 sizeof(zAxis.v.testHomeMax),           /* 0x21 test home maximum */
 sizeof(zAxis.v.testProbe),             /* 0x22 test probe position */
 sizeof(xAxis.v.stepsInch),             /* 0x23 steps per inch */
 sizeof(xAxis.v.savedLoc),              /* 0x24 saved for que op */
 sizeof(xAxis.v.homeOffset),            /* 0x25 home offset */
 sizeof(xAxis.v.loc),                   /* 0x26 location */
 sizeof(xAxis.v.dro),                   /* 0x27 dro */
 sizeof(xAxis.v.jogInc),                /* 0x28 jog increment */
 sizeof(xAxis.v.homeStatus),            /* 0x29 home status */
 sizeof(xAxis.v.homeFindFwd),           /* 0x2a max homing distance */
 sizeof(xAxis.v.homeFindRev),           /* 0x2b max rev homing distance */
 sizeof(xAxis.v.homeBackoff),           /* 0x2c home backoff dist */
 sizeof(xAxis.v.homeSlow),              /* 0x2d home backoff dist */
 sizeof(xAxis.v.testLimMin),            /* 0x2e test minimum limit */
 sizeof(xAxis.v.testLimMax),            /* 0x2f test maximum limit */
 sizeof(xAxis.v.testHomeMin),           /* 0x30 test home minimum */
 sizeof(xAxis.v.testHomeMax),           /* 0x31 test home maximum */
 sizeof(xAxis.v.testProbe),             /* 0x32 test probe position */
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

 case R_SP_RPM:                  /*  4 0x04 spindle rpm */
  rVar.rSpRpm = val.t_int;
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

 case R_STEP_DRV:                /* 10 0x0a stepper drive */
  rVar.rStepDrv = val.t_int;
  break;

 case R_ENC_PER_REV:             /* 11 0x0b spindle encoder count */
  rVar.rEncPerRev = val.t_int;
  break;

 case R_SP_STEP_MULT:            /* 12 0x0c spindle step multiplier */
  rVar.rSpStepMult = val.t_int;
  break;

 case R_TURN_SYNC:               /* 13 0x0d turn sync */
  rVar.rTurnSync = val.t_int;
  break;

 case R_THREAD_SYNC:             /* 14 0x0e thread sync */
  rVar.rThreadSync = val.t_int;
  break;

 case R_RUNOUT_SYNC:             /* 15 0x0f runout sync */
  rVar.rRunoutSync = val.t_int;
  break;

 case R_THREAD_FLAGS:            /* 16 0x10 threading flags */
  rVar.rThreadFlags = val.t_int;
  break;

 case R_RUNOUT_DIST:             /* 17 0x11 runout distance */
  rVar.rRunoutDist = val.t_int;
  break;

 case R_RUNOUT_DEPTH:            /* 18 0x12 runout depth */
  rVar.rRunoutDepth = val.t_int;
  break;

 case R_Z_STEPS_INCH:            /* 19 0x13 steps per inch */
  zAxis.v.stepsInch = val.t_int;
  break;

 case R_Z_SAVED_LOC:             /* 20 0x14 saved for que op */
  zAxis.v.savedLoc = val.t_int;
  break;

 case R_Z_HOME_OFFSET:           /* 21 0x15 home offset */
  zAxis.v.homeOffset = val.t_int;
  break;

 case R_Z_LOC:                   /* 22 0x16 location */
  zAxis.v.loc = val.t_int;
  break;

 case R_Z_DRO:                   /* 23 0x17 dro */
  zAxis.v.dro = val.t_int;
  break;

 case R_Z_JOG_INC:               /* 24 0x18 jog increment */
  zAxis.v.jogInc = val.t_int;
  break;

 case R_Z_HOME_STATUS:           /* 25 0x19 home status */
  zAxis.v.homeStatus = val.t_int;
  break;

 case R_Z_HOME_FIND_FWD:         /* 26 0x1a max homing distance */
  zAxis.v.homeFindFwd = val.t_int;
  break;

 case R_Z_HOME_FIND_REV:         /* 27 0x1b max rev homing distance */
  zAxis.v.homeFindRev = val.t_int;
  break;

 case R_Z_HOME_BACKOFF:          /* 28 0x1c home backoff dist */
  zAxis.v.homeBackoff = val.t_int;
  break;

 case R_Z_HOME_SLOW:             /* 29 0x1d home backoff dist */
  zAxis.v.homeSlow = val.t_int;
  break;

 case R_Z_TEST_LIM_MIN:          /* 30 0x1e test minimum limit */
  zAxis.v.testLimMin = val.t_int;
  break;

 case R_Z_TEST_LIM_MAX:          /* 31 0x1f test maximum limit */
  zAxis.v.testLimMax = val.t_int;
  break;

 case R_Z_TEST_HOME_MIN:         /* 32 0x20 test home minimum */
  zAxis.v.testHomeMin = val.t_int;
  break;

 case R_Z_TEST_HOME_MAX:         /* 33 0x21 test home maximum */
  zAxis.v.testHomeMax = val.t_int;
  break;

 case R_Z_TEST_PROBE:            /* 34 0x22 test probe position */
  zAxis.v.testProbe = val.t_int;
  break;

 case R_X_STEPS_INCH:            /* 35 0x23 steps per inch */
  xAxis.v.stepsInch = val.t_int;
  break;

 case R_X_SAVED_LOC:             /* 36 0x24 saved for que op */
  xAxis.v.savedLoc = val.t_int;
  break;

 case R_X_HOME_OFFSET:           /* 37 0x25 home offset */
  xAxis.v.homeOffset = val.t_int;
  break;

 case R_X_LOC:                   /* 38 0x26 location */
  xAxis.v.loc = val.t_int;
  break;

 case R_X_DRO:                   /* 39 0x27 dro */
  xAxis.v.dro = val.t_int;
  break;

 case R_X_JOG_INC:               /* 40 0x28 jog increment */
  xAxis.v.jogInc = val.t_int;
  break;

 case R_X_HOME_STATUS:           /* 41 0x29 home status */
  xAxis.v.homeStatus = val.t_int;
  break;

 case R_X_HOME_FIND_FWD:         /* 42 0x2a max homing distance */
  xAxis.v.homeFindFwd = val.t_int;
  break;

 case R_X_HOME_FIND_REV:         /* 43 0x2b max rev homing distance */
  xAxis.v.homeFindRev = val.t_int;
  break;

 case R_X_HOME_BACKOFF:          /* 44 0x2c home backoff dist */
  xAxis.v.homeBackoff = val.t_int;
  break;

 case R_X_HOME_SLOW:             /* 45 0x2d home backoff dist */
  xAxis.v.homeSlow = val.t_int;
  break;

 case R_X_TEST_LIM_MIN:          /* 46 0x2e test minimum limit */
  xAxis.v.testLimMin = val.t_int;
  break;

 case R_X_TEST_LIM_MAX:          /* 47 0x2f test maximum limit */
  xAxis.v.testLimMax = val.t_int;
  break;

 case R_X_TEST_HOME_MIN:         /* 48 0x30 test home minimum */
  xAxis.v.testHomeMin = val.t_int;
  break;

 case R_X_TEST_HOME_MAX:         /* 49 0x31 test home maximum */
  xAxis.v.testHomeMax = val.t_int;
  break;

 case R_X_TEST_PROBE:            /* 50 0x32 test probe position */
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

 case R_SP_RPM:                  /*  4 0x04 spindle rpm */
  val->t_int = rVar.rSpRpm;
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

 case R_STEP_DRV:                /* 10 0x0a stepper drive */
  val->t_int = rVar.rStepDrv;
  break;

 case R_ENC_PER_REV:             /* 11 0x0b spindle encoder count */
  val->t_int = rVar.rEncPerRev;
  break;

 case R_SP_STEP_MULT:            /* 12 0x0c spindle step multiplier */
  val->t_int = rVar.rSpStepMult;
  break;

 case R_TURN_SYNC:               /* 13 0x0d turn sync */
  val->t_int = rVar.rTurnSync;
  break;

 case R_THREAD_SYNC:             /* 14 0x0e thread sync */
  val->t_int = rVar.rThreadSync;
  break;

 case R_RUNOUT_SYNC:             /* 15 0x0f runout sync */
  val->t_int = rVar.rRunoutSync;
  break;

 case R_THREAD_FLAGS:            /* 16 0x10 threading flags */
  val->t_int = rVar.rThreadFlags;
  break;

 case R_RUNOUT_DIST:             /* 17 0x11 runout distance */
  val->t_int = rVar.rRunoutDist;
  break;

 case R_RUNOUT_DEPTH:            /* 18 0x12 runout depth */
  val->t_int = rVar.rRunoutDepth;
  break;

 case R_Z_STEPS_INCH:            /* 19 0x13 steps per inch */
  val->t_int = zAxis.v.stepsInch;
  break;

 case R_Z_SAVED_LOC:             /* 20 0x14 saved for que op */
  val->t_int = zAxis.v.savedLoc;
  break;

 case R_Z_HOME_OFFSET:           /* 21 0x15 home offset */
  val->t_int = zAxis.v.homeOffset;
  break;

 case R_Z_LOC:                   /* 22 0x16 location */
  val->t_int = zAxis.v.loc;
  break;

 case R_Z_DRO:                   /* 23 0x17 dro */
  val->t_int = zAxis.v.dro;
  break;

 case R_Z_JOG_INC:               /* 24 0x18 jog increment */
  val->t_int = zAxis.v.jogInc;
  break;

 case R_Z_HOME_STATUS:           /* 25 0x19 home status */
  val->t_int = zAxis.v.homeStatus;
  break;

 case R_Z_HOME_FIND_FWD:         /* 26 0x1a max homing distance */
  val->t_int = zAxis.v.homeFindFwd;
  break;

 case R_Z_HOME_FIND_REV:         /* 27 0x1b max rev homing distance */
  val->t_int = zAxis.v.homeFindRev;
  break;

 case R_Z_HOME_BACKOFF:          /* 28 0x1c home backoff dist */
  val->t_int = zAxis.v.homeBackoff;
  break;

 case R_Z_HOME_SLOW:             /* 29 0x1d home backoff dist */
  val->t_int = zAxis.v.homeSlow;
  break;

 case R_Z_TEST_LIM_MIN:          /* 30 0x1e test minimum limit */
  val->t_int = zAxis.v.testLimMin;
  break;

 case R_Z_TEST_LIM_MAX:          /* 31 0x1f test maximum limit */
  val->t_int = zAxis.v.testLimMax;
  break;

 case R_Z_TEST_HOME_MIN:         /* 32 0x20 test home minimum */
  val->t_int = zAxis.v.testHomeMin;
  break;

 case R_Z_TEST_HOME_MAX:         /* 33 0x21 test home maximum */
  val->t_int = zAxis.v.testHomeMax;
  break;

 case R_Z_TEST_PROBE:            /* 34 0x22 test probe position */
  val->t_int = zAxis.v.testProbe;
  break;

 case R_X_STEPS_INCH:            /* 35 0x23 steps per inch */
  val->t_int = xAxis.v.stepsInch;
  break;

 case R_X_SAVED_LOC:             /* 36 0x24 saved for que op */
  val->t_int = xAxis.v.savedLoc;
  break;

 case R_X_HOME_OFFSET:           /* 37 0x25 home offset */
  val->t_int = xAxis.v.homeOffset;
  break;

 case R_X_LOC:                   /* 38 0x26 location */
  val->t_int = xAxis.v.loc;
  break;

 case R_X_DRO:                   /* 39 0x27 dro */
  val->t_int = xAxis.v.dro;
  break;

 case R_X_JOG_INC:               /* 40 0x28 jog increment */
  val->t_int = xAxis.v.jogInc;
  break;

 case R_X_HOME_STATUS:           /* 41 0x29 home status */
  val->t_int = xAxis.v.homeStatus;
  break;

 case R_X_HOME_FIND_FWD:         /* 42 0x2a max homing distance */
  val->t_int = xAxis.v.homeFindFwd;
  break;

 case R_X_HOME_FIND_REV:         /* 43 0x2b max rev homing distance */
  val->t_int = xAxis.v.homeFindRev;
  break;

 case R_X_HOME_BACKOFF:          /* 44 0x2c home backoff dist */
  val->t_int = xAxis.v.homeBackoff;
  break;

 case R_X_HOME_SLOW:             /* 45 0x2d home backoff dist */
  val->t_int = xAxis.v.homeSlow;
  break;

 case R_X_TEST_LIM_MIN:          /* 46 0x2e test minimum limit */
  val->t_int = xAxis.v.testLimMin;
  break;

 case R_X_TEST_LIM_MAX:          /* 47 0x2f test maximum limit */
  val->t_int = xAxis.v.testLimMax;
  break;

 case R_X_TEST_HOME_MIN:         /* 48 0x30 test home minimum */
  val->t_int = xAxis.v.testHomeMin;
  break;

 case R_X_TEST_HOME_MAX:         /* 49 0x31 test home maximum */
  val->t_int = xAxis.v.testHomeMax;
  break;

 case R_X_TEST_PROBE:            /* 50 0x32 test probe position */
  val->t_int = xAxis.v.testProbe;
  break;

 }
}

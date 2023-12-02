// c2File

#include <stdint.h>
#define NO_REM_MACROS
#include "riscvParm.h"
#include "riscvStruct.h"
#if !defined(EXT)
#define EXT extern
#endif
#include "axisCtl.h"

T_RISCV_VAR rVar;

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
 sizeof(rVar.rPwmCtr),                  /* 0x06 pwm counter maxy */
 sizeof(zAxis.v.stepsInch),             /* 0x07 steps per inch */
 sizeof(zAxis.v.savedLoc),              /* 0x08 saved for que op */
 sizeof(zAxis.v.homeOffset),            /* 0x09 home offset */
 sizeof(zAxis.v.loc),                   /* 0x0a location */
 sizeof(zAxis.v.dro),                   /* 0x0b dro */
 sizeof(zAxis.v.jogInc),                /* 0x0c jog increment */
 sizeof(zAxis.v.homeStatus),            /* 0x0d home status */
 sizeof(zAxis.v.homeFindFwd),           /* 0x0e max homing distance */
 sizeof(zAxis.v.homeFindRev),           /* 0x0f max rev homing distance */
 sizeof(zAxis.v.homeBackoff),           /* 0x10 home backoff dist */
 sizeof(zAxis.v.homeSlow),              /* 0x11 home backoff dist */
 sizeof(zAxis.v.testLimMin),            /* 0x12 test minimum limit */
 sizeof(zAxis.v.testLimMax),            /* 0x13 test maximum limit */
 sizeof(zAxis.v.testHomeMin),           /* 0x14 test home minimum */
 sizeof(zAxis.v.testHomeMax),           /* 0x15 test home maximum */
 sizeof(zAxis.v.testProbe),             /* 0x16 test probe position */
 sizeof(xAxis.v.stepsInch),             /* 0x17 steps per inch */
 sizeof(xAxis.v.savedLoc),              /* 0x18 saved for que op */
 sizeof(xAxis.v.homeOffset),            /* 0x19 home offset */
 sizeof(xAxis.v.loc),                   /* 0x1a location */
 sizeof(xAxis.v.dro),                   /* 0x1b dro */
 sizeof(xAxis.v.jogInc),                /* 0x1c jog increment */
 sizeof(xAxis.v.homeStatus),            /* 0x1d home status */
 sizeof(xAxis.v.homeFindFwd),           /* 0x1e max homing distance */
 sizeof(xAxis.v.homeFindRev),           /* 0x1f max rev homing distance */
 sizeof(xAxis.v.homeBackoff),           /* 0x20 home backoff dist */
 sizeof(xAxis.v.homeSlow),              /* 0x21 home backoff dist */
 sizeof(xAxis.v.testLimMin),            /* 0x22 test minimum limit */
 sizeof(xAxis.v.testLimMax),            /* 0x23 test maximum limit */
 sizeof(xAxis.v.testHomeMin),           /* 0x24 test home minimum */
 sizeof(xAxis.v.testHomeMax),           /* 0x25 test home maximum */
 sizeof(xAxis.v.testProbe),             /* 0x26 test probe position */
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

 case R_PWM_CTR:                 /*  6 0x06 pwm counter maxy */
  rVar.rPwmCtr = val.t_int;
  break;

 case R_Z_STEPS_INCH:            /*  7 0x07 steps per inch */
  zAxis.v.stepsInch = val.t_int;
  break;

 case R_Z_SAVED_LOC:             /*  8 0x08 saved for que op */
  zAxis.v.savedLoc = val.t_int;
  break;

 case R_Z_HOME_OFFSET:           /*  9 0x09 home offset */
  zAxis.v.homeOffset = val.t_int;
  break;

 case R_Z_LOC:                   /* 10 0x0a location */
  zAxis.v.loc = val.t_int;
  break;

 case R_Z_DRO:                   /* 11 0x0b dro */
  zAxis.v.dro = val.t_int;
  break;

 case R_Z_JOG_INC:               /* 12 0x0c jog increment */
  zAxis.v.jogInc = val.t_int;
  break;

 case R_Z_HOME_STATUS:           /* 13 0x0d home status */
  zAxis.v.homeStatus = val.t_int;
  break;

 case R_Z_HOME_FIND_FWD:         /* 14 0x0e max homing distance */
  zAxis.v.homeFindFwd = val.t_int;
  break;

 case R_Z_HOME_FIND_REV:         /* 15 0x0f max rev homing distance */
  zAxis.v.homeFindRev = val.t_int;
  break;

 case R_Z_HOME_BACKOFF:          /* 16 0x10 home backoff dist */
  zAxis.v.homeBackoff = val.t_int;
  break;

 case R_Z_HOME_SLOW:             /* 17 0x11 home backoff dist */
  zAxis.v.homeSlow = val.t_int;
  break;

 case R_Z_TEST_LIM_MIN:          /* 18 0x12 test minimum limit */
  zAxis.v.testLimMin = val.t_int;
  break;

 case R_Z_TEST_LIM_MAX:          /* 19 0x13 test maximum limit */
  zAxis.v.testLimMax = val.t_int;
  break;

 case R_Z_TEST_HOME_MIN:         /* 20 0x14 test home minimum */
  zAxis.v.testHomeMin = val.t_int;
  break;

 case R_Z_TEST_HOME_MAX:         /* 21 0x15 test home maximum */
  zAxis.v.testHomeMax = val.t_int;
  break;

 case R_Z_TEST_PROBE:            /* 22 0x16 test probe position */
  zAxis.v.testProbe = val.t_int;
  break;

 case R_X_STEPS_INCH:            /* 23 0x17 steps per inch */
  xAxis.v.stepsInch = val.t_int;
  break;

 case R_X_SAVED_LOC:             /* 24 0x18 saved for que op */
  xAxis.v.savedLoc = val.t_int;
  break;

 case R_X_HOME_OFFSET:           /* 25 0x19 home offset */
  xAxis.v.homeOffset = val.t_int;
  break;

 case R_X_LOC:                   /* 26 0x1a location */
  xAxis.v.loc = val.t_int;
  break;

 case R_X_DRO:                   /* 27 0x1b dro */
  xAxis.v.dro = val.t_int;
  break;

 case R_X_JOG_INC:               /* 28 0x1c jog increment */
  xAxis.v.jogInc = val.t_int;
  break;

 case R_X_HOME_STATUS:           /* 29 0x1d home status */
  xAxis.v.homeStatus = val.t_int;
  break;

 case R_X_HOME_FIND_FWD:         /* 30 0x1e max homing distance */
  xAxis.v.homeFindFwd = val.t_int;
  break;

 case R_X_HOME_FIND_REV:         /* 31 0x1f max rev homing distance */
  xAxis.v.homeFindRev = val.t_int;
  break;

 case R_X_HOME_BACKOFF:          /* 32 0x20 home backoff dist */
  xAxis.v.homeBackoff = val.t_int;
  break;

 case R_X_HOME_SLOW:             /* 33 0x21 home backoff dist */
  xAxis.v.homeSlow = val.t_int;
  break;

 case R_X_TEST_LIM_MIN:          /* 34 0x22 test minimum limit */
  xAxis.v.testLimMin = val.t_int;
  break;

 case R_X_TEST_LIM_MAX:          /* 35 0x23 test maximum limit */
  xAxis.v.testLimMax = val.t_int;
  break;

 case R_X_TEST_HOME_MIN:         /* 36 0x24 test home minimum */
  xAxis.v.testHomeMin = val.t_int;
  break;

 case R_X_TEST_HOME_MAX:         /* 37 0x25 test home maximum */
  xAxis.v.testHomeMax = val.t_int;
  break;

 case R_X_TEST_PROBE:            /* 38 0x26 test probe position */
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

 case R_PWM_CTR:                 /*  6 0x06 pwm counter maxy */
  val->t_int = rVar.rPwmCtr;
  break;

 case R_Z_STEPS_INCH:            /*  7 0x07 steps per inch */
  val->t_int = zAxis.v.stepsInch;
  break;

 case R_Z_SAVED_LOC:             /*  8 0x08 saved for que op */
  val->t_int = zAxis.v.savedLoc;
  break;

 case R_Z_HOME_OFFSET:           /*  9 0x09 home offset */
  val->t_int = zAxis.v.homeOffset;
  break;

 case R_Z_LOC:                   /* 10 0x0a location */
  val->t_int = zAxis.v.loc;
  break;

 case R_Z_DRO:                   /* 11 0x0b dro */
  val->t_int = zAxis.v.dro;
  break;

 case R_Z_JOG_INC:               /* 12 0x0c jog increment */
  val->t_int = zAxis.v.jogInc;
  break;

 case R_Z_HOME_STATUS:           /* 13 0x0d home status */
  val->t_int = zAxis.v.homeStatus;
  break;

 case R_Z_HOME_FIND_FWD:         /* 14 0x0e max homing distance */
  val->t_int = zAxis.v.homeFindFwd;
  break;

 case R_Z_HOME_FIND_REV:         /* 15 0x0f max rev homing distance */
  val->t_int = zAxis.v.homeFindRev;
  break;

 case R_Z_HOME_BACKOFF:          /* 16 0x10 home backoff dist */
  val->t_int = zAxis.v.homeBackoff;
  break;

 case R_Z_HOME_SLOW:             /* 17 0x11 home backoff dist */
  val->t_int = zAxis.v.homeSlow;
  break;

 case R_Z_TEST_LIM_MIN:          /* 18 0x12 test minimum limit */
  val->t_int = zAxis.v.testLimMin;
  break;

 case R_Z_TEST_LIM_MAX:          /* 19 0x13 test maximum limit */
  val->t_int = zAxis.v.testLimMax;
  break;

 case R_Z_TEST_HOME_MIN:         /* 20 0x14 test home minimum */
  val->t_int = zAxis.v.testHomeMin;
  break;

 case R_Z_TEST_HOME_MAX:         /* 21 0x15 test home maximum */
  val->t_int = zAxis.v.testHomeMax;
  break;

 case R_Z_TEST_PROBE:            /* 22 0x16 test probe position */
  val->t_int = zAxis.v.testProbe;
  break;

 case R_X_STEPS_INCH:            /* 23 0x17 steps per inch */
  val->t_int = xAxis.v.stepsInch;
  break;

 case R_X_SAVED_LOC:             /* 24 0x18 saved for que op */
  val->t_int = xAxis.v.savedLoc;
  break;

 case R_X_HOME_OFFSET:           /* 25 0x19 home offset */
  val->t_int = xAxis.v.homeOffset;
  break;

 case R_X_LOC:                   /* 26 0x1a location */
  val->t_int = xAxis.v.loc;
  break;

 case R_X_DRO:                   /* 27 0x1b dro */
  val->t_int = xAxis.v.dro;
  break;

 case R_X_JOG_INC:               /* 28 0x1c jog increment */
  val->t_int = xAxis.v.jogInc;
  break;

 case R_X_HOME_STATUS:           /* 29 0x1d home status */
  val->t_int = xAxis.v.homeStatus;
  break;

 case R_X_HOME_FIND_FWD:         /* 30 0x1e max homing distance */
  val->t_int = xAxis.v.homeFindFwd;
  break;

 case R_X_HOME_FIND_REV:         /* 31 0x1f max rev homing distance */
  val->t_int = xAxis.v.homeFindRev;
  break;

 case R_X_HOME_BACKOFF:          /* 32 0x20 home backoff dist */
  val->t_int = xAxis.v.homeBackoff;
  break;

 case R_X_HOME_SLOW:             /* 33 0x21 home backoff dist */
  val->t_int = xAxis.v.homeSlow;
  break;

 case R_X_TEST_LIM_MIN:          /* 34 0x22 test minimum limit */
  val->t_int = xAxis.v.testLimMin;
  break;

 case R_X_TEST_LIM_MAX:          /* 35 0x23 test maximum limit */
  val->t_int = xAxis.v.testLimMax;
  break;

 case R_X_TEST_HOME_MIN:         /* 36 0x24 test home minimum */
  val->t_int = xAxis.v.testHomeMin;
  break;

 case R_X_TEST_HOME_MAX:         /* 37 0x25 test home maximum */
  val->t_int = xAxis.v.testHomeMax;
  break;

 case R_X_TEST_PROBE:            /* 38 0x26 test probe position */
  val->t_int = xAxis.v.testProbe;
  break;

 }
}

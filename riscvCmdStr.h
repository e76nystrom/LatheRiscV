#if !defined(RISCV_CMD_STR_INC)
#define RISCV_CMD_STR_INC

struct S_RISCV_CMD_STR
{
 char c0;
 char c1;
};

struct S_RISCV_CMD_STR riscvCmdStr[] =
{
 {'N', 'O',},            /*  0  0  no operation */
 {'R', 'A',},            /*  1  1  read all status */
 {'R', 'D',},            /*  2  2  read debug info */
 {'S', 'U',},            /*  3  3  setup */
 {'R', 'E',},            /*  4  4  resume */
 {'S', 'P',},            /*  5  5  stop */
 {'S', 'X',},            /*  6  6  stop x */
 {'S', 'Z',},            /*  7  7  stop z */
 {'D', 'N',},            /*  8  8  done */
 {'N', 'D',},            /*  9  9  send data done */
 {'S', '+',},            /*  a 10  start spindle */
 {'S', '-',},            /*  b 11  stop spindle */
 {'U', 'S',},            /*  c 12  update spindle speed */
 {'L', 'X',},            /*  d 13  set x loc */
 {'L', 'Z',},            /*  e 14  set z loc */
 {'S', 'A',},            /*  f 15  set accel parm */
 {'S', 'D',},            /* 10 16  set data */
 {'G', 'D',},            /* 11 17  set data */
 {'J', 'Z',},            /* 12 18  jog move z */
 {'J', 'X',},            /* 13 19  jog move x */
 {'H', 'Z',},            /* 14 20  home z */
 {'H', 'X',},            /* 15 21  home x */
 {'O', 'S',},            /* 16 22  start first queued command */
 {'O', 'D',},            /* 17 23  done */
 {'P', 'A',},            /* 18 24  pause */
 {'E', 'S',},            /* 19 25  encoder scale start */
 {'Q', '+',},            /* 1a 26  start spindle */
 {'Q', '-',},            /* 1b 27  stop spindle */
 {'P', 'S',},            /* 1c 28  pass */
 {'S', 'Q',},            /* 1d 29  set accel parm queued */
 {'S', 'D',},            /* 1e 30  set data */
 {'M', 'Z',},            /* 1f 31  move z */
 {'M', 'X',},            /* 20 32  move x */
 {'R', 'Z',},            /* 21 33  move rel z */
 {'R', 'X',},            /* 22 34  move rel x */
};

#define RISCV_CMD_STR_SIZE 35

#endif  /* RISCV_CMD_STR_INC */

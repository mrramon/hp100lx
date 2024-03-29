/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SRLPORT.C                           TESTED on TC 2.0 OK
   Author:  Harry Konstas
   Started: 11/24/1994
   Subject: Sets the serial port (RS232) on/off
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */

#include <dos.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   SerialPort:
   Turns the serial port on or off (SERIAL_ON, SERIAL_OFF)
   -------------------------------------------------------------------- */

void SetSerialPort(int Mode)
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.al = Mode;
   regs.h.ah = 0x4a;
   int86x(0x15, &regs, &regs, &segregs);
}


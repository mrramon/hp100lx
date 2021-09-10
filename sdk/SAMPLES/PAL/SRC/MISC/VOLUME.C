/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  VOLUME.C                           TESTED on TC 2.0 OK
   Author:  Harry Konstas
   Started: 11/20/1994
   Subject: Sets the volume of the internal speaker
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
   Volume:
   Sets the volume level of the built-in speaker
   available definitions: (VOL_OFF, VOL_LOW, VOL_MEDIUM, VOL_HI)
   -------------------------------------------------------------------- */

void Volume(int Level)
{
    union REGS regs;
    struct SREGS segregs;

    regs.h.al = Level;
    regs.h.ah = 0x48;
    int86x(0x15, &regs, &regs, &segregs);
}


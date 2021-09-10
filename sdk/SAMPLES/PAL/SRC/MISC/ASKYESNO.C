/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  ASKYESNO.C
   Author:  Gilles Kohl
   Started: 10. Apr. 94
   Subject: Ask the user yes or no
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>


/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

int AskYesNo(char *Question)
{
   int r;

   r = MsgBox(Question, " ", NULL, " No  | Yes ");
   return r == 1 || r == DN_OK;
}


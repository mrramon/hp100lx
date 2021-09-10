/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LABELOUT.C
   Author:  Gilles Kohl
   Started: 17.06.95
   Subject: Output label string - same as TextOut, with provision for
            special '&' handling
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   LabelOut:
   Will return the width of a menu or dialog label given a string
   and a font.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
void LabelOut(int x, int y, int Style, int FontId, char *Lbl)
{
   int w;
   int Pos = -1;
   int lx;
   int y2;
   char buf[80];
   char *s;

   y2 = y+FontHeight(FontId);
   strcpy(buf, Lbl);
   if(s = strchr(buf, '&')) {
      Pos = s - buf;
      *s = '\0'; lx = TextExt(FontId, buf); *s = '&';
      strcpy(buf+Pos, Lbl+Pos+1);
   }
   TextOut(x, y, Style, FontId, buf);
   if(Pos >= 0) {
      Line(x+lx, y2, x+lx+TextExt(FontId, "&")-1, y2);
   }
}


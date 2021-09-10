/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  FIRSTNXT.C
   Author:  GK
   Started: 12/03/94
   Subject: Get first and subsequent entries from a CFG section
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* ----------------------------- GetFirstEntry ----------------------------- */
/* Used to search entire sections. Initiates search.                         */

char *GetFirstEntry(char *Section, char **pKey)
{
   CONFIGENTRY *pCfg;

   for(pCfg = CfgList; pCfg; pCfg = pCfg->Next) {
      if(strcmp(Section, pCfg->Section) == 0) {
         TmpEntry   = pCfg;
         TmpSection = Section;
         if(pKey) *pKey = pCfg->Key;
         return pCfg->Value;
      }
   }
   TmpEntry   = NULL;
   TmpSection = NULL;
   return NULL;
}

/* ----------------------------- GetNextEntry ----------------------------- */
/* Used to search entire sections. Continues search.                        */

char *GetNextEntry(char **pKey)
{
   if(TmpEntry == NULL || TmpSection == NULL ||
      TmpEntry->Next == NULL   ||
      strcmp(TmpEntry->Next->Section, TmpSection) != 0) {
      TmpEntry   = NULL;
      TmpSection = NULL;
      return NULL;
   }
   TmpEntry = TmpEntry->Next;
   if(pKey) *pKey = TmpEntry->Key;
   return TmpEntry->Value;
}

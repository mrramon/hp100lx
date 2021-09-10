/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DIALOG.C
   Author:  Gilles Kohl
   Started: 21. Jan 95
   Subject: Toplevel PAL dialog function entry points
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

int InitDialog(DIALOG *pDlg)
{
   return (pDlg->DlgProc)(DM_INIT, 0, NULL, pDlg);
}

int SetDlgItem(DIALOG *pDlg, int ItmNbr, int n, void *pData)
{
   return (pDlg->Items[ItmNbr].ItmProc)(IM_SETDATA, n, pData,
                                        pDlg, &pDlg->Items[ItmNbr]);
}

int ShowDialog(DIALOG *pDlg, int x, int y, char *Caption)
{
   int retval;

   if(!PalStateSaved++) SaveState(&PalState);
   if(!pDlg->pWin) {
      pDlg->pWin = OpenWin(WS_HP200, x, y,
                           x+pDlg->Width, y+pDlg->Depth, Caption);
   }
   retval = (pDlg->DlgProc)(DM_SHOW, 0, NULL, pDlg);

   if(!--PalStateSaved) RestoreState(&PalState);
   return retval;
}

int SetFocus(DIALOG *pDlg, int ItmNbr)
{
   return (pDlg->DlgProc)(DM_SETFOCUS, ItmNbr, NULL, pDlg);
}

int HandleDialog(DIALOG *pDlg, int *pKey)
{
   int Key;
   int RetVal;

   do {
      Key = GetKey();
      RetVal = (pDlg->DlgProc)(DM_KEY, Key, NULL, pDlg);
   } while(RetVal < DN_CANCEL && !pKey);
   if(pKey) *pKey = Key;

   return RetVal;
}

int GetDlgItem(DIALOG *pDlg, int ItemNbr, int n, void *Dest)
{
   return (pDlg->Items[ItemNbr].ItmProc)(IM_GETDATA, n, Dest,
                                         pDlg, &pDlg->Items[ItemNbr]);
}

int CloseDialog(DIALOG *pDlg)
{
   int v;

   v = (pDlg->DlgProc)(DM_KILL, 0, NULL, pDlg);
   CloseWin(pDlg->pWin);
   pDlg->pWin = NULL;
   return v;
}


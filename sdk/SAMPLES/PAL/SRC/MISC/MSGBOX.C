/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MSGBOX.C
   Author:  Gilles Kohl
   Started: 10. Apr. 94
   Subject: Display a message box dialog
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                            constants
   -------------------------------------------------------------------- */
#define TTLDIST 4
#define INPDIST 6
#define LINDIST 4
#define BTNDIST 8
#define XOFF    4
#define YOFF    4
#define BTNX    2
#define BTNY    2
#define BTNOFF  3

/* --------------------------------------------------------------------
                            globals
   -------------------------------------------------------------------- */
static DLGITEM MsgItm[] = {
/*X  Y  W  D   FLAGS LABEL  INIT  TYPE      SIZE              PRIV */
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhEdit,   sizeof(IHEDIT),   NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
};

static DIALOG MsgDlg = {
   0, 0, 0L, 0, 0, &MsgItm[1], DhStandard, NULL, NULL, 0
};

static int StrSplit(char **pPtr, char *Txt, int Max)
{
   char *s;
   int i;

   if(Txt == NULL || strlen(Txt) == 0) return 0;

   i = 0;
   s = Txt;
   do {
      if(i < Max) pPtr[i++] = s;
      s = strchr(s, '|');
      if(s) *s++ = '\0';
   } while(s);

   return i;
}

int MsgBox(char *Title, char *Fmt, char *Inp, char *Btns, ...)
{
   va_list ArgPtr;        /* used for variable paramater list   */
   char *LinPtr[10];      /* array of pointers to line starts   */
   char *BtnPtr[5];       /* array of pointers to button labels */
   char *Contents = NULL; /* pointer to contents line */
   char *Prompt   = NULL; /* pointer to prompt line */
   int LinCnt;            /* number of text lines */
   int BtnCnt;            /* number of buttons */
   int Focus  =-1;        /* button that gets the focus by default */
   int BtnSum = 0;        /* accumulated button width */
   int MaxLin = 0;        /* width of largest line */
   int MaxBtn = 0;        /* width of largest button */
   int i, x, y, w, d;     /* auxiliary variables */
   int fh = FontHeight(MEDIUM_FONT);
   char *Msg = NULL;

   va_start(ArgPtr, Btns);

   /* duplicate/allocate strings since StrSplit modifies them */
   if(Fmt)  Msg  = malloc(1024);
   if(Btns) Btns = strdup(Btns);

   if(Msg) vsprintf(Msg, Fmt, ArgPtr);

   /* Insert terminators, compute number of lines and buttons */
   LinCnt = StrSplit(LinPtr, Msg,  ELMS(LinPtr));
   BtnCnt = StrSplit(BtnPtr, Btns, ELMS(BtnPtr));

   /* if input line requested, find contents and prompt */
   if(Inp) {
      if(LinCnt) Contents = LinPtr[--LinCnt];
      if(LinCnt) Prompt   = LinPtr[--LinCnt];
   }

   /* find largest line */
   for(i = 0; i < LinCnt; i++) {
      int n = TextExt(MEDIUM_FONT, LinPtr[i]);
      if(n > MaxLin) MaxLin = n;
   }

   /* if we have an input field, adjust largest line */
   if(Inp) MaxLin = max(MaxLin,
                       (Contents ? TextExt(MEDIUM_FONT, Contents) : 0) +
                       (Prompt   ? TextExt(MEDIUM_FONT, Prompt)   : 0) +
                        48);

   if(BtnCnt) Focus = 0;
   /* find largest button, compute total button width */
   for(i = 0; i < BtnCnt; i++) {
      int n;
      if(*BtnPtr[i] == '!') {
         ++BtnPtr[i];
         Focus = i;
      }
      n = TextExt(MEDIUM_FONT, BtnPtr[i]);
      BtnSum += n+2*BTNX+BTNOFF;
      if(n > MaxBtn) MaxBtn = n;
   }

   /* Compute depth of dialog window */
   d =   LinCnt * (fh+LINDIST);
   if(Title)  d += fh+TTLDIST;
   if(Inp)    d += fh+INPDIST;
   if(BtnCnt) d += fh+BTNDIST;
   d += 2*YOFF;

   /* compute width of dialog window */
   w = max(BtnSum, Title ? TextExt(MEDIUM_FONT, Title) : 0);
   w = max(w, MaxLin+4) + 2*XOFF;

   /* store dialog data*/
   MsgDlg.Width = w;
   MsgDlg.Depth = d;
   MsgDlg.InitFocus = Inp ? 0 : Focus;
   MsgDlg.Items  = &MsgItm[!Inp];
   MsgDlg.ItmCnt = BtnCnt + !!Inp;

   /* compute vertical position of button/input line */
   y = YOFF+LinCnt*(fh+LINDIST);

   /* initialize Input field, if any */
   if(Inp) {
      MsgItm[0].PosX = XOFF;
      MsgItm[0].PosY = y;
      y += fh+INPDIST;
      MsgItm[0].Width = w-2*XOFF-4;
      MsgItm[0].Depth = fh + INPDIST;
      MsgItm[0].Text  = Prompt;
   }

   x  = (w - BtnSum)/2;  /* center group of buttons inside dialog */

   /* now store button data */
   for(i = 0; i < BtnCnt; i++) {
      MsgItm[i+1].PosX = x;
      MsgItm[i+1].PosY = y;
      w = TextExt(MEDIUM_FONT, BtnPtr[i])+2*BTNX;
      MsgItm[i+1].Width = w;
      MsgItm[i+1].Depth = fh + 2*BTNY;
      MsgItm[i+1].Text  = BtnPtr[i];
      x += w+BTNOFF;
   }

   /* Center dialog */
   x = (MAX_X - MsgDlg.Width)/2;
   y = (MAX_Y - MsgDlg.Depth)/2;

   /* Initialize and show dialog */
   InitDialog(&MsgDlg);
   if(Contents) SetDlgItem(&MsgDlg, 0, EDSI_TXT, Contents);
   ShowDialog(&MsgDlg, x, y, Title);

   /* Display strings */
   for(i = 0; i < LinCnt; i++) {
      WinText(MsgDlg.pWin, XOFF, YOFF+i*(fh+LINDIST), LinPtr[i]);
   }

   /* handle dialog */
   i = HandleDialog(&MsgDlg, NULL);
   if(Inp) GetDlgItem(&MsgDlg, 0, EDGI_TXT, Inp);
   CloseDialog(&MsgDlg);

   /* free storage for temporary strings */
   if(Msg)  free(Msg);
   if(Btns) free(Btns);
   va_end(ArgPtr);

   return i;
}


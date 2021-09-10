/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHEDIT.C
   Author:  Gilles Kohl
   Started: 21. Jan 95
   Subject: Functions that implement the 'Edit' dialog item class
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
                           functions
   -------------------------------------------------------------------- */

void IhEdInitBuf(IHEDIT *pDat, char *s)
{
   strncpy(pDat->Buf, s, pDat->BufLen); /* copy at most BufLen characters */
   pDat->Buf[pDat->BufLen-1] = '\0';    /* prophylactic terminator */

   /* position cursor at end of buffer contents */
   pDat->CurPos = strlen(pDat->Buf);

   /* compute position of first character visible in window */
   pDat->WinOff = max(0, pDat->CurPos - pDat->WinChr+1);
}

int IhEdInit(DLGITEM *pItm, int GotFocus)
{
   IHEDIT   *pDat; /* points to private data of item */
   IHEDINIT *pIni = pItm->Init; /* points to eventual init data */

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(pItm->Size))) return FALSE;

   /* if Ini structure there, use its font, otherwise default font */
   pDat->Font = pIni ? pIni->Font : MEDIUM_FONT;

   /* compute position of edit window based upon label length */
   pDat->WinPos = LabelExt(pDat->Font, pItm->Text);

   /* now compute pixel length of edit window and # of chrs in there */
   if(pItm->Width > 0) {
      /* Width is given as total width in pixels */
      pDat->WinLen = pItm->Width - pDat->WinPos;
      pDat->WinChr = pDat->WinLen/TextExt(pDat->Font, "M");
   }
   else {
      /* Width is given as # characters in window */
      pDat->WinChr = -pItm->Width;
      pDat->WinLen = pDat->WinChr*TextExt(pDat->Font, "M");
   }

   /* set total length of buffer, depending on additional init data.
      If init data not there, buffer length is set to max chrs in window
      minus one - this last position is for cursor display only */
   pDat->BufLen = pIni ? pIni->RealLen : pDat->WinChr-1;

   pDat->Buf    = malloc(pDat->BufLen+1); /* alloc edit buffer + '\0' */
   if(!pDat->Buf) {
      /* alloc failed, free private data and return FALSE */
      free(pDat);
      return FALSE;
   }

   /* init buffer contents and cursor */
   IhEdInitBuf(pDat, pIni ? pIni->Contents : "");

   /* set insert mode */
   pDat->InsMode = TRUE;

   /* set focus information */
   pDat->Focus = GotFocus;

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   /* that's all folks */
   return TRUE;
}

int IhEdSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, char *p)
{
   IHEDIT *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case EDSI_TXT: /* only settable value currently: the buffer */
         /* init buffer contents and cursor */
         IhEdInitBuf(pDat, p);
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhEdGetItem(DLGITEM *pItm, int n, void *p)
{
   IHEDIT *pDat = pItm->Data; /* point to private data */

   switch(n) { /* get what data ? */
      case EDGI_TXT: /* the buffer per value */
         strcpy(p, pDat->Buf);
         break;
      case EDGI_PTR:
         /* the buffer per reference (caution ! invalid after a KILL!) */
         *(char **)p = pDat->Buf;
         break;
      case EDGI_INT: /* buffer contents as an integer  */
         *(int *)p = atoi(pDat->Buf);
         break;
      default:  return FALSE;
   }
   return TRUE;
}

int IhEdShow(DIALOG *pDlg, DLGITEM *pItm)
{
   int  x, y;
   int  c, x1, x2, h;
   int  WinPos;
   int  WinLen;
   int  WinOff;
   int  WinChr;
   int  CurPos;
   BYTE *Buf;
   IHEDIT *pDat = pItm->Data;

   /* set some local variables for speed */
   WinPos = pDat->WinPos;
   WinLen = pDat->WinLen;
   WinOff = pDat->WinOff;
   WinChr = pDat->WinChr;
   CurPos = pDat->CurPos;
   Buf    = pDat->Buf;

   /* compute absolute position of item */
   x = pDlg->pWin->PosX + pItm->PosX;
   y = pDlg->pWin->PosY + pItm->PosY;

   /* compute height of font */
   h = FontHeight(pDat->Font);

   /* output label text */
   LabelOut(x, y, TXT_RULE, pDat->Font, pItm->Text);

   /* draw frame */
   SetLineType(0xaaaa);
   SetColor(BLACK_COLOR);
   Rectangle(x+WinPos-2, y-2, x+WinPos+WinLen+1, y+h+1, OUTLINE_FILL);
   SetLineType(0xffff);

   /* --- output current buffer contents --- */

   /* save character under right border of window */
   c = Buf[WinOff+WinChr];

   /* insert terminator there */
   Buf[WinOff+WinChr]= '\0';

   /* output resulting string */
   TextOut(x+WinPos, y, TXT_RULE, pDat->Font, Buf+WinOff);

   /* restore saved character */
   Buf[WinOff+WinChr] = c;

   c = TextExt(pDat->Font, Buf+WinOff);
   if(c < WinLen) { /* if there is an area to clear, do that */
      SetColor(WHITE_COLOR);
      /* SetColor(BLACK_COLOR); Test only */
      Rectangle(x+WinPos+c, y, x+WinPos+WinLen-1, y+h-1, SOLID_FILL);
      SetColor(BLACK_COLOR);
   }

   if(pDat->Focus) {
      /* show cursor if we have the focus */

      /* save character under cursor, insert terminator */
      c = Buf[CurPos];
      Buf[CurPos] = '\0';

      /* compute start and end of cursor */
      x1 = x+WinPos+TextExt(pDat->Font, Buf+WinOff);

      /* double-tricky code ahead. Relies upon INTEL-ints, and uses space
         width if cursor is over terminator at end of buffer */
      x2 = x1+TextExt(pDat->Font, c ? (char *)&c : " ")-1;

      /* Invert cursor rectangle */
      RevBlock(x1, pDat->InsMode ? y : y+h/2, x2, y+h-1);

      /* restore saved character */
      Buf[CurPos] = c;
   }

   return TRUE;
}


int IhEdKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   int  c, l;
   char *Buf;

   IHEDIT *pDat = pItm->Data;
   Buf = pDat->Buf;

   l = strlen(Buf);

   if(Key & 0xff) Key &= 0xff;

   switch(Key) {
   case DEL_KEY:
      if(pDat->CurPos < l) {
         memmove(Buf+pDat->CurPos, Buf+pDat->CurPos+1, l-pDat->CurPos);
      }
      break;
   case BACK_KEY:
      if(pDat->CurPos > 0) {
         memmove(Buf+pDat->CurPos-1, Buf+pDat->CurPos, l-pDat->CurPos+1);
         --l;
      }
      else break;
      /* fall into left-key case */
   case LEFT_KEY:
      if(pDat->CurPos > 0) {
         --pDat->CurPos;
         if(pDat->CurPos < pDat->WinOff) pDat->WinOff = pDat->CurPos;
      }
      break;
   case HOME_KEY:
      pDat->CurPos = pDat->WinOff = 0;
      break;
   case END_KEY:
      pDat->CurPos = l;
      /* compute position of first character visible in window */
      pDat->WinOff = max(0, pDat->CurPos - pDat->WinChr+1);
      break;
   case INS_KEY:
      pDat->InsMode ^= TRUE;
      break;
   default:
      if(Key < 32 || Key > 255) return FALSE;
      if(l < pDat->BufLen) {
         if(pDat->InsMode) {
            memmove(Buf+pDat->CurPos+1, Buf+pDat->CurPos, l-pDat->CurPos+1);
         }
         Buf[pDat->CurPos] = Key;
         ++l;
      }
      else break;
      /* fall into right-key case */
   case RIGHT_KEY:
      if(pDat->CurPos < l) {
         ++pDat->CurPos;
         /* adjust WinOff if cursor moved out of window */
         if(pDat->CurPos >= pDat->WinOff+pDat->WinChr) ++pDat->WinOff;
      }
      break;
   }
   (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
   return TRUE;
}


int IhEdFocus(DIALOG *pDlg, DLGITEM *pItm, int State)
{
   IHEDIT *pDat = pItm->Data;

   pDat->Focus = State;
   (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
   return TRUE;
}


int IhEdKill(DLGITEM *pItm)
{
   IHEDIT *pDat = pItm->Data;

   free(pDat->Buf);
   free(pDat);
   return TRUE;
}


int IhEdit(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhEdInit(pItm, n);
      case IM_SHOW:      return IhEdShow(pDlg, pItm);
      case IM_SETDATA:   return IhEdSetItem(pDlg, pItm, n, p);
      case IM_GETDATA:   return IhEdGetItem(pItm, n, p);
      case IM_SETFOCUS:
      case IM_LOSEFOCUS: return IhEdFocus(pDlg, pItm, Msg == IM_SETFOCUS);
      case IM_KILL:      return IhEdKill(pItm);
      case IM_KEY:       return IhEdKey(pDlg, pItm, n);
      default:           return FALSE;
   }
}


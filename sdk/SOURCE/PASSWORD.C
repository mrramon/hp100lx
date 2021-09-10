/*
  This is an example subclass of an edit handler that only shows "*" 
  instead of characters.  Just use PasswordEdit in place of Edit in
  a dialog box that needs to enter a password.
*/

#include "interfac.h"
#include "lhapi.h"

extern LHAPIBLOCK LhapiData;


int far PasswordEdit(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra)
{
  int n;

  switch (Message) {
    case KEYSTROKE:
      switch (Data) {
        case DELKEY:
        case BACKSPACEKEY:
          if (LhapiData.BeginHighlight!=LhapiData.EndHighlight)
              SubclassMsg(Edit,Wnd,KEYSTROKE,DELKEY,0);

          if (Data==DELKEY)  return TRUE;

          n = lstrlen(Wnd->Data);
          if (n) {
            Wnd->Data[--n] = 0;
            SendMsg(Wnd,DRAW,DRAW_CLIENT,0);
            }
          else
            m_beep();
          return TRUE;

        default:
          if (Data>=' ' && Data<=255) {
            if (LhapiData.BeginHighlight!=LhapiData.EndHighlight)
                SubclassMsg(Edit,Wnd,KEYSTROKE,DELKEY,0);

            n = lstrlen(Wnd->Data);
            if (n+1<Wnd->LogicalSize) {
              Wnd->Data[n++] = Data;
              Wnd->Data[n] = 0;
              SendMsg(Wnd,DRAW,DRAW_CLIENT,0);
              }
            else
              m_beep();

            return TRUE;
            }
        }
      return SubclassMsg(Object,Wnd,Message,Data,Extra);

    case SETFOCUS:
    case DRAW:
      {
      char PasswordBuffer[DB_MAXPASSWORD+1];
      int i;

      lstrcpy(PasswordBuffer,Wnd->Data);
      for (i=0; Wnd->Data[i]; i++)  Wnd->Data[i] = '*';
      SubclassMsg(Edit,Wnd,Message,Data,Extra);

      Wnd->Selection = lstrlen(PasswordBuffer);

      if (Message==DRAW)  {
        Data=DRAW_CURSOR|DRAW_CLIENT_UPDATE;
        SubclassMsg(Edit,Wnd,Message,Data,Extra);
        }

      lstrcpy(Wnd->Data,PasswordBuffer);
      }
      return TRUE;
    }
  return SubclassMsg(Edit,Wnd,Message,Data,Extra);
}

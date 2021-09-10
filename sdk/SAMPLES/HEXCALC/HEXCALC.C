/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */

/*************
 ** HexCalc **
 *************/

/******** Include header files *******/

#include "cap2.h"         /* Included in all CAP programs */

#include "interfac.h"    /* System manager includes for interface */
#include "event.h"       /* and event types */
#include "cougraph.h"
#include "lstring.h"     /* Include long string functions */
#include "chtype.h"
#include "dosfile.h"
#include "dtn_edit.h"
#include "sysdefs.h"

#include "dosfile.h"

#ifdef TKERNEL
#include "chktsr.c"
#endif


#define  TITLE_HEIGHT 10

#define STACK_TOP     14
#define STACK_BOTTOM 174
#define STACK_LEFT   112

char Target[80],Search[80],Dir[80],Answer[80];

char far *msgTestApp="Hex Calc";
char far *msgAppTopLine="Hex/Dec/Oct/Bin Calculator";

char far *fkeyHex="Hex";
char far *fkeyDec="Dec";
char far *fkeyOct="Oct";
char far *fkeyBin="Bin";
char far *fkeyXor="Xor";
char far *fkeyAnd="And";
char far *fkeyOr ="Or";
char far *fkeyNot="Not";
char far *fkeySwp="Swap";
char far *fkeyRll="R ";
char far *menuHex="Hex\tF1";
char far *menuDec="Dec\tF2";
char far *menuOct="Oct\tF3";
char far *menuBin="Bin\tF4";
char far *menuAnd="And\tF5";
char far *menuOr="Or\tF6";
char far *menuXor="Xor\tF7";
char far *menuNot="Not\tF8";
char far *menuAdd="Add\t+";
char far *menuSub="Subtract\t-";
char far *menuMult="Multiply\t*";
char far *menuDiv="Divide\t/";
char far *menuChs="Change Sign\tL";
char far *menuShl="Shift Left\t";
char far *menuShr="Shift Right\t\032";
char far *menuLogical="Logical";
char far *menuArithmetic="Arithmetic";
char far *menuMisc="Miscellaneous";
char far *menuDrop="Drop\tBackspace";
char far *menuDup="Dup\tEnter";
char far *menuSwap="Swap\tF9";
char far *menuRoll="Roll\tF10";
char far *menuOver="Over\tO";
char far *menuClear="Clear\tEsc";
char far *menuBase="Base";
char far *menuOperation="Operation";
char far *menuStack="Stack";
char far *menuQuit="&Quit";

char far *msgENVName="C:\\_DAT\\HEXCALC.ENV";

char far *msgNull="";

char far **StringTable[]={
&msgTestApp,
&msgAppTopLine,
&fkeyHex,
&fkeyDec,
&fkeyOct,
&fkeyBin,
&fkeyXor,
&fkeyAnd,
&fkeyOr,
&fkeyNot,
&fkeySwp,
&fkeyRll,
&menuQuit,
&msgENVName,
&msgNull,
&menuHex,
&menuDec,
&menuOct,
&menuBin,
&menuAnd,
&menuOr,
&menuXor,
&menuNot,
&menuAdd,
&menuSub,
&menuMult,
&menuDiv,
&menuChs,
&menuShl,
&menuShr,
&menuLogical,
&menuArithmetic,
&menuMisc,
&menuDrop,
&menuDup,
&menuSwap,
&menuRoll,
&menuOver,
&menuClear,
&menuBase,
&menuOperation,
&menuStack,
&menuQuit,
};





char cursordata[]={
0,0,0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00};


/**************** Handlers *******************/
int far MyCardHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);

void far DoQuit(void);
void far DoHex(void);
void far DoDec(void);
void far DoOct(void);
void far DoBin(void);
void far DoAnd(void);
void far DoOr(void);
void far DoXor(void);
void far DoNot(void);
void far DoShl(void);
void far DoShr(void);
void far DoChs(void);
void far DoAdd(void);
void far DoSub(void);
void far DoMult(void);
void far DoDiv(void);
void far DoSwap(void);
void far DoRoll(void);
void far DoClear(void);
void far DoOver(void);
void far DoDrop(void);
void far DoDup(void);

void Uninitialize(void);


#define STACK_DEPTH 32


/******** Global state data *******/
EVENT app_event;          /* System manager event struct */
CAPBLOCK CapData;         /* CAP application data block */


/* Stuff we save in the .ENV */
long int Stack[STACK_DEPTH];
int depth=0;
int Base=10;


BOOL Done;                /* Global flag for program termination */
int err;
long int temp;
int startnumber=1;

FKEY MyFKeys[]= {
 {  &fkeyHex,    DoHex,              1,           0 },
 {  &fkeyDec,    DoDec,              2,           0 },
 {  &fkeyOct,    DoOct,              3,           0 },
 {  &fkeyBin,    DoBin,              4,           0 },
 {  &fkeyAnd,    DoAnd,              5,           0 },
 {  &fkeyOr,     DoOr,               6,           0 },
 {  &fkeyXor,    DoXor,              7,           0 },
 {  &fkeyNot,    DoNot,              8,           0 },
 {  &fkeySwp,    DoSwap,             9,           0 },
 {  &fkeyRll,    DoRoll,            10+LAST_FKEY, 0 }
};

/******* Menu structures *******/
// Reminder of MENU structure:  (for complete details, refer to CAP.H)
// MENU = { {Title, Handler, HotKey, Style}, ...}
// End of menu indicated by null record.

MENU BaseMenu[] = {
 { &menuHex,       DoHex, 0 ,0, NO_HELP},
 { &menuDec,       DoDec, 0 ,0, NO_HELP},
 { &menuOct,       DoOct, 0 ,0, NO_HELP},
 { &menuBin,       DoBin, 0 ,0, NO_HELP},
 { 0, 0, 0, 0}
};

MENU LogicalMenu[] = {
 { &menuAnd,       DoAnd, 0 ,0, NO_HELP},
 { &menuOr,        DoOr, 0 ,0, NO_HELP},
 { &menuXor,       DoXor, 0 ,0, NO_HELP},
 { &menuNot,       DoNot, 0 ,0, NO_HELP},
 { 0, 0, 0, 0}
};

MENU ArithMenu[] = {
 { &menuAdd,       DoAdd, 0 ,0, NO_HELP},
 { &menuSub,       DoSub, 0 ,0, NO_HELP},
 { &menuMult,       DoMult, 0 ,0, NO_HELP},
 { &menuDiv,       DoDiv, 0 ,0, NO_HELP},
 { 0, 0, 0, 0}
};

MENU MiscMenu[] = {
 { &menuChs,       DoChs, 0 ,0, NO_HELP},
 { &menuShl,       DoShl, 0 ,0, NO_HELP},
 { &menuShr,       DoShr, 0 ,0, NO_HELP},
 { 0, 0, 0, 0}
};


MENU OpMenu[] = {
 { &menuLogical,      (PFUNC) LogicalMenu,     0, MENU_PULLDOWN },
 { &menuArithmetic,   (PFUNC) ArithMenu,       0, MENU_PULLDOWN },
 { &menuMisc,         (PFUNC) MiscMenu,        0, MENU_PULLDOWN },
 { 0, 0, 0, 0}
};

MENU StackMenu[] = {
 { &menuDrop,       DoDrop, 0 ,0, NO_HELP},
 { &menuDup,        DoDup, 0 ,0, NO_HELP},
 { &menuSwap,       DoSwap, 0 ,0, NO_HELP},
 { &menuRoll,       DoRoll, 0 ,0, NO_HELP},
 { &menuOver,       DoOver, 0 ,0, NO_HELP},
 { &menuClear,      DoClear, 0 ,0, NO_HELP},
 { 0, 0, 0, 0}
};


/**** TopMenu "hangs" all the previous menus off itself with MENU_PULLDOWN ****/
MENU TopMenu[] = {
 { &menuBase,      (PFUNC) BaseMenu,     0, MENU_PULLDOWN },
 { &menuOperation, (PFUNC) OpMenu,       0, MENU_PULLDOWN },
 { &menuStack,     (PFUNC) StackMenu,    0, MENU_PULLDOWN },
 { &menuQuit,      DoQuit,               0 },
 { 0, 0, 0, 0}
};


/*** MyCard Dialog Window ***/
WINDOW MyCard={MyCardHandler,
       0,0,640,190,
       &msgAppTopLine,0,
       0,0,
       NULL,MyFKeys,TopMenu,NO_HELP};



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/******                                                                ******/
/******                END OF STRUCTURES--CODE BELOW                   ******/
/******                                                                ******/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/


void FixupFarPtrs(void)
{
  int i;
  int dataseg;

  _asm {
    mov ax,ds
    mov dataseg,ax
  }

  for (i=0; i<countof(StringTable); i++)
      *(((int *)(StringTable[i]))+1) = dataseg;
}



void far DoBeep(void)
{
  m_beep();
}


void far DoQuit(void)
{
  Done = TRUE;
}

void CreateMainView(void)
{
  SendMsg(&MyCard, CREATE, CREATE_FOCUS, 0);
}

void ShowBase(void)
{
  char far *basemsg;

  switch (Base) {
    case 2:  basemsg = fkeyBin; break;
    case 8:  basemsg = fkeyOct; break;
    case 10: basemsg = fkeyDec; break;
    case 16: basemsg = fkeyHex; break;
    }
  (DrawText)(0,STACK_BOTTOM, basemsg,DRAW_NORM,FONT_LARGE);
}


void Cursor(int on)
{
 if (on) {
   _asm {
     mov ax,0dc00h       ; define cursor data
     mov si,offset cursordata
     int 10h

     mov ax,0dc03h       ; move cursor
     mov cx,640-16
     mov dx,STACK_BOTTOM
     int 10h

     mov ax,0dc04h       ; blink cursor
     int 10h

     mov ax,0dc02h       ; blink rate
     mov cx,9
     int 10h

     mov ax,0dc06h       ; turn on cursor
     int 10h
     }
   }
 else {
   _asm {
     mov ax,0dc07h       ; turn off graphics cursor
     int 10h
     }
   }
}


void PushStack(long int num)
{
  if (depth<STACK_DEPTH)  Stack[depth++] = num;
}

long int PopStack(int *err)
{
  if (!depth) *err=1;
         else return Stack[--depth];
}

int ShowEntry(char far *ptr, int y)
{
  char buffer[33];
  int i,len;
  char ch;

  /* Get length of string */
  for (len=0; ptr[len]; len++)  ;

  /* Fill up buffer from back to front */
  for (i=sizeof(buffer)-2; i>=0; i--) {
    if (len) ch = ptr[--len];
        else ch = ' ';
    buffer[i] = ch;
    }

  /* terminate string */
  buffer[sizeof(buffer)-1] = 0;

  (DrawText)(STACK_LEFT,y,buffer,DRAW_NORM,FONT_LARGE);
}


char *FormatNum(unsigned long int num)
{
  static char buffer[33];
  char *p=buffer;
  int x,i;
  int ch;

  if (Base==10 && ((long int)num<0)) {
    num = -num;
    buffer[0]='-';
    p++;
    }

  x = 0;

  /* generate digits */
  do {
    ch = num % Base;
    num /= Base;

    ch+='0';
    if (ch>'9')  ch+=7;
    p[x++] = ch;
  } while (num);

  /* reverse them */
  for (i=0; i<(x/2); i++) {
    ch = p[i];
    p[i] = p[x-i-1];
    p[x-i-1] = ch;
    }

  p[x] = 0;
  return buffer;
}



void Redisplay(int deep)
{
  int i,y;

  if (startnumber || (depth && !Stack[depth-1])) Cursor(0);

  y = STACK_BOTTOM;
  for (i=0; i<=deep; i++) {
    if (i<depth)
      ShowEntry(FormatNum(Stack[depth-i-1]),y);
    else
      ShowEntry(msgNull,y);
    y -= 16;

    if (y<STACK_TOP)  return;
    }
}

void far DoHex(void)
{
  Base=16;
  ShowBase();
  startnumber=1;
  Redisplay(depth);
}

void far DoDec(void)
{
  Base=10;
  ShowBase();
  startnumber=1;
  Redisplay(depth);
}

void far DoOct(void)
{
  Base=8;
  ShowBase();
  startnumber=1;
  Redisplay(depth);
}

void far DoBin(void)
{
  Base=2;
  ShowBase();
  startnumber=1;
  Redisplay(depth);
}

void far DoAnd(void)
{
  if (depth<2)  return;
  err=0;
  temp = PopStack(&err)&PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoOr(void)
{
  if (depth<2)  return;
  err=0;
  temp = PopStack(&err)|PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoXor(void)
{
  if (depth<2)  return;
  err=0;
  temp = PopStack(&err)^PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoNot(void)
{
  if (depth<1)  return;
  err=0;
  temp = ~PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(1);
}

void far DoChs(void)
{
  if (depth<1)  return;
  err=0;
  temp = -PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(1);
}

void far DoShl(void)
{
  if (depth<1)  return;
  err=0;
  temp = PopStack(&err)*2;
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(1);
}

void far DoShr(void)
{
  if (depth<1)  return;
  err=0;
  temp = PopStack(&err)/2;
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(1);
}

void far DoMult(void)
{
  if (depth<2)  return;
  err=0;
  temp = PopStack(&err)*PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoAdd(void)
{
  if (depth<2)  return;
  err=0;
  temp = PopStack(&err)+PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoSub(void)
{
  if (depth<2)  return;
  err=0;
  temp = -PopStack(&err)+PopStack(&err);
  if (!err) PushStack(temp);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoDiv(void)
{
  long int top;
  long int bottom;

  if (depth<2)  return;
  err=0;
  bottom = PopStack(&err);
  top = PopStack(&err);

  if (bottom) {
    temp = top/bottom;
    if (!err) PushStack(temp);
    }
  else {
    m_beep();
    PushStack(top);
    PushStack(bottom);
    }
  startnumber=1;
  Redisplay(depth+1);
}

void far DoSwap(void)
{
  if (depth<2)  return;
  temp = Stack[depth-1];
  Stack[depth-1] = Stack[depth-2];
  Stack[depth-2] = temp;

  startnumber=1;
  Redisplay(2);
}

void far DoDup(void)
{
  if (depth<1)  return;
  PushStack(Stack[depth-1]);
  startnumber=1;
  Redisplay(depth+1);
}

void far DoClear(void)
{
  depth=0;
  startnumber=1;
  SendMsg(&MyCard,DRAW,DRAW_ALL,0);
}

void far DoDrop(void)
{
  if (depth)  PopStack(&err);
  Redisplay(depth);
}

void far DoOver(void)
{
  if (depth<2)  return;

  PushStack(Stack[depth-2]);
  Redisplay(depth);
}

void far DoRoll(void)
{
  if (depth<3)  return;
  temp = Stack[depth-1];
  Stack[depth-1] = Stack[depth-2];
  Stack[depth-2] = Stack[depth-3];
  Stack[depth-3] = temp;

  startnumber=1;
  Redisplay(3);
}

int far MyCardHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra)
{
  switch (Message) {
    case KEYSTROKE:
      if (Data>='a' && Data<='z')  Data-=32;

      switch (Data) {
        case LEFTKEY:
          DoShl(); return TRUE;
        case RIGHTKEY:
          DoShr(); return TRUE;
        case 'O':
          DoOver(); return TRUE;
        case 'L':
          DoChs(); return TRUE;
        case '*':
          DoMult(); return TRUE;
        case '/':
          DoDiv(); return TRUE;
        case '-':
          DoSub(); return TRUE;
        case '+':
          DoAdd(); return TRUE;
        case '^':
          DoXor(); return TRUE;
        case '|':
          DoOr(); return TRUE;
        case '&':
          DoAnd(); return TRUE;
        case '~':
          DoNot(); return TRUE;

        case 27:
          if (startnumber)
            DoClear();
          else {
            startnumber=1; Cursor(0); DoDrop();
            }
          return TRUE;

        case 13:
          if (startnumber)
            DoDup();
          else {
            startnumber=1; Cursor(0);
            }
          return TRUE;


        case 8:
          if (depth && !Stack[depth-1])  startnumber=1;

          if (!depth || startnumber) {
            if (depth)  PopStack(&err);
            Redisplay(depth);
            }
          else {
            PushStack(PopStack(&err)/Base);
            Redisplay(1);
            }
          return TRUE;

        case '0':
        case '1':
          Data -= '0';
ProcessDigit:
          if (startnumber || !depth) {
            startnumber=1;
            PushStack(0);
            }

          PushStack(PopStack(&err)*Base+Data);
          Redisplay(startnumber?depth:1);
          Cursor(1);
          startnumber=0;
          return TRUE;

        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
          if (Base<8)
            DoBeep();
          else {
            Data -= '0';
            goto ProcessDigit;
            }
          return TRUE;

        case '8':
        case '9':
          if (Base<10)
            DoBeep();
          else {
            Data -= '0';
            goto ProcessDigit;
            }
          return TRUE;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
          if (Base<16)
            DoBeep();
          else {
            Data -= 'A'-10;
            goto ProcessDigit;
            }
          return TRUE;
        }
      break;

    case DRAW:
       if (Data&DRAW_FRAME) {
          ClearRect(Wnd->x,Wnd->y,Wnd->w,Wnd->h);
          }
       if (Data&DRAW_TITLE) {
         Rectangle(Wnd->x, Wnd->y, Wnd->w, TITLE_HEIGHT, 1, G_SOLIDFILL);
         (DrawText)(Wnd->x+(Wnd->w>>1)-lstrlen(*(Wnd->Title))*(CHAR_WIDTH(FONT_NORM)/2),
                    Wnd->y+1, *(Wnd->Title), DRAW_INVERT, FONT_SMALL);
         }
       Redisplay(depth);
       ShowBase();
       break;

    }

  SubclassMsg(Object, Wnd, Message, Data, Extra);
}




int ProcessEvent(EVENT *app_event)
{
    switch (app_event->kind) {      /* Branch on SysMgr event */
      case E_REFRESH:
      case E_ACTIV:
         FixupFarPtrs();
         ReactivateCAP(&CapData);
         if (!startnumber && (depth && Stack[depth]))  Cursor(1);
         break;

      case E_DEACT:
         Cursor(0);
         DeactivateCAP();
         break;

      case E_TERM:
         FixupFarPtrs();
         Done = TRUE;
         break;

      case E_KEY:
         /* Now send key off to current focus (KeyCode converts gray 101-key */
         /*   arrows/movement scan codes into "normal" scan codes) */
         SendMsg(GetFocus(), KEYSTROKE,
                 Fix101Key(app_event->data,app_event->scan),
                 app_event->scan);   /* Make sure we send the scan code too */
         break;
      }
}



void EventDispatcher(void)
/***
 ***  EventDispatcher grabs events from the System Manager and translates
 ***  them into CAP messages.  Every program will have an Event Dispatcher,
 ***  and the structure should follow this one.
 ***/
{
  Done = FALSE;                    /* Set terminate flag to FALSE */

  while (!Done) {                  /* While loop not terminated */
    app_event.do_event = DO_EVENT;

    m_action(&app_event);           /* Grab system manager event */
    ProcessEvent(&app_event);
    }

}


void LoadENV(void)
{
  int handle,e;

  startnumber=1;

  if (_dos_open(msgENVName,0,&handle)==-1)  goto BadEnv;
  _dos_read(handle,Stack,sizeof(Stack),&e);
  _dos_read(handle,&depth,sizeof(depth),&e);
  _dos_read(handle,&Base,sizeof(Base),&e);
  _dos_close(handle);

  /* Couldn't do it properly */
  if (e!=sizeof(Base)) {
BadEnv:
    depth=0;
    PushStack(0);      /* Start off showing something (if very first time)*/
    startnumber=0;
    }
}


void SaveENV(void)
{
  int handle,e;
  if (_dos_creat(msgENVName,0,&handle)==-1) return;
  _dos_write(handle,Stack,sizeof(Stack),&e);
  _dos_write(handle,&depth,sizeof(depth),&e);
  _dos_write(handle,&Base,sizeof(Base),&e);
  _dos_close(handle);
}



void Uninitialize(void)
/***
 *** Deinstall CAP and SysMgr
 ***/
{
  Cursor(0);
  SaveENV();
  m_fini();
}




void Initialize(void)
/***
 *** Initialize CAP, SysMgr and data structures
 ***/
{
  m_init_app(SYSTEM_MANAGER_VERSION);
  InitializeCAP(&CapData);
//  m_reg_far(&SysMgrFarPtrs, countof(SysMgrFarPtrs), 0);

  SetMenuFont(FONT_NORMAL);
  SetFont(FONT_NORMAL);

//  InitResourcedMessages();

  m_reg_app_name(msgTestApp);

  LoadENV();
  CreateMainView();  /* Now create the index view (it will display itself) */
  EnableClock(TRUE);
}







void main(void)
/***
 *** C main code
 ***/
{
#ifdef TKERNEL
  CheckTSRs();
#endif
  Initialize();           /* Get started */
  EventDispatcher();      /* Do stuff */
  Uninitialize();         /* Get outta there */
}

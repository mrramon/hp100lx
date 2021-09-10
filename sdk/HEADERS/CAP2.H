/*************************************************************************
 **
 **  You should include this file *instead* of including "cap.h".
 **
 **************************************************************************/

#define DISTANCE _far

#include "lhapi.h"

#include "keytab.h"

#undef NULL
#define NULL 0

#define ROM_VAR _based(_segname("_CODE"))

typedef unsigned int WORD;
typedef unsigned char BYTE;
typedef long int LONG;

#define LOBYTE(x) ((BYTE) ((x)&0xff))
#define HIBYTE(x) ((BYTE) ((x)>>8)&0xff)
#define MAKEWORD(lo,hi) (((WORD) hi)<<8 | (lo))
#define LOWORD(x) ((WORD) ((x)&0xffff))
#define HIWORD(x) ((WORD) ((x)>>16))
#define MAKELONG(lo,hi) (((LONG) hi)<<16 | (lo))

#define countof(x)  (sizeof(x)/(sizeof((x)[0])))


// #ifdef PCKEYBOARD
// #define Fix101Key(data,extra) (((data)!=0x00e0)?(data):((extra)<<8))
// #else
#define Fix101Key(data,extra) (data)
// #endif


typedef char far *LPSTR;
typedef char far **HANDLE;


#define CAPBLOCK   LHAPIBLOCK
#define PWINDOW    PLHWINDOW
#define WINDOW     LHWINDOW
#define FKEY       LHFKEY
#define LAST_FKEY  FKEY_LAST
#define PCLASS     PLHCLASS
#define MENU       LHMENU
#define PMENU      PLHMENU
#define PSTR       PLHDATA
#define PFUNC      PLHFUNC
#define PRES       PLHRES

#define DeactivateCAP DeactivateLHAPI
#define ReactivateCAP ReactivateLHAPI
#define InitializeCAP InitializeLHAPI
#define UninitializeCAP UninitializeLHAPI
#define SetFont SetDefaultFont
#define GetFont GetDefaultFont

int far CountEscapes(char far *str);


#ifndef NODRAWTEXTSHELL
void far DrawTextShell(int x,int y,char far *str, int style,int font);
#define DrawText(x,y,str,style) ((DrawTextShell)(x,y,str,style,0))
#else
#define DrawText(x,y,str,style) ((DrawText)(x,y,str,style,0))
#endif

//#undef  PushButton
//int far PushButtonHP(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
//#define PushButton PushButtonHP

#define DRAW_INVERT DRAW_INVERSE
#define FONT_NORM  FONT_NORMAL





#define MENU_CARDGRAY      0x8000
#define MENU_INDEXGRAY     0x4000
#define DRAW_NORM          0
#define SHADOW_WIDTH       8
#define SHADOW_HEIGHT      6
#define BORDER_THICKNESS   1

#define FKEY_WHITE 0
#define PUSHBUTTON_DEFAULT 0



//#define RADIO_GRAY  6
//#define CHECK_GRAY  7
//#define CAT_SPIN    8

#define SYMBOL_WIDTH 14
#define SYMBOL_HEIGHT 14

#define SCREEN_WIDTH DISPLAY_W
#define SCREEN_HEIGHT DISPLAY_H
#define CLIENT_TOP (TITLEBAR_H+2)
#define FKEY_TOP FKEYS_START_Y
#define CLIENT_HEIGHT (FKEY_TOP-CLIENT_TOP)
#define CLIENT_WIDTH (SCREEN_WIDTH)


#define STATUS_FIRSTEDIT STATUS_NOT_EDITED


#define CATEGORY_NOADD 0x80


//int far FileOpenDialogHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
//#undef FileOpenDialogBox
//#define FileOpenDialogBox FileOpenDialogHandler

//int far FileBoxHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
//#undef FileListBox
//#define FileListBox FileBoxHandler

//int far DirBoxHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
//#undef DirListBox
//#define DirListBox DirBoxHandler

//int far PathEditHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
//#undef PathEdit
//#define PathEdit PathEditHandler

//#undef PushButton
//int far PushButton(PWINDOW Window,WORD Message, WORD Data, WORD Extra);


#undef PhoneEdit
#define PhoneEdit Edit

#undef MultiLine
#define MultiLine Edit

int far NoteEdit(PWINDOW Window,WORD Message, WORD Data, WORD Extra);

#undef Card
int far CardHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
#define Card CardHandler

#undef TimeEdit
#define TimeEdit TimeEditHandler

#undef NumberEdit
#define NumberEdit NumberEditHandler

#undef DateEdit
#define DateEdit DateEditHandler

int far TimeEditHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far NumberEditHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far DateEditHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);


int far HandleClock(void);
int far EnableClock(int x);

void far HourglassOn(int x,int y);
void far HourglassOff(void);


#define ShowFKeys(Win) DrawFKeys(Win,FKEY_ALONE)
//#define ShowFKeys(Win)  SubclassMsg(Object, Win, DRAW, DRAW_FKEYS, 0);

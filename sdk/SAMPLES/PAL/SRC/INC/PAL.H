/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PAL.H
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Main PAL include file
   -------------------------------------------------------------------- */

#ifndef _PAL_H
#define _PAL_H

#include <time.h>

/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */

#ifndef FALSE
#define FALSE 0
#define TRUE 1
#endif

#include "palkeys.h"

/* screen dimensions */
#define MAX_X 640
#define MAX_Y 200

/* Color definitions */
#define BLACK_COLOR 1
#define WHITE_COLOR 0

/* Annunciator positions */
#define AN_LEFT  0x20
#define AN_RIGHT 0x60

/* Font ids */
#define SMALL_FONT  0x0808
#define MEDIUM_FONT 0x0a0b
#define LARGE_FONT  0x100c

/* Output rule definitions */
#define FORCE_RULE  0
#define AND_RULE    1
#define OR_RULE     2
#define XOR_RULE    3
#define INVFOR_RULE 4
#define INVAND_RULE 5
#define INVOR_RULE  6
#define INVXOR_RULE 7
#define TXT_RULE    8

/* Rectangle fill variations */
#define OUTLINE_FILL 0
#define SOLID_FILL   1
#define PATTERN_FILL 2

/* Line fill styles */
#define SOLID_LINE  0xffff
#define DASHED_LINE 0x5555

/* Scroll directions */
#define SCROLL_LEFT  0
#define SCROLL_RIGHT 1
#define SCROLL_UP    2
#define SCROLL_DOWN  3

/* Cursor function definitions */
#define CURSOR_ON       0xdc06
#define CURSOR_OFF      0xdc07
#define CURSOR_BLINK    0xdc04
#define CURSOR_NOBLINK  0xdc05
#define CURSOR_INIT     0xdc00

/* Volume control definitions */
#define VOL_OFF      0
#define VOL_LOW      1
#define VOL_MEDIUM   2
#define VOL_HI       3

/* Serial port definitions */
#define SERIAL_ON 1
#define SERIAL_OFF 0

/* Power Management definitions */
#define SLEEP_ON 1
#define SLEEP_OFF 0

/* Window styles */
#define WS_BORDER   1
#define WS_DBORDER  2
#define WS_FRAME   13
#define WS_BMASK   15
#define WS_SHADOW  16
#define WS_SMLFONT 32
#define WS_SAVEBG  64

/* Composed window styles */
#define WS_MENU  (WS_BORDER|WS_SHADOW|WS_SAVEBG)
#define WS_HP100 (WS_BORDER|WS_SHADOW|WS_SMLFONT|WS_SAVEBG)
#define WS_HP200 (WS_DBORDER|WS_SHADOW|WS_SAVEBG)
#define WS_HELP  (WS_FRAME|WS_SMLFONT|WS_SAVEBG)

/* Menu item (basic) flags */
#define MI_ACTMSK     7  /* Mask for menu item action: lower 3 bits */
#define MI_TYPMSK (7<<3) /* Mask for menu item type      */

#define MI_GRY       64  /* Menu item is greyed          */
#define MI_SKIP     128  /* Menu item will be skipped    */

/* Menu item types */
#define MT_TXT   (0<<3)
#define MT_SEP   (1<<3)
#define MT_BMP   (2<<3)

/* Menu item (composite) flags */
#define MT_SEPARATOR (MT_SEP|MI_SKIP)

/* Menu style flags */
#define MS_HORIZ  1
#define MS_TOPLVL 2
#define MS_PULLDN 4

/* Menu special commands, all negative */
#define MSC_ESC     -1
#define MSC_STAY    -2

/* --------------------------------------------------------------------
                          macro definitions
   -------------------------------------------------------------------- */

/* Macros to extract the font width and height from a font id */
#define FNTW(fnt)  (((fnt)&0xff00)>>8)
#define FNTD(fnt)  ((fnt)&0xff)

/* Cast an int into a type acceptable as a 'menu value' */
#define MENUVAL(v) ((void *)(v))

/* new name for MENUVAL, to be used in more general context */
#define VALUE(v) ((void *)(v))

/* Compute the size of a image bitmap given the corners */
#define IMGSIZE(x1,y1,x2,y2) (8+(((x2)-(x1)+8)/8)*((y2)-(y1)+1))

/* Compute the number of elements of an array */
#define ELMS(a) (sizeof(a)/sizeof(a[0]))

/* --------------------------------------------------------------------
                           type definitions
   -------------------------------------------------------------------- */

/* some utility types */
typedef unsigned char BYTE;
typedef unsigned int  WORD;
typedef unsigned long DWORD;

/* actions a menu item can trigger when selected */
typedef enum {
   MA_VAL, /* return a given value     */
   MA_MNU, /* show another menu        */
   MA_FUN  /* invoke callback function */
} MNU_ACTION;

/* the window descriptor structure */
typedef struct {
   void *Buffer;        /* background storage */
   WORD Style;          /* Style that window has */
   WORD Font;           /* System font to use for caption & menues     */
   int  x1, y1, x2, y2; /* physical dimensions of window, INCL. border */
   int  PosX, PosY;     /* upper left coordinates of client area       */
   int  Width, Depth;   /* width and depth of client area              */
} PALWIN;

/* menu item descriptor structure */
typedef struct {
   char *Text;          /* Text to be displayed */
   WORD Flags;          /* flags: state item is in, action triggered */
   void *ActionPtr;     /* additional parameter for action */
} MENUITEM;

/* menu descriptor structure */
typedef struct {
   PALWIN *pMnuWin;     /* a menu has an associated window */
   WORD Flags;          /* flags: kind of menu */
   int CurItem;         /* currently selected item */
   int TopItem;         /* first visible item in menu */
   int ItmDisp;         /* number of visible items */
   int ItmCount;        /* total number of items */
   MENUITEM *Items;     /* array of items */
} MENU;

/* pointer to a menu callback function */
typedef int (*MNUCALLBACK)(MENU *pMnu);

/* Header of a GetImg bitmap  */
typedef struct {
   int Planes; /* on HP100/200, always 1 */
   int Bits;   /* on HP100/200, always 1 */
   int Width;
   int Depth;
} IMGHDR;


/* Font File Header */
typedef struct {
    int  FontType;         /* Type (ID) of font           */
    int  MinChar;          /* First ASCII character       */
    int  MaxChar;          /* Last ASCII character        */
    int  MaxWidth;         /* Width of widest bitmap      */
    int  MaxHeight;        /* Height of highest bitmap    */
} FNTFHDR;

/* Font Descriptor */
typedef struct {
    int     Discardable;   /* Flag: can be freed, non-resident  */
    IMGHDR  *pChar[256];   /* font character pointer table      */
    FNTFHDR FontHeader;    /* 6-byte header followed by bitmaps */
} FONTDESC;                /* font descriptor                   */

/* Structure representing saved graphics state */
typedef struct {
   int sh_xpos;           /* shadow xpos      */
   int sh_ypos;           /* shadow ypos      */
   int sh_font;           /* shadow font type */
   int sh_rule;           /* shadow rep. rule */
   int sh_color;          /* shadow color     */
   unsigned int sh_Ltype; /* shadow line type */
   BYTE sh_mask[8];       /* shadow fill mask */
} SAVEDSTATE;


/* --------------------------------------------------------------------
                              prototypes
   -------------------------------------------------------------------- */

/* -------------
   Miscellaneous
   ------------- */
void AnnuncPos(int position);
int  Check5f(void);
int  CheckSysMgr(void);
void FatalExit(char *Msg, int ExitCode);
int  PalInit(int Graphics);
void PalDeInit(int Text);
int  WhichHp(void);
WORD GetKey(void);
int  KeyWaiting(void);
void PushKeys(WORD *pKeys);
void Volume(int Level);
void SetSerialPort(int Mode);
void SetLightSleep(int Mode);
void SetTimeOut(WORD value);
int MsgBox(char *Title, char *Fmt, char *Inp, char *Btns, ...);
int AskYesNo(char *Question);
void MsDelay(unsigned int Millisec);
WORD AltKey(unsigned Letter);

/* ------------------
   Low level graphics
   ------------------ */
int  GraphMode(void);
int  TextMode(void);
void RestoreGraph(void);
void PlotDot(int x, int y);
void SelectFont(int Size);
void SetPos(int x, int y);
void SetColor(int Color);
void SetRule(int Rule);
void SetLineType(unsigned int LineType);
void SetMask(char far *FillMask);
void PutImg(int x1, int y1, int Rule, void far *Bitmap);
void GetImg(int x1, int y1, int x2, int y2, void far *Bitmap);
void WriteText(int x, int y, char far *Str);

void DefineCursor(char far *CursorDef);
void MoveCursor(int x1, int y1);
void Cursor(WORD Function);
void BlinkRate(int Rate);

void SetClip(int x1, int y1, int x2, int y2);
void Scroll(int Direction, int Distance, int Color,
            int x1, int y1, int x2, int y2);
void SaveState(SAVEDSTATE *pState);
void RestoreState(SAVEDSTATE *pState);

/* -------------------
   High level graphics
   ------------------- */
void Line(int x1, int y1, int x2, int y2);
void Rectangle(int x1, int y1, int x2, int y2, int FillStyle);
void RevBlock(int x1, int y1, int x2, int y2);
void ClrBlock(int x1, int y1, int x2, int y2, int Color);
IMGHDR *LoadImg(char *Name);
IMGHDR *LoadPcx(char *Name, int Invert);
int SaveImg(IMGHDR *pImg, char *Name);
void ShowFKeys(char **pKeys);
void ShowTopLine(char *pTitle, char *pTime);
void ShowTopTime(char *pTitle, int ForceUpdate);
char *FormatDate(char *DateStr, struct tm *pTm);
char *FormatTime(char *TimeStr, struct tm *pTm);


/* -------------------
   Proportional fonts
   ------------------- */
FONTDESC *LoadHfn(char *FileName);
void InitHfn(FONTDESC *pFnt, int Discardable, void *pBitmap);
int WriteHfn(FONTDESC *pFont, int x, int y, char *s);
int TextExtHfn(FONTDESC *pFont, char *s);
int FontHeightHfn(FONTDESC *pFont);
void DiscardHfn(FONTDESC *pFont);

/* --------------------------------------------
               Unified font support
   -------------------------------------------- */
/* the following three functions are actually globals, not prototypes */
extern int (*TextOut)(int x, int y, int Style, int FntId, char *s);
extern int (*TextExt)(int FntId, char *s);
extern int (*FontHeight)(int FntId);

void DiscardFont(int FntId);
int LoadFont(void *Font, int Resident);

/* ----------------
   Window functions
   ---------------- */
void    WinText(PALWIN *pWin, int OffsetX, int OffsetY, char *Txt);
PALWIN *OpenWin(WORD Style, int x1, int y1, int x2, int y2, char *Caption);
void    CloseWin(PALWIN *pWin);

/* --------------
   Menu functions
   -------------- */
int HandleMenu(MENU *pMnu, int PosX, int PosY);
int MenuSelect(char *Items, int PosX, int PosY, int Default, int Show);

/* ---------------
   Config routines
   --------------- */
void  ReadConfig(char *CfgName);
void  ReadMyConfig(char *MyName);
char *GetConfigString(char *Section, char *Key, char *Default);
int   GetConfigInt(char *Section, char *Key, int Default);
char *GetFirstEntry(char *Section, char **pKey);
char *GetNextEntry(char **pKey);
int   GetCfgLine(void);
void  DiscardConfig(void);

#include "paldlg.h"

#endif


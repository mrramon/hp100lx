/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
/******************************************************************************
 *
 *  Prototypes and structures for CAP database engine.
 *
 ******************************************************************************/
#pragma pack(1)

#include "g_sm.h"

#ifndef NULL
#define NULL 0
#endif

/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 *
 *  Function prototypes for GLED.
 *
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/

int far _THspare1();
#define	THspare1() (G_DIRECT(1) ? _THspare1() : c_service(F_GLED_THSPARE1))

int far _THspare2();
#define	THspare2() (G_DIRECT(1) ? _THspare2() : c_service(F_GLED_THSPARE2))

int far _THspare3();
#define	THspare3() (G_DIRECT(1) ? _THspare3() : c_service(F_GLED_THSPARE3))

int far _TextHandlerTSR(GSMPREFIX1 PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
#define	TextHandlerTSR(W,M,D,E) (G_DIRECT(1) ? _TextHandlerTSR(GSMCALL1 (W),(M),(D),(E)) : c_service(F_GLED_TEXTHANDLERTSR, (W),(M),(D),(E)))

int far _ShowRuler(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowRuler(GS) (G_DIRECT(2) ? _ShowRuler(GSMCALL1 (GS)) : c_service(F_GLED_SHOWRULER, (void near *)(GS)))

int far _InitCursor(GSMPREFIX1 PGLEDSTUFF GS);
#define	InitCursor(GS) (G_DIRECT(2) ? _InitCursor(GSMCALL1 (GS)) : c_service(F_GLED_INITCURSOR, (void near *)(GS)))

int far _ShowModified(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowModified(GS) (G_DIRECT(2) ? _ShowModified(GSMCALL1 (GS)) : c_service(F_GLED_SHOWMODIFIED, (void near *)(GS)))

int far _ShowCaps(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowCaps(GS) (G_DIRECT(2) ? _ShowCaps(GSMCALL1 (GS)) : c_service(F_GLED_SHOWCAPS, (void near *)(GS)))

int far _ShowBold(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowBold(GS) (G_DIRECT(2) ? _ShowBold(GSMCALL1 (GS)) : c_service(F_GLED_SHOWBOLD, (void near *)(GS)))

int far _ShowUnder(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowUnder(GS) (G_DIRECT(2) ? _ShowUnder(GSMCALL1 (GS)) : c_service(F_GLED_SHOWUNDER, (void near *)(GS)))

int far _ShowLinCol(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowLinCol(GS) (G_DIRECT(2) ? _ShowLinCol(GSMCALL1 (GS)) : c_service(F_GLED_SHOWLINCOL, (void near *)(GS)))

int far _UpdateLinCol(GSMPREFIX1 PGLEDSTUFF GS);
#define	UpdateLinCol(GS) (G_DIRECT(2) ? _UpdateLinCol(GSMCALL1 (GS)) : c_service(F_GLED_UPDATELINCOL, (void near *)(GS)))

int far _ShowInsert(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowInsert(GS) (G_DIRECT(2) ? _ShowInsert(GSMCALL1 (GS)) : c_service(F_GLED_SHOWINSERT, (void near *)(GS)))

int far _ShowAscForm(GSMPREFIX1 PGLEDSTUFF GS);
#define	ShowAscForm(GS) (G_DIRECT(2) ? _ShowAscForm(GSMCALL1 (GS)) : c_service(F_GLED_SHOWASCFORM, (void near *)(GS)))

unsigned int far _GetBoldUnder(GSMPREFIX1 PGLEDSTUFF GS);
#define	GetBoldUnder(GS) (G_DIRECT(2) ? _GetBoldUnder(GSMCALL1 (GS)) : c_service(F_GLED_GETBOLDUNDER, (void near *)(GS)))

int far _JustFormat(GSMPREFIX1 PGLEDSTUFF GS, unsigned char far **a);
#define	JustFormat(GS,a) (G_DIRECT(2) ? _JustFormat(GSMCALL1 (GS),(unsigned char far **)(a)) : c_service(F_GLED_JUSTFORMAT, (void near *)(GS),(unsigned char far **)(a)))

int far _PrintInit(GSMPREFIX1 PGLEDSTUFF GS, unsigned char *a);
#define	PrintInit(GS,a) (G_DIRECT(2) ? _PrintInit(GSMCALL1 (GS),(void near *)(a)) : c_service(F_GLED_PRINTINIT, (void near *)(GS),(void near *)(a)))

int far _PrintText(GSMPREFIX1 PGLEDSTUFF GS);
#define	PrintText(GS) (G_DIRECT(2) ? _PrintText(GSMCALL1 (GS)) : c_service(F_GLED_PRINTTEXT, (void near *)(GS)))

int far _CursOn(GSMPREFIX1 PGLEDSTUFF GS);
#define	CursOn(GS) (G_DIRECT(2) ? _CursOn(GSMCALL1 (GS)) : c_service(F_GLED_CURSON, (void near *)(GS)))

int far _LocateCursor(GSMPREFIX1 PGLEDSTUFF GS);
#define	LocateCursor(GS) (G_DIRECT(2) ? _LocateCursor(GSMCALL1 (GS)) : c_service(F_GLED_LOCATECURSOR, (void near *)(GS)))

int far _CursOff(GSMPREFIX1 PGLEDSTUFF GS);
#define	CursOff(GS) (G_DIRECT(2) ? _CursOff(GSMCALL1 (GS)) : c_service(F_GLED_CURSOFF, (void near *)(GS)))

int far _RenumOutline(GSMPREFIX1 PGLEDSTUFF GS);
#define	RenumOutline(GS) (G_DIRECT(2) ? _RenumOutline(GSMCALL1 (GS)) : c_service(F_GLED_RENUMOUTLINE, (void near *)(GS)))

int far _MoveCursCurLine(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a);
#define	MoveCursCurLine(GS,a) (G_DIRECT(2) ? _MoveCursCurLine(GSMCALL1 (GS),(a)) : c_service(F_GLED_MOVECURSCURLINE, (void near *)(GS),(a)))

int far _MoveToSOL(GSMPREFIX1 PGLEDSTUFF GS);
#define	MoveToSOL(GS) (G_DIRECT(2) ? _MoveToSOL(GSMCALL1 (GS)) : c_service(F_GLED_MOVETOSOL, (void near *)(GS)))

int far _InsertByte(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a);
#define	InsertByte(GS,a) (G_DIRECT(2) ? _InsertByte(GSMCALL1 (GS),(a)) : c_service(F_GLED_INSERTBYTE, (void near *)(GS),(a)))

int far _InsertBufMovCurs(GSMPREFIX1 PGLEDSTUFF GS, unsigned char far *ap, unsigned int b);
#define	InsertBufMovCurs(GS,ap,b) (G_DIRECT(2) ? _InsertBufMovCurs(GSMCALL1 (GS),(void far *)(ap),(b)) : c_service(F_GLED_INSERTBUFMOVCURS, (void near *)(GS),(void far *)(ap),(b)))

int far _ReplaceByte(GSMPREFIX1 PGLEDSTUFF GS,unsigned int a);
#define	ReplaceByte(GS,a) (G_DIRECT(2) ? _ReplaceByte(GSMCALL1 (GS),(a)) : c_service(F_GLED_REPLACEBYTE, (void near *)(GS),(a)))

unsigned int far _DeleteByte(GSMPREFIX1 PGLEDSTUFF GS);
#define	DeleteByte(GS) (G_DIRECT(2) ? _DeleteByte(GSMCALL1 (GS)) : c_service(F_GLED_DELETEBYTE, (void near *)(GS)))

int far _DeleteBlock(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a);
#define	DeleteBlock(GS,a) (G_DIRECT(2) ? _DeleteBlock(GSMCALL1 (GS),(a)) : c_service(F_GLED_DELETEBLOCK, (void near *)(GS),(a)))

int far _GetBlock(GSMPREFIX1 PGLEDSTUFF GS, unsigned char *ap);
#define	GetBlock(GS, ap) (G_DIRECT(2) ? _GetBlock(GSMCALL1 (GS), (unsigned char *)(ap)) : c_service(F_GLED_GETBLOCK, (void near *)(GS), (void near *)(ap)))

int far _IsBlock(GSMPREFIX1 PGLEDSTUFF GS);
#define	IsBlock(GS) (G_DIRECT(2) ? _IsBlock(GSMCALL1 (GS)) : c_service(F_GLED_ISBLOCK, (void near *)(GS)))

int far _MovePaneDown(GSMPREFIX1 PGLEDSTUFF GS,unsigned int a);
#define	MovePaneDown(GS,a) (G_DIRECT(2) ? _MovePaneDown(GSMCALL1 (GS),(a)) : c_service(F_GLED_MOVEPANEDOWN, (void near *)(GS),(a)))

int far _ScrollTextUp(GSMPREFIX1 PGLEDSTUFF GS, int a);
#define	ScrollTextUp(GS,a) (G_DIRECT(2) ? _ScrollTextUp(GSMCALL1 (GS),(a)) : c_service(F_GLED_SCROLLTEXTUP, (void near *)(GS),(a)))

int far _ScrollTextDown(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a);
#define	ScrollTextDown(GS,a) (G_DIRECT(2) ? _ScrollTextDown(GSMCALL1 (GS),(a)) : c_service(F_GLED_SCROLLTEXTDOWN, (void near *)(GS),(a)))

int far _IsAtCR(GSMPREFIX1 PGLEDSTUFF GS);
#define	IsAtCR(GS) (G_DIRECT(2) ? _IsAtCR(GSMCALL1 (GS)) : c_service(F_GLED_ISATCR, (void near *)(GS)))

int far _OnWord(GSMPREFIX1 PGLEDSTUFF GS);
#define	OnWord(GS) (G_DIRECT(2) ? _OnWord(GSMCALL1 (GS)) : c_service(F_GLED_ONWORD, (void near *)(GS)))

int far _AtEof(GSMPREFIX1 PGLEDSTUFF GS,unsigned int a);
#define	AtEof(GS,a) (G_DIRECT(2) ? _AtEof(GSMCALL1 (GS),(a)) : c_service(F_GLED_ATEOF, (void near *)(GS),(a)))

int far _SkipOneFF(GSMPREFIX1 PGLEDSTUFF GS);
#define	SkipOneFF(GS) (G_DIRECT(2) ? _SkipOneFF(GSMCALL1 (GS)) : c_service(F_GLED_SKIPONEFF, (void near *)(GS)))

int far _ForceFormatBlock(GSMPREFIX1 PGLEDSTUFF GS);
#define	ForceFormatBlock(GS) (G_DIRECT(2) ? _ForceFormatBlock(GSMCALL1 (GS)) : c_service(F_GLED_FORCEFORMATBLOCK, (void near *)(GS)))

int far _SetFormatBlock(GSMPREFIX1 PGLEDSTUFF GS);
#define	SetFormatBlock(GS) (G_DIRECT(2) ? _SetFormatBlock(GSMCALL1 (GS)) : c_service(F_GLED_SETFORMATBLOCK, (void near *)(GS)))

int far _GetFormatBlock(GSMPREFIX1 PGLEDSTUFF GS);
#define	GetFormatBlock(GS) (G_DIRECT(2) ? _GetFormatBlock(GSMCALL1 (GS)) : c_service(F_GLED_GETFORMATBLOCK, (void near *)(GS)))

int far _RemoveBoldUnder(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a, unsigned int b);
#define	RemoveBoldUnder(GS,a,b) (G_DIRECT(2) ? _RemoveBoldUnder(GSMCALL1 (GS),(a),(b)) : c_service(F_GLED_REMOVEBOLDUNDER, (void near *)(GS),(a),(b)))

int far _CompressBoldUnder(GSMPREFIX1 PGLEDSTUFF GS);
#define	CompressBoldUnder(GS) (G_DIRECT(2) ? _CompressBoldUnder(GSMCALL1 (GS)) : c_service(F_GLED_COMPRESSBOLDUNDER, (void near *)(GS)))

int far _BackSkipBoldUnder(GSMPREFIX1 PGLEDSTUFF GS);
#define	BackSkipBoldUnder(GS) (G_DIRECT(2) ? _BackSkipBoldUnder(GSMCALL1 (GS)) : c_service(F_GLED_BACKSKIPBOLDUNDER, (void near *)(GS)))

int far _UnLFit(GSMPREFIX1 PGLEDSTUFF GS);
#define	UnLFit(GS) (G_DIRECT(2) ? _UnLFit(GSMCALL1 (GS)) : c_service(F_GLED_UNLFIT, (void near *)(GS)))

int far _LFit(GSMPREFIX1 PGLEDSTUFF GS);
#define	LFit(GS) (G_DIRECT(2) ? _LFit(GSMCALL1 (GS)) : c_service(F_GLED_LFIT, (void near *)(GS)))

int far _LFtext(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a);
#define	LFtext(GS,a) (G_DIRECT(2) ? _LFtext(GSMCALL1 (GS),(a)) : c_service(F_GLED_LFTEXT, (void near *)(GS),(a)))

int far _Cut2Clip(GSMPREFIX1 PGLEDSTUFF GS, unsigned char far **app, unsigned int *c, unsigned int *b);
#define	Cut2Clip(GS,app,b,c) (G_DIRECT(2) ? _Cut2Clip(GSMCALL1 (GS),(unsigned char far **)(app),(unsigned int *)(b),(unsigned int *)(c)) : c_service(F_GLED_CUT2CLIP, (void near *)(GS),(void far **)(app),(void near *)(b),(void near *)(c)))

int far _DrawTitle(GSMPREFIX1 PGLEDSTUFF GS);
#define	DrawTitle(GS) (G_DIRECT(2) ? _DrawTitle(GSMCALL1 (GS)) : c_service(F_GLED_DRAWTITLE, (void near *)(GS)))

int far _DrawFrame(GSMPREFIX1 PGLEDSTUFF GS);
#define	DrawFrame(GS) (G_DIRECT(2) ? _DrawFrame(GSMCALL1 (GS)) : c_service(F_GLED_DRAWFRAME, (void near *)(GS)))

int far _SetCurLineLevel(GSMPREFIX1 PGLEDSTUFF GS, int a);
#define	SetCurLineLevel(GS,a) (G_DIRECT(2) ? _SetCurLineLevel(GSMCALL1 (GS),(a)) : c_service(F_GLED_SETCURLINELEVEL, (void near *)(GS),(a)))

int far _ChkNullHead(GSMPREFIX1 PGLEDSTUFF GS);
#define	ChkNullHead(GS) (G_DIRECT(2) ? _ChkNullHead(GSMCALL1 (GS)) : c_service(F_GLED_CHKNULLHEAD, (void near *)(GS)))

int far _MovePane(GSMPREFIX1 PGLEDSTUFF GS,int a);
#define	MovePane(GS,a) (G_DIRECT(2) ? _MovePane(GSMCALL1 (GS), (a)) : c_service(F_GLED_MOVEPANE, (void near *)(GS),(a)))

int far _AtStartOfHeader(GSMPREFIX1 PGLEDSTUFF GS);
#define	AtStartOfHeader(GS) (G_DIRECT(2) ? _AtStartOfHeader(GSMCALL1 (GS)) : c_service(F_GLED_ATSTARTOFHEADER, (void near *)(GS)))

int far _DrawTextPane(GSMPREFIX1 PGLEDSTUFF GS,int a,int b);
#define	DrawTextPane(GS,a,b) (G_DIRECT(2) ? _DrawTextPane(GSMCALL1 (GS),(a),(b)) : c_service(F_GLED_DRAWTEXTPANE, (void near *)(GS),(a),(b)))

int far _SetTextPaneFont(GSMPREFIX1 PGLEDSTUFF GS, int a);
#define	SetTextPaneFont(GS, a) (G_DIRECT(2) ? _SetTextPaneFont(GSMCALL1 (GS),(a)) : c_service(F_GLED_SETTEXTPANEFONT, (void near *)(GS),(a)))

int far _InsertBlock(GSMPREFIX1 PGLEDSTUFF GS, unsigned char far *ap, unsigned int a);
#define	InsertBlock(GS,ap,b) (G_DIRECT(2) ? _InsertBlock(GSMCALL1 (GS),(ap),(b)) : c_service(F_GLED_INSERTBLOCK, (void near *)(GS),(unsigned char far *)(ap),(b)))

int far _InsertBlockMovCurs(GSMPREFIX1 PGLEDSTUFF GS,unsigned char far *ap, unsigned int b);
#define	InsertBlockMovCurs(GS,ap,b) (G_DIRECT(2) ? _InsertBlockMovCurs(GSMCALL1 (GS),(ap),(b)) : c_service(F_GLED_INSERTBLOCKMOVCURS, (void near *)(GS),(unsigned char far *)(ap),(b)))

int far _MoveCursor(GSMPREFIX1 PGLEDSTUFF GS, int a, int b);
#define	MoveCursor(GS,a,b) (G_DIRECT(2) ? _MoveCursor(GSMCALL1 (GS),(a),(b)) : c_service(F_GLED_MOVECURSOR, (void near *)(GS),(a),(b)))

int far _SetCurs(GSMPREFIX1 PGLEDSTUFF GS, int a);
#define	SetCurs(GS, a) (G_DIRECT(2) ? _SetCurs(GSMCALL1 (GS), (a)) : c_service(F_GLED_SETCURS, (void near *)(GS), (a)))

int far _Spare1(GSMPREFIX1 PGLEDSTUFF GS);
#define	Spare1(GS) (G_DIRECT(2) ? _Spare1(GSMCALL1 (GS)) : c_service(F_GLED_SPARE1, (GS)))

unsigned char far * far _MarkPtrSize(GSMPREFIX1 PGLEDSTUFF GS, unsigned int *ap, unsigned int *b);
#define	MarkPtrSize(GS, a, b) (G_DIRECT(2) ? _MarkPtrSize(GSMCALL1 (GS), (a), (b)) : (unsigned char far *)(c_service_far_ptr(F_GLED_MARKPTRSIZE, (void near *)(GS), (void near *)(a), (void near *)(b))))

unsigned char far * far _MakePtr(GSMPREFIX1 PGLEDSTUFF GS, unsigned int a, unsigned int b);
#define	MakePtr(GS, a, b) (G_DIRECT(2) ? _MakePtr(GSMCALL1 (GS), (a), (b)) : (unsigned char far *)(c_service_far_ptr(F_GLED_MAKEPTR, (void near *)(GS), (a), (b))))

#pragma pack()

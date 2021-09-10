/* SCCS Information -- Module %M%  Version %I%  Date %G%  Time %U% */
//************************************************************************
//************************************************************************
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

// GLED.H  Copyright 1992 Hewlett-Packard Company.  All rights reserved.

//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#define	GLED_OUTBUFLEN	256

#define	GF_FORMOUTLINE	0x0001
#define	GF_SHOWCTL	0x0002
#define	GF_RULER	0x0004
#define	GF_STATUSBAR	0x0008
#define	GF_CASESENSITIVE 0x0010
#define	GF_PASTEGLEDBIN	0x0020
#define	GF_MODIFIED	0x0040
#define	GF_NOTITLE	0x0080
#define	GF_GROWABLE	0x0100
#define	GF_INSERT	0x0200
#define	GF_NEEDUPDATE	0x0400
#define	GF_CURSON	0x0800
#define	GF_BOX		0x1000
#define	GF_NODRAW	0x2000
#define	GF_VALID	0x4000
#define	GF_FORMATTED	0x8000

#define	OF_MARKSET	0x0001
#define	OF_MAX20K	0x0002
#define	OF_CAPS		0x0004
#define	OF_LFITFLAG	0x0008	/* used by LFit to keep PANE from moving */
#define	OF_TABCOLCURS	0x0010
#define	OF_ASCIIONLY	0x0020	/* causes no format info during print */
#define	OF_NOBLANKLINE	0x0040	/* Set during print to stop blank lines */
#define	OF_CURSENABLE	0x0080
#define	OF_HANDLETABS	0x0100
#define	OF_PATSKLUDGE	0x0200
#define	OF_NOCRS	0x0400	/* set for terminating PASTE on CR or LF */
#define	OF_NOFF		0x0800	/* set for no formfeed on last page of PrintText */
#define	OF_PRINT_INT	0x1000	/* set for printing to be interruptable */
#define	OF_NOTECREATED	0x2000	/* NOTE HANDLER's flag that note has been created previously */
#define	OF_MAX32K	0x4000	/* NOTE HANDLER's flag that note is limited to 32K */

#define	F_OLRELEASE	0x0010
#define	F_OLHEAD	0x0020
#define	F_unused1	0x0040
#define	F_SHOWCTLCHARS	0x0080

#define	F_UNDERLINE	0x4000
#define	F_BOLD		0x8000

#define	LEV_HIGHER	-1
#define	LEV_PREVSAME	0
#define	LEV_LOWER	1
#define	LEV_PREVPLUSONE	2
#define	LEV_RELEASE	3
#define	LEV_FORCECR	4

#define	CURS_ACTUAL	0
#define	CURS_WANT	1

#pragma	pack(1)

struct lp {
	int	LinIndent;
	int	HeadWidth;
	int	TextOffset;
	int	LinFlags;
};

struct gp {
	unsigned int	InitTextBufLen;	/* initial len of text buff in paras */
	char far	**TextBuf;	/* near ptr to far ptr to text buff  */

	int	PaneY;			/* first line of TEXT pane display   */
	int	PaneX;			/* first column of TEXT pane display */

	int	PaneHeight;		/* height of display TEXT pane       */
	int	PaneWidth;		/* width of display TEXT pane        */

	int	FontCurrent;		/* current font for TEXT within pane */
	int	RowSpacing;		/* # pixels between rows vertically  */

	unsigned int	MainFlags;
					// bit	0 Word Wrap on
					//	1 Show Ctl characters
					//	2 Ruler displayed
					//	3 Status bar displayed
					//	4 case sensitive search
					//	5 verify save as
					//	6 buffer modified
					//	7 Don't display title
					//	8 TextBuffer is growable
					//	9 InsMode
					//     10 Need screen update
					//     11 Cursor Flag
					//     12 Box around TextPane
					//     13 NoDraw (don't draw anything)
					//     14 Valid LinPtr structure
					//     15 buffer is formatted

// THE UPPER BYTE OF THE 3 FOLLOWING 'Flags' CONTAINS THE ACTUAL 'current'
// FORMATTING INFORMATIOIN.  THE LOWER BYTE IS USED AS A HANDY PLACE TO STORE
// GLOBAL DISPLAY FORMATTING INFORMATION FOR USE DURING DISPLAY/PRINT
// GENERATION.

	unsigned int	PaneFlags;
	unsigned int	CursFlags;
	unsigned int	MarkFlags;
					// bit	 0 
					//	 1 
					//	 2 
					//	 3 
					//	 4 
					//	 5 THIS LINE IS OUTLINE HEADER
					//	 6 DON'T OUTPUT FORMAT DATA
					//	 7 SHOW-CTL-CHARS
					//	 8 
					//	 9 
					//	10 
					//	11 
					//	12
					//	13
					//	14 UNDERLINE on
					//	15 BOLD on

	int	XTitleLoss, YTitleLoss;
	int	TextPaneX, TextPaneY;
	int	TextPaneCharHeight, TextPaneCharWidth;

	int	CursWidth;
	int	TextPaneRowCount, TextPaneColCount;

	int	TextPaneRow;		/* 1st row of TEXT buff in TEXT pane */
	int	TextPaneCol;		/* 1st col of TEXT buff in TEXT pane */
	int	TextCursRow;		/* cur curs line within TEXT buff    */
	int	TextCursCol;		/* cur curs column within TEXT buff  */
	int	TextCursColWant;	/* desired column                    */
	int	TextMarkRow;
	int	TextMarkCol;
	int	TextMarkPaneRow;
	int	TextMarkPaneCol;

	int	LastRulerCol;		/* last Pane Column at which ruler was displayed */

	int	OtherFlags;		//      bit
					//	 0 Mark SET
					//	 1 Shift DOWN

	unsigned int	PanePage, PaneOffset, CursOffset;
	unsigned int	MarkPage, MarkPaneOffset, MarkOffset;
	unsigned int	EndPage, EndOffset;

	unsigned int	LeftMargin, RightMargin, TopMargin, BottomMargin;
	unsigned int	LinesPerPage, PrintLines, PrintPages;

// Numeration is:
//		=1 DECIMAL outline numeration
//		=2 ROMAN NUMERAL outline numeration

	unsigned int	Numeration, Indent;

	unsigned int	FontSave;
	unsigned int	CursorBlink;
	unsigned int	HeadLen, FootLen;
	unsigned char	Header[128];
	unsigned char	Footer[128];
	unsigned char	CharBuf[2*GLED_OUTBUFLEN];

	unsigned char	Tabs[32];

	struct	lp	 LinPtr[27];

	void		(far *ExtendFunc)();
	unsigned char	ExtendID;

// set prnbuff==NULL before sending GLED_PRINT command to print to printer
// set prnbuff ptr to far ptr to buffer and prnbufflen=buffers_length to print
//   to a buffer.

	char far	**prnbuff;
	unsigned int	prnbufflen;

// set prnhandle==0 before sending GLED_PRINT command to print to printer
// set prnhandle pointing to FILE ptr to open SysMgr file handle and
//   prnbuff==NULL to print to file

	void	far	**prnhandle;

	unsigned int	OldCursX1, OldCursY1, OldCursX2, OldCursY2;

	unsigned int	BackEndOffset, BackPaneOffset, BackCursOffset;
	unsigned int	BackPanePage, BackEndPage;
	unsigned int	BackCursFlags, BackPaneFlags;
	unsigned int	BackInitTextBufLen;
	char	far	**BackTextBuf;
	unsigned int	BackES, BackBX, BackENDFILE;

	unsigned char	*BoldOn, *BoldOff, *UnderOn, *UnderOff;

	unsigned int	SaveTPR, SaveTCR, SaveTCC;

	char	far	**StatusStrings;
	unsigned int	spare1, spare2, spare3, spare4;
};

#pragma	pack()

typedef struct gp GLEDSTUFF;

typedef GLEDSTUFF * PGLEDSTUFF;

#define	GLED_SETFONT	0x454B
#define	GLED_SETFORM	0x454C
#define	GLED_DEL	0x454D
#define	GLED_CUT	0x454E
#define	GLED_COPY	0x454F
#define	GLED_PASTE	0x4550
#define	GLED_PROMOTE	0x4551
#define	GLED_DEMOTE	0x4552
#define	GLED_INSBLK	0x4553
#define	GLED_DELBLK	0x4554
#define	GLED_GETBLK	0x4555
#define	GLED_ISBLK	0x4556
#define	GLED_REGEXT	0x4557
#define	GLED_RELEXT	0x4558
#define	GLED_PRINT	0x4559
#define	GLED_INSBUF	0x455A
#define	GLED_SETMOD	0x455B
#define	GLED_CLRMOD	0x455C
#define	GLED_UPDATEFORMAT 0x455D
#define	GLED_GOTOLINE	0x455E
#define	GLED_FINDPREV	0x455F
#define	GLED_FINDNEXT	0x4560
#define	GLED_GETFORMAT	0x4561
#define	GLED_SETFORMAT	0x4562
#define	GLED_KILLBUFFER	0x4563
#define	GLED_TOGTABCURS	0x4564
#define	GLED_GOTO1ST64K	0x4565
#define	GLED_SETTABCURS	0x4566
#define	GLED_UPDATERULER 0x4567
#define	GLED_SELECTALL	0x4568
#define	GLED_ACTIVATECURS 0x4569
#define	GLED_WRAPIT	0x456A
#define	GLED_UNWRAPIT	0x456B
#define	GLED_PRINTINIT	0x456C
#define	GLED_GETFORM	0x456D
#define	GLED_DEACTIVATECURS 0x456E
#define	GLED_FIXNULLHEAD	0x456F
#define	GLED_SCROLL	0x4570
#define	GLED_FINDNEXTHERE	0x4571

#define	CMD_NOBOLD	0
#define	CMD_BOLD	1
#define	CMD_NOUNDER	2
#define	CMD_UNDER	3

#define	GLEDERR_BASE		0x656b
#define	GLEDERR_MAXERR		0x656e

#define	GLEDERR_NOCLIPBRD	0x656b	/* error opening clipboard */
#define	GLEDERR_BIGCUT		0x656c	/* marked area too large for clipbrd */
#define	GLEDERR_NOMEM		0x656d	/* insufficient memory for insert */
#define	GLEDERR_INTERRUPT	0x656e	/* printing interrupted by key */

/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
/*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*/
/* Warning: the following macros have been known to cause  */
/* sterility, blindness, cardiac arrythmia, partial hair   */
/* loss, severe stomach ulcers, loss of feeling in the     */
/* extremities and skin rashes.   Use at your own risk.    */
/*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*!*/

#ifndef CLASS_BASE
#error You must #include interfac.h before you #include g.h
#endif

/* G_DIRECT is set to 1 if the calls go directly to the library */

#ifndef TKERNEL
#define  G_DIRECT(ovl_num) (G_OVL==ovl_num)
#else
#define  G_DIRECT(ovl_num) (1)
#endif


/* If G_OVL is not defined, set it to 0 so that the macros expand properly */
#ifndef G_OVL
#define G_OVL 0
#endif


/* This nonsense is what is pushed on the stack internally by the SysMgr */
/* when it calls a function.  It must passed by value as the first param */
/* so that it lines up with that the stack already looks like.  NOTE: This */
/* "lines up" with the appropriate fields ONLY when the DB function has */
/* been called from outside.  That is, if a DB function is called from an */
/* app, SMAPI.AppDS (for example) will be equal to the app's DS when */
/* inside the function.  THIS IS NOT TRUE for any internal calls, because */
/* these calls are made directly, and junk is copied onto the stack to */
/* satisfy the stack positioning.  The effect of this is that NONE OF THESE */
/* VALUES CAN BE RELIABLY CHECKED/MODIFIED. */

typedef struct {
	long BankSwitchInfo;
	_segment AppDS;
	long LHSTUBReturnAddr;
	int CPUFlags;
	long AppReturnAddr;
	int ClassFnNumber;
} SMAPI;


/* From outside the engine, the call is "normal"; from inside the engine, */
/* the dummy SMAPI parameter must be passed to line up the stack.  The two */
/* versions are for calls with 0 and for 1 or more parameters.  SMPREFIX is */
/* used in the prototype, and SMCALL is used in the function call.  Note that */
/* if the DB_LIB option is defined, no dummy param is used--only if it is */
/* compiled for a system manager overlay. */

#ifndef TKERNEL
	#define GSMPREFIX0 SMAPI gDummyParm
	#define GSMPREFIX1 SMAPI gDummyParm,
	#define GSMCALL0   gDummy
	#define GSMCALL1   gDummy,
#else
	#define GSMPREFIX0
	#define GSMPREFIX1
	#define GSMCALL0
	#define GSMCALL1
#endif


/* The following sets up the call tables for the SysMgr--two classes (ovls) */

#define SC_GLEDSKI ((CLASS_BASE + 43)*256 + FNCT_BASE)
#define SC_GASM    ((CLASS_BASE + 44)*256 + FNCT_BASE)


#define F_GLED_TEXTHANDLERTSR      (SC_GLEDSKI + 0)
#define	F_GLED_THSPARE1		   (SC_GLEDSKI + 1)
#define	F_GLED_THSPARE2		   (SC_GLEDSKI + 2)
#define	F_GLED_THSPARE3		   (SC_GLEDSKI + 3)

#define F_GLED_SHOWRULER           (SC_GASM + 0)
#define F_GLED_INITCURSOR          (SC_GASM + 1)
#define F_GLED_SHOWMODIFIED        (SC_GASM + 2)
#define F_GLED_SHOWCAPS            (SC_GASM + 3)
#define F_GLED_SHOWBOLD            (SC_GASM + 4)
#define F_GLED_SHOWUNDER           (SC_GASM + 5)
#define F_GLED_SHOWLINCOL          (SC_GASM + 6)
#define F_GLED_UPDATELINCOL        (SC_GASM + 7)
#define F_GLED_SHOWINSERT          (SC_GASM + 8)
#define F_GLED_SHOWASCFORM         (SC_GASM + 9)
#define F_GLED_GETBOLDUNDER        (SC_GASM + 10)
#define F_GLED_JUSTFORMAT          (SC_GASM + 11)
#define F_GLED_PRINTINIT           (SC_GASM + 12)
#define F_GLED_PRINTTEXT           (SC_GASM + 13)
#define F_GLED_CURSON              (SC_GASM + 14)
#define F_GLED_LOCATECURSOR        (SC_GASM + 15)
#define F_GLED_CURSOFF             (SC_GASM + 16)
#define F_GLED_RENUMOUTLINE        (SC_GASM + 17)
#define F_GLED_MOVECURSCURLINE     (SC_GASM + 18)
#define F_GLED_MOVETOSOL           (SC_GASM + 19)
#define F_GLED_INSERTBYTE          (SC_GASM + 20)
#define F_GLED_INSERTBUFMOVCURS    (SC_GASM + 21)
#define F_GLED_REPLACEBYTE         (SC_GASM + 22)
#define F_GLED_DELETEBYTE          (SC_GASM + 23)
#define F_GLED_DELETEBLOCK         (SC_GASM + 24)
#define F_GLED_GETBLOCK            (SC_GASM + 25)
#define F_GLED_ISBLOCK             (SC_GASM + 26)
#define F_GLED_MOVEPANEDOWN        (SC_GASM + 27)
#define F_GLED_SCROLLTEXTUP        (SC_GASM + 28)
#define F_GLED_SCROLLTEXTDOWN      (SC_GASM + 29)
#define F_GLED_ISATCR              (SC_GASM + 30)
#define F_GLED_ONWORD              (SC_GASM + 31)
#define F_GLED_ATEOF               (SC_GASM + 32)
#define F_GLED_SKIPONEFF           (SC_GASM + 33)
#define F_GLED_FORCEFORMATBLOCK    (SC_GASM + 34)
#define F_GLED_SETFORMATBLOCK      (SC_GASM + 35)
#define F_GLED_GETFORMATBLOCK      (SC_GASM + 36)
#define F_GLED_REMOVEBOLDUNDER     (SC_GASM + 37)
#define F_GLED_COMPRESSBOLDUNDER   (SC_GASM + 38)
#define F_GLED_BACKSKIPBOLDUNDER   (SC_GASM + 39)
#define F_GLED_UNLFIT              (SC_GASM + 40)
#define F_GLED_LFIT                (SC_GASM + 41)
#define F_GLED_LFTEXT              (SC_GASM + 42)
#define F_GLED_CUT2CLIP            (SC_GASM + 43)
#define F_GLED_DRAWTITLE           (SC_GASM + 44)
#define F_GLED_DRAWFRAME           (SC_GASM + 45)
#define F_GLED_SETCURLINELEVEL     (SC_GASM + 46)
#define F_GLED_CHKNULLHEAD         (SC_GASM + 47)
#define F_GLED_MOVEPANE            (SC_GASM + 48)
#define F_GLED_ATSTARTOFHEADER     (SC_GASM + 49)
#define F_GLED_DRAWTEXTPANE        (SC_GASM + 50)
#define F_GLED_SETTEXTPANEFONT     (SC_GASM + 51)
#define F_GLED_INSERTBLOCK         (SC_GASM + 52)
#define F_GLED_INSERTBLOCKMOVCURS  (SC_GASM + 53)
#define F_GLED_MOVECURSOR          (SC_GASM + 54)
#define F_GLED_SETCURS             (SC_GASM + 55)
#define F_GLED_MARKPTRSIZE         (SC_GASM + 56)
#define F_GLED_MAKEPTR             (SC_GASM + 57)
#define F_GLED_SPARE1              (SC_GASM + 58)
#define F_GLED_SPARE2              (SC_GASM + 59)
#define F_GLED_SPARE3              (SC_GASM + 60)
#define F_GLED_SPARE4              (SC_GASM + 61)
#define F_GLED_SPARE5              (SC_GASM + 62)
#define F_GLED_SPARE6              (SC_GASM + 63)


/* SCCS Info - Module appts.h      Version 1.36      Date 08/17/93      Time 10:03:40 */
/************************************************************
 *                                                          *
 *                           tt                    hh       *
 *                           tt                    hh       *
 *  aaaa   pppppp  pppppp  tttttt   ssssss         hh       *
 *     aa  pp   pp pp   pp   tt    ss              hh hhh   *
 *  aaaaa  pp   pp pp   pp   tt     sssss          hhh  hh  *
 * aa  aa  pppppp  pppppp    tt         ss   ..    hh   hh  *
 *  aaaa a pp      pp        tt    ssssss    ..    hh   hh  *
 *         pp      pp                                       *
 *                                                          *
 ************************************************************/


#include "datetime.h"
#include "errors.h"
#include "fileio.h"

typedef void _far REPFUNC (char *);

/*** Data Structures ***/

#pragma pack(1)
/* Appointment/Event/ToDo struct */
typedef struct {     
  WORD  item_length; 
  int   categoryoffset;
  int   locationoffset;
  int   repeatoffset;
  int   noterecordnum;
  int  prevrecord_link;  /* For 1-time item, record# for prev item this day */
  int  nextrecord_link;  /* For 1-time item, record# for next item this day */
/* Indicates appointment (as opposed to to-do item), alarm enable, 
      display in weekly/monthly view, existence of associated to-do */
  unsigned char flags;    
  union {         
/* year# since 1900; month 0-11; day# 0-30 */
   unsigned char start_date[3];	
   unsigned char orig_date[3];
  } sharedfield0 ;
  union {
   int  start_time;      /* minutes since midnight */
   char priority[2];
  } sharedfield1 ;
  union {
   unsigned int end_date;        /* #days since start date */
   unsigned int due_date ;       /* #days since orig. date */
  } sharedfield2 ;
  union {
   int end_time;         /* minutes since midnight */
   unsigned char ckoff_date[2];  /*  1st 2 bytes of ckoff_date (y#/m#) */
   int todostub_link; /* For rpting todo->pts to 1st completed stub in chain */
  } sharedfield3 ;
  union {
   int leadtime;         /* #minutes (0-1439 for 23hrs 59 min) */
   unsigned char ckoff_date2[2];  /* 3rd byte of ckoff_date (day#); 4th byte unused */
  } sharedfield4 ;
  char repeat_type;
} APPOINTMENT;
#pragma pack()

/* Structure used to store Next Appt info */
typedef struct {
   unsigned char start_date[3];
   int start_time;
   unsigned char description[52];
} MINIAPPT ;

/* Structure used for SglOccurDate Table pointed to by *sgloccurdateblock */
#pragma pack(1)
typedef struct {
  unsigned char date[3];
  int  recordNo;
} DAYSTART ;
#pragma pack()

#pragma pack(1)
/* Structure used for Current_Items Table pointed to by *Current_Items */
typedef struct {
  unsigned int offset;
  int recordNo;
  unsigned char flags;
} CURRITEMINFO ;
#pragma pack()

#pragma pack(1)
/* Structure used for Multi_Items Table pointed to by *Multi_Items */
typedef struct {
  unsigned int offset;
  int recordNo;
} MULTIITEMINFO ;
#pragma pack()

#pragma pack(1)
/* Structure used for storing configuration information for a given file */
typedef struct {
  unsigned char Today[3];
  WORD Settings;
  WORD BeepSettings;
  WORD CompViewSettings;
  int  FirstHour;
  int  Duration;
  int  Leadtime;
  char Priority[2];
  int  FirstRptRecordNo;
  int  SglOccurDaysCount;
  int  EventLines;
} APPTBKFILEINFO;
#pragma pack()

/* Structure used for storing a 'virtual day' of appts/events in ApptScreen */
typedef struct {
  int time;
  int itemNo;
  unsigned char flags;
} SCREENLINE ;

#define maxdescriplen  52
#define maxlocationlen 20
#define maxcategorylen DB_MAXSTRING
#define maxitemascii   (maxdescriplen+maxlocationlen+maxcategorylen)

/* Structure used for storing card information during an item edit */
#pragma pack(1)
typedef struct {
/* flags indicates appt/event/todo, alarm enable(appts),week view(appts),
      month view(appts), carryforward(todo), etc */
  char filler1[6];
  /* Offset 6 is reserved for repeat offset */
  int kludgrptoffset;
  char filler2[6];
  /* Offset 14 is reserved for flags */
  unsigned char flags;             
  /* Offset 15 is reserved for tokenized RptStart for setting 1st 
     occurrence for a proposed repeating item */
  unsigned char startdatetoken[3]; 
  char filler3[8];
  /* Offset <descripoffset-1> is reserved for the repeat type */
  char repeat_type;
  /* Offset <descripoffset> is reserved for repeatinfo */
  unsigned char rptfreq;
  unsigned char rptdays;
  unsigned char rptweeks;
  unsigned int rptmonths;  
  unsigned char rptstart[3];
  unsigned char rptend[3];
  /* Offset descripoffset+11 is reserved for #deleted occurrences */
  int kludgdeloccur;
  /* The remaining buffers are garden variety ascii card buffers */
  char bydayno[4];
  char frequency[5];
  char start_date[15];   
  char start_time[10];   
  char end_date[15];
  char end_time[10];
  char lead_time[8];
  char description[maxdescriplen];
  char location[maxlocationlen];
  /* Unused in this version, except for ascii null */
  /* Allocate 256 bytes for category to allow for backward compatibility, in
     the event a later version of Cougar implements Category in the appt book */
  char category[maxcategorylen];
  char duration_start[15];
  char duration_end[15];
  char bydate[15];
} APPTBKCARDINFO;
#pragma pack()

typedef APPOINTMENT far *APPTPTR;
typedef int near *RECORDNOPTR;
typedef unsigned char near *DATEPTR;
typedef unsigned long ulong;

#define FILEWILDCARD "*.ADB"

#define NOTIFY_LOSEFOCUS  NOTIFY_USER

/******** Commands ******/

#define CMD_ADDAPPT       0xCF    /* Add an appointment */
#define CMD_ADDEVENT      0xCE    /* Add an event */
#define CMD_ADDTODO       0xCD    /* Add a todo item */
#define CMD_PREVAPPTDAY   0xCC    /* Go to prev day w/ appts */
#define CMD_NEXTAPPTDAY   0xCB    /* Go to next day w/ appts */
#define CMD_STRINGFOUND   0xCA    /* Display item containing specified text */
#define CMD_RESURRECT     0xC9    /* Resurrect range of deleted occurrences */
#define CMD_CHECKOFF      0xC8    /* Check-off a todo item */
#define CMD_TEST          0xC7    /* Test alarm beep in Alarm Beep Dialog */
#define CMD_INIT          0xC6    /* Initial Find */
#define CMD_VIEWAPPT      0xC5    /* View Appointment Screen */
#define CMD_VIEWTODO      0xC4    /* View ToDo Screen */
#define CMD_VIEWWAG       0xC3    /* View Week Screen */
#define CMD_VIEWMAG       0xC2    /* View Month Screen */
#define CMD_VIEWSMAG      0xC1    /* View 6-Month Screen */
#define CMD_VIEWMAGEVENT  0xC0    /* View Event Screen */
#define CMD_PRINT         0xBF    /* Prepare to print item(s) */
#define CMD_SCLIP         0xBE    /* Put up Smart Clip dialog */

/* Update based on current time/date & draw */
#define DRAW_UPDATE       (DRAW_CLIENT | DRAW_TITLE | DRAW_CURSOR | DRAW_USER)


/******** Defines *******/

/* Window Dimensions */

#define DAG_NEXTAPPT_Y   (DAG_CALWIN_Y+DAG_CALWIN_H)
#define DAG_NEXTAPPT_H   24

#define DAG_THERM_X	 0
#define DAG_THERM_W      32
#define DAG_THERM_Y      19

#define DAG_SMALLITEM_Y  (DAG_CALWIN_Y+DAG_CALWIN_H)
#define DAG_SMALLITEM_H  (FKEY_TOP-DAG_SMALLITEM_Y)

/* Card Field Numbers */
#define StartTimeField  1
#define EndTimeField    2
#define LocationField   3
#define LeadTimeField   6
#define ApptNoteField   7
#define ApptDateField   8
#define ConsecDaysField 9
#define ToDoDateField   1
#define DueDateField    2
#define PriorityField   3
#define ToDoNoteField   4

#define REPEAT_FIELD         (USER_FIELD+2)
#define STARTDATE_FIELD      (USER_FIELD+3)
#define DUEDATE_FIELD        (USER_FIELD+4)
#define COMPLETIONDATE_FIELD (USER_FIELD+5)
#define PRIORITY_FIELD       (USER_FIELD+6)
#define CONSECDAY_FIELD      (USER_FIELD+7)
#define LEADTIME_FIELD       (USER_FIELD+8)
#define STARTTIME_FIELD      (USER_FIELD+9)
#define ENDTIME_FIELD        (USER_FIELD+10)

#define BiggestUInt          (65535)

int far MyCallBack(DBBLOCK *dblk, int action, int buffersize, int intarg,
                     void far *ptrarg1, void far *ptrarg2);

void SetHelpWndFocus(WORD helpnum);
void far DoHelp(void);
void far HelpIndex(void);
void far HelpGetStarted(void);
void far HelpOnHelp(void);
void far HelpAbout(void);

int far MakeFieldNameList(char **ptrlist,char *fieldnames,int *fielddefindexlist,
                                                      PWINDOW dwin, int numwnd);
int ApptSmartClipSetup(char **ptrlist, char *fieldnames, int *fielddefindexlist);
int EventSmartClipSetup(char **ptrlist, char *fieldnames, int *fielddefindexlist);
int ToDoSmartClipSetup(char **ptrlist, char *fieldnames, int *fielddefindexlist);
void GenericClipSetup(char ***ptrlist, char **fieldnames, 
           int **fielddefindexlist, int fieldstart, int fieldend);

int far HelpHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far NoteHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far ApptBookFieldDisp(DBBLOCK *db, char far *rec, int fld, char far *str,
			    int bufsize, int recnum);
int far ApptBookFieldNameDisp(int fld,char far *str,int bufsize);

/* Error Msg Defines */
/*----------------------------------------------------------------------
* error handling stuff
*----------------------------------------------------------------------*/
int far ErrorHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);

extern PRES ApptErrMsgArray[ERR_ARRAY_COUNT];

#define initsglblksize	   1500         // about 30 sgl occurrence items
#define initmultiblksize   3500         // about 70 multi occur items
#define initsgloccurtblsize 150         // enough for 30 days
#define initcurritemsize  (mincurritemdim*7*sizeof(CURRITEMINFO)) // 700
#define initmultiitemsize (minmultiitemdim*sizeof(MULTIITEMINFO)) // 280

#define mincurritemdim       20         // 700 bytes- 20 items ea of 7 days
#define minmultiitemdim      70         // 280 bytes

/* To Bring Up Application: Need approx 6K of Memory */

#define sglblkincr 	256             // enough for approx 5 items 
#define multiblkincr    256             // enough for approx 5 items
#define curritemsincr   (5*7*sizeof(CURRITEMINFO)) // 175
#define multiitemsincr  (5*sizeof(MULTIITEMINFO))  //  20

#define max(A,B)        ((A) > (B) ? (A) : (B))
#define min(A,B)        ((A) < (B) ? (A) : (B))
#define curritemdim     (curritemsend/(7*sizeof(CURRITEMINFO)))
#define multiitemdim    (multiitemsend/(sizeof(MULTIITEMINFO)))  

#define PrevToDoRecPtr(pToDo)	((int far *) ((char far *)(pToDo)+(pToDo)->item_length-6))
#define NextToDoRecPtr(pToDo)	((int far *) ((char far *)(pToDo)+(pToDo)->item_length-4))
#define ToDoSpwnrRecPtr(pToDo)	((int far *) ((char far *)(pToDo)+(pToDo)->item_length-2))
#define RptFreqPtr(pItem)       ((unsigned char far *)(pItem) + (pItem)->repeatoffset)
#define RptDaysPtr(pItem)       ((unsigned char far *)(pItem) + (pItem)->repeatoffset + 1)
#define RptWeeksPtr(pItem)      ((unsigned char far *)(pItem) + (pItem)->repeatoffset + 2)
#define RptMonthsPtr(pItem)     ((unsigned int far *)((unsigned char far *)(pItem) + (pItem)->repeatoffset + 3))
#define RptStartPtr(pItem)      ((unsigned char far *)(pItem) + (pItem)->repeatoffset + 5)
#define RptEndPtr(pItem)        ((unsigned char far *)(pItem) + (pItem)->repeatoffset + 8)
#define DeletedOccurPtr(pItem)  ((unsigned char far *)(pItem) + (pItem)->repeatoffset + 11)
#define CurrItemRecNo(itemno,dayofweek)   ((*Current_Items)[(7*(itemno))+(dayofweek)].recordNo)

/* Ensure that sgl occurrence items of same priority, occur before multi-occurrence items */
#define ToDoOrder(dayofweek,itemno,pToDo) ( (itemno<ItemCount[dayofweek]) && (PriorityCmp((GetPtr(dayofweek,itemno))->sharedfield1.priority,(pToDo)->sharedfield1.priority)<0) )
#define ToDoOrder2(dayofweek,itemno,pToDo) ( (itemno<ItemCount[dayofweek]) && SglOccurItem(GetPtr(dayofweek,itemno)) && (!PriorityCmp((GetPtr(dayofweek,itemno))->sharedfield1.priority,(pToDo)->sharedfield1.priority)) )

#define m_ApptScrnOffset   (ApptScrnIndx+CursorLineAppt-EventLines-1)
#define maxApptScrnOffset  (max(0,LastApptScrnEntry-14)+EventLines)

#define	fNoRepeat	0
#define	fDaily		1
#define fWeekly		2
#define fMonthly	3
#define fYearly		4
#define fCustom		5


#define maskWkdays	0x03E0
#define maskAllDays	0x0FE0
#define maskAllWeeks	0x001F
#define maskAllMonths   0x0FFF

#define fLastWeek       0x10     // Repeat on Last Week of month

#define maskDayList     128

#define ByDayNo         1
#define ByDayPosition   2

#define fAlarmSymbol    1

#define TIMEMGMT	0x32
#define TYPE_APPTBKINFO  TYPE_USER    // 0x0E
#define TYPE_APPTLINKDEF TYPE_LINKDEF // 0x0C
#define TYPE_TODOLINKDEF TYPE_USER+1  // 0x0F

#define Noon	          720
#define LastMinofDay     1439
#define MidNight         1440
#define LastDayNumber   73048

#define fOrigDateChge    0x01
#define RptStubSize        50	// descripoffset + 6 link bytes + 12 + extra
#define delitemsize         4

/* flags for GetNextDaywItems */
#define fFind            0x01
#define fUseDayPassed    0x02
// #define fNextAppt        0x04 -- defined below

/* Relevant to appts/events/todos */
#define fToDo   0x10
#define fEvent  0x20
#define fAppt	0x80
#define fAlarm	0x01
#define fRptToDoStub  0x40

/* Relevant only to todos */
/* Save a bit just in case we implement IN PROGRESS status for todos */
#define fCheckedOff   0x02
#define fCarryForward 0x04

/* Relevant only to appts/events */
#define fMonth  0x02
#define fWeek	0x04

#define fByDayName 0x80

#define ToDoDetailDlgMask    0x0800   // Carry Forward Default status
#define DetailDlgMask        0x07C0   // AG Defaults & Alarm Enable
#define TimelineDlgMask      0x3F  // WeekStart & Timeline Setting
#define WeekStartMask        0x30
#define TimelineSettingsMask 0x0F
#define EventCardFlagsMask   0x06  // week/month view for events
#define ApptCardFlagsMask    0x07  // week/month view for appts & alarm status

#define fMultiOccurItem 0xF

#define fSmallItems   0x03
#define fCompOptions  0x0C
#define fClock        0x01
#define fCalendar     0x02
#define fNextAppt     0x04
#define fDAGtherm     0x08
#define maskApptCnfg  0x00FF
#define maskToDoCnfg  0xFF00

#define ViewsMask     0x7000

enum ViewValues {
  DAGVIEW,
  TODOVIEW,
  WAGVIEW,
  MAGVIEW,
  SMAGVIEW,
  EVENTSVIEW,
} ;

/* Appointment Book Timeline Modes */
#define fAonly        0x08
#define f60min        0x04
#define f30min        0x02
#define f15min        0x01

#define fSunday 0x20

#define fCol2Duration 0x80

#define descripoffset (sizeof(APPOINTMENT))
#define startdateoffset 15

#define apptsize         (descripoffset+12)    // repeatinfo is 12 bytes long
#define maxnodeletedrpts 100
#define biggestapptsize (apptsize+maxitemascii+max(440,(maxnodeletedrpts*4)))

#define EditMenuCurrItemOffset 0
#define EditMenuDeleteOffset   1
#define EditMenuUndoOffset     2
#define EditMenuCutOffset      3
#define EditMenuCopyOffset     4
#define EditMenuPasteOffset    5
#define EditMenuRstRptOffset   6
#define EditMenuToDoCkOffset   7

#define ViewMenuApptOffset   0
#define ViewMenuToDoOffset   1
#define ViewMenuWagOffset    2
#define ViewMenuMagOffset    3
#define ViewMenuSMagOffset   4
#define ViewMenuEventsOffset 5
#define ViewMenuNoteOffset   6

#define SearchMenuPrevItemOffset    0
#define SearchMenuNextItemOffset    1
#define SearchMenuPrevApptDayOffset 2
#define SearchMenuNextApptDayOffset 3
#define SearchMenuGotoOffset        4
#define SearchMenuFindOffset        5
#define SearchMenuRepeatFindOffset  6

#define OptionsMenuRptOffset        6
#define OptionsMenuSmartClipOffset  7

extern PWINDOW NewRepeatDlg, TopView;

extern WINDOW ToDoCardDialog[], ApptCardDialog[];
extern WINDOW PrintDialog,FindDialog,LinkDlg,GotoDlg,ViewptDlg,ErrorDlgArray[];
extern WINDOW RemoveItemsArray[];
extern WINDOW WSmallToDo,WSmallAppt,WNextAppt,WClock,WDagTherm;
extern WINDOW WIndexView,WIndexViewToDo,WToDoCard,WApptCard,ErrorDlg,WNoFile;
extern WINDOW WagView, SMagView, MagView, EventsView;
extern WINDOW WRptItemUnDelete, WRemoveItems, WExtractItems;
extern WINDOW WApptPrint;

extern APPTBKCARDINFO tempappt;
extern SCREENLINE ApptScreen[200];
extern MINIAPPT NextApptWinInfo;
/* Repeat Variables */
extern BOOL RepeatFlags, FirstAm, FirstPm, AddEvent, FindFlag, AlarmUpdateFlag;
extern char APPTSACTIVE,ApptSearchstring[],FileSpec[],EnvFileName[];
WORD APPTINDEX_WIDTH, TODOINDEX_WIDTH;

extern DAYSTART far **sgloccurdateblock;
extern char tempbuffer[RptStubSize+maxitemascii];
extern unsigned int sglblkend, sglblkptr, multiblkend, multiblkptr;
extern unsigned int curritemsend, multiitemsend;
extern int OrigApptNo,OrigToDoNo,OrigData_Item,OrigNextToDoNo,OrigCursorLine,Field;

extern unsigned int Position;
extern char far **sglblock, far **multiblock;
extern char far **Months[], far **DaysofWeek[];
extern unsigned char Current_Day[3],Today[3],OldCurrent_Day[3];
extern unsigned char RptFreq, RptDays, RptWeeks, RptStart[3], RptEnd[3];
extern unsigned int  RptMonths;

extern unsigned char BigRecordBuffer[];

extern unsigned char TempStartDate[3];

extern const unsigned char EndOfTime[3];
extern long int WeekStart,OrigWeekStart;
extern int DayinWeek, Curr_Time, CursorLineAppt, CursorLineToDo, EventLines;
extern int EventCount[7], ApptCount[7], ToDoCount[7], ItemCount[7], MultiCount;
extern int ApptScrnIndx;
extern int DayChange, Timeline_Incr; 
extern int DailyEntry, bytes_read;
extern int ItemNo, FirstSlotInScreen, CurrentToDoNo, NextToDoNo;

extern PFUNC pRptItemEditedUtil;

extern WORD TempSettings;
extern int  TempFirstHr, TempIncr;

extern BOOL ReDoTimeLine, Data_Item, EditCard, Done, DelayDone;
extern CURRITEMINFO far **Current_Items;
extern MULTIITEMINFO far **Multi_Items;
extern CAPBLOCK CapData;
extern char far **BasicRptOptions[6];
extern APPTBKFILEINFO ApptBkInfo;  

char far ** far GetMemory(unsigned int size,WORD flags,
                                    unsigned int duration,unsigned int *offset);
void far ApptCopyToCB(int itemno,BOOL cutflag);
void far ApptPasteFromCB(unsigned char itemtype);
void far ApptBkUndo(void);
void far DoSmartClip(void);
void far CreateFileAskPasswordDialog(void);
void GetOldItemInfo(APPTPTR pItem);
void far RestorePreFindState(void);
void far SavePreFindState(void);
void far ChangeTimelineUtil(void);
void far NextApptDraw(WORD DrawType);
void far DateChangeUtil(void);
void far UpdateDailyView(char apptflag);
void AddItem(int itemnumber, int recordnumber, APPTPTR pData, BOOL existingrecord);
void DeleteItem(int dayofweek,int itemnumber);
void far UpdateOrigDates(void);
void far EndUnDelRptOptionsDlg(void);
void far DoRemove(void);
void far DoExtract(void);
void far EndRemoveDlg(void);
void far EndExtractDlg(void);
void far EndApptBookPrintDlg(void);
APPTPTR far GetPtr(int dayofweek, int itemnumber);

BOOL far VerifyPassword(char *password);
void far NewPassword(char *oldpassword,char *newpassword);
void far DoNewPassword(void);
void far DoNew2Password(void);
void far DoNew3Password(void);
void far DoAskPassword(void);
void far CreateFileNewPasswordDialog(void);
void far CreateFileAskPasswordDialog(void);
void VariationOnPassword(PRES DialogTitle,PRES EditTitle,PFUNC Doit);
int  far PasswordEdit(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);

void far MergeFileShell(char *password);
void far DoMerge(void);
void far DoReplacement(void);
void DontOverwrite(REPFUNC *whatToDo);
void far DoCopy(void);
void far DoNew(void);
void far DoTranslate(void);

BOOL far NextApptOutDated(void);
BOOL far PrevToDoDay(WORD lastitem);
BOOL far NextToDoDay(WORD firstitem);
BOOL far PrevApptDay(WORD lastitem);
BOOL far NextApptDay(WORD firstitem);
BOOL far GetNextDay(void);
BOOL far GetPrevDay(void);
BOOL far GetAdjacentWeek(int days);
BOOL far DateArith(unsigned char far *date, long int deltadays);
BOOL far SetUndoPointUtil(void);
BOOL WAGParent(void);
BOOL PriorityCmp(char far *priority1, char far *priority2);
BOOL GetNextDaywItems(unsigned char *nextday,unsigned char apptflag,
             BOOL flags, unsigned char *findstring);
BOOL GetPrevDaywItems(unsigned char *nextday,unsigned char apptflag,
             BOOL find, unsigned char *findstring);
void ShrinkCurrItems(void);
void ShrinkSglOccurBlk(void);
void ShrinkMultiOccurBlk(void);
void ShrinkMultiItems(void);
void MemMove(unsigned int destoffset, unsigned int sourceoffset, unsigned int blocksize, BOOL flag);
void DispPriority(char far *priority,char *string);
void SetPriority(char far *buffer,char *sourcebuffer);
BOOL CheckPriority(char *digit);
BOOL CheckPriorityUtil(char *digit);
BOOL RealStringFound(APPTPTR pItem);
APPTPTR GetCurrentAppt(void);
APPTPTR GetCurrentToDo(void);
int  CurrDayTodayCmp(void);
void DoPrint(void);
#define FBUFSIZ   512

BOOL PrintRecord(int dayofweek,int itemno,BOOL tofile,FILE far *fp);
BOOL PrintString(char *buffer,int len,BOOL tofile,FILE far *fp,int newlines);
void DoTopMargin(void);
int  ApptRemoveMrSunshine(unsigned char *buffer);
void PrinterSetUtil(WORD codes);
void UndoUtil(void);
void AssignDate(unsigned char *datedest,unsigned char *datesource);
void ApptBkCloseUtil(void);
void ViewSetUtil(void);
int far MakeCustomApptList(void);
void far FlushKBUtil(void);
void far WriteApptCard(void);
void far WriteToDoCard(void);
void far GotoUtil(PWINDOW Wnd);
void far IndexViewInvisible(void);
void far FormatDate(char *string,unsigned char *date);
void ReleaseAllFarMem(void);
void DrawRadioButtons(void);
void SetFocusUtil(int controlnum);
void UnGrayAlarmFields(void);
void GrayAlarmFields(void);
void UnGrayViewMenu(int viewmenugray,BOOL indexlevel);
void DrawNone(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
void DrawIncrementedTime(PWINDOW Wnd);
void PrintRadio1GetFocusUtil(void);
void SyncUpClipStyles(char linkdeftype);
void IndexHorizArrowFini(WORD arrowcode,WORD settings);
void AddExtension(PWINDOW Wnd);
void DosCopyUtil(char *filename);

/*** Window handlers ***/

int GB_RB1FieldUtil(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra,PCLASS ObjectHandler);
int GB_RB2Field1Util(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra,PCLASS ObjectHandler);
int GB_RB2Field2Util(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra,PCLASS ObjectHandler);
int GB_PostGBObjectUtil(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra,PCLASS ObjectHandler);
int far GB_PostGBPathEditHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_PostGBPushButtonHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_PostGBCheckBoxHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_PostGBDateHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_IndexPrintRadioHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_DetailPrintRadioHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_PrinterRadioHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_FilePrintRadioHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_PrintCopiesHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_CurrentItemPrintRadioHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_SpecifiedItemsPrintHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB1DateBox1Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2DateBox1Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2DateBox2Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB1NumberHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB1YearlyDateHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2ComboBox1Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2ComboBox2Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2CheckBox1Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far GB_RB2CheckBox2Handler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far PrintComboBoxHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);

int far PrintHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far PrintFileHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far PrintSetupHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far ConfirmHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far RptChgeConfirmHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far NoFileHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far CustomRptDlgHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far CustomRptRngDlgHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far EnterKeyHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far DelRptDlgHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far EditRptDlgHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far UnDelRptDlgHandler(PWINDOW Wnd,WORD Message,WORD Data,WORD Extra);
int far CompViewDlgHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far CompViewRadioButton(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far RemoveDialogHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far OneTrueFileDlgHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far ArchiveCheckBoxHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far RptDialogBox(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far TimelineDlgHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far DAGThermometerHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far IndexViewHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far IndexViewHandlerToDo(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far ApptBkIndexHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far ApptCardHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far ToDoCardHandler(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far ApptBkCard(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far BgnTimeHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far EndTimeHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far DueDateEdit(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far DateHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far PriorityHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far YearlyDateHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far DayNumberHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far SmallToDoHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far SmallApptHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far NextApptHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far Numeric(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far TimeHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far FindDlgHandler(PWINDOW Window, WORD Message, WORD Data, WORD Extra);
int far RptTypeRBHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far AlarmBeepHandler(PWINDOW Wnd, WORD Message, WORD Data, WORD Extra);
int far CustomRptCheckBox(PWINDOW Window,WORD Message, WORD Data, WORD Extra);
int far NextNewToDoThisDay(int dayofweek,int todono);
int far PrevNewToDoThisDay(int dayofweek,int todono);

int DelItemLinkAdjust(APPTPTR pData,int itemnumber,int dayofweek,char flags);
int SglOccurSlot(unsigned char far *date);
void InitEndDuration(unsigned char far *date,unsigned char far *durationenddate);
void WindowDrawUtil(PWINDOW Wnd);
void DrawAll(PWINDOW Wnd);
void DrawAllWindows(void);
void DrawUpdate(PWINDOW Wnd);
void DestroyCurrDlgUtil(void);
void CreateWindowUtil0(PWINDOW Wnd);
int  far CreateWindowUtil(PWINDOW Wnd);
void far DestroyNoDrawUtil(PWINDOW Wnd);
void far UpdateToDoScreen(void);
void far UpdateAllScreens(void);
void far ApptChangedDraw(void);
void far InitNextApptInfo(void);
void far PrintNotifyCardDone(void);
void far AddToDoFromNonToDoIndex(void);
void far AddToDoFromApptCard(void);
void far AddToDoFromToDoCard(void);
void far AddApptFromToDoCard(void);
void far AddApptFromApptCard(void);
void far AddEventFromToDoCard(void);
void far AddEventFromApptCard(void);
int ProcessDoneToDo(APPTPTR pToDo);
int ProcessUnDoneToDo(APPTPTR pToDo,int dayofweek);
void far ToDoChkOff(void);
int far GetDaysinYr(unsigned char *date);
void StoreItemInfo(APPTPTR pItem,BOOL keepdeletes);
void SetItemRepeat(APPTPTR pItem,BOOL keepdeletes);
BOOL TimeIncrement(PWINDOW Wnd, WORD keystroke, BOOL dothud);
BOOL ShowAnyEvents(void);
void WriteFileInfoToDisk(void);
void WriteSglOccurTableToDisk(void);
void InvertIndexViewLine(void);
void InvertToDoLine(void);
void show_nextslot(void);
void show_prevslot(void);
char * far TimeDisp(int minutes);
void DrawApptDuration(int index, int offset, int endtime);
BOOL DispAppt(int Minutes,APPTPTR Apptptr);
void AdjustApptScrn(int index);
void DiskErrRecov(void);
void far GetWeekErrRecovUtil(BOOL fullblown);
void GetWeekErrRecov(void);
void DiskOrMemErrRecov(void);
void UpdateOrigDate(int i,unsigned char far *prevtoday);
int TLArryOverrun(void);
int CreateApptFieldDefs(void);
int  far NoteDoneUtil(int itemno,BOOL writedatarecord);
void far CreateTimeLineArray(void);
void IncrDailyApptView(int keystroke, int incr);
void IncrDailyToDoView(int keystroke, int incr);
void far FormatPriorityNoteInfo(APPTPTR pToDo, char *string);
void HighlightApptUtil(int scrnindx);
void DAGScroll(PWINDOW Wnd,int dir,int rect_height,BOOL f_apptscroll);
void DrawEvent(PWINDOW Wnd, int displayline);
void FormatDescAndLoc(APPTPTR pItem, char *string);
void DrawApptItem(PWINDOW Wnd,int displayline,BOOL noduration);
void DrawToDoItem(PWINDOW Wnd,int displayline);
void DrawPriorDuration(void);
void ShowEventsScrolled(void);
void DrawApptScreen(PWINDOW Wnd,WORD Data);
void DrawToDoScreen(PWINDOW Wnd,WORD Data);
BOOL UpdateToDos(void);
BOOL far ApptBkClosed(void);
BOOL far CheckAllFields(PWINDOW Wnd);
BOOL far CardModified(PWINDOW Wnd);
void far SetRepeatDlgsToNotEdit(void);
void MarkCardAsSaved(void);
void LocalClippingUtil(PWINDOW Wnd);
void DefaultLocalClipping(void);
void DispApptBkTitle(PWINDOW Wnd);
void far IndexViewVisible(void);
void far DispApptTopClientLine(PWINDOW Wnd);
BOOL OrigToDoIndexUtil(WORD keystroke);
BOOL OrigToDoDetailUtil(WORD keystroke);
BOOL PrevApptThisDay(void);
BOOL NextApptThisDay(void);
void far GetApptDetail();
void far GetToDoDetail();
void EmptyListHighlight(PWINDOW Wnd);
void ApptBkCardDraw(PWINDOW Wnd, WORD *Data);
void CreateUnDeleteRptWindow(int itemno);
void SetTempRptVars(void);
void SetApptStartDate(void);
void SetToDoStartDate(void);
BOOL far FirstOccur(APPTPTR pItem);

void far NextApptCard(void);
void far PrevApptCard(void);
void far NextApptDayCard(void);
void far NextApptDayCard0(void);
void far PrevApptDayCard(void);
void far PrevApptDayCard0(void);
void PrevApptDayUtil(WORD Extra);
void NextApptDayUtil(WORD Extra);

void far NextToDoCard(void);
void far PrevToDoCard(void);
void far NextToDoDayCard(void);
void far NextToDoDayCard0(void);
void far PrevToDoDayCard(void);
void far PrevToDoDayCard0(void);
void PrevToDoDayUtil(WORD Extra);
void NextToDoDayUtil(WORD Extra);

void ContinueFindUtil(void);
void far EndNextFindDlg(void);
void far EndPrevFindDlg(void);
void far EndInitFindDlg(void);
void far RepeatLastFind(void);
void far ForwardSearchUtil(void);
void far BackwardSearchUtil(void);

BOOL far SetApptError(int error);
void far DestroyNote(void);
void far DestroyApptCard(void);
void far DestroyToDoCard(void);
void far DestroyApptCardNoDraw(void);
void far DestroyToDoCardNoDraw(void);
void far FormatAppt(APPTPTR pItem,char *string);
void far FormatToDo(APPTPTR pItem,char *string);
void far ItemAttributeStringUtil(APPTPTR pItem, char * string);
void far CancelDialog(void);
void far PosItoA(unsigned int number, char * string, int base);
void DspViewDlgUtil(PWINDOW Wnd);
void far DspApptCnfgDlg(void);
void far DspToDoCnfgDlg(void);
void far DspAlarmBeepDlg(void);
void far DspSnoozeDlg(void);
void far DspTimelineDlg(void) ;
void far DispRptOptions(void);
void far DoQuit(void);
void far EndDialog(void);
void far EndToDoCard(void);
void far EndCard(void);
void far EndRptOptionsDlg(void);
void far GetExistingApptInfo(void);
void far GetExistingToDoInfo(void);
void far EditItem(void);
void far InitRptDlg(int index);
void DrawCard(void);
void DrawBoxesinCustom(int startwindow, int endwindow);
void far CheckAllBoxes(void);
void far CheckWkdayBoxes(void);
void far ClearAllBoxes(void);
int  far GetCurrentDayNumber(void);
void far CreateIndexView(void);
void far CreateIndexViewToDo(void);
void far VisibleApptIndexView(void);
void far VisibleToDoIndexView(void);

void far DoBeep(void);
void far DelWConfirm(void);
void far DelApptBookItem(BOOL cutflag);
void far CreateToDoDetailDlg(void) ;
void far CreateApptDetailDlg(void) ;

long int far GetWeekAndOffset(unsigned char far *date, int *dayofweek);
BOOL far WriteRecordUtil(char recordtype,int recordno,void far *src,unsigned int recsize);
BOOL far WriteInPlaceUtil(char recordtype,int recordno,unsigned int offset,void far *src,unsigned int size);

#pragma alloc_text(APPHANDLERS_TEXT,IndexViewHandler,IndexViewHandlerToDo,\
   ApptBkIndexHandler,ApptBkCard,IndexHorizArrowFini,PrinterSetUtil,\
   ApptCardHandler,ToDoCardHandler,BgnTimeHandler,EndTimeHandler,UpdateToDos,\
   SmallToDoHandler,SmallApptHandler,GetDaysinYr,TimeIncrement,\
   ShowAnyEvents,InvertIndexViewLine,InvertToDoLine,show_nextslot,show_prevslot,\
   TimeDisp,DrawApptDuration,DispAppt,AdjustApptScrn,TLArryOverrun,\
   CreateTimeLineArray,IncrDailyApptView,IncrDailyToDoView,DoQuit,\
   HighlightApptUtil,DrawApptScreen,RestorePreFindState,EditItem,\
   DrawToDoScreen,DispApptBkTitle,LocalClippingUtil,GetPtr,FormatDate,\
   DefaultLocalClipping,Numeric,IndexViewVisible,IndexViewInvisible,\
   FindDlgHandler,TimeHandler,EndRptOptionsDlg,DrawCard,\
   RemoveDialogHandler,ArchiveCheckBoxHandler,GrayAlarmFields,DrawNone,\
   CompViewDlgHandler,CompViewRadioButton,UnGrayAlarmFields,DrawIncrementedTime,\
   RptDialogBox,RptTypeRBHandler,DispRptOptions,CustomRptCheckBox,\
   DrawBoxesinCustom,CheckAllBoxes,CheckWkdayBoxes,ClearAllBoxes,DateHandler,\
   DueDateEdit,YearlyDateHandler,DayNumberHandler,GetApptDetail,GetToDoDetail,\
   OrigToDoIndexUtil,OrigToDoDetailUtil,PrevApptThisDay,NextApptThisDay,\
   NextApptHandler,AddToDoFromNonToDoIndex,DispApptTopClientLine,\
   AddToDoFromApptCard,AddEventFromApptCard,AddApptFromApptCard,\
   AddToDoFromToDoCard,AddEventFromToDoCard,AddApptFromToDoCard,DestroyNote,\
   EmptyListHighlight,DestroyApptCard,DestroyToDoCard,CreateUnDeleteRptWindow,\
   DestroyToDoCardNoDraw,DestroyApptCardNoDraw,\
   ItemAttributeStringUtil,TimelineDlgHandler,CancelDialog,DspViewDlgUtil,\
   PosItoA,DspApptCnfgDlg,DspToDoCnfgDlg,DspAlarmBeepDlg,DspSnoozeDlg,EndDialog,\
   DspTimelineDlg,EndToDoCard,EndCard,SavePreFindState,NoteDoneUtil,\
   DrawPriorDuration,DrawApptItem,DrawToDoItem,DrawEvent,FormatDescAndLoc,\
   DAGScroll,CustomRptDlgHandler,FormatAppt,FormatToDo,ShowEventsScrolled,\
   GB_RB1DateBox1Handler,GB_RB2DateBox1Handler,GB_RB2DateBox2Handler,\
   GB_PostGBPathEditHandler,GB_PostGBCheckBoxHandler,GB_PostGBDateHandler,\
   GB_IndexPrintRadioHandler,GB_PostGBPushButtonHandler,ApptBkCardDraw,\
   GB_DetailPrintRadioHandler,GB_CurrentItemPrintRadioHandler,\
   GB_SpecifiedItemsPrintHandler,ApptRemoveMrSunshine,DoTopMargin,\
   DoPrint,MakeCustomApptList,PrintHandler,PrintRecord,PrintString,\
   NoFileHandler,ConfirmHandler,PrintSetupHandler,PrintFileHandler,\
   RptChgeConfirmHandler,GetCurrentDayNumber,\
   GB_PrinterRadioHandler,GB_FilePrintRadioHandler,GB_PrintCopiesHandler,\
   GB_RB1NumberHandler,GB_RB2ComboBox1Handler,GB_RB2ComboBox2Handler,\
   GB_RB1YearlyDateHandler,SetFocusUtil,DrawRadioButtons,GB_PostGBObjectUtil,\
   GB_RB2Field1Util,GB_RB2Field2Util,GB_RB1FieldUtil,\
   GB_RB2CheckBox1Handler,GB_RB2CheckBox2Handler,GB_RB2Handler,EnterKeyHandler,\
   EditRptDlgHandler,DelRptDlgHandler,UnDelRptDlgHandler,SyncUpClipStyles,\
   CustomRptRngDlgHandler,PrintRadio1GetFocusUtil,PrintComboBoxHandler,\
   NextApptCard,PrevApptCard,NextToDoCard,PrevToDoCard,\
   NextApptDayCard,NextApptDayCard0,NextApptDayUtil,\
   PrevApptDayCard,PrevApptDayCard0,PrevApptDayUtil,\
   NextToDoDayCard,NextToDoDayCard0,NextToDoDayUtil,\
   PrevToDoDayCard,PrevToDoDayCard0,PrevToDoDayUtil,\
   VisibleApptIndexView,VisibleToDoIndexView,GetWeekAndOffset,GetMemory,\
   NextNewToDoThisDay,PrevNewToDoThisDay,CreateApptDetailDlg,DateArith,\
   WriteRecordUtil,DelWConfirm,DoSmartClip,DrawUpdate,UpdateToDoScreen,\
   DestroyNoDrawUtil,SetHelpWndFocus,DoHelp,\
   HelpHandler,HelpIndex,HelpGetStarted,HelpOnHelp,HelpAbout,GenericClipSetup,\
   MakeFieldNameList,ToDoSmartClipSetup,EventSmartClipSetup,ApptSmartClipSetup,\
   ApptBookFieldNameDisp,UnGrayViewMenu,WindowDrawUtil,DrawAll,\
   VerifyPassword,NewPassword,DoNewPassword,DoNew2Password,DoNew3Password,\
   DoAskPassword,CreateFileNewPasswordDialog,CreateFileAskPasswordDialog,\
   VariationOnPassword,PasswordEdit,DoMerge,DoReplacement,\
   DontOverwrite,DoCopy,DoNew,DoExtract,DoRemove,DoTranslate,\
   ContinueFindUtil,EndNextFindDlg,EndPrevFindDlg,RepeatLastFind,EndInitFindDlg)

void far CreateApptPrintDialog(void);
void far CreateRemoveWnd(void);
void far CreateExtractWnd(void);
char ApptsActive(void);
void SetCurrentDay(void);
void DeleteAnyNote(APPTPTR pItem);
BOOL DelStubLinkAdjust(int stubrecno);
BOOL far KeyCheckUtil(void);
APPTPTR FindSpawner(int recordnumber);
void SaveRepeatStuff(void);
void RestoreRepeatStuff(void);
void ExitApptChgeDlg(void);
void DelSpawnerFromMem(APPTPTR pItem);
void DispEditRptOptions(APPTPTR pItem);
void DelRptFromCurrItems(unsigned char itemflags,int recordnumber);
void DelMultiItem(int MultiNo,unsigned char itemflags);
void ReadInMultiOccurItems(void);
void far UpdateCurrentDayUtil(void);
void SetNewDayParms(void);
void InitApptParms(void);
void DagInitialize(void);
int EditEntireAppt(BOOL editflag,BOOL keepdeletes);
int EditEntireToDo(BOOL editflag,BOOL keepdeletes,unsigned int spwnroffset,int recordno);
void WriteToDoToDisk(int recordno,APPTPTR pToDo,BOOL editflag);
void WriteApptToDisk(int recordno,APPTPTR pAppt,BOOL editflag);
int EditEntireToDoUtil(BOOL editflag,BOOL keepdeletes,unsigned int spwnroffset,
                                              int recordno,APPTPTR *pToDoPtr);
void far DelEntireItem(APPTPTR pItem,int itemnumber);
void DelSglOccurTblEntry(unsigned char far *date);
void InsertCurrentItem(APPTPTR pData,long int daynumber,int weekday,int recno);
int GetRepeatFactor(APPTPTR pItem);
int SetRepeatVars(APPTPTR pItem);
BOOL AccelerateDate(APPTPTR pItem, unsigned char *date, BOOL repeattype, 
       int * increment);
BOOL AccelerateDateBack(APPTPTR pItem, unsigned char *date, BOOL repeattype, 
       int * increment);
BOOL OKToOptimize(APPTPTR pItem,unsigned char *date);
BOOL OKToOptimizeBack(APPTPTR pItem);
BOOL RelevantRpt(APPTPTR pItem,unsigned char far *date,int RepeatType);
BOOL OccurThisDay(APPTPTR pItem,unsigned char far *date,int RepeatType);
BOOL NewToDo(APPTPTR pToDo,unsigned char *date);
BOOL DeleteRecordUtil(char recordtype,int recnum);
BOOL ReadRecordUtil(DBBLOCK *dbblock,char recordtype,int recnum,void far *dest,unsigned int bufsize);
int  AddRecordUtil(char recordtype,void far *src,unsigned int recsize);
void UnCkOffRpt(APPTPTR pToDoStub);
void CkOffRpt(APPTPTR pToDoSpawner);
void QualifyingOccur(APPTPTR pItem,unsigned char *bgndate,unsigned char *enddate);
void ChangeRptRange(APPTPTR pItem,unsigned char *bgndate,unsigned char *enddate,int datecmp);
void DelOneEndOfRptToDoUtil(int recordno,int stubrecno,
                  unsigned char *bgndate,unsigned char *enddate,int datecmpres);
void DelRptUtil(APPTPTR pItem, int recordnumber,BOOL memadjusttoo);
void InitApptBkInfo(void);
void far GetWeek(long int WeekStart, BOOL NoToDos);

void far SwitchSMag(void);
void far SwitchMag(void);
void far SwitchWag(void);

void far CreateEventCard(void) ;
void far CreateNewApptCard(void);
void far CreateNewToDoCard(void);
void far CreateCardView(void);
void far CreateToDoCard(void);

void far EndEditRptOptionsDlg(void);
void far EndDelRptOptionsDlg(void);
void far EndEditRptOptionsDlg(void);
void far EndTimelineDlg(void);
void far EndDetailDlg(void);
void far EndToDoDetailDlg(void);
void far EndBeepDlg(void);
void far EndSnoozeDlg(void);
void far EndApptCnfgDlg(void);
void far EndToDoCnfgDlg(void);
void far EndNextFindDlg(void);
void far EndPrevFindDlg(void);
void far EndInitFindDlg(void);
void far RepeatLastFind(void);

void far CheckAllBoxes(void);
void far ClearAllBoxes(void);
void far CheckWkdayBoxes(void);

void far CreateFileCopyDialog(void);
void far CreateFileNewDialog(void);
void far CreateFileOpenDialog(void);
void far CreateFileRenameDialog(void);
void far CreateFileDeleteDialog(void);


/**** TopMenu "hangs" all the previous menus off itself with MENU_PULLDOWN ****/

extern MENU FileMenu[], EditMenu[], AddMenu[], ViewMenu[], SearchMenu[], 
            OptionsMenu[], HelpMenu[], TopMenu[];


/**** Function key structure for Index view softkeys ****/
extern FKEY IndexFKeys[], IndexToDoFKeys[], CardFKeys[], ToDoCardFKeys[],
            DialogFKeys[], BeepDlgFKeys[], SnoozeDlgFKeys[],
            ApptDefaultFKeys[], ToDoDefaultFKeys[],
            CustomRptFKeys[],AddlCustomRptFKeys[],
            ApptCnfgFKeys[],ToDoCnfgFKeys[],FindFKeys[],ApptBookPrintFKeys[],
            NoteFKeys[],AddItemFKeys[],ViewptDlgFKeys[];




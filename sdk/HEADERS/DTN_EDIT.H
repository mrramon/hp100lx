
/*** These are style bits for the Time edit field.  These can be used
     in addition to the normal Edit styles. ***/

#define TIMEEDIT_ABSOLUTE 0x00          /* Default -- Input is actual times */
#define TIMEEDIT_OFFSET   0x08          /* Input is hours:mins, but no am/pm */

#define TIMEEDIT_UNSIGNED 0x00          /* Default -- Times are unsigned */
#define TIMEEDIT_SIGNED   0x10          /* Only for TIMEEDIT_OFFSET--times */
                                        /*   may have preceeding +/- for negative */
                                        /*   offsets */

#define TIMEEDIT_PARSE    0x00          /* Default -- Times are interactively */
                                        /*   parsed at each keystroke. */
#define TIMEEDIT_NOPARSE  0x20          /* Times are parsed on leaving the */
                                        /*   field only. */

#define TIMEEDIT_HOURS    0x00          /* Default -- Numbers with no other punc.*/
                                        /*   are interpreted as hours */
#define TIMEEDIT_MINUTES  0x40          /* Numbers with no punc are minutes */


#define TIMEEDIT_NOBLANKS    0x00       /* Default--No blank times allowed */
#define TIMEEDIT_ALLOWBLANK  0x80       /* Blank times are allowed */




/*** These are style bits for the Date edit field.  These can be used
     in addition to the normal Edit styles. ***/

#define DATEEDIT_PARSE    0x00          /* Default -- Dates are interactively */
                                        /*   parsed at each keystroke. */
#define DATEEDIT_NOPARSE  0x20          /* Dates are parsed on leaving the */
                                        /*   field only. */

#define DATEEDIT_YEAR        0x00       /* Default--The year is required (defaulted if left off)*/
#define DATEEDIT_NOYEAR      0x40       /* The year is left off for entry of yearly repeating dates */


#define DATEEDIT_NOBLANKS    0x00       /* Default--No blank dates allowed */
#define DATEEDIT_ALLOWBLANK  0x80       /* Blank dates are allowed */





/* Formatting flags for timestrtomin and mintotimestr */

#define TIME_FORMAT    0    /* Time is a real time */
#define OFFSET_FORMAT  1    /* Time is an offset in hours and minutes (can be negative) */

#define HOURS_FORMAT   0    /* A number w/ no punc. is assumed to be hours */
#define MINUTES_FORMAT 2    /* A number w/ no punc. is assumed to be minutes */

#define PAD_FORMAT     4    /* Blank-pad 1-digit hour */
#define SUPPRESSTOD_FORMAT 8 /* Suppress am/pm designator */



/*** These are style bits for the Number edit field.  These can be used
     in addition to the normal Number styles.  If there is a maximum
     value for the field, it should be placed in the window's Menu field.
     If no maximum is given (Menu==0), the maximum will be determined
     from the field size. ***/

#define NUMBEREDIT_UNSIGNED   0x00    /* Default--signed numbers not allowed */
#define NUMBEREDIT_SIGNED     0x08    /* Negative numbers allowed */

#define NUMBEREDIT_NOBLANKS   0x00    /* Default--No blank numbers allowed */
#define NUMBEREDIT_ALLOWBLANK 0x10    /* Blank numbers are allowed */

#define NUMBEREDIT_NOZERO     0x00    /* Default--Unsigned only; Range is 1..Menu*/
#define NUMBEREDIT_ZERO       0x20    /* Zero is included in the range */

#define NUMBEREDIT_NOFLOAT    0x00    /* Default--Number is an integer only; +/- keys inc/dec number */
#define NUMBEREDIT_FLOAT      0x40    /* Number is floating point--,/. are allowed, +/- keys don't inc/dec number */







#define INVALID_TIME    0x8000
#define INVALID_DATE    0x8000
#define ADDED_YEAR       1
#define INVALID_INT     (-1)
#define OVERFLOW_INT    (-2)

/* Converts a time string to a number of minutes.  Flags are TIME_FORMAT,
     OFFSET_FORMAT, HOURS_FORMAT, MINUTES_FORMAT (see above).
     timestrtomin returns INVALID_TIME (0x8000) if the time is invalid.
     The time does not have to be in the current SysMgr time format. */
int far timestrtomin(char far *buffer, int flags);

/* Converts a number of minutes to a string, formatted according to the current
     SysMgr time format.  Flags are TIME_FORMAT and OFFSET_FORMAT. */
int far mintotimestr(char far *string, int minutes, int format);


/* Converts a date string to a 3 byte date.
     strtodate returns INVALID_DATE (and fills the date struct with 
     0xFF,0xFF,0xFF) if the time is invalid.  If the date is invalid,
     the return value is either 0, or ADDED_YEAR to indicate the
     year position was missing.  NOTE: the date MUST BE
     the current SysMgr date format.  */
int far strtodate(char far *buffer, char far *date,int flags);

/* Converts a date struct to a string, formatted according to the 
     current SysMgr time format.  If DATEEDIT_NOYEAR flag is passed, no 
     year is used (even if the current SysMgr format defines it so).  If 
     the flag is not set, a year is inserted (even if the current SysMgr 
     format is missing a year field).  */
int far datetostr(char far *string, char far *date, int flags);


/* Converts an int to a string. */
int far itoa(int number,char far *string);

/* Converts a string to an int.  The return result is 0 if the string
     was valid.  INVALID_INT (-1) or OVERFLOW_INT (-2) are returned if
     the string could not be processed. */
int far atoi(char far *string,int *number);


/* Used to set the split in TimeEdit where non-specified am/pm is defaulted.
     The default is 7*60 or 7:00.  E.g. for times with no am/pm only, if they are
     before 7:00, they are assigned "PM"; times >=7:00 are assigned "AM". */
void far SetTimeEditBreakpoint(int minutes);


char GetTimeSep(void);
BOOL GetIntl(void);
BOOL GetDecPoint(void);
int InsertTime(char *buffer, int *pos, int key, long int flags);
BOOL AddTime(PWINDOW Wnd, int adjust);
BOOL AddNum(PWINDOW Wnd, int checkonly, int adjust);
void far DoEditDelete(PWINDOW Window);
int far SyncCursor(PWINDOW Wnd);

int far AdjustDate(char *dt, int adjust, int AlwaysLeap);



//#pragma alloc_text(CAPSEG, SetTimeEditBreakpoint,atoi,itoa,mintotimestr,timestrtomin,TimeEditHandler,NumberEditHandler,\
//                   GetTimeSep,GetIntl,GetDecPoint,InsertTime,AddTime,AddNum,DoEditDelete)

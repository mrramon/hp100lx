/***
 *** Miscellaneous functions dealing with date/time
 ***/

#ifndef _DB_TIMESTAMP_

#ifndef DB_OVL
#error  You should now #include db.h instead of #including datetime.h
#endif

typedef unsigned char DB_DATE[3]; /* Stored in same format as used by DB_DATE fields */
typedef int DB_TIME;              /* same format as DB_TIME fields */

typedef struct {
  DB_DATE date;
  DB_TIME time;
  } DB_TIMESTAMP;


int far _GetDate(DBSMPREFIX1  DB_DATE APP_NEAR *date);
#define GetDate(date) \
        (DB_DIRECT(1)? \
         _GetDate(DBSMCALL1 (date)): \
         c_service(F_DB_GETDATE,(void *)(date)))


int far _GetTime(DBSMPREFIX1  DB_TIME APP_NEAR *time);
#define GetTime(time) \
        (DB_DIRECT(1)? \
        _GetTime(DBSMCALL1 (time)): \
        c_service(F_DB_GETTIME,(void *)(time)))


int far _GetTimeStamp(DBSMPREFIX1  DB_TIMESTAMP APP_NEAR *ts);
#define GetTimeStamp(ts) \
        (DB_DIRECT(1)? \
        _GetTimeStamp(DBSMCALL1 (ts)): \
        c_service(F_DB_GETTIMESTAMP,(void *)(ts)))



int far _TimeToString(DBSMPREFIX1  DB_TIME far *time, char far *string);
#define TimeToString(time,string) \
        (DB_DIRECT(1)? \
        _TimeToString(DBSMCALL1 (time),(string)): \
        c_service(F_DB_TIMETOSTRING,(void far *)(time),(void far *)(string)))


int far _DateToString(DBSMPREFIX1  DB_DATE far *date, char far *string);
#define DateToString(date,string) \
        (DB_DIRECT(1)? \
        _DateToString(DBSMCALL1 (date),(string)): \
        c_service(F_DB_DATETOSTRING,(void far *)(date),(void far *)(string)))



int far _StringToDate(DBSMPREFIX1  char far *string, DB_DATE far *date);
#define StringToDate(string,date) \
        (DB_DIRECT(1)? \
        _StringToDate(DBSMCALL1 (string),(date)): \
        c_service(F_DB_STRINGTODATE,(void far *)(string),(void far *)(date)))


int far _StringToTime(DBSMPREFIX1  char far *string, DB_TIME far *time);
#define StringToTime(string,time) \
        (DB_DIRECT(1)? \
        _StringToTime(DBSMCALL1 (string),(time)): \
        c_service(F_DB_STRINGTOTIME,(void far *)(string),(void far *)(time)))


#define _DB_TIMESTAMP_
#endif

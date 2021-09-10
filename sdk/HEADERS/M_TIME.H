/* $Header*/
/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/*
 * $Log:   T:/HH/INCLUDE/VCS/M_TIME.H_V  $
 * 
 *    Rev 1.5   15 Oct 1992 10:51:54   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.4   28 Aug 1992 08:43:14   PETERR
 * Added pragma to force byte packing of structures
 * 
 *    Rev 1.3   03 Aug 1992 10:07:02   PETERR
 * Changed extra field def from long to int per Wee Lei's request
 * 
 *    Rev 1.2   30 Jun 1992 12:37:06   PETERR
 * Added new structure entry for distinction between old and new calls
 * 
 *    Rev 1.1   18 Jun 1992 16:24:44   KIM
 * Added NEWALARM structure, to synchronize with Peter Ricker's changes to
 * \HH\SYSMGR\sysmgr.h.
 * 
 *    Rev 1.0   07 May 1992 17:05:38   DAVID
 * Initial revision.
 * 
 */



#ifndef M_TIME
#define M_TIME


#pragma pack(1)

typedef struct {
	char	dt_order;		/* month-day-year order, see below */
	char	dt_dsep;		/* date separator */
	char	dt_tsep;		/* time separator */
	char	dt_24hr;		/* non-zero => 24-hour time */
} DTINFO;

#define DATE_MDY		0
#define DATE_DMY		1
#define DATE_YMD		2

typedef struct {
	int	dt_year;
	char	dt_month;
	char	dt_date;
	char	dt_day;
	char	dt_hour;
	char	dt_minute;
	char	dt_second;
	char	dt_hundreth;
} DTM;


typedef struct {
	char	t_hundreth;		/* 0 - 99 */
	char	t_second;		/* 0 - 59 */
	char	t_minute;		/* 0 - 59 */
	char	t_hour;			/* 0 - 23 */
} TIME;


/*  Note:  This structure is now obsolete and has been superceded by NEWALARM */
#define ALARM_MSG_LEN		40
#define ALARM_EXTRA_LEN		4

typedef struct {
	char	a_hour;			/* time of alarm, hours */
	char	a_minute;		/* time of alarm, minutes */
	char 	a_second;		/* time of alarm, seconds */
	char	a_pad;			/* supplied by caller */
	int 	a_interval;		/* reschedule interval (seconds) */
	char	a_use_seconds;		/* are seconds signficant */
	char	a_sound;		/* alarm sound */
	char	message[ALARM_MSG_LEN];	/* message displayed when alarm goes off */
	char	owner;			/* task id of the owner */
	char	special;		/* app's own use for sub-class */
	char	extra[ALARM_EXTRA_LEN];	/* app's own use for spcific data */
} ALARM;


/* -------------------------------------------------------------------
 *
 * NEWALARM - Replaces obsolete ALARM structure
 *
 *
 */

#define NALARM_MSG_LEN		36
#define NALARM_EXTRA_LEN	4

typedef struct {
	int	    na_year;		/* Year  (as returned by DOS)	*/
	char	na_month;		/* Month (as returned by DOS)	  */
	char	na_date;		/* Day of month (as returned by DOS)*/
	char	na_hour;		/* Time of alarm, hours	*/
	char	na_minute;		/* Time of alarm, minutes   */
	char	na_second;		/* Time of alarm, seconds   */
	char	na_pad;			/* (place holder for pack) */
	char	na_sound;		/* Alarm sound	*/
	char	na_message[NALARM_MSG_LEN];	/* Message displayed when alarm goes off */
	char	na_special; /* App's own use for sub-class. Passed back to
			       app through event notification */
	union	{
	unsigned int na_extral[2];
	char	na_extrac[NALARM_EXTRA_LEN]; /* App's own use for specific data
					       Passed back to app through
					       event notification */
		};
	char	na_old; 		/* old style alarm indicator */
	unsigned long int na_tot_secs; /* Total secs since 1-1-80 @12:00am */
	char	na_owner;		  /* Task id of the owner */
	} NEWALARM;


#define ALARM_FORCE	 	1
#define ALARM_IF_EARLY	 	2
#define ALARM_IF_CLEAR	 	4

#define DATE_ONLY	0
#define TIME_ONLY	1
#define DATE_N_TIME	2
#define DAY_N_DATE	3
#define SUPP_AMPM	0x10
#define SUPP_SECOND	0x20
#define SHOW_YEAR	0x40
#define FOUR_DIGIT_YEAR	0x80



typedef struct {
        DTM     oldtime;
        DTM     newtime;
} TIMECHANGE;

#pragma pack()

#endif


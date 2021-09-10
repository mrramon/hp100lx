/*
 * $Header:   T:/HH/SYSMGR/VCS/SYSMGR.H_V   1.9   04 Nov 1992 14:44:16   PETERR  $
 *
 * SYSMGR.H
 *
 * STRUCTURES, DEFINES, and FUNCTION PROTOTYPES for the System Manager.
 *
 * This file contains the above for only the "public" interfaces (API) to
 * the System Manager. Any interfaces that are "internal" to the System
 * Manager are defined elsewhere.
 *
 * Copyright(c) 1990,1991
 * Lotus Development Corporation, Inc., Cambridge, MA. 02142
 * All Rights Reserved.
 * ------------------------------------------------------------------------
 * $Log:   T:/HH/SYSMGR/VCS/SYSMGR.H_V  $
 * 
 *    Rev 1.9   04 Nov 1992 14:44:16   PETERR
 * Added new define for Ctrl'C rupt vector
 * 
 *    Rev 1.8   15 Oct 1992 10:15:20   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.ASM, HELPSTUB.ASM, and HELPDATA.INC have post-Build 19 changes.
 * 
 *    Rev 1.7   10 Sep 1992 16:58:56   HOMI
 * Updated m_get_cb_info() function declaration.
 * 
 *    Rev 1.6   28 Aug 1992 08:41:54   PETERR
 * Added pragma to force byte alaingment packing of structures
 * 
 *    Rev 1.5   30 Jul 1992 09:39:00   PETERR
 * Added union for new alarm structure to enable usage of extra field as
 * an unsigned long
 * 
 *    Rev 1.4   29 Jul 1992 16:29:38   HOMI
 * Added DATA_SEG_VECTOR_2 equate for int 7Fh
 * 
 *    Rev 1.3   07 Jul 1992 16:39:26   HOMI
 * New API changes.
 * 
 *    Rev 1.2   30 Jun 1992 12:33:46   PETERR
 * Added new NEWALARM structure entry: na_old -- to handle differnet processing
 * for old and new apps
 * 
 *    Rev 1.1   12 Jun 1992 11:36:20   PETERR
 * Added new alarm structure NEWALARM which obsoloetes old ALARM structure.
 * 
 *    Rev 1.0   03 Dec 1991 16:21:20   DAVID
 * Initial revision.
 *
 */

#define SYSTEM_VERSION_ID 0x0101		/* System Manager Version 1.01 */
#pragma pack(1)

#define YES 1
#define NO	0

#define COM1_INT_NUM    	0x0C
#define COM2_INT_NUM    	0x0B
#define C_BREAK_INT			0x1B
#define CTRL_C_INT			0x23
#define TIMER_INT_NUM		0x1C		/* use BIOS hook instead of INT 8 */
#define KB_INT_NUM			0x15
#define SERVICES_INT_NUM 	0x60
#define SM_DISPATCHER_INT	0x7E     /* homi */
#define DATA_SEG_VECTOR 	0x61
#define DATA_SEG_VECTOR_2 	0x7F     /* homi */
#define RTC_INT_NUM   		0x4A
#define DOSCRIT_INT_NUM		0x24
#define PRSCRN_INT_NUM		0x05
#define WAKEUP_HOOK_INT		0x06

/*
#define FIRST_ARG_LEN  12
*/
#define FIRST_ARG_LEN  20              /* homi */


/* -------------------------------------------------------------------
 *
 * Dummy argument for every service routine (C-like routines)
 *
 */
typedef struct { 
	char arg_array[FIRST_ARG_LEN - 2];         /* homi */
	} FIRST_ARG;

extern FIRST_ARG dummy;

/* -------------------------------------------------------------------
 *
 * MENU - Menu Services Section
 *
 */

	/* ASSUME the following:
	 *		MAX_KWDS - a maximum number of keywords
	 * 	MAX_MENU - a maximum number of lines in a menu
	 * 					(NOTE: two menu lines, plus long prompt)
	 * 	MAX_MWID - a maximum window width of 80
	 */
#define   MAX_KWDS	20
#define   MAX_MENU	3
#define   MAX_MWID	80

typedef struct {
	/* Define the menu display storage area.
	 * The intent is to have one string per display line to make display
    * management easier.
	 */
	char menu_text [MAX_MENU] [MAX_MWID];
	int  menu_count;       						/* Number of keywords.				*/
	int  menu_highlight;							/* Index of highlighted keyword.	*/
														/* (-1 for no highlight).			*/
	int  menu_tprompt;							/* Flag indicating special mode	*/
														/* with single prompt on top		*/
														/* line. ( 0 ==> no).				*/
	char menu_line   [MAX_KWDS];    			/* Line of menu this word is on 	*/
	char menu_offset [MAX_KWDS];    			/* Offset of keyword in string 	*/
	char menu_length [MAX_KWDS];    			/* Length of keyword 				*/
	char menu_letter [MAX_KWDS];    			/* First letter of keyword 		*/
	unsigned menu_prompt[MAX_KWDS];			/* Pointers to the long prompts.	*/
														/* DS-relative offsets.				*/
	} MENUDATA;

	/* "menu_key" returns the index of the selected keyword.  It returns
	 * a negative number if selection is not complete.
	 */

/* homi n2f start */
void	far menu_dis  (FIRST_ARG,MENUDATA far *);
int	far menu_key  (FIRST_ARG,MENUDATA far *,int,int far *);
void	far menu_off  (FIRST_ARG,MENUDATA far *);
void	far menu_on   (FIRST_ARG,MENUDATA far *);
int	far menu_setup(FIRST_ARG,MENUDATA far *,char far *,int,int,char far *,int,char * far *);
/* homi n2f end */

/* -------------------------------------------------------------------
 *
 * MDIT - Multi-line Edit Section
 *
 *
 */

#define MDIT_SPEC_NOTABEXP 1		/* Treat TAB as single char 					*/
#define EOL       			0

typedef struct {
	char	far *m_buffer;    		/* User-supplied edit buffer  				*/
	int	m_length;         		/* Length of buffer  							*/
	int	m_pos;            		/* Curr cursor position  						*/
	int	m_row, m_col;     		/* Location of edit area  						*/
	int	m_nrows, m_ncols; 		/* Dimensions of edit area  					*/
	int	m_yoff,m_disprows;		/* Curr top row, num of rows displayable 	*/
	char	m_ccol;           		/* Cursor column  								*/
	char	m_modified;       		/* 1 -> buffer has been modified  			*/
	char	m_xoff;		  				/* 1st disp. col (for ticker fields only)	*/
	char	wrapflag;	 	 			/* 1 -> wordwrap enabled,-1 ticker field 	*/
	int	far *m_line;      		/* Pointer to array used for line starts	*/
				  							/* must be at least m_nrows+1 long 			*/
	char	markon;		  				/* Currency marking? 							*/
	char	spec_flag;
	int	markst;		  				/* Offset of start of mark, inclusive 		*/
	int	markend;	  					/* Offset of end of mark, inclusive 		*/
	int	m_dispcols;	  				/* Displayable columns 							*/
	} MDITDATA;

/* homi n2f start */
void	far mdit_cutmark(FIRST_ARG,MDITDATA far *);
void	far mdit_dis    (FIRST_ARG,MDITDATA far *);
void	far mdit_fil    (void);
void	far mdit_ini    (FIRST_ARG,MDITDATA far *,int,int,int,int,char far *,int,int,int,int far *);
int	far mdit_ins_str(FIRST_ARG,MDITDATA far *,char far *,int);
int	far mdit_key    (FIRST_ARG,MDITDATA far *,int);
void	far mdit_mark   (FIRST_ARG,MDITDATA far *);
void	far mdit_unmark (FIRST_ARG,MDITDATA far *);
/* homi n2f end */

/* -------------------------------------------------------------------
 *
 * EDIT - Edit Manager (single line) Section
 *
 *
 */

 	/*
	 * ASSUME: the maximum number of cols in an edit area is 80
	 */

typedef struct {
	int	edit_length;				/* Curr length of the edit buffer 			*/
	char	first_time;					/* Flag: special processing on first char */
	char	spec_flags;					/* Bit 0 is tab handling 						*/
	int	prompt_window;				/* Belongs to the prompt window? 			*/
	char	far *message_line;		/* Top line message for prompt 				*/
	int	message_line_length;		/* Length of same 								*/
	char	far *prompt_line;			/* Second line of prompt window 				*/
	int	prompt_line_length;		/* Length of same 								*/
	char	edit_buffer[80];			/* Work space for the buffer 					*/
	int	line_array[2];				/* Line array needed by mdit 					*/
	MDITDATA	mdit;						/* Multi-line edit structure 					*/
	int	e_dispcols;					/* Displayable columns 							*/
	} EDITDATA;

	/*
	 * edit_key returns: "1" if user has accepted the edit buffer contents.
	 *     					"0" otherwise.
	 */

/* homi n2f start */
int far edit_dis (FIRST_ARG,EDITDATA far *);
int far edit_init(FIRST_ARG,EDITDATA far *,char far *,int,int,int,int);
int far edit_key (FIRST_ARG,EDITDATA far *,int,int far *);
int far edit_top (FIRST_ARG,EDITDATA far *,char far *,int,int,char far *,int,char far *,int);
/* homi n2f end */

/* -------------------------------------------------------------------
 *
 * FILEIO - File I/O Services Section
 *
 *
 */

	/*
	 * Open file control block
	 *
	 * The caller must not modify this block except by Mosaic system calls.
	 *
	 * The "physical" seek address is our copy of the actual DOS file offset.
	 * If we aren't sure of this value, it is -1.
	 *
	 * The "logical" seek address is the file offset of the block currently
	 * in the buffer (if the buffer contents are valid).
	 *
	 * The "virtual" seek address is the effective file offset as implemented
	 * by this buffered I/O package.
	 */

#define FBUFSIZ   512

typedef struct {
	int	f_handle;         	/* DOS file handle  							*/
	int	f_flag;           	/* flag word -- see below  				*/
	long	f_pseek;          	/* physical seek address  					*/
	long	f_lseek;          	/* logical seek address  					*/
	long	f_vseek;          	/* virtual seek address  					*/
	int	f_len;            	/* length of valid portion of buffer  	*/
	char	f_buf[FBUFSIZ];   	/* I/O buffer  								*/
	} FCB;

	/* unbuffered file  */
typedef char NBFILE[sizeof(FCB)-FBUFSIZ];

	/* flags  */
#define VALID     1                /* buffer contents are valid  		*/
#define DIRTY     2                /* buffer must be written  			*/
#define NOBUF     4                /* force unbuffered I/O  			*/
#define DEVICE    8                /* file is a device  					*/

	/* maximum file name length  */
#define MAXPATH   63
#define MAXNAM    (3+MAXPATH+1+8+1+3+1)     /*  d:\path\name.ext<nul>  	*/

	/* seek modes  */
#define SEEK_ABS  0              /* relative to the beginning  			*/
#define SEEK_REL  1              /* relative to the current position  	*/
#define SEEK_END  2              /* relative to end-of-file  				*/

	/*
	 * Pattern-match control block
	 *
	 */

typedef struct {
	char	m_reserved[21];   		/* data transfer area  						*/
	char	m_attr;                 /* file attribute  							*/
	int 	m_time;                 /* time modified  							*/
	int 	m_date;                 /* date modified  							*/
	long	m_size;                 /* file length  								*/
	char	m_name[13];             /* file name  									*/
	char	m_path[MAXNAM];   		/* full path name  							*/
	char	m_ofs;           		 	/* offset of last portion of path name	*/
	char	m_fn;             		/* DOS call number to use  				*/
	} MATCH;

	/*
	 * Codes returned by m_ident
	 *
	 */

#define iNONE     0
#define iFILE     1
#define iDIR      2
#define iDEV      3

	/* file attribute settings */

#define FA_NORMAL		0
#define FA_READONLY	1

/* homi n2f start */
int	far m_open   (FIRST_ARG,FCB far *,char far *,int,int,int);
int	far m_openro (FIRST_ARG,FCB far *,char far *,int,int,int);
int	far m_create (FIRST_ARG,FCB far *,char far *,int,int,int);
int	far m_fcreat (FIRST_ARG,FCB far *,char far *,int,int,int);
int	far m_read   (FIRST_ARG,FCB far *,char far *,unsigned,unsigned far *);
int	far m_write  (FIRST_ARG,FCB far *,char far *,unsigned);
int	far m_seek   (FIRST_ARG,FCB far *,int,long);
int	far m_tell   (FIRST_ARG,FCB far *,long far *);
int	far m_close  (FIRST_ARG,FCB far *);
int	far m_setpat (FIRST_ARG,MATCH far *,char far *,int,int);
int	far m_match  (FIRST_ARG,MATCH far *,int far *);
int	far m_ident  (FIRST_ARG,char far *,int,int,int far *);
int	far m_delete (FIRST_ARG,char far *,int,int);
int	far m_rename (FIRST_ARG,char far *,int,int,char far *,int,int);
int	far m_getdrv (FIRST_ARG,char far *);
int	far m_setdrv (FIRST_ARG,int);
int	far m_getdir (FIRST_ARG,int,char far *,int far *);
int	far m_setdir (FIRST_ARG,char far *,int);
int	far m_copydt (FIRST_ARG,FCB far *,FCB far *);
int	far m_fdate  (FIRST_ARG,FCB far *,long far *);
int	far m_putfdt (FIRST_ARG,FCB far *,long);
int	far m_getfdt (FIRST_ARG,FCB far *,long far *);
int	far m_volume (FIRST_ARG,char far *,long far *);
int	far m_mkdir  (FIRST_ARG,char far *,int,int);
int	far m_rmdir  (FIRST_ARG,char far *,int,int);
int	far m_getattr(FIRST_ARG,char far *,int,int,unsigned far *);
int	far m_setattr(FIRST_ARG,char far *,int,int,unsigned);
void	far m_get_sysdir(FIRST_ARG,char far *);
/* homi n2f end */

/* -------------------------------------------------------------------
 *
 * CLIPB - Clipboard Services Section
 *
 *
 */

/* homi n2f start */
int	far m_cb_read    (FIRST_ARG,int,unsigned,char far *,unsigned);
int	far m_cb_write   (FIRST_ARG,char far *,unsigned);
int	far m_close_cb   (void);
int	far m_fini_rep   (void);
int	far m_get_cb_info(FIRST_ARG,int far *,char far *,unsigned far *, 
                                                       unsigned far *);
int	far m_new_rep    (FIRST_ARG,char far *);
int	far m_open_cb    (void);
int	far m_rep_name   (FIRST_ARG,int,char far *,unsigned far *);
int	far m_rep_index  (FIRST_ARG,char far *,int far *,unsigned far *);
int	far m_reset_cb   (FIRST_ARG,char far *);
/* homi n2f end */

/* -------------------------------------------------------------------
 *
 * ALARM - Alarm Services Section
 *
 *
 */

/*  Note:  This structure is now obsolete and has been superceded by NEWALARM */
#define ALARM_MSG_LEN		40
#define ALARM_EXTRA_LEN		4

typedef struct {
	char	a_hour;						/* Time of alarm, hours */
	char	a_minute;					/* Time of alarm, minutes	*/
	char	a_second;					/* Time of alarm, seconds	*/
	char	a_pad;						/* Supplied by caller		*/
	int	a_interval;					/* Reschedule interval (seconds)*/
	char	a_use_seconds;				/* Are seconds significant?	*/
	char	a_sound;						/* Alarm sound	*/
	char	message[ALARM_MSG_LEN];	/* Message displayed when alarm goes off	*/
	char	owner;						/* Task id of the owner 						*/
	char	special;						/* App's own use for sub-class 				*/
	union	{
	unsigned long int a_extral;
	char	extra[ALARM_EXTRA_LEN]; /* App's own use for specific data
					       Passed back to app through
					       event notification */
		};

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
	int	na_year;		/* Year  (as returned by DOS)	*/
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
	unsigned long int na_extral;
	char	na_extrac[NALARM_EXTRA_LEN]; /* App's own use for specific data
					       Passed back to app through
					       event notification */
		};

	char	na_old; 		/* old style alarm indicator */
	unsigned long int na_tot_secs; /* Total secs since 1-1-80 @12:00am */
	char	na_owner;		  /* Task id of the owner */
	} NEWALARM;



typedef struct {
	char	dt_order;					/* Month-day-year order, see below 			*/
	char	dt_dsep;						/* Date separator 								*/
	char	dt_tsep;						/* Time separator 								*/
	char	dt_24hr;						/* Non-zero => 24-hour time 					*/
	} DTINFO;

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

/* homi n2d start */
int	far m_alarm (FIRST_ARG,ALARM far *,int);   /* OBSOLETE!!! Use m_nalarm */
int	far m_nalarm (FIRST_ARG,NEWALARM far *,int);
int	far m_xalarm(FIRST_ARG,unsigned char);
int	far m_dtinfo(FIRST_ARG,DTINFO far *);
int	far m_getdtm(FIRST_ARG,DTM far *);
/* homi n2d end */

/* -------------------------------------------------------------------
 *
 * Process Management Services Section
 *
 *
 */

typedef struct {
	char	t_hundreth;		/* 0 - 99 */
	char	t_second;		/* 0 - 59 */
	char	t_minute;		/* 0 - 59 */
	char	t_hour;			/* 0 - 23 */
	} TIME;

/* homi n2d start */
void	far m_init      (void);
void	far m_fini      (void);
void	far m_get_timer (FIRST_ARG,DTM far *,TIME far *,char far *);
void	far m_stop_timer(FIRST_ARG,TIME far *);
void	far m_telltime  (FIRST_ARG,int,int,int);
void	far m_get_sw    (FIRST_ARG,TIME far *,TIME far *,char far *);
int	far m_spawn     (FIRST_ARG,char far *,int,int,char far *);
void	far m_stop_sw   (FIRST_ARG,TIME far *);
void	far m_lock      (FIRST_ARG);
void	far m_unlock    (FIRST_ARG);
char far * far m_tell_anytime(FIRST_ARG,int,int,int,DTINFO far *,DTM far *);
/* homi n2d end */

/* -------------------------------------------------------------------
 *
 * Miscellaneous Services Section
 *
 *
 */

/* homi n2d start */
void	far showname(FIRST_ARG,char far *);
void	far drawbox (FIRST_ARG,char far *);
void	far m_errmsg(FIRST_ARG,int,char far *,int,int far *);

extern	void	far init_sysmgr_resources(void);
extern	int	far m_get_sysver(void);
extern	void	far m_set_daterule(FIRST_ARG, int);
extern	void	far m_set_timerule(FIRST_ARG, int);
extern	char far * far * far m_sys_rsrc_addr(void);
/* homi n2d end */
/* -------------------------------------------------------------------
 *
 *
 *
 *
 */

/* ------------------------------------------------------------------ */


#pragma pack()

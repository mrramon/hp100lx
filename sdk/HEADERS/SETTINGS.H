/* $Header:   T:/HH/INCLUDE/VCS/SETTINGS.H_V   1.27   19 Feb 1993 11:23:16   KIM  $ */
/****************************************************************
 *
 * Copyright(c) 1990  Lotus Development Corporation
 * All Rights Reserved
 *
 *  Technical Note:
 *
 *  When modifying this file, please make sure to synchronize those changes
 *  with ALL of the following files (11/9/92 Peter Kim):
 *
 *  1) \hh\include\settings.mac
 *  2) \hh\task0\task0.c
 *  3) \hh\sysmgr\qbtask.c
 *  4) \hh\sysmgr\tkernel.c
 *
 *  Also, please make sure the TAB settings on your editor is set to
 *  5,9,13 etc.
 *
 ****************************************************************/

/*
 * $Log:   T:/HH/INCLUDE/VCS/SETTINGS.H_V  $
 * 
 *    Rev 1.27   19 Feb 1993 11:23:16   KIM
 * Added ComIOAddr and CommINT fields.
 * 
 *    Rev 1.26   17 Dec 1992 17:30:58   HOMI
 * Added SMACRO_CHAIN_ERROR #define.
 * 
 *    Rev 1.25   16 Dec 1992 15:54:44   DAVID
 * Added fields for printer interface setting, macro modified flags, macro
 * filename.
 * 
 *    Rev 1.24   15 Dec 1992 17:30:48   DAVID
 * 
 *    Rev 1.23   14 Dec 1992 14:41:22   KIM
 * Changed format of CodePage and DOSCodePage: Code is is represented by
 * a binary decimal value, e.g. 0x0850 represents 850.
 * 
 *    Rev 1.22   09 Dec 1992 15:18:50   DAVID
 * Removed extraneous fields from redirector settings, moved
 * EnableDOSPort (or whatever it's called) together with the rest of
 * the misc. setup settings.
 * 
 *    Rev 1.21   30 Nov 1992 03:18:40   DAVID
 * Changed length of macro description field
 * 
 *    Rev 1.20   23 Nov 1992 10:12:44   SANDY
 * No change.
 * 
 *    Rev 1.19   20 Nov 1992 10:58:48   SANDY
 * Added new Baud values, new CodePage values, and into the struct, I added
 * EnableDOSPort.
 * 
 *    Rev 1.18   19 Nov 1992 11:40:14   SANDY
 * Added #defines for baud rates of 38400, 57600, 115200.
 * 
 *    Rev 1.17   16 Nov 1992 12:09:52   SANDY
 * Added #define INTERFALTN to be 0x4 in the printer interface section.
 * 
 *    Rev 1.16   09 Nov 1992 21:01:52   KIM
 * Added cMainMeter and cBackupMeter for use by SETUP.
 * 
 *    Rev 1.15   09 Nov 1992 15:22:26   PETERR
 * Moved yet another item below initted stuff.
 * 
 *    Rev 1.14   09 Nov 1992 15:01:10   PETERR
 * Moved DOS code page below inited vars
 * 
 *    Rev 1.13   05 Nov 1992 17:37:02   SANDY
 * Added DOSCodePage and CountryIndex to the Settings struct.
 * 
 *    Rev 1.12   30 Oct 1992 16:07:00   SANDY
 * Ooops, they changed it again, max baud rate for infrared is 9600 now.
 * 
 *    Rev 1.11   30 Oct 1992 15:53:38   SANDY
 * Changed MAX_IR_BAUD from BAUD2400 to BAUD4800.
 * 
 *    Rev 1.10   27 Oct 1992 14:49:54   SANDY
 * Changed CONTLEV15 from 0xf to 0x1f.
 * 
 *    Rev 1.0   23 Oct 1992 15:23:16   SANDY
 * Initial revision.
 * 
 *    Rev 1.9   22 Oct 1992 16:40:42   DAVID
 * Added LaunchServer field
 * 
 *    Rev 1.8   19 Oct 1992 16:52:18   PETERR
 * Added pragma pack to settings structure
 * 
 *    Rev 1.7   19 Oct 1992 02:20:48   DAVID
 * Changed size of memory settings to int
 * 
 *    Rev 1.6   18 Oct 1992 23:55:46   DAVID
 * Added memory fields (clipboard, static, DOS) to settings.struct
 * 
 *    Rev 1.5   15 Oct 1992 10:52:16   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.8   13 Oct 1992 15:03:08   DAVID
 * Fixed typo
 * 
 *    Rev 1.7   13 Oct 1992 14:50:52   DAVID
 * Added field ShowPic to Setting struct
 * 
 *    Rev 1.6   13 Oct 1992 00:54:14   DAVID
 * Misspelled 'S_' as 'R_'
 * 
 *    Rev 1.5   13 Oct 1992 00:31:46   DAVID
 * Changed settings struct so it doesn't use nested structs--this is a 
 * step towards eliminating SETTINGS.MAC: it should be generated from
 * SETTINGS.H by H2INC.
 * 
 *    Rev 1.4   02 Oct 1992 22:42:00   DAVID
 * Modified the redirector structs to use the common configuration file
 * structs now in COMIO.H
 * 
 *    Rev 1.3   01 Oct 1992 23:38:36   DAVID
 * Created struct for Redirector Client & Server settings and
 * added them to Settings structure.
 * 
 *    Rev 1.2   25 Sep 1992 15:56:20   SANDY
 * Added "#define DR_MY_LIM 6" for the date format of just month-year.
 * 
 *    Rev 1.1   02 Aug 1992 02:25:58   DAVID
 * Added "#if !defined(SETTINGS_H)", "#define SETTINGS_H" etc.
 * 
 *    Rev 1.0   03 Dec 1991 20:14:26   DAVID
 * Initial revision.
 *
 */


#if !defined(SETTINGS_H)
#define SETTINGS_H


/* volume level range */

#define VOLLEVOFF       0x0
#define VOLLEVHI        0x4


/* contrast level range */

#define CONTLEV0	0x0
#define CONTLEV15	0x1f


/* date formats */

#define DATEFOR1	0x0		/* dd-mmm-yy */
#define DATEFOR2	0x1		/* dd-mmm */
#define DATEFOR3	0x2		/* mmm-yy */
#define DATEFOR4a	0x3		/* mm/dd/yy */
#define DATEFOR4b	0x4		/* dd/mm/yy */
#define DATEFOR4c	0x5		/* dd.mm.yy */
#define DATEFOR4d	0x6		/* yy-mm-dd */
#define DATEFOR5a	0x7		/* mm/dd */
#define DATEFOR5b	0x8		/* dd/mm */
#define DATEFOR5c	0x9		/* dd.mm */
#define DATEFOR5d	0xa		/* mm-dd */


/* week starts */

#define WEEKSTARTSUN	0x0
#define WEEKSTARTMON	0x1


/* time formats */

#define TIMEFOR1	0x0		/* HH:MM:SS am/pm */
#define TIMEFOR2	0x1		/* HH:MM am/pm */
#define TIMEFOR3a	0x2		/* HH:MM:SS */
#define TIMEFOR3b	0x3		/* HH.MM.SS */
#define TIMEFOR3c	0x4		/* HH,MM,SS */
#define TIMEFOR3d	0x5		/* HHhMMmSSs */
#define TIMEFOR4a	0x6		/* HH:MM */
#define TIMEFOR4b	0x7		/* HH.MM */
#define TIMEFOR4c	0x8		/* HH,MM */
#define TIMEFOR4d	0x9		/* HHhMMm */


/* punctuation sets (follows 1-2-3)
 *   (decimal, argument, thousands)
 */

#define PUNCFORA	0x0		/* (.,,) */
#define PUNCFORB	0x1		/* (,..) */
#define PUNCFORC	0x2		/* (.;;) */
#define PUNCFORD	0x3		/* (,;.) */
#define PUNCFORE	0x4		/* (., ) */
#define PUNCFORF	0x5		/* (,. ) */
#define PUNCFORG	0x6		/* (.; ) */
#define PUNCFORH	0x7		/* (,; ) */


/* languages -- only US is defined; it is the default.  All
 * others are found at run-time by the presense of QBTASK??.RI
 * resource images, where ?? used to indicated the language 
 */


#define LANG_US	0x5355		/*  'US', bytes swapped */



/* collating sequences */

#define NUMBERSFIRST	0x0
#define LETTERSFIRST	0x1
#define ASCII		    0x2


/* character sizes */

#define CHR7		0x0
#define CHR8		0x1



/* baud rates, Max baud for non-infrared selections is 115200
 * max for IR link is 115200  (or sometimes, for jag compatibility, 2400 or 9600)
 */

#define MENU_CNT_IR	3
#define MENU_CNT_NOT_IR	6

#define BAUD300 	   0x0
#define BAUD1200	   0x1
#define BAUD2400	   0x2
#define BAUD4800	   0x3
#define BAUD9600	   0x4
#define BAUD19200	   0x5
#define BAUD38400	   0x6
#define BAUD57600	   0x7
#define BAUD115200	0x8

#define MAX_IR_BAUD	BAUD115200


/* interface options for printers */

#define INTERFCOM1	0x0
#define INTERFCOM2	0x1
#define INTERFINFR	0x2
#define INTERFLPT1	0x3
#define INTERFALTN	0x4


/* supported printers -- this is static because the printer
 * drivers are in ROM, no RAM drivers supported
 */

#define EP_FX80 	0x0
#define HP_LJ		0x1
#define IBM_PP		0x2



/* currency prefix/suffix selection */

#define CUR_PREF	0x0
#define CUR_SUFF	0x1



/* keyboard selections */

#define KEYB_BEL	0x0001		/* Belgium */
#define KEYB_FCA	0x0002		/* French Canadian */
#define KEYB_DEN	0x0004		/* Denmark */
#define KEYB_FIN	0x0008		/* Finland */
#define KEYB_FRA	0x0010		/* French */
#define KEYB_GER	0x0020		/* Finland */
#define KEYB_ITA	0x0040		/* Italy */
#define KEYB_NED	0x0080		/* Netherlands */


#define KEYB_NOR	0x0100		/* Norway */
#define KEYB_POR	0x0200		/* Portugal */
#define KEYB_SPA	0x0400		/* Spain */
#define KEYB_SWE	0x0800		/* Sweden */
#define KEYB_SFR	0x1000		/* Swiss French */
#define KEYB_SGR	0x2000		/* Swiss German */
#define KEYB_UNK	0x4000		/* United Kingdom */
#define KEYB_USA	0x8000		/* USA */




#define MAX_DESC_LEN	56       /* Changed for Cougar (was 56) */
#define MAX_MACRO_LEN	256


/* date parsing rules */

#define DR_DMY_LIM	1
#define DR_MDY_LIM	2
#define DR_YMD_LIM	3
#define DR_DMYO_LIM  4
#define DR_MDYO_LIM  5
#define DR_MY_LIM    6

/* or this bit onto patterns above to get any date */
#define DR_ANY_YEAR	8


/* time parsing rules */

#define TM_H_M_S_P	1
#define TM_H_M_S_24	2
#define TM_HM_S_24	3
#define TM_H_M_S_C_24	4
#define TM_H_M_P        5
#define TM_H_M_24       6
#define TM_HM_24        7

/* Code Page Settings */
#define CDPG_850    0x0850
#define CDPG_437    0x0437
#define CDPG_865	0x0865
#define CDPG_860    0x0860

#define NAMEBUFLEN	30
#define TITLEBUFLEN	30
#define COMPANYBUFLEN	28

#define MAGIC1_VALUE	0x61766961
#define MAGIC2_VALUE	0x6c657261

#define FILER_SETTINGS    2             /* created by filer (signature)  */
#define TERMINAL_SETTINGS 3             /* created by termulator         */
#define RSERVER_SETTINGS  4             /* created by  Redirector-server */
#define RCLIENT_SETTINGS  5             /* created by  Redirector-client */

#define SETTINGS_REVISION 3             /* revision # for config file   */
#if !defined(MAX_FILE_NAME)
#define MAX_FILE_NAME           12      /* maximum length of a filename */
#endif

#if !defined(MAX_FILE_STRING)
#define MAX_FILE_STRING         13      /* max filename plus one for null */
#endif

#if !defined(MAX_DIR)
#define MAX_DIR                 64      /* max DOS directory path */
#endif

#if !defined(MAX_PATH)
#define MAX_PATH                MAX_DIR + MAX_FILE_NAME  /* max DOS path */
#endif

#if !defined(CORE_LEN)
#define CORE_LEN                8       /* max length of filename's core */
#endif

#if !defined(EXT_LEN)
#define EXT_LEN                 3       /* length of filename extension */
#endif

#if !defined(MAX_DIAL_STRING)
#define MAX_DIAL_STRING 64
#endif

#if !defined(SMACROS_MODIFIED)         /* Macros modified by Sysmgr */
#define SMACROS_MODIFIED        1
#endif

#if !defined(MACRO_NEW_FILE)           /* New file read by Sysmgr */
#define SMACRO_NEW_FILE         2
#endif

#if !defined(SMACRO_CHAIN_ERROR)       /* Error is accessing chain file */
#define SMACRO_CHAIN_ERROR      4
#endif




#pragma pack(1)

/*  settings */
typedef struct {
    int     Country;	    /* */
	int	    Volume; 	    /* 1 speaker volume level 0ffh for off and 0-3 */
	int	    Contrast;	    /* 2 contrast level 1-5 */
	int	    WeekStart;	    /* 3 week starts SUNDAY or Monday */
	int	    Punctuation;	/* 4 Punctuation format */
	int     Language;	    /* two character code for current language */
	int	    DateFormat;	    /* 6 current date format */
	int	    TimeFormat;	    /* 7 current time format */
	int	    Sort;		    /* 8 collating sequence */
	char	PicFile[80];	/* picture file name */
	char	Name_Buff[NAMEBUFLEN];	
	char	Title_Buff[TITLEBUFLEN]; 
	char	Company_Buff[COMPANYBUFLEN]; 
	int 	LanguageCount;
	int	    LanguagesAvail[6];	/* MAX_LANG */
	char	LanguageMenu[66];	

	char	DateSeparator[2];   /* date separator  can be /-. */
	char	TimeSeparator[2];   /* time separator can be :., or h (for hms) */
	char	DateOrder;	        /* */
	char	Time24;		        /* use 24 hour time? */

	char	Currency[16];       /* currency string */
	int	    CurrencyFix;        /* prefix or sufix */

	int	    Keyboard;
	int	    PrBaud; 	        /* printer baud rate */
	int	    PrDriver;	        /* epson, hp, etc, by code */
	int 	PrInterface;	    /* com 1, etc  */
	int	    SystemRupt;		    /* sysmgr interupt number - 60 default */
	int	    CodePage;		    /* 437 or 850 (format is 0x0850 for 850, etc.) */
	int	    ExitKeyCode;	    /* for PC based - active exit key */
	int	    MenuKeyCode;	    /* for PC based - active menu key */
	int	    CharKeyCode;	    /* for PC based - active CHAR key toggle */
	int	    VidType;			/* for PC based - video type
									(0=MDA, 1=CGA, -1 = auto) */
	int	    Alarm[6];

/* New fields - misc. setup information */

    char    ShowPic;            /* Show top-level picture file? */
    char    LaunchServer;       /* Launch server at startup     */
    int     ClipMemory;         /* Clipboard memory     */
    int     StatMemory;         /* Static memory        */
    int	   DOSMemory;	        /* DOS memory	   */
    int	   DOSCodePage;	     /* Format is 0x0850 for 850, etc. */
    int	   CountryIndex;	     /* Index into the setup Country listbox  */
    char    cMainMeter;         /* Battery setting */
    char    cBackupMeter;       /* Battery setting */
    int     CommIOAddr;         /* Comm IO address */
    char    CommINT;            /* Comm INT */
    char    EnableDOSPort;      /* Comm power on/off in DOS */
    char    PrInfraRed;         /* Infrared setting for printer */
    char    MacroFlags;         /* Macros or macro file changed ? */
    char    MacroFileName[MAX_PATH]; /* Filename of current macro set */

/* Redirector Client comm settings (Filer config file compatible) */

   /* com_settings structure
    *
    */
    char    C_UnusedDial;       /* Jaguar compatibility. */
    int     C_Baud;             /* Line speed (really UART's BRG value). */
    char    C_Parity;           /* Line Parity. */
    char    C_Stop;             /* Number of stop bits. */
    char    C_Data;             /* Number of data bits. */
    char    C_Flow;             /* Flow control. */
    char    C_InfraRed;         /* Infrared mode selected. */
    char    C_UnusedDuplex;     /* Jaguar compatibility. */
    char    C_UnusedEcho;       /* Really a terminal emulator thingie. */

   /* Additional COMMON_CONFIG fields */
    int     C_comm_port;                    /* comm port (com1 or com2)     */
	char	C_CreatedBy;                    /* who created these settings   */
	char	C_SettingsRev;                  /* version of the structure     */
	long	C_Magic1;                       /* Magic number for file typing */
	char	C_Number[MAX_DIAL_STRING];      /* Phone number. */
	char	C_Script[MAX_FILE_STRING+1];	/* Script file (+even pad) */
	char	C_Ctf[MAX_FILE_STRING+1];       /* Conversion translation file */
	char	C_Emulation;	                /* Emulation - NONE, VT100, GLASS TTY, MAP */
	char	C_Wrap;	                        /* Screen wrap on or off. */
	char	C_Backspace;                    /* Backspace char */
	long	C_Magic2;

   /* Additional fields for Client */
    char    C_DriveMap[14];     /* Drive Mappings: G: H: I: J: + 10 spare */


/* Redirector Server comm settings (Filer config file compatible) */

   /* com_settings structure*/
    char    S_UnusedDial;       /* Jaguar compatibility. */
    int     S_Baud;             /* Line speed (really UART's BRG value). */
    char    S_Parity;           /* Line Parity. */
    char    S_Stop;             /* Number of stop bits. */
    char    S_Data;             /* Number of data bits. */
    char    S_Flow;             /* Flow control. */
    char    S_InfraRed;         /* Infrared mode selected. */
    char    S_UnusedDuplex;     /* Jaguar compatibility. */
    char    S_UnusedEcho;       /* Really a terminal emulator thingie. */

   /* Additional COMMON_CONFIG fields */
    int     S_comm_port;                 /* comm port (com1 or com2)     */
	char	S_CreatedBy;                 /* who created these settings   */
	char	S_SettingsRev;               /* version of the structure     */
	long	S_Magic1;                    /* Magic number for file typing */
	char	S_Number[MAX_DIAL_STRING];   /* Phone number. */
	char	S_Script[MAX_FILE_STRING+1]; /* Script file (+even pad) */
	char	S_Ctf[MAX_FILE_STRING+1];    /* Conversion translation file */
	char	S_Emulation;	             /* Emulation - NONE, VT100, GLASS TTY, MAP */
	char	S_Wrap;	                     /* Screen wrap on or off. */
	char	S_Backspace;                 /* Backspace char */
	long	S_Magic2;


} SETTINGS;


typedef struct {
	char	      MacroFK;	     /* MacroFunctionKey Number 1-10 0 for not used */
    unsigned char MacroSize;                   /* elements in the macro */
    char          CommentString[MAX_DESC_LEN]; /* Macro Key's Comment line */
	unsigned int   MacroBody[MAX_MACRO_LEN];
} MACROHD;


#endif


#pragma pack()

/* $Header:   T:/HH/INCLUDE/VCS/EVENT.H_V   1.11   23 Dec 1992 14:24:22   PETERR  $

/****************************************************************
 * Jaguar Accessories Kernel
 *
 *	Copyright(c) 1990
 *	Lotus Development Corporation
 *	All Rights Reserved
 *
 * EVENT.H - Structures and Defines for  for EVENT services.
 *
 ****************************************************************/

/*
 * $Log:   T:/HH/INCLUDE/VCS/EVENT.H_V  $
 * 
 *    Rev 1.11   23 Dec 1992 14:24:22   PETERR
 * put event.mac
 * Added new structure entry in alarm event structure
 * Added new DO_ type event to enable app to be placed at end of q but in 
 * front of task0
 * 
 *    Rev 1.10   23 Nov 1992 14:19:54   PETERR
 * Added new event DO_EXIT_SYSMGR
 * 
 *    Rev 1.9   16 Nov 1992 17:33:52   PETERR
 * Added union of new event structure
 * @
 * 
 *    Rev 1.8   16 Nov 1992 14:25:10   PETERR
 * Added new DOS_EXEC_FULL event and two new structures
 * 
 *    Rev 1.7   19 Oct 1992 12:30:20   PETERR
 * Added a new event to enable one app to request the closing of another app
 * 
 *    Rev 1.6   15 Oct 1992 10:50:04   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.5   30 Sep 1992 17:24:52   PETERR
 * Added defines for special handler events
 * 
 *    Rev 1.4   10 Sep 1992 12:31:14   PETERR
 * Added new event for ALARM notification
 * E_ALARM_EXP_NO_FOREGROUND
 * 
 *    Rev 1.3   28 Aug 1992 08:42:44   PETERR
 * Added pragma to force byte packing of structs
 * 
 *    Rev 1.2   03 Aug 1992 10:07:38   PETERR
 * Changed event item from long to int per Wei Lee's request
 * 
 *    Rev 1.1   18 Jun 1992 15:17:34   KIM
 * Moved latest version from \HH\SYSMGR to here.  Now shared by Apps and SysMgr.
 *
 * ---------------------------------------------------------------------
 * Moved from \HH\SYSMGR directory.  Shared by Apps and SysMgr (6/18/92 KIM)
 * ---------------------------------------------------------------------
 *    Rev 1.7   12 Jun 1992 11:30:40   PETERR
 * Added new union values to process alarm returns
 * 
 *    Rev 1.6   27 May 1992 09:11:36   PETERR
 * Improved comment for service structure
 * 
 *    Rev 1.5   20 May 1992 12:54:18   PETERR
 * Added new service structure
 * 
 *    Rev 1.4   13 May 1992 13:51:42   PETERR
 * New event added. New structure for service calls added
 * 
 *    Rev 1.3   01 May 1992 17:14:48   HOMI
 * Added Refresh and Return types
 * 
 *    Rev 1.2   03 Apr 1992 16:36:36   PETERR
 * Added ne Event DO_DOS_CLOSE
 * 
 *    Rev 1.1   09 Mar 1992 12:01:16   PETERR
 * DOS mem & launch changes
 * 
 *    Rev 1.0   03 Dec 1991 16:20:42   DAVID
 * Initial revision.
 *
 */


#pragma pack(1)

#define LO_BYTE		0x00ff
#define FLAG_SCAN	0xE800

enum event_kind {
	E_NONE,		   /* no events available */
	E_KEY,		   /* keystroke available */
	E_BREAK,	      /* CNTRL_BREAK encountered */
	E_ACTIV,	      /* reactivation event, always follows E_DEACT */
	E_DEACT,	      /* about to sleep, next event request is delayed */
	E_TERM,		   /* forced closure of application */
	E_BRIDGE,	   /* 123 bridge service request, only ret. to 123 */
	E_GROW,	 	   /* request to 123 to grow */
	E_SHRINK,	   /* request to 123 to shrink */
	E_ALARM_EXP,	/* application's alarm has expired */
	E_ALARM_DAY,	/* daily chance to set an alarm */
	E_TIMECHANGE,	/* system date or time has been changed */
	E_SWAP,		   /* swap back to app from link	*/
	E_IC_INIT,	   /* init IC communications   */
	E_IC_CLOSE,    /* close channel */
   E_REFRESH,     /* refresh sent to foreground app */
   E_RETURN,	  /* return from refresh MAY BE SCRAPPED */
	E_SERVICE,	/*  Request from asychronous process (int handler ) */
	E_ALARM_EXP_NOT_CURRENT /* Alarm event and was not current foregnd */
};


/* Note this enumerated table (do_event) must match the corresponding jump
   table located in AQBTASK.ASM (event_jtab)
   Also as with all items in this file, a corresponding .ASM version is located
   in EVENT.MAC
*/

enum edo_event {
	DO_EVENT,	   /* get event */
	DO_YIELD,	   /* give up foreground */
	DO_FINI,	      /* terminate task */
	DO_NO_EVENT,	/* return even if no event */
	DO_NO_FINI,	   /* refuse to close when asked */
	DO_BRIDGE,	   /* do brigde services */
	DO_IC_INIT,	   /* set up comm channel */
	DO_IC_CLOSE,	/* close comm channel */
	DO_SWAP,	      /* swap to linked task */
	DO_LAUNCH,	   /* launch a new system manger compliant task */
	DO_EXEC,		   /* Launch a DOS program */
	DO_DOS_CLOSE,  /* Close DOS task request */
   DO_REFRESH,    /* Request foreground app to refresh the screen */
   DO_RETURN,	  /* Return to background task after refresh */
   DO_SERVICE_COMPLETE,	/*  Task has completed background process */
   DO_REQUEST_FOREGROUND,	/* Request to make this task foreground */
   DO_CLOSE_APP,		/* Request a particular app to be closed */
   DO_EXEC_FULL,		 /* Launch a DOS program with full control*/
   DO_EXIT_SYSMGR,	       /* Close all apps & exit to DOS */
   DO_MOVE_TO_END	/*Move current app to end of q */
			/*    (but in front of task0   */
};









typedef struct {
	enum event_kind kind;

	unsigned int data;	/* ASCII (CP 850) translation of keystroke */
	unsigned char scan;	/* scan code from BIOS */
	unsigned char shifts;	/* shifts register, when function returns */
				/* not necessarily when key struck */
	unsigned int lics;	/* LICS translation of keystroke */
	unsigned int fkey_num;	/* function key number for 123 only */
	void far * bridge;	/* pointer to 123 bridge data structure */
	enum edo_event do_event;/* User set event type	*/
	unsigned int  return_status;	    /* Return status from call	  */
} EVENT_NORM;



					    /*	DO_LAUNCH	*/

typedef struct {
	enum event_kind kindie;		     /* Place holder */
	unsigned int   e_dummy; 	     /* Place holder */

	unsigned char e_launch_task;	    /* TCB task entry # to launch */
	unsigned char e_launch_type;	    /* Type of launch request	  */
} EVENT_L;


					     /*  DO_EXEC  */

typedef struct {
	enum	 event_kind exec_kind;	     /* Place holder */
	unsigned int   exec_dummy;	     /* Place holder */
	void far  *exec_struc_ptr;	   /* Ptr to EXEC structure */
} EVENT_EXECF;



typedef struct {
	char far *ex_file_spec;	     /* Full path to file spec (ASCIIZ)*/
	char far *ex_command_line;   /* Command line text */
	unsigned int ex_DOSsizek;    /* #Kbytes needed for this task (0=use default)*/
	unsigned int ex_pause;	     /* Pause before returning (0=No pause)*/
	unsigned int ex_lock;	     /* Inhibit task switches (0=Allow)*/
} EXEC_STRUCT;



typedef struct {
	enum	 event_kind kindex;	     /* Place holder */
	unsigned int   e_exdummy;	     /* Place holder */

	char far   *e_exec_file_ptr;	    /* Full path of programto EXEC */
	char far   *e_cmd_tail_ptr;	    /* Command tail */
	unsigned char	e_task_id;	    /* TCB entry of DOS task */
} EVENT_EXEC;


					    /* DO_IC_INIT  */

typedef struct {
	enum	 event_kind kindic;	     /* Place holder */
	unsigned int   e_icdummy;	     /* Place holder */

	void far       **e_icdata_ptr;	    /* Near ptr to Far ptr entry */
	unsigned char  e_link_task;	    /* Task to link to (TCB entry) */
	void far       **e_slave_offset;    /* Slave task pointer table off */
} EVENT_IC;



typedef struct {
	enum	 event_kind kindsvc;	     /* Place holder */
	unsigned int   e_svcdummy;	     /* Place holder */

	unsigned char far *e_svc_user_ptr;    /* General purpose use	*/
	unsigned char	  e_svc_user_type;    /* General purpose use */
} EVENT_SERVICE;



typedef struct {
	enum	 event_kind kindalm;	     /* Place holder */
	unsigned int	e_alm_special;	     /* User alarm type */
	unsigned char	e_almdummy[3];	     /* Place holder */
	unsigned char	e_almcurtask;	     /* Task id that was current */
	unsigned int	e_alm_extra[2];	    /* User bytes */
} EVENT_ALARM;
					      /* E_ALARM_EXP  */



typedef union
{
	EVENT_NORM;
	EVENT_L;
	EVENT_IC;
	EVENT_EXEC;
	EVENT_SERVICE;
	EVENT_ALARM;
	EVENT_EXECF;

}EVENT;




/*;  Asynchronous task scheduling structure  */

typedef struct {
   unsigned char  SVC_request;	   /* Flag set by handler (ff=highest priority) */
   unsigned char  SVC_app_index;   /* TCB entry to invoke (ff= special callback)  */
   void far	 *SVC_user_ptr;	   /* Genl param passed thru E_SERVICE event to app */
   unsigned char  SVC_user_type;   /* Genl param passed thru E_SERVICE event to app   */
   unsigned char  SVC_user_id[4];  /* Identifying signature set by service originator */
   void far	 *SVC_user_entry;  /* Entry point for app -> service originator calls */
   void far	 *SVC_safe_entry;  /* Ent pnt for safe svc handler -> DOS & sysm calls*/
   unsigned char  SVC_used;	   /* Flag ind entry avail. Set and reset by sysmgr */
   unsigned int   SVC_status;	   /* Status of request set by sysmgr */
}S_SERVICE;

#define  CALLBACK_SERVICE_REQUEST 0xff	/* Special SVC_app_index value*/
#define  FREEMEM_SERVICE_REQUEST  0xfe	/* Special SVC_app_index value*/






/* Field usages by event_kind 

	TYPE		FIELDS USED
	-----------------------------------------------	
	E_NONE		shifts
	E_KEY		data,scan,fkey_num,lics,shifts
	E_BREAK		none
	E_ACTIV		none
	E_DEACT		none
	E_TERM		none
	E_BRIDGE	bridge
	E_GROW		data
	E_SHRINK	data
	E_ALARM_EXP	data,bridge
	E_ALARM_DAY	none
	E_TIMECHANGE	bridge (-> timechange structure )
 */
	



extern unsigned int kb_read(void);
extern unsigned int kb_test(void);
extern void kb_flush(void);


#pragma pack()

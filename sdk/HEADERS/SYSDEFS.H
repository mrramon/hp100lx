/*
 * $Header:   T:/HH/INCLUDE/VCS/SYSDEFS.H_V
 *
 * TKERNEL.C
 *
 * Copyright(c) 1990-1991
 * Lotus Development Corporation, Inc., Cambridge, MA. 02142
 * All Rights Reserved.
 * ------------------------------------------------------------------------
 * $Log:   T:/HH/INCLUDE/VCS/SYSDEFS.H_V  $
 * 
 *    Rev 1.43   25 Feb 1993 15:27:00   PETERR
 * Changed keycode conflict with new shift make code.
 * 
 *    Rev 1.42   25 Feb 1993 09:02:46   PETERR
 * Changed Shift make and break low byte value from "03" to "83"
 * This was required in order to prevent DOS from incorrectly assuming
 * that the user has entered a Ctrl'C key.
 * See full comment in ALTSHIFT.ASM
 * 
 *    Rev 1.41   22 Feb 1993 19:51:48   DARYL
 * Changed the KEY_MAKE_SHIFT constant from 0x7E33 to 0x7E03 and the
 * KEY_BREAK_SHIFT constant from 0xFE33 to 0xFE03.
 * 
 *    Rev 1.40   19 Feb 1993 16:21:22   PETERR
 * Added new def for CALARMAPP hot key.
 * 
 *    Rev 1.39   09 Feb 1993 10:59:16   DAVID
 * Corrected typo in #define ROMLaunchDOSsize (or whatever): 0ffffh -> 0xffff
 * 
 *    Rev 1.38   04 Feb 1993 20:49:58   PETERR
 * Changed DOS memory requested for drive D: apps (set by FILER) to request
 * all available memory as at least one app (memcard.exe) requires A LOT
 * OF MEMORY.
 * 
 *    Rev 1.37   03 Feb 1993 15:26:28   PETERR
 * Added error code for failure upon return from DOS EXEC call:
 * ERROR_EXEING_DOS_APP
 * Correct above line. Should be ERROR_EXECING_DOS_APP
 * 
 *    Rev 1.36   25 Jan 1993 23:04:30   MAX
 * Changed the communications error range to match the Jaguar range (first
 * eight errors only) so we can communicate with Jags.
 * 
 *    Rev 1.35   25 Jan 1993 17:17:40   PETERR
 * Added define for Alarm or Service request pending.
 * 
 *    Rev 1.34   20 Jan 1993 16:45:14   PETERR
 * Added new error return value for m_get_far_block -- zero size illegal
 * 
 *    Rev 1.33   19 Jan 1993 12:39:22   PETERR
 * Added new define for m_QuickKeyCheck return to indicate change in
 * Caps, Num or Scroll state.
 * 
 *    Rev 1.32   07 Jan 1993 14:35:12   PETERR
 * Added new definition for Ctrl-break return indicator from
 * m_QuickKeyCheck
 * 
 *    Rev 1.31   04 Jan 1993 16:18:26   WEI-LEE
 * Changed datacomm error range.
 * 
 *    Rev 1.30   29 Dec 1992 11:39:52   PETERR
 * Added nes PS_ type for enabling launch and quue position to just
 * in front of task0
 * 
 *    Rev 1.29   21 Dec 1992 16:30:26   PETERR
 * Added DOS default DOS TCB hot-key value when not using
 * 
 *    Rev 1.28   21 Dec 1992 14:51:24   PETERR
 * Added new definition ROMLaunchSizeK for FILER use in launching DOS
 * programs from ROM drive "D"
 * 
 *    Rev 1.27   08 Dec 1992 20:48:38   PETERR
 * Added new ERROR return type for close app request
 * 
 *    Rev 1.26   30 Nov 1992 10:42:22   PETERR
 * Added new define for FILER use in reserving DOS memory for built in (drive 
 * "D") executables. --   DOS_COMMAND_MEM_REQ    =   70      (70k)
 * 
 *    Rev 1.25   23 Nov 1992 14:21:02   PETERR
 * Added new PS_ state for unloading sysmgr
 * 
 *    Rev 1.24   16 Nov 1992 14:26:02   PETERR
 * Added new DOS state
 * 
 *    Rev 1.23   09 Nov 1992 18:29:04   HOMI
 * Added error value 
 * 
 *    Rev 1.22   09 Nov 1992 16:43:12   HOMI
 * Redefined error values for TaskInfo, Api and macros.
 * 
 *    Rev 1.21   09 Nov 1992 15:26:02   MAX
 * defined a range of errors cor comms.
 * 
 *    Rev 1.20   04 Nov 1992 14:26:50   HOMI
 * Added constants for m_playback_macro() return status values.
 * 
 *    Rev 1.19   04 Nov 1992 12:55:22   PETERR
 * Changed charcode for shift make and break keys
 * 
 *    Rev 1.18   26 Oct 1992 15:24:52   PETERR
 * Added new keycodes for FN key
 * 
 *    Rev 1.17   26 Oct 1992 12:33:56   PETERR
 * Added new keycodes per comm request
 * 
 *    Rev 1.16   22 Oct 1992 13:01:32   HOMI
 * Added equates for memory availability checks.
 * 
 *    Rev 1.15   19 Oct 1992 12:30:48   PETERR
 * Added new process state PS_CLOSE_APP
 * 
 *    Rev 1.14   15 Oct 1992 10:52:20   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.13   05 Oct 1992 15:50:02   PETERR
 * Fixed brain dead bug in choice of new scan code
 * 
 *    Rev 1.12   05 Oct 1992 13:13:26   PETERR
 * Changed key code definitions for shifted make keys to avoid conflict with
 * compose key identifier (from 'ff' to 'fe'
 * 
 *    Rev 1.11   16 Sep 1992 18:29:34   HOMI
 * Added #define's for taskinfo and API function return values.
 * 
 *    Rev 1.10   10 Sep 1992 12:32:14   PETERR
 * Added new process status for alarm notification
 * 
 *    Rev 1.9   11 Aug 1992 11:14:08   PETERR
 * Added define to prevent re-definitions
 * 
 *    Rev 1.8   03 Aug 1992 14:07:06   PETERR
 * Added new key codes for Alt-Down and Alt-Up keystrokes
 * 
 *    Rev 1.7   28 Jul 1992 09:53:18   PETERR
 * Changed scan code values
 * 
 *    Rev 1.6   28 Jul 1992 08:03:48   PETERR
 * Changed sysmgr version to 200 (hex)
 * Corrected Caps lock and Scroll Lock scan code values
 * 
 *    Rev 1.5   23 Jul 1992 11:35:40   PETERR
 * Added support for SCROLL and NUM lock state change key reporting
 * 
 *    Rev 1.4   24 Jun 1992 11:49:18   PETERR
 * Changed values for shifted keys
 * 
 *    Rev 1.3   22 Jun 1992 12:47:52   PETERR
 * Added keycodes for shifted keys (temp) Will be moved elseware
 */


/*
      DOS Extended ERROR reporting results.  The values in this table
      are identical to those found in DOS 5.0 extended error return
      (DOS function call AH=59H)
      sysmgr will set the EVENT structure entry return_status to a non
      zero value if an error is encountered. The high bit will always
      be set (negative value) to indicate an error condition. This will
      enable later use of a non-zero return status for other than error
      reporting.
*/

#define SYSDEFS_H	1

#define ERROR_BIT	0x8000

#define ERROR_INVALID_FUNCTION		ERROR_BIT+0x0001
#define ERROR_FILE_NOT_FOUND		ERROR_BIT+0x0002
#define ERROR_PATH_NOT_FOUND		ERROR_BIT+0x0003
#define ERROR_TOO_MANY_OPEN_FILES	ERROR_BIT+0x0004
#define ERROR_ACCESS_DENIED		ERROR_BIT+0x0005
#define ERROR_INVALID_HANDLE		ERROR_BIT+0x0006
#define ERROR_ARENA_TRASHED		ERROR_BIT+0x0007
#define ERROR_NOT_ENOUGH_MEMORY 	ERROR_BIT+0x0008
#define ERROR_INVALID_BLOCK		ERROR_BIT+0x0009
#define ERROR_BAD_ENVIRONMENT		ERROR_BIT+0x000A
#define ERROR_BAD_FORMAT		ERROR_BIT+0x000B
#define ERROR_INVALID_ACCESS		ERROR_BIT+0x000C
#define ERROR_INVALID_DATA		ERROR_BIT+0x000D

#define ERROR_INVALID_DRIVE		ERROR_BIT+0x000F
#define ERROR_CURRENT_DIRECTORY 	ERROR_BIT+0x0010
#define ERROR_NOT_SAME_DEVICE		ERROR_BIT+0x0011
#define ERROR_NO_MORE_FILES		ERROR_BIT+0x0012
#define ERROR_WRITE_PROTECT		ERROR_BIT+0x0013
#define ERROR_BAD_UNIT			ERROR_BIT+0x0014
#define ERROR_NOT_READY 		ERROR_BIT+0x0015
#define ERROR_BAD_COMMAND		ERROR_BIT+0x0016
#define ERROR_CRC			ERROR_BIT+0x0017
#define ERROR_BAD_LENGTH		ERROR_BIT+0x0018
#define ERROR_SEEK			ERROR_BIT+0x0019
#define ERROR_NOT_DOS_DISK		ERROR_BIT+0x001A
#define ERROR_SECTOR_NOT_FOUND		ERROR_BIT+0x001B
#define ERROR_OUT_OF_PAPER		ERROR_BIT+0x001C
#define ERROR_WRITE_FAULT		ERROR_BIT+0x001D
#define ERROR_READ_FAULT		ERROR_BIT+0x001E
#define ERROR_GEN_FAILURE		ERROR_BIT+0x001F
#define ERROR_SHARING_VIOLATION 	ERROR_BIT+0x0020
#define ERROR_LOCK_VIOLATION		ERROR_BIT+0x0021
#define ERROR_WRONG_DISK		ERROR_BIT+0x0022
#define ERROR_FCB_UNAVAILABLE		ERROR_BIT+0x0023
#define ERROR_SHARING_BUFFER_EXCEEDED	ERROR_BIT+0x0024
#define ERROR_CODE_PAGE_MISMATCHED	ERROR_BIT+0x0025
#define ERROR_HANDLE_EOF		ERROR_BIT+0x0026
#define ERROR_HANDLE_DISK_FULL		ERROR_BIT+0x0027

#define ERROR_NOT_SUPPORTED		ERROR_BIT+0x0032
#define ERROR_REM_NOT_LIST		ERROR_BIT+0x0033
#define ERROR_DUP_NAME			ERROR_BIT+0x0034
#define ERROR_BAD_NETPATH		ERROR_BIT+0x0035
#define ERROR_NETWORK_BUSY		ERROR_BIT+0x0036
#define ERROR_DEV_NOT_EXIST		ERROR_BIT+0x0037
#define ERROR_TOO_MANY_COMMANDS		ERROR_BIT+0x0038
#define ERROR_ADAP_HDW_ERR		ERROR_BIT+0x0039
#define ERROR_BAD_NET_RESP		ERROR_BIT+0x003A
#define ERROR_UNEXP_NET_ERR		ERROR_BIT+0x003B
#define ERROR_BAD_REM_ADAP		ERROR_BIT+0x003C
#define ERROR_PRINTQ_FULL		ERROR_BIT+0x003D
#define ERROR_NO_SPOOL_SPACE		ERROR_BIT+0x003E
#define ERROR_PRINT_CANCELLED		ERROR_BIT+0x003F
#define ERROR_NETNAME_DELETED		ERROR_BIT+0x0040
#define ERROR_NETWORK_ACCESS_DENIED	ERROR_BIT+0x0041
#define ERROR_BAD_DEV_TYPE		ERROR_BIT+0x0042
#define ERROR_BAD_NET_NAME		ERROR_BIT+0x0043
#define ERROR_TOO_MANY_NAMES		ERROR_BIT+0x0044
#define ERROR_TOO_MANY_SESS		ERROR_BIT+0x0045
#define ERROR_SHARING_PAUSED		ERROR_BIT+0x0046
#define ERROR_REQ_NOT_ACCEP		ERROR_BIT+0x0047
#define ERROR_REDIR_PAUSED		ERROR_BIT+0x0048

#define ERROR_FILE_EXISTS		ERROR_BIT+0x0050
#define ERROR_DUP_FCB			ERROR_BIT+0x0051
#define ERROR_CANNOT_MAKE		ERROR_BIT+0x0052
#define ERROR_FAIL_I24			ERROR_BIT+0x0053
#define ERROR_OUT_OF_STRUCTURES		ERROR_BIT+0x0054
#define ERROR_ALREADY_ASSIGNED		ERROR_BIT+0x0055
#define ERROR_INVALID_PASSWORD		ERROR_BIT+0x0056
#define ERROR_INVALID_PARAMETER		ERROR_BIT+0x0057
#define ERROR_NET_WRITE_FAULT		ERROR_BIT+0x0058

#define ERROR_SYS_COMP_NOT_LOADED	ERROR_BIT+0x005A



/*
 * Note - errors codes should all be unique within and without this file.
 * If you need to define system errors outside of this file, declare 
 * a minimum and maximum here and reference them in your file.
 */



/* Defines for memory error returns    */
#define ERROR_INSUFFICIENT_MEMORY	0xFFFF
#define ERROR_TABLE_ENTRY_RANGE 	0xFFFE
#define ERROR_TABLE_FULL		0xFFFD
#define ERROR_TABLE_NOT_REGISTERED	0xFFFC
#define ERROR_NO_STATIC_MEM_AVAIL	0xFFFB
#define ERROR_CANNOT_EXP_STATIC_AREA	0xFFFA
#define ERROR_STATIC_BLOCK_RANGE	0xFFF9
#define ERROR_BLOCK_SIZE_ZERO		0xFFF8


/* Error defines for launching of other apps */
#define ERROR_LOADING_APP		0xFFEF
#define ERROR_ALREADY_LOADED		0xFFEE
#define ERROR_INPUT_OUT_OF_RANGE	0xFFED
#define ERROR_CANNOT_LAUNCH_SELF	0xFFEC
#define ERROR_APP_NOT_LOADED		0xFFEB
#define ERROR_APP_NOT_FOUND		0xFFEA
#define ERROR_DOS_APP_ALREADY_LOADED	0xFFE9
#define ERROR_TCB_TABLE_FULL		0xFFE8
#define ERROR_NO_DOS_APP_LOADED		0xFFE7
#define ERROR_SERVICE_TABLE_FULL	0xFFE6
#define ERROR_APP_NOT_OPEN		0xFFE5
#define ERROR_EXECING_DOS_APP		0xFFE4


/* Defines for inter task communication */
#define ERROR_CHANNEL_NOT_SET		0xFFDF
#define ERROR_CHANNEL_BUSY		0xFFDE
#define ERROR_I_AM_SLAVE		0xFFDD
#define ERROR_SLAVE_TABLE_FULL		0XFFDC
#define ERROR_SLAVE_TABLE_NOT_REG	0XFFDB
#define ERROR_NOT_OWNER			0xFFDA

/*
 * Communications errors - these are defined in comerrs.h, only
 * the min and max are defined here. Do NOT declare any other
 * errors within this range. Note that communications with old
 * Jaguars depends on these numbers as they are - don't change 
 * them!!
 */

#define ERROR_COMMS_MIN                 0xFF38  /* MUST START WITH THIS! */
#define ERROR_COMMS_MAX                 0xFF9F                 

/* Task Info Errors */
#define  ERROR_TASK_INFO_TYPE       0xFFBF      /* Invalid task info type */
#define  ERROR_TASK_NO              0xFFBE      /* Invalid task number */

/* API */
#define  ERROR_FNCT_TBL_NOT_FOUND   0xFFAF   /* function table not found */
#define  ERROR_INVALID_CLASS        0xFFAE   /* invalid class number     */
#define  ERROR_INVALID_CLASS_TYPE   0xFFAD   /* invalid class number     */
#define  ERROR_NO_FREE_CLASS        0xFFAC   /* no free class available  */
#define  ERROR_CLASS_NOT_FOUND      0xFFAB   /* class not found          */

#define  API_FNCT_IMPLEMENTED       0x0000   /* function implemented  */
#define  API_FNCT_UNIMPLEMENTED     0x0001   /* function unimplemented  */
#define  API_FNCT_DISABLED          0x0002   /* function disabled       */
#define  API_CLASS_UNIMPLEMENTED    0x0003   /* class unimplemented     */

                                             /* MUST have 0xFFFF value  */
                                             /* DO NOT CHANGE */
#define  API_NO_FAILED_FUNCTION     0xFFFF   /* no failed api functions */

/* MACRO */
#define  ERROR_MACRO_NUMBER         0xFF9F   /* invalid macro number       */
#define  ERROR_MACRO_RECORDING      0xFF9E   /* macro currently recording  */
#define  ERROR_MACRO_PLAYING        0xFF9D   /* macro currently playing    */
#define  ERROR_MACRO_DISABLED       0xFF9C   /* macros disabled            */

/*  Errors for alarm functions	  */
#define ERROR_QUEUE_FULL		0xFFCF



/* Error codes for special TSR */
#define ERROR_BAD_TSR_SIGNATURE		0xFFBF




/* Defines for launching other apps   */
#define LT_CLOSE_THEN_LAUNCH		0x0000
#define LT_YIELD_THEN_LAUNCH		0x0001
#define LT_LAUNCH_THEN_RETURN		0x0002
#define LT_LAUNCH_THEN_Q		0x0003



/* defines for task run status return (_m_get_run_status)	 */
#define TASK_STS_NORMAL		0x0000
#define TASK_STS_IC_SLAVE	0x0001
#define TASK_STS_RETURN		0x0002
#define TASK_STS_TMP_IC_SLAVE	0x0003	/* User invoked but currently slave*/
#define TASK_STS_OWNER		0x0004
#define TASK_STS_IS_DOS		0x0005	/* This is DOS task */

#define TASK_STS_BG		0x0080	/* Background task */
#define TASK_STS_ALARM		0x0040	/* Alarm pending invocation */



/*   Sysmgr task state values	*/
#define PS_CLOSED	0x0000	/* not loaded in memory, no bits set   */
#define PS_ACTIVE	0x0001	/* foreground process  */
#define PS_SUSP		0x0002	/* suspended  */
#define PS_EXIT 	0x0004	/* task called m_fini, waiting to be unloaded  */
#define PS_YIELD	0x0008	/* task called m_yield, goes to end of queue */
#define PS_EXIT_REFUSED	0x0010	/* task called m_no_fini */

#define PS_NOWAIT	0x0001	/* MASK for the nowait bit */
#define PS_123		0x0001	/* Bit mask for 123  */

#define SYSMGR_SIGNATURE 0x7072  /* Sysmgr's mailbox ID */


/*
;****************************************************************************
;*******************			CAUTION
;****************************************************************************
;
;    NOTE:   All COUGAR PS states have bit 5 (20h) set.  This was done for
;	     quick return if no special processing required.
*/


/* ;COUGAR states:		 */
#define PS_SPECIAL	0x0020	 /*   Special task in effect */
#define PS_LAUNCH_INIT	0x0021	 /* task called m_launch command not implemented */
#define PS_LAUNCH_WAIT	0x0022	 /* task called m_launch waiting for return */
#define PS_SWAP		0x0023	 /* Swap to linked task  */
#define PS_IC_INIT	0x0024	 /* Initiating IC link */
#define PS_IC_CLOSE	0x0025	 /* Close open IC link */
#define PS_EXEC		0x0026	 /* EXEC DOS task */
#define PS_CLOSE_DOS	0x0027	 /* Close DOS task */
#define PS_REFRESH	0x0028    /* Send refresh to foreground */
#define PS_RETURN	0x0029	/* return from foreground to background */
#define PS_REQUEST_FORE 0x002a	/* Make this task a foregtround task */
#define PS_CLOSE_APP	0x002b	/* Request to close app */
#define PS_EXEC_FULL	0x002c	 /* EXEC DOS task - full capability */
#define PS_EXIT_SYSMGR	0x002d	 /* Exit from sysmgr and return to DOS */
#define PS_MOVE_TO_END	0x002e	/* Move this task to end of q but before t0)



/**************************************************************************/
/* Special TSR defines	      */

/*  Supported command types:	 */
#define CMD_SERVICE_LINK	0x0001	/* Associate an app with service */
#define CMD_SERVICE_UNLINK	0x0002	/* App no longer linked to svc	 */
#define CMD_SERVICE_COMPLETE	0x0003	/* App response complete	 */
#define CMD_SERVICE_UNHOOK	0x00f0	/* Remove any interrupt hooks	 */
#define CMD_SERVICE_HOOK	0x00f1	/* Hook required rupts		 */
#define CMD_SERVICE_UNLOAD	0x00f2	/* Remove self from memory	 */



/**************************************************************************/
/* DOS programs partition size	      */
#define ROMLaunchSizeK	       0xffff	/*  bytes required for drive "D"*/
				    /* DOS commands. */

#define DOS_DEFAULT_FREE	0xff0f	  /* Hot key for unused DOS entry */


/*
    Keyboard constants for shifted keys
    NOTE: low nibble matches affected bits in BIOS shift status
*/

#define KEY_MAKE_SHIFT		0x7e83	/* New make key for shift key  */
#define KEY_MAKE_CTRL		0x7e04	/* New make key for ctrl key   */
#define KEY_MAKE_ALT		0x7e08	/* New make key for alt key    */
#define KEY_BREAK_SHIFT 	0xfe83	/* New break key for shift key */
#define KEY_BREAK_CTRL		0xfe04	/* New break key for ctrl key  */
#define KEY_BREAK_ALT		0xfe08	/* New break key for alt key   */
#define KEY_CAPS_ON		0x7e40	/* Caps lock enabled key       */
#define KEY_CAPS_OFF		0xfe40	/* Caps lock disabled key      */
#define KEY_SCROLL_ON		0x7e10	/* Scroll lock enabled key     */
#define KEY_SCROLL_OFF		0xfe10	/* Scroll lock disabled key    */


#define KEY_ALT_DOWN		0x7e80	/* Alt-down key pressed	  */
#define KEY_ALT_UP		0x7e81	/* Alt-up key pressed	*/

#define KEY_ALT_RIGHT		0x7e82	/* Alt-right key pressed	*/

#define KEY_ALT_HOME		0x7e84	/* Alt-home key pressed	*/
#define KEY_ALT_PGDN		0x7e85	/* Alt-pgdn key pressed	*/
#define KEY_ALT_PGUP		0x7e86	/* Alt-pgup key pressed	*/
#define KEY_ALT_END		0x7e87	/* Alt-end  key pressed */
#define KEY_MAKE_FN		0x7e88	/* FN key make key pressed */
#define KEY_BREAK_FN		0xfe88	/* FN key break	key pressed */
#define KEY_ALT_LEFT		0x7e89	/* Alt-left key pressed */


#define SPECIAL_MAKE_HIGH_BYTE	 0x7e
#define SPECIAL_BREAK_HIGH_BYTE  0xfe



#define BREAK_PENDING		0x7e7e	 /* Break indicator code returned */
					 /* m_QuickKeyCheck for Ctrl-Break */
#define SHIFT_STATE_CHANGE	0x7e7d	 /* Caps, Num, Scroll state changed */
					/* from last m_QuickKeyCheck call */

#define SWAP_PENDING		0x7e7c	/* Return from m_QuickKeyCheck if */

					/* ALarm or special request in effect */

#define CALARMAP_HOTKEY 	0xff00	 /* CALARMAP.EXE hot key  */

/*

     stored at 0000:0418h.

     Input			 Output

     AH = 02h for shift flags	 AL = Flags (both 02h and 12h)
	= 12h for extended
	  shift flags		  7 6 5 4 3 2 1 0
				 ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
				 ³ ³ ³ ³ ³ ³ ³ ³ ³
				 ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙ
				  ³ ³ ³ ³ ³ ³ ³ ³
				  ³ ³ ³ ³ ³ ³ ³ ÀÄ Right SHIFT key is down
				  ³ ³ ³ ³ ³ ³ ÀÄÄÄ Left SHIFT key is down
				  ³ ³ ³ ³ ³ ÀÄÄÄÄÄ CTRL key is down
				  ³ ³ ³ ³ ÀÄÄÄÄÄÄÄ ALT key is down
				  ³ ³ ³ ÀÄÄÄÄÄÄÄÄÄ SCROLL LOCK on
				  ³ ³ ÀÄÄÄÄÄÄÄÄÄÄÄ NUM LOCK on
				  ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄ CAPS LOCK on
				  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ INS on



				 AH = Extended flags (12h only)

				  7 6 5 4 3 2 1 0
				 ÚÄÂÄÂÄÂÄÂÄÂÄÂÄÂÄ¿
				 ³ ³ ³ ³ ³ ³ ³ ³ ³
				 ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÁÄÙ
				  ³ ³ ³ ³ ³ ³ ³ ³
				  ³ ³ ³ ³ ³ ³ ³ ÀÄ Left CTRL key is down
				  ³ ³ ³ ³ ³ ³ ÀÄÄÄ Left ALT key is down
				  ³ ³ ³ ³ ³ ÀÄÄÄÄÄ Right CTRL key is down
				  ³ ³ ³ ³ ÀÄÄÄÄÄÄÄ Right ALT key is down
				  ³ ³ ³ ÀÄÄÄÄÄÄÄÄÄ Scroll key is down
				  ³ ³ ÀÄÄÄÄÄÄÄÄÄÄÄ NUM LOCK key is down
				  ³ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄ CAPS LOCK key is down
				  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ SYSREQ key is down


*/


#define SYSTEM_MANAGER_VERSION	0x200	/* Version number of system manager */


/* ==========================================================================
** Memory check values
========================================================================== */

/* Minimum number of paras required in paras */

#define  MEM_SYSMGR           50  * 64       /* System manager */
#define  MEM_SETUP            15  * 64       /* Set up */
#define  MEM_FILER            55  * 64       /* Filer */
#define  MEM_CLIPBOARD        1   * 64       /* Clipboard */

#define  MEM_CLIENT           5   * 64       /* Redirector client */
#define  MEM_SERVER           5   * 64       /* Redirector server */

#define  MEM_EXTRA            MEM_SETUP + MEM_FILER 

#define  MEM_MINIMUM_TO_LOAD     MEM_SYSMGR + MEM_EXTRA + MEM_CLIPBOARD
                                                       



/* SCCS info:   Module interfac.h  Version 1.2  Date 11/11/93  Time 13:21:58 */

/*---------------------------------------------------------------------------
 * HP Modification History
 *
 * 11-11-93 ltw	Incorporated Andy's changes for adding m_qualify_path.
 *
 *-------------------------------------------------------------------------*/

/* $Header:   T:/HH/INCLUDE/VCS/INTERFAC.H_V   1.64   25 Mar 1993 05:41:10   KIM  $
/****************************************************************
 *
 *      Copyright(c) 1990  Lotus Development Corporation
 *      All Rights Reserved
 *
 ****************************************************************/

/*
 * $Log:   T:/HH/INCLUDE/VCS/INTERFAC.H_V  $
 * 
 *    Rev 1.64   25 Mar 1993 05:41:10   KIM
 * Moved entry for ComAutoSet to private.h/mac. Non-public routine.
 * 
 *    Rev 1.63   09 Mar 1993 00:47:16   RICHB
 * New function ComAutoSet for the autostart listener.
 * 
 *    Rev 1.62   08 Mar 1993 15:38:44   PETERR
 * Added new entry for resetting sysmgr critical error flag. -_m_reset_DOS_error
 * 
 *    Rev 1.61   02 Mar 1993 00:46:56   KIM
 * Changed scheme: m_signal_via_dispatcher not needed after all.
 * 
 *    Rev 1.60   01 Mar 1993 23:06:28   KIM
 * Added new internal (and private) function: m_signal_via_dispatcher.
 * 
 *    Rev 1.59   03 Feb 1993 17:13:44   KIM
 * Added ComPower() function.
 * 
 *    Rev 1.58   22 Jan 1993 16:39:42   PETERR
 * Made order of new functions compatible w/ HP
 * 
 *    Rev 1.56   21 Jan 1993 16:47:50   PETERR
 * Added call to return size in bytes of user avail mem.
 * m_get_initial_mem
 * 
 *    Rev 1.55   20 Jan 1993 16:44:08   PETERR
 * Added new function m_get_far_user_handle
 * 
 *    Rev 1.54   05 Jan 1993 20:00:14   KIM
 * Added SC_RESV (57h) for use by ccMail.
 * 
 *    Rev 1.53   22 Dec 1992 02:07:56   KIM
 * Reserved Class 59h for internal use by System Manager.
 * 
 *    Rev 1.52   02 Dec 1992 10:48:26   HOMI
 * Removed definition of m_rep_name(); this function being un-coded.
 * 
 *    Rev 1.51   30 Nov 1992 14:52:38   HOMI
 * Rename m_task_sp_status() and m_current_sp_status() to ..._state() as 
 * named in the documentation.
 * 
 *    Rev 1.50   25 Nov 1992 15:28:50   PETERR
 * Added two new routines
 * 
 *    Rev 1.49   05 Nov 1992 20:22:32   KIM
 * SPR(27269): m_get_far_handle defined to c_service, since it returns an INT.
 * 
 *    Rev 1.48   05 Nov 1992 12:03:18   PETERR
 * Again??
 * 
 *    Rev 1.47   05 Nov 1992 11:35:22   PETERR
 * Try again!!
 * 
 *    Rev 1.46   05 Nov 1992 10:33:32   PETERR
 * Fixed problems with get_static prototype
 * 
 *    Rev 1.45   04 Nov 1992 14:19:06   PETERR
 * Added keybez routine
 * 
 *    Rev 1.44   04 Nov 1992 10:11:16   HOMI
 * Added playback_macro call.
 * 
 *    Rev 1.43   24 Oct 1992 00:55:12   KIM
 * Added m_set_video_mode() function.  For Cougar: replaces m_setmode().
 * 
 *    Rev 1.42   19 Oct 1992 11:41:08   PETERR
 * Fixed sup function name problem
 * 
 *    Rev 1.41   19 Oct 1992 03:44:42   RICHB
 * Added a pair of com driver function calls for internal use.
 * 
 *    Rev 1.40   17 Oct 1992 13:39:32   PETERR
 * Added new functions
 * 
 *    Rev 1.39   15 Oct 1992 10:50:56   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.42   13 Oct 1992 16:00:20   HOMI
 * Added macro start and stop recording calls.
 * 
 *    Rev 1.41   09 Oct 1992 10:18:24   PETERR
 * Changed prototype to avoid conflict with already prototyped parameter for
 * far pointer.
 * 
 *    Rev 1.40   08 Oct 1992 18:37:10   KIM
 * Added HELPSTRUCT parameter to m_help2_getmetrics.
 * 
 *    Rev 1.39   08 Oct 1992 18:17:58   KIM
 * Added m_help2_getmetrics().
 * 
 *    Rev 1.38   06 Oct 1992 16:46:44   HOMI
 * Removed extra param in m_keylock() and m_keyunlock() #defines. 
 * 
 *    Rev 1.37   01 Oct 1992 17:57:36   PETERR
 * Added new function to return drive list m_get_drive_list
 * 
 *    Rev 1.36   30 Sep 1992 17:23:44   PETERR
 * Changed params on calls
 * 
 *    Rev 1.35   28 Sep 1992 16:54:38   DARYL
 * Added angle brackets ("<>") around the XBS_INTER.H #include file so that
 * the apps can actually be compiled.
 * 
 *    Rev 1.34   25 Sep 1992 17:16:24   PETERR
 * Added include call for XBS functions.
 * 
 *    Rev 1.33   24 Sep 1992 11:48:22   HOMI
 * Added m_keylock() and m_keyunlock() #defines.
 * 
 *    Rev 1.32   22 Sep 1992 20:05:08   KIM
 * Changed parameter list for m_help2_? routines.
 * 
 *    Rev 1.31   18 Sep 1992 16:39:30   PETERR
 * Moved quickkeycheck to TSR/EMUL
 * 
 *    Rev 1.30   18 Sep 1992 11:33:14   KIM
 * Design change: Removed m_help_subsys and added m_help2_? routines,
 * which duplicate the HP95LX m_help_? routines.
 * 
 *    Rev 1.29   17 Sep 1992 20:42:04   KIM
 * Added m_help_subsys() for the new help subsystem in Sys Mgr.
 * 
 *    Rev 1.28   16 Sep 1992 18:25:50   HOMI
 * Added m_write_printer_atb() and m_trans_printer_atb().
 * 
 *    Rev 1.27   09 Sep 1992 11:06:06   PETERR
 * Added two new functions to get and set DOS hot key
 * 
 *    Rev 1.26   08 Sep 1992 16:57:42   HOMI
 * Added new m_get_cb_info() #define.
 * 
 *    Rev 1.25   08 Sep 1992 16:43:46   PETERR
 * Added new calls
 * 
 *    Rev 1.24   01 Sep 1992 18:12:32   HOMI
 * Added DISTANCE #define for HP. Allows c_service routines to be declared near 
 * or far, overriding the compiler model.
 * 
 *    Rev 1.23   31 Aug 1992 13:35:10   RICHB
 * Fixed cut and paste error.
 * 
 *    Rev 1.22   28 Aug 1992 08:08:48   PETERR
 * Fixed syntax error in m_play prototype
 * 
 *    Rev 1.21   17 Aug 1992 11:46:36   HOMI
 * Added dummy parameter to c_service_get_avail_mem() to avoid compilation 
 * warning.
 * 
 *    Rev 1.20   12 Aug 1992 16:20:16   HOMI
 * Added SC_MAXNUM_JAG for jaguar dispatching check
 * 
 *    Rev 1.19   11 Aug 1992 13:24:54   RICHB
 * Fixed misspelled status
 * 
 *    Rev 1.18   10 Aug 1992 07:44:14   RICHB
 * Added a modem status call.
 * 
 *    Rev 1.17   31 Jul 1992 17:10:20   HOMI
 * Modified for API2, functions now start from 1 instead of 0.
 * 
 *    Rev 1.16   30 Jul 1992 09:48:12   PETERR 
 * Added new function m_play 
 * $Log$ 
 * 
 *    Rev 1.15   29 Jul 1992 12:55:00   HOMI
 * Added API management function declarations
 * 
 *    Rev 1.14   26 Jul 1992 21:06:42   RICHB
 * Added Cougar com driver calls
 * 
 *    Rev 1.13   24 Jul 1992 14:27:48   PETERR
 * Added new key codes for Scroll and Caps Lock
 * 
 *    Rev 1.12   23 Jul 1992 21:50:22   DAVID
 * Revised definitions for PARSEKEY functions (m_InitKeyLabels, m_GetScanCode,
 * m_GetKeyLabel)
 * 
 *    Rev 1.11   22 Jul 1992 15:50:06   HOMI
 * Added m_task_info() declarations from taskinfo.h
 * 
 *    Rev 1.10   08 Jul 1992 22:29:16   KIM
 * Changed the location of "2" in the entry point name for new API2 routines
 * in CSVC.ASM.
 * 
 *    Rev 1.9   08 Jul 1992 11:28:00   HOMI
 * New API changes.
 * 
 *    Rev 1.8   25 Jun 1992 16:21:18   PETERR
 * Changed order of M_action in jump table for tkernel support
 * 
 *    Rev 1.7   12 Jun 1992 11:28:24   PETERR
 * New alarm call m_nalarm obsoletes m_alarm call
 * 
 *    Rev 1.6   20 May 1992 13:14:20   HOMI
 * Added #define for m_current_task
 * 
 *    Rev 1.5   20 May 1992 13:06:02   PETERR
 * Added new calls for service requests
 * 
 *    Rev 1.4   20 May 1992 12:01:30   KIM
 * Recovered changes that were lost.
 * 
 *    Rev 1.3   13 May 1992 13:46:58   PETERR
 * Added calls for get and free service entry
 * 
 * -------------- Recovered from R1.2 --------------------------------------
 *
 *    Rev 1.2   11 May 1992 15:48:04   KIM
 * Missed Homi's changes when I migrated this file from the SysMgr directory.
 * 
 *    Rev 1.1   11 May 1992 15:23:18   KIM
 * I'm putting the latest version of this file, into this common directory,
 * and deleting it from the SysMgr directory.
 *
 *    Rev 1.12   08 May 1992 17:21:36   HOMI
 * Added #define for file flush
 * 
 * -------------- Recovered from R1.2 --------------------------------------
 * 
 *    Rev 1.11   01 May 1992 16:58:24   DAVID
 * Moved parsekey functions from MISC to RSRC table
 * 
 *    Rev 1.10   01 May 1992 12:04:42   HOMI
 * Moved task_info define from event section to memory section
 * 
 *    Rev 1.9   30 Apr 1992 17:27:08   HOMI
 * Added task_info service macro
 * 
 *    Rev 1.8   30 Apr 1992 15:48:10   PETERR
 * New static RAM functionality
 * 
 *    Rev 1.7   09 Apr 1992 10:22:28   HOMI
 * Added Modify Hot Key function
 * 
 *    Rev 1.6   08 Apr 1992 23:52:58   DAVID
 * Fixed typo
 * 
 *    Rev 1.5   08 Apr 1992 23:43:08   DAVID
 * Resolved name conflicts
 * 
 *    Rev 1.4   08 Apr 1992 18:15:06   DAVID
 * Added key parsing functions
 * 
 *    Rev 1.3   25 Mar 1992 12:39:36   PETERR
 * Syntax error correction
 * 
 *    Rev 1.2   25 Mar 1992 11:46:28   PETERR
 * Added get far handle entry
 * 
 *    Rev 1.1   23 Mar 1992 15:14:10   PETERR
 * Added new code for DOS launch
 * 
 *    Rev 1.0   09 Mar 1992 12:01:54   PETERR
 * DOS mem & launch changes
 * 
 *    Rev 1.0   03 Dec 1991 20:14:42   DAVID
 * Initial revision.
 */

/****************************************************************
 * Jaguar Accessories Kernel
 *
 *      Copyright(c) 1990
 *      Lotus Development Corporation
 *      All Rights Reserved
 *
 *
 * INTERFAC.H -- prototypes and macros for System Services
 *               Can be used by callers from applications and
 *               from the System Manager itself.
 ****************************************************************/




/**************************************************************
  Services that are implemented in C in the System Manager
  are accessed through a common interface routine:

	c_service()

  Each of the applicable services has a macro implemented here.
  In addition to the documented arguments for the services,
  the macros makes the call to service with an argument 
  identifying the function.


  The extra constant is the service identifier.  This should be
  a word sized value, with the high byte equaling the service 
  class, and the low byte  equal to service number.  These 
  values must match the values in the general services 
  dispatcher (SERVICES.ASM) and the class specific dispatchers.  

  Note that ALL
  pointer arguments are to be FAR (4 bytes) when passed to the
  System Manager, despite what the specification may currently
  say.  


  Finally, the macro itself makes a call to c_service(), with
  the above constants used as the first argument.  Following
  that are the actual arguments as supplied.  IMPORTANT NOTE: 
  all pointer arguments should be cast INSIDE THE MACRO 
  DEFINITION to far.  This is the simplest way to make sure
  that near pointers inside applications get expanded to the
  proper size.



 **************************************************************

  All other functions that may be called by applications should
  have function prototypes in this file.  Again, ALL pointers
  will be FAR.  By using the prototype, the compiler will push  
  the segment value onto the stack, even though the parameter
  actually supplied may be a near pointer. 

  Each of these functions must have assembly language wrappers
  written to make the SERVICES calls.

 **************************************************************


 





/**************************************************************

 Below are function prototypes for the various forms of
 c_service(), depending on return type.  The services 
 below should use the correct form in order to specify
 the proper return value of the function.
 The two pointer versions of the call are just labels
 in csvc.asm to c_service() itself, thus only one
 function is called.
 -mdb

 **************************************************************/
 
#ifdef   API2
   #define  c_service                  c_service2
   #define  c_service_near_ptr         c_service2_near_ptr
   #define  c_service_far_ptr          c_service2_far_ptr

   /**************  Memory block prototypes *********************/

   #define  c_service_based_ptr        c_service2_based_ptr
   #define  c_service_far_mem          c_service2_far_mem
   #define  c_service_reg_based        c_service2_reg_based
   #define  c_service_long_based       c_service2_long_based
   #define  c_service_long_far         c_service2_long_far
   #define  c_service_reg_far          c_service2_reg_far
   #define  c_service_free_static      c_service2_free_static
   #define  c_service_get_avail_mem    c_service2_get_avail_mem

   #define  CLASS_BASE     0x40
   #define  FNCT_BASE      0x01
   #define  SC_APIMGT      (CLASS_BASE + 0)
#else
   #define  CLASS_BASE     0x0
   #define  FNCT_BASE      0x0
#endif      

/* ----------------------------------------------------------------------- */

#if !defined(DISTANCE)
   #define  DISTANCE
#endif

int DISTANCE c_service(unsigned int,...);
void near * DISTANCE c_service_near_ptr (unsigned int,...);
void far * DISTANCE c_service_far_ptr(unsigned int,...);

/**************  Memory block prototypes *********************/
int  DISTANCE c_service_based_ptr(unsigned int dummy1,_segment *dummy2,unsigned long int dummy3);
int  DISTANCE c_service_far_mem(unsigned int dummy1,void *dummy2,unsigned long int dummy3);
int  DISTANCE c_service_get_static(unsigned int dummy1,void far *dummy2,unsigned long int dummy3);
void DISTANCE c_service_reg_based(unsigned int dummy1, _segment *dummy2 ,unsigned int dummy3);
int  DISTANCE c_service_long_based(unsigned int dummy1,_segment *dummy2,unsigned long int dummy3);
int  DISTANCE c_service_long_far(unsigned int dummy1,void *dummy2,unsigned long int dummy3);
void DISTANCE c_service_reg_far(unsigned int dummy1, void *dummy2 ,unsigned int dummy3, unsigned int dummy4);
int  DISTANCE c_service_free_static(unsigned int dummy1,void far *dummy2);
unsigned long int DISTANCE c_service_get_avail_mem(unsigned int dummy1);

/* assign constants for the service class numbers */
/* SC_MAXNUM is the maximum class number allowed */

#define SC_FP           (CLASS_BASE + 0)
#define SC_EVENT        (CLASS_BASE + 1)
#define SC_MENU         (CLASS_BASE + 2)
#define SC_SCREEN       (CLASS_BASE + 3)
#define SC_EDIT         (CLASS_BASE + 4)
#define SC_FILE         (CLASS_BASE + 5)
#define SC_PM           (CLASS_BASE + 6)
#define SC_CB           (CLASS_BASE + 7)
#define SC_SOUND        (CLASS_BASE + 8)
#define SC_MM           (CLASS_BASE + 9)
#define SC_BG           (CLASS_BASE + 10)
#define SC_DTIME        (CLASS_BASE + 11)
#define SC_PRINTER      (CLASS_BASE + 12)
#define SC_CONFIG       (CLASS_BASE + 13)
#define SC_COMM         (CLASS_BASE + 14)
#define SC_MISC         (CLASS_BASE + 15)
#define SC_VIDVIR       (CLASS_BASE + 16)
#define SC_VIDPHY       (CLASS_BASE + 17) /* not used directly by accessories */
#define SC_GRDISP       (CLASS_BASE + 18)
#define SC_KEY          (CLASS_BASE + 19)
#define SC_RSRC         (CLASS_BASE + 20)
#define SC_HELP         (CLASS_BASE + 21)
#define SC_COL		    (CLASS_BASE + 22)
#define SC_RESV		    (CLASS_BASE + 23) /* Reserved for ccMail use */

#define SC_MAXNUM_JAG   22                  /* for jag error check */

#define SC_RESERVED1    (CLASS_BASE + 25)   /* For Sys Mgr internal use */

#ifdef XBS
#define SC_XBS_UNIQUE	(CLASS_BASE + 26)   /* Different platform */
else
#define IT_IS_RESERVED	(CLASS_BASE + 26)   /* Do not use */
#endif

#define SC_QUALIFY     (CLASS_BASE + 29)    /* QP!AJG m_qualify_path */

#ifdef   API2

/* SC_APIMGT */

#define F_M_GET_CLASS_TBL              (SC_APIMGT * 256) + 0 + FNCT_BASE
#define m_get_class_tbl()                    \
	c_service_far_ptr(F_M_GET_CLASS_TBL)
   
#define F_M_INSERT_FNCT_TBL            (SC_APIMGT * 256) + 1 + FNCT_BASE
#define m_insert_fnct_tbl(Class,FnctTbl)     \
	c_service(F_M_INSERT_FNCT_TBL,Class,(void far *)(FnctTbl))

#define F_M_REMOVE_FNCT_TBL            (SC_APIMGT * 256) + 2 + FNCT_BASE
#define m_remove_fnct_tbl(Class,FnctTbl)    \
	c_service(F_M_REMOVE_FNCT_TBL,Class,(void far *)(FnctTbl))

#define F_M_DISABLE_FNCT_TBL           (SC_APIMGT * 256) + 3 + FNCT_BASE
#define m_disable_fnct_tbl(ClassFnct)        \
	c_service(F_M_DISABLE_FNCT_TBL,ClassFnct)

#define F_M_ENABLE_FNCT_TBL            (SC_APIMGT * 256) + 4 + FNCT_BASE
#define m_enable_fnct_tbl(ClassFnct)          \
	c_service(F_M_ENABLE_FNCT_TBL,ClassFnct)

#define F_M_GET_FREE_API_CLASS         (SC_APIMGT * 256) + 5 + FNCT_BASE
#define m_get_free_api_class(ClassType)        \
	c_service(F_M_GET_FREE_API_CLASS,ClassType)

#define F_M_SEARCH_API_CLASS           (SC_APIMGT * 256) + 6 + FNCT_BASE
#define m_search_api_class(ClassType,String,Len)   \
	c_service(F_M_SEARCH_API_CLASS,ClassType,(void far *)String,Len)

#define F_M_GET_API_ERROR              (SC_APIMGT * 256) + 7 + FNCT_BASE
#define m_get_api_error() \
	c_service(F_M_GET_API_ERROR)

#define F_M_CLEAR_API_ERROR            (SC_APIMGT * 256) + 8 + FNCT_BASE
#define m_clear_api_error() \
	c_service(F_M_CLEAR_API_ERROR)

#define F_M_VALID_API_FNCT             (SC_APIMGT * 256) + 9 + FNCT_BASE
#define m_valid_api_fnct(ClassFnct) \
	c_service(F_M_VALID_API_FNCT,ClassFnct)

#define  API_RSRC_LIB   0           /* resource lib service */     
#define  API_DRIVERS    1           /* drivers              */
#define  API_SYSMGR     2           /* system manager       */
#define  API_APPS       3           /* application specific */

#endif


/* SC_EVENT */

#define F_M_EVENT       (SC_EVENT * 256) + 0 + FNCT_BASE
#define m_event(a)      \
	c_service(F_M_EVENT,(void far *)(a) )

#define F_M_NEVENT    (SC_EVENT * 256) + 1 + FNCT_BASE
#define m_nevent(a)     \
	c_service(F_M_NEVENT,(void far *)(a))

#define F_M_SH_STATUS (SC_EVENT * 256) + 2 + FNCT_BASE
#define m_sh_status()   \
	c_service(F_M_SH_STATUS)

#define F_M_SETSHIFTS  (SC_EVENT * 256) + 3 + FNCT_BASE
#define m_setshifts()   \
	c_service(F_M_SETSHIFTS)

#define F_M_BRIDGE_SERV (SC_EVENT * 256) + 4 + FNCT_BASE
#define bridge_serv(a) \
	c_service(F_M_BRIDGE_SERV,(void far *)(a))

#define F_M_FLUSH_KB    (SC_EVENT * 256) + 5 + FNCT_BASE
#define m_flush_kb()    \
	c_service(F_M_FLUSH_KB)

#define F_M_YIELD       (SC_EVENT * 256) + 6 + FNCT_BASE
#define m_yield(a)      \
	c_service(F_M_YIELD,(void far *)(a) )

#define F_M_NO_FINI     (SC_EVENT * 256) + 7 + FNCT_BASE
#define m_no_fini(a)    \
	c_service(F_M_NO_FINI,(void far *)(a) )

#define F_M_ACTION	(SC_EVENT * 256) + 8 + FNCT_BASE
#define m_action(a)     \
	c_service(F_M_ACTION,(void far *)(a) )


#define F_M_LAUNCH	(SC_EVENT * 256) + 9 + FNCT_BASE
#define m_launch(a)     \
	c_service(F_M_LAUNCH,(void far *)(a) )

#define F_M_GET_RUN_MODE        (SC_EVENT * 256) + 10 + FNCT_BASE
#define m_get_run_mode()        \
	c_service(F_M_GET_RUN_MODE )

#define F_M_IC_INIT     (SC_EVENT * 256) + 11 + FNCT_BASE
#define m_ic_init(a)    \
	c_service(F_M_IC_INIT,(void far *)(a) )


#define F_M_IC_CLOSE    (SC_EVENT * 256) + 12 + FNCT_BASE
#define m_ic_close(a)   \
	c_service(F_M_IC_CLOSE,(void far *)(a) )


#define F_M_SWAP	(SC_EVENT * 256) + 13 + FNCT_BASE
#define m_swap(a)       \
	c_service(F_M_SWAP,(void far *)(a) )

#define F_M_EXEC        (SC_EVENT * 256) + 14 + FNCT_BASE
#define m_do_exec(a)    \
	c_service(F_M_EXEC,(void far *)(a) )


#define F_M_DOS_CLOSE   (SC_EVENT * 256) + 15 + FNCT_BASE
#define m_do_DOS_close(a)       \
	c_service(F_M_EXEC,(void far *)(a) )












/*SC_MENU */


#define F_MENU_SETUP    (SC_MENU * 256) + 0 + FNCT_BASE
#define menu_setup(a,b,c,d,e,f,g)       \
	c_service(F_MENU_SETUP,(void far *)(a),(void far *)(b), \
		(c),(d),(void far *)(e),(f),(void far *)(g) )

#define F_MENU_DIS      (SC_MENU * 256) + 1 + FNCT_BASE
#define menu_dis(a)     \
	c_service(F_MENU_DIS,(void far *)(a))


#define F_MENU_ON       (SC_MENU * 256) + 2 + FNCT_BASE
#define menu_on(a)      \
	c_service(F_MENU_ON,(void far *)(a))

#define F_MENU_OFF      (SC_MENU * 256) + 3 + FNCT_BASE
#define menu_off(a)     \
	c_service(F_MENU_OFF,(void far *)(a))

#define F_MENU_KEY      (SC_MENU * 256) + 4 + FNCT_BASE
#define menu_key(a,b,c) \
	c_service(F_MENU_KEY,(void far *)(a),(b),(void far *)(c))


#define F_FMENU_INIT      (SC_MENU * 256) + 5 + FNCT_BASE
#define fmenu_init(a,b,c,d,e)     \
	c_service(F_FMENU_INIT,(void far *)(a),(void far *)(b),(char far *)(c),(d),(e))


#define F_FMENU_DIS      (SC_MENU * 256) + 6 + FNCT_BASE
#define fmenu_dis(a,b)    \
	c_service(F_FMENU_DIS,(void far *)(a),(void far *)(b))

#define F_FMENU_KEY      (SC_MENU * 256) + 7 + FNCT_BASE
#define fmenu_key(a,b,c) \
	c_service(F_FMENU_KEY,(void far *)(a),(void far *)(b),(c))

#define F_FMENU_OFF       (SC_MENU * 256) + 8 + FNCT_BASE
#define fmenu_off(a,b)   \
	c_service(F_FMENU_OFF,(void far *)(a),(void far *)(a))




/*SC_SCREEN*/


#define F_M_DISP        (SC_SCREEN * 256) + 0 + FNCT_BASE
#define m_disp(a,b,c,d,e,f)             \
	c_service(F_M_DISP,a,b,(void far *)(c),d,e)

#define F_M_CLEAR       (SC_SCREEN * 256) + 1 + FNCT_BASE
#define m_clear(a,b,c,d)             \
	c_service(F_M_CLEAR,a,b,c,d)

#define F_M_SCROLL      (SC_SCREEN * 256) + 2 + FNCT_BASE
#define m_scroll(a,b,c,d,e)             \
	c_service(F_M_SCROLL,a,b,c,d,e)

#define F_M_XCHG        (SC_SCREEN * 256) + 3 + FNCT_BASE
#define m_xchg(a,b,c,d,e)             \
	c_service(F_M_XCHG,a,b,c,d,(void far *)(e))

#define F_M_CHRATTR     (SC_SCREEN * 256) + 4 + FNCT_BASE
#define m_chrattr(a,b)             \
	c_service(F_M_CHRATTR,(void far *)(a),b)

#define F_M_CHRRVRT     (SC_SCREEN * 256) + 5 + FNCT_BASE
#define m_chrrvrt(a,b,c,d)             \
	c_service(F_M_CHRRVRT,a,b,(void far *)(c),d)

#define F_M_CHRGET      (SC_SCREEN * 256) + 6 + FNCT_BASE
#define m_chrget(a,b,c,d)             \
	c_service(F_M_CHRGET,a,b,(void far *)(c),d)

#define F_M_CHRINV      (SC_SCREEN * 256) + 7 + FNCT_BASE
#define m_chrinv(a,b,c)             \
	c_service(F_M_CHRINV,a,b,c)

#define F_M_ROWS_COLS   (SC_SCREEN * 256) + 8 + FNCT_BASE
#define m_rows_cols()             \
	c_service(F_M_DISP)

#define F_M_SETMODE     (SC_SCREEN * 256) + 9 + FNCT_BASE
#define m_setmode(a)            \
	c_service(F_M_SETMODE,a)

#define F_M_GETMODE     (SC_SCREEN * 256) +10 + FNCT_BASE
#define m_getmode(a)            \
	c_service(F_M_GETMODE,a)

#define F_M_SETCUR      (SC_SCREEN * 256) +11 + FNCT_BASE
#define m_setcur(a,b)            \
	c_service(F_M_SETCUR,a,b)

#define F_M_SET_VIDEO_MODE (SC_SCREEN * 256) + 12 + FNCT_BASE
#define m_set_video_mode(a)            \
	c_service(F_M_SET_VIDEO_MODE,a)



/*SC_EDIT*/

#define F_EDIT_INIT     (SC_EDIT * 256) + 0 + FNCT_BASE
#define edit_init(a,b,c,d,e,f)          \
	c_service(F_EDIT_INIT,(void far *)(a),(void far *)(b),c,d,e,f)

#define F_EDIT_TOP      (SC_EDIT * 256) + 1 + FNCT_BASE
#define edit_top(a,b,c,d,e,f,g,h)          \
	c_service(F_EDIT_TOP,(void far *)(a),(void far *)(b),c,d,(void far *)(e), \
		   f,(void far *)(g),h)

#define F_EDIT_DIS      (SC_EDIT * 256) + 2 + FNCT_BASE
#define edit_dis(a)                     \
	c_service(F_EDIT_DIS,(void far *)(a))

#define F_EDIT_KEY      (SC_EDIT * 256) + 3 + FNCT_BASE
#define edit_key(a,b,c)                 \
	c_service(F_EDIT_KEY,(void far *)(a),b,(void far *)(c))

#define F_MDIT_INI      (SC_EDIT * 256) + 4 + FNCT_BASE
#define mdit_ini(a,b,c,d,e,f,g,h,i,j)        \
	c_service(F_MDIT_INI,(void far *)(a),b,c,d,e,(void far *)(f),g,h,i,(void far *)j)

#define F_MDIT_DIS      (SC_EDIT * 256) + 5 + FNCT_BASE
#define mdit_dis(a)                    \
	c_service(F_MDIT_DIS,(void far *)(a))

#define F_MDIT_KEY      (SC_EDIT * 256) + 6 + FNCT_BASE
#define mdit_key(a,b)                   \
	c_service(F_MDIT_KEY,(void far *)(a),b)

#define F_MDIT_FIL      (SC_EDIT * 256) + 7 + FNCT_BASE
#define mdit_fil(a,b)                   \
	c_service(F_MDIT_FIL,(void far *)(a),(void far *)(b))


#define F_MDIT_MARK     (SC_EDIT * 256) + 8 + FNCT_BASE
#define mdit_mark(a)                    \
	c_service(F_MDIT_MARK,(void far *)(a))

#define F_MDIT_UNMARK   (SC_EDIT * 256) + 9 + FNCT_BASE
#define mdit_unmark(a)                  \
	c_service(F_MDIT_UNMARK,(void far *)(a))


#define F_MDIT_CUTMARK  (SC_EDIT * 256) + 10 + FNCT_BASE
#define mdit_cutmark(a)                 \
	c_service(F_MDIT_CUTMARK,(void far *)(a))

#define F_MDIT_INS_STR  (SC_EDIT * 256) + 11 + FNCT_BASE
#define mdit_ins_str(a,b,c)                     \
	c_service(F_MDIT_INS_STR,(void far *)(a),(void far *)(b),c)



/*SC_FILE*/

#define F_M_OPEN        (SC_FILE * 256) + 0 + FNCT_BASE
#define m_open(a,b,c,d,e)               \
	c_service(F_M_OPEN,(void far *)(a),(void far *)(b),c,d,e)

#define F_M_OPENRO      (SC_FILE * 256) + 1 + FNCT_BASE
#define m_openro(a,b,c,d,e)               \
	c_service(F_M_OPENRO,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_CREATE      (SC_FILE * 256) + 2 + FNCT_BASE
#define m_create(a,b,c,d,e)               \
	c_service(F_M_CREATE,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_FCREAT      (SC_FILE * 256) + 3 + FNCT_BASE
#define m_fcreat(a,b,c,d,e)               \
	c_service(F_M_FCREAT,(void far *)(a),(void far *)(b),c,d,e)


#define F_M_READ        (SC_FILE * 256) + 4 + FNCT_BASE
#define m_read(a,b,c,d)               \
	c_service(F_M_READ,(void far *)(a),(void far *)(b),c,(void far *)(d))


#define F_M_WRITE       (SC_FILE * 256) + 5 + FNCT_BASE
#define m_write(a,b,c)               \
	c_service(F_M_WRITE,(void far *)(a),(void far *)(b),c)


#define F_M_SEEK        (SC_FILE * 256) + 6 + FNCT_BASE
#define m_seek(a,b,c)               \
	c_service(F_M_SEEK,(void far *)(a),b,c)


#define F_M_TELL        (SC_FILE * 256) + 7 + FNCT_BASE
#define m_tell(a,b)               \
	c_service(F_M_TELL,(void far *)(a),(void far *)(b))


#define F_M_CLOSE       (SC_FILE * 256) + 8 + FNCT_BASE
#define m_close(a)               \
	c_service(F_M_CLOSE,(void far *)(a))


#define F_M_SETPAT      (SC_FILE * 256) + 9 + FNCT_BASE
#define m_setpat(a,b,c,d)               \
	c_service(F_M_SETPAT,(void far *)(a),(void far *)(b),c,d)


#define F_M_MATCH       (SC_FILE * 256) + 10 + FNCT_BASE
#define m_match(a,b)               \
	c_service(F_M_MATCH,(void far *)(a),(void far *)(b))


#define F_M_IDENT       (SC_FILE * 256) + 11 + FNCT_BASE
#define m_ident(a,b,c,d)               \
	c_service(F_M_IDENT,(void far *)(a),b,c,(void far *)(d))


#define F_M_DELETE      (SC_FILE * 256) + 12 + FNCT_BASE
#define m_delete(a,b,c)               \
	c_service(F_M_DELETE,(void far *)(a),b,c)


#define F_M_RENAME      (SC_FILE * 256) + 13 + FNCT_BASE
#define m_rename(a,b,c,d,e,f)               \
	c_service(F_M_RENAME,(void far *)(a),b,c,(void far *)(d),e,f)


#define F_M_GETDIR      (SC_FILE * 256) + 14 + FNCT_BASE
#define m_getdir(a,b,c)               \
	c_service(F_M_GETDIR,a,(void far *)(b),(void far *)(c))


#define F_M_SETDIR      (SC_FILE * 256) + 15 + FNCT_BASE
#define m_setdir(a,b)               \
	c_service(F_M_SETDIR,(void far *)(a),b)


#define F_M_VOLUME      (SC_FILE * 256) + 16 + FNCT_BASE
#define m_volume(a,b)               \
	c_service(F_M_VOLUME,(void far *)(a),(void far *)(b))


#define F_M_MKDIR       (SC_FILE * 256) + 17 + FNCT_BASE
#define m_mkdir(a,b,c)               \
	c_service(F_M_MKDIR,(void far *)(a),b,c)


#define F_M_RMDIR       (SC_FILE * 256) + 18 + FNCT_BASE
#define m_rmdir(a,b,c)               \
	c_service(F_M_RMDIR,(void far *)(a),b,c)


#define F_M_GETDRV      (SC_FILE * 256) + 19 + FNCT_BASE
#define m_getdrv(a)               \
	c_service(F_M_GETDRV,(void far *)(a))


#define F_M_SETDRV      (SC_FILE * 256) + 20 + FNCT_BASE
#define m_setdrv(a)               \
	c_service(F_M_SETDRV,a)


#define F_M_FDATE       (SC_FILE * 256) + 21 + FNCT_BASE
#define m_fdate(a,b)               \
	c_service(F_M_FDATE,(void far *)(a),(void far *)(b))


#define F_M_GET_SYSDIR  (SC_FILE * 256) + 22 + FNCT_BASE
#define m_get_sysdir(a)               \
	c_service(F_M_GET_SYSDIR,(void far *)(a))


#define F_M_GETATTR     (SC_FILE * 256) + 23 + FNCT_BASE
#define m_getattr(a,b,c,d)               \
	c_service(F_M_GETATTR,(void far *)(a),b,c,(void far *)(d))


#define F_M_SETATTR     (SC_FILE * 256) + 24 + FNCT_BASE
#define m_setattr(a,b,c,d)               \
	c_service(F_M_SETATTR,(void far *)(a),b,c,d)


#define F_M_COMMON_OPEN (SC_FILE * 256) + 25 + FNCT_BASE
#define m_common_open(a,b,c,d,e,f,g)              \
	c_service(F_M_COMMON_OPEN,(void far *)(a),(void far *)(b),c,d,e,f,g)

#define F_M_COPYDT      (SC_FILE * 256) + 26 + FNCT_BASE
#define m_copydt(a,b)   \
	c_service(F_M_COPYDT,(void far *)(a),(void far *)(b))


#define F_M_GETFDT      (SC_FILE * 256) + 27 + FNCT_BASE
#define m_getfdt(a,b)   \
	c_service(F_M_GETFDT,(void far *)(a),(void far *)(b))


#define F_M_PUTFDT      (SC_FILE * 256) + 28 + FNCT_BASE
#define m_putfdt(a,b)   \
	c_service(F_M_PUTFDT,(void far *)(a),b)

#define F_M_CARD_CHANGES (SC_FILE * 256) + 29 + FNCT_BASE
#define m_card_changes()   \
	c_service(F_M_CARD_CHANGES)

#define F_M_FLUSH (SC_FILE * 256) + 30 + FNCT_BASE
#define m_flush(a)         \
    c_service(F_M_FLUSH,(void far *)(a))

#define F_M_GET_DRIVE_LIST (SC_FILE * 256) + 31 + FNCT_BASE
#define m_get_drive_list(a)	    \
    c_service(F_M_GET_DRIVE_LIST,(void far *)(a))

#define F_M_RESET_DOS_ERROR (SC_FILE * 256) + 32 + FNCT_BASE
#define m_reset_DOS_error()	    \
    c_service(F_M_RESET_DOS_ERROR)





/*SC_PM*/


#define F_M_INIT        (SC_PM * 256) + 0 + FNCT_BASE
#define m_init()        c_service(F_M_INIT)

#define F_M_FINI        (SC_PM * 256) + 1 + FNCT_BASE
#define m_fini()        c_service(F_M_FINI)

#define F_M_LOCK        (SC_PM * 256) + 2 + FNCT_BASE
#define m_lock()        c_service(F_M_LOCK)

#define F_M_UNLOCK      (SC_PM * 256) + 3 + FNCT_BASE
#define m_unlock()      c_service(F_M_UNLOCK)

#define F_M_SPAWN       (SC_PM * 256) + 4 + FNCT_BASE
#define m_spawn(a,b,c,d)        \
	c_service(F_M_SPAWN,(void far *)(a),b,c, (void far *)(d))

#define F_M_APPCOUNT    (SC_PM * 256) + 5 + FNCT_BASE
#define m_appcount()    c_service(F_M_APPCOUNT)

#define F_M_REBOOT      (SC_PM * 256) + 6 + FNCT_BASE
#define m_reboot()      c_service(F_M_REBOOT)

#define F_M_SPAWNARG    (SC_PM * 256) + 7 + FNCT_BASE
#define m_spawnarg(a,b,c,d)     \
	c_service(F_M_SPAWNARG,(void far *)(a),b,(void far *)(c),d)

#define F_M_REG_APP_NAME (SC_PM * 256) + 8 + FNCT_BASE
#define m_reg_app_name(a)       \
	c_service(F_M_REG_APP_NAME,(void far *)(a))

#define  F_APP_NAME (SC_PM * 256) + 9 + FNCT_BASE
#define app_name(a)     \
	c_service_far_ptr(F_APP_NAME,(void far *)(a))

#define  F_M_INIT_APP (SC_PM * 256) + 10 + FNCT_BASE
#define m_init_app(a)	 \
	c_service(F_M_INIT_APP,a)

#define  F_M_KEYLOCK (SC_PM * 256) + 11 + FNCT_BASE
#define m_keylock()	      c_service(F_M_KEYLOCK)

#define  F_M_KEYUNLOCK (SC_PM * 256) + 12 + FNCT_BASE
#define m_keyunlock()	   c_service(F_M_KEYUNLOCK)



/*SC_CB*/


#define F_M_OPEN_CB     (SC_CB * 256) + 0 + FNCT_BASE
#define m_open_cb()             \
	c_service(F_M_OPEN_CB)

#define F_M_CLOSE_CB    (SC_CB * 256) + 1 + FNCT_BASE
#define m_close_cb()            \
	c_service(F_M_CLOSE_CB)

#define F_M_RESET_CB    (SC_CB * 256) + 2 + FNCT_BASE
#define m_reset_cb(a)           \
	c_service(F_M_RESET_CB,(void far *)(a))

#define F_M_GET_CB_INFO (SC_CB * 256) + 3 + FNCT_BASE
#define m_get_cb_info(count,author,cbsize,inuse)         \
	c_service(F_M_GET_CB_INFO, (void far *)(count), (void far *)(author),   \
                              (void far *)(cbsize), (void far *)(inuse))

#define F_M_NEW_REP     (SC_CB * 256) + 4 + FNCT_BASE
#define m_new_rep(a)             \
	c_service(F_M_NEW_REP,(void far *)(a))

#define F_M_FINI_REP    (SC_CB * 256) + 5 + FNCT_BASE
#define m_fini_rep()            \
	c_service(F_M_FINI_REP)

#define F_M_REP_INDEX   (SC_CB * 256) + 7 + FNCT_BASE
#define m_rep_index(a,b,c)      \
	c_service(F_M_REP_INDEX,(void far *)(a),(void far *)(b),(void far *)(c))

#define F_M_CB_WRITE    (SC_CB * 256) + 8 + FNCT_BASE
#define m_cb_write(a,b)         \
	c_service(F_M_CB_WRITE,(void far *)(a),b)

#define F_M_CB_READ     (SC_CB * 256) + 9 + FNCT_BASE
#define m_cb_read(a,b,c,d)      \
	c_service(F_M_CB_READ,a,b,(void far *)(c),d)



/*SC_SOUND*/


#define F_M_BEEP        (SC_SOUND * 256) + 0 + FNCT_BASE
#define m_beep()        c_service(F_M_BEEP)

#define F_M_THUD        (SC_SOUND * 256) + 1 + FNCT_BASE
#define m_thud()        c_service(F_M_THUD)

#define F_M_ASOUND      (SC_SOUND * 256) + 2 + FNCT_BASE
#define m_asound(a)     c_service(F_M_ASOUND,a)

#define F_M_SOUNDOFF    (SC_SOUND * 256) + 3 + FNCT_BASE
#define m_soundoff()    c_service(F_M_SOUNDOFF,a)

#define F_M_PLAY        (SC_SOUND * 256) + 4 + FNCT_BASE
#define m_play(a)	 c_service(F_M_PLAY,(void far *)(a))

/*SC_MM*/

#define F_M_ALLOC       (SC_MM * 256) + 0 + FNCT_BASE
#define m_alloc(a)              \
	c_service_near_ptr(F_M_ALLOC,a)

#define F_M_SHARE       (SC_MM * 256) + 1 + FNCT_BASE
#define m_share(a,b,c)       \
	c_service_far_ptr(F_M_SHARE,(void far *)(a),b,(void far *)(c))

#define F_M_FREE        (SC_MM * 256) + 2 + FNCT_BASE
#define m_free(a)            \
	c_service(F_M_FREE,a)

#define F_M_ALLOC_LARGE (SC_MM * 256) + 3 + FNCT_BASE
#define m_alloc_large(a)                \
	c_service_near_ptr(F_M_ALLOC_LARGE,a)

#define F_M_FREE_LARGE  (SC_MM * 256) + 4 + FNCT_BASE
#define m_free_large(a)         \
	c_service_near_ptr(F_M_FREE_LARGE,a)


#define F_M_GET_BLOCK   (SC_MM * 256) + 5 + FNCT_BASE
#define m_get_based_block(a,b)         \
	c_service_based_ptr(F_M_GET_BLOCK,a,b)

#define F_M_SET_BLOCK   (SC_MM * 256) + 6 + FNCT_BASE
#define m_set_based_block(a,b)  \
	c_service_long_based(F_M_SET_BLOCK,a,b)

#define F_M_REG_BLOCK   (SC_MM * 256) + 7 + FNCT_BASE
#define m_reg_based(a,b)        \
	c_service_reg_based(F_M_REG_BLOCK,a,b)


#define F_M_GET_FAR_BLOCK (SC_MM * 256) + 8 + FNCT_BASE
#define m_get_far_block(a,b)           \
	c_service_far_mem(F_M_GET_FAR_BLOCK,a,b)

#define F_M_SET_FAR_BLOCK       (SC_MM * 256) + 9 + FNCT_BASE
#define m_set_far_block(a,b)    \
	c_service_long_far(F_M_SET_FAR_BLOCK,a,b)

#define F_M_REG_FAR_BLOCK       (SC_MM * 256) + 10 + FNCT_BASE
#define m_reg_far(a,b,c)  \
	c_service_reg_far(F_M_REG_FAR_BLOCK,a,b,c)

#define F_M_GET_AVAIL_MEM       (SC_MM * 256) + 11 + FNCT_BASE
#define m_get_avail_mem()       \
	c_service_get_avail_mem(F_M_GET_AVAIL_MEM)


#define F_M_GET_TCB            (SC_MM * 256) + 12 + FNCT_BASE
#define m_get_TCB()             \
	c_service_far_ptr(F_M_GET_TCB)

#define F_M_GET_FAR_HANDLE      (SC_MM * 256) + 13 + FNCT_BASE
#define m_get_far_handle(a)		\
	c_service(F_M_GET_FAR_HANDLE,a)

#define  F_M_MODIFY_HOT_KEY      (SC_MM * 256) + 14 + FNCT_BASE
#define m_modify_hot_key(a)		\
	c_service_far_ptr(F_M_MODIFY_HOT_KEY,a)

#define F_M_TASK_INFO            (SC_MM * 256) + 15 + FNCT_BASE
#define m_task_info(a, b, c)           \
    c_service(F_M_TASK_INFO, (a), (b), (void far *)(c))

#define F_M_CURRENT_TASK         (SC_MM * 256) + 16 + FNCT_BASE
#define m_current_task()               \
    c_service(F_M_CURRENT_TASK)

#define F_M_QUICK_KEY_CHECK	 (SC_MM * 256) + 17 + FNCT_BASE
#define m_QuickKeyCheck()		\
    c_service(F_M_QUICK_KEY_CHECK)

#define F_M_GET_AVAIL_STATIC_MEM  (SC_MM * 256) + 18 + FNCT_BASE
#define m_get_avail_static_mem()	 \
	c_service_get_avail_mem(F_M_GET_AVAIL_STATIC_MEM)

#define F_M_GET_STATIC_BLOCK (SC_MM * 256) + 19 + FNCT_BASE
#define m_get_static_block(a,b)		\
	c_service_long_far(F_M_GET_STATIC_BLOCK,(a),(b))

#define F_M_FREE_STATIC_BLOCK (SC_MM * 256) + 20 + FNCT_BASE
#define m_free_static_block(a)	      \
	c_service_free_static(F_M_FREE_STATIC_BLOCK, (void far *)(a))

#define F_M_GET_SERVICE_ENTRY (SC_MM * 256) + 21 + FNCT_BASE
#define m_get_SVC_entry(a,b)	      \
	c_service_free_static(F_M_GET_SERVICE_ENTRY,(void far *)(a),(void far *)(b))

#define F_M_FREE_SERVICE_ENTRY (SC_MM * 256) + 22 + FNCT_BASE
#define m_free_SVC_entry(a)	      \
	c_service_free_static(F_M_FREE_SERVICE_ENTRY,a)

#define F_M_LOAD_TSR (SC_MM * 256) + 23 + FNCT_BASE
#define m_load_TSR(a)	      \
	c_service(F_M_LOAD_TSR,(void far *)(a))

#define F_M_FIND_SERVICE_ENTRY (SC_MM * 256) + 24 + FNCT_BASE
#define m_locate_SVC(a)		\
	c_service_far_ptr(F_M_FIND_SERVICE_ENTRY,(void far *)(a))


#define F_M_GET_STATIC_SIZE	 (SC_MM * 256) + 25 + FNCT_BASE
#define m_get_static_size()		\
    c_service(F_M_GET_STATIC_SIZE)


#define F_M_SET_STATIC_SIZE	 (SC_MM * 256) + 26 + FNCT_BASE
#define m_set_static_size(a)		\
    c_service(F_M_SET_STATIC_SIZE,a)


#define F_M_GET_STATIC_USED	 (SC_MM * 256) + 27 + FNCT_BASE
#define m_get_static_used()		\
    c_service(F_M_GET_STATIC_USED)


#define F_M_GET_DOS_PART_SIZE	 (SC_MM * 256) + 28 + FNCT_BASE
#define m_get_DOS_part_size()		\
    c_service(F_M_GET_DOS_PART_SIZE)


#define F_M_SET_DOS_PART_SIZE	 (SC_MM * 256) + 29 + FNCT_BASE
#define m_set_DOS_part_size(a)		\
    c_service(F_M_SET_DOS_PART_SIZE,a)


#define F_M_GET_ACT_DOS_SIZE	 (SC_MM * 256) + 30 + FNCT_BASE
#define m_get_active_DOS_size()		\
    c_service(F_M_GET_ACT_DOS_SIZE)

#define F_M_GET_DOS_HOT_KEY	 (SC_MM * 256) + 31 + FNCT_BASE
#define m_get_DOS_hot_key()		\
    c_service(F_M_GET_DOS_HOT_KEY )


#define F_M_SET_DOS_HOT_KEY	 (SC_MM * 256) + 32 + FNCT_BASE
#define m_set_DOS_hot_key(a)		\
    c_service(F_M_SET_DOS_HOT_KEY,a )

#define F_M_GET_AVAIL_123_MEM	 (SC_MM * 256) + 33 + FNCT_BASE
#define m_get_avail_123_mem()		\
    c_service_get_avail_mem(F_M_GET_AVAIL_123_MEM)

#define F_M_LOCK_OUT_APP_KEY	 (SC_MM * 256) + 34 + FNCT_BASE
#define m_lock_out_app_key()		\
    c_service(F_M_LOCK_OUT_APP_KEY)

#define F_M_ENABLE_APP_KEY	 (SC_MM * 256) + 35 + FNCT_BASE
#define m_enable_app_key()		\
    c_service(F_M_ENABLE_APP_KEY)

#define F_M_GET_FAR_USER_HANDLE	(SC_MM * 256) + 36 + FNCT_BASE
#define m_get_far_user_handle(a)		\
	c_service(F_M_GET_FAR_USER_HANDLE,a)

#define F_M_GET_AVAIL_INIT_MEM	 (SC_MM * 256) + 37 + FNCT_BASE
#define m_get_avail_init_mem()		\
    c_service_get_avail_mem(F_M_GET_AVAIL_INIT_MEM)

#define F_M_LOAD_CLIENT	(SC_MM * 256) + 38 + FNCT_BASE
#define m_load_client()		\
	c_service(F_M_LOAD_CLIENT)

#define F_M_UNLOAD_CLIENT	 (SC_MM * 256) + 39 + FNCT_BASE
#define m_unload_client()		\
    c_service(F_M_UNLOAD_CLIENT)



/*SC_BG*/



/*SC_DTIME*/


#define F_M_DTINFO      (SC_DTIME * 256) + 0 + FNCT_BASE
#define m_dtinfo(a)             \
	c_service(F_M_DTINFO,(void far *)(a))

#define F_M_GETDTM      (SC_DTIME * 256) + 1 + FNCT_BASE
#define m_getdtm(a)           \
	c_service(F_M_GETDTM,(void far *)(a))

#define F_M_SETDTM      (SC_DTIME * 256) + 2 + FNCT_BASE
#define m_setdtm(a)             \
	c_service(F_M_SETDTM,(void far *)(a))

#define F_M_XALARM      (SC_DTIME * 256) + 3 + FNCT_BASE
#define m_xalarm(a)      \
	c_service(F_M_XALARM,a)

#define F_M_ALARM       (SC_DTIME * 256) + 4 + FNCT_BASE
#define m_alarm(a,b)            \
	c_service(F_M_ALARM,(void far *)(a),b)

#define F_M_START_SW    (SC_DTIME * 256) + 5 + FNCT_BASE
#define m_start_sw(a)      \
	c_service(F_M_START_SW,(void far *)(a))

#define F_M_GET_SW      (SC_DTIME * 256) + 6 + FNCT_BASE
#define m_get_sw(a,b,c)     \
	c_service(F_M_GET_SW,(void far *)(a),(void far *)(b),(void far *)(c))

#define F_M_STOP_SW     (SC_DTIME * 256) + 7 + FNCT_BASE
#define m_stop_sw(a)           \
	c_service(F_M_STOP_SW,(void far *)(a))

#define F_M_TELLTIME     (SC_DTIME * 256) + 8 + FNCT_BASE
#define m_telltime(a,b,c)      \
	c_service(F_M_TELLTIME,a,b,c)

#define F_M_GET_SETTINGS (SC_DTIME *256) +9 + FNCT_BASE
#define m_get_settings(a,b)      \
	c_service(F_M_GET_SETTINGS, (void far *)(a),(void far *)(b))

#define F_M_SET_SETTINGS (SC_DTIME *256) +10 + FNCT_BASE
#define m_set_settings(a,b)      \
	c_service(F_M_SET_SETTINGS, (void far *)(a),(void far *)(b))

#define F_M_START_TIMER (SC_DTIME * 256) + 11 + FNCT_BASE
#define m_start_timer(a)           \
	c_service(F_M_START_TIMER,(void far *)(a))

#define F_M_STOP_TIMER  (SC_DTIME * 256) + 12 + FNCT_BASE
#define m_stop_timer(a)        \
	c_service(F_M_STOP_TIMER, (void far *)(a))

#define F_M_GET_TIMER   (SC_DTIME * 256) + 13 + FNCT_BASE
#define m_get_timer(a,b,c)    \
	c_service(F_M_GET_TIMER,(void far *)(a),(void far *)(b),(void far *)(c))


#define F_M_TELL_ANYTIME   (SC_DTIME * 256) + 14 + FNCT_BASE
#define m_tell_anytime(a,b,c,d,e)      \
	c_service_far_ptr(F_M_TELL_ANYTIME,a,b,c, (void far *)(d), (void far *)(e))


#define F_M_GET_SETTINGS_ADDR (SC_DTIME *256) +15 + FNCT_BASE
#define m_get_settings_addr()      \
	c_service_far_ptr(F_M_GET_SETTINGS_ADDR)

#define F_M_PARSE_DATE (SC_DTIME * 256) + 16 + FNCT_BASE
#define m_parse_date(a,b,c)      \
	c_service(F_M_PARSE_DATE,a,(void far *)(b),(void far *)(c))

#define F_M_PARSE_TIME (SC_DTIME * 256) + 17 + FNCT_BASE
#define m_parse_time(a,b,c)      \
	c_service(F_M_PARSE_TIME,a,(void far *)(b),(void far *)(c))


#define F_M_SET_DATERULE (SC_DTIME * 256) + 18 + FNCT_BASE
#define m_set_daterule(a)      \
	c_service(F_M_SET_DATERULE,a)

#define F_M_SET_TIMERULE (SC_DTIME * 256) + 19 + FNCT_BASE
#define m_set_timerule(a)      \
	c_service(F_M_SET_TIMERULE,a)

#define F_M_POSTTIME (SC_DTIME * 256) + 20 + FNCT_BASE
#define m_posttime()      \
	c_service(F_M_POSTTIME)

#define F_M_DAY_TRIGGER (SC_DTIME * 256) + 21 + FNCT_BASE
#define m_day_trigger(a)        \
	c_service(F_M_DAY_TRIGGER,a)

#define F_M_NALARM	(SC_DTIME * 256) + 22 + FNCT_BASE
#define m_nalarm(a,b)		 \
	c_service(F_M_NALARM,(void far *)(a),b)

#define F_M_NFALARM	(SC_DTIME * 256) + 23 + FNCT_BASE
#define m_nfalarm(a,b)		 \
	c_service(F_M_NFALARM,(void far *)(a),b)

#define F_M_KEYBEZ	(SC_DTIME * 256) + 24 + FNCT_BASE
#define m_keybez()		 \
	c_service(F_M_KEYBEZ)



/*SC_PRINTER */

#define F_M_OPEN_PRINTER        (SC_PRINTER * 256) + 0 + FNCT_BASE
#define m_open_printer()        \
	c_service(F_M_OPEN_PRINTER)



#define F_M_CLOSE_PRINTER       (SC_PRINTER * 256) + 1 + FNCT_BASE
#define m_close_printer()       \
	c_service(F_M_CLOSE_PRINTER)


#define F_M_WRITE_PRINTER       (SC_PRINTER * 256) + 2 + FNCT_BASE
#define m_write_printer(a,b)    \
	c_service(F_M_WRITE_PRINTER,(void far *)(a),b)

#define F_M_INIT_PRINTER        (SC_PRINTER * 256) + 3 + FNCT_BASE
#define m_init_printer()        \
	c_service(F_M_INIT_PRINTER)

#define F_M_TRANS_PRINTER       (SC_PRINTER * 256) + 4 + FNCT_BASE
#define m_trans_printer(a,b)    \
	c_service(F_M_TRANS_PRINTER,a,(void far *)(b))

#define F_M_WRITE_PRINTER_ATB   (SC_PRINTER * 256) + 6 + FNCT_BASE
#define m_write_printer_atb(a)    \
	c_service(F_M_WRITE_PRINTER_ATB,(a))

#define F_M_TRANS_PRINTER_ATB    (SC_PRINTER * 256) + 7 + FNCT_BASE
#define m_trans_printer_atb(a,b)    \
	c_service(F_M_TRANS_PRINTER_ATB,a,(void far *)(b))


/*SC_CONFIG*/






/*SC_COMM*/

#define F_COMM_INIT     (SC_COMM * 256) + 0 + FNCT_BASE
#define InitCom(a)      \
	c_service(F_COMM_INIT,(void far *)(a))

#define F_COMM_OPEN     (SC_COMM * 256) + 1 + FNCT_BASE
#define ComOpen(a,b)    \
	c_service(F_COMM_OPEN,(int far *) (a), (int)(b))

#define F_COMM_CLOSE    (SC_COMM * 256) + 2 + FNCT_BASE
#define ComClose(a)     \
	c_service(F_COMM_CLOSE,(int) (a))

#define F_COMM_GETMDM   (SC_COMM * 256) + 3 + FNCT_BASE
#define ComGetModem(a)  \
	c_service(F_COMM_GETMDM, (int)(a))

#define F_COMM_ANSWER   (SC_COMM * 256) + 4 + FNCT_BASE
#define ComAnswer(a,b)  \
	c_service(F_COMM_ANSWER, (int) a, (int) b)

#define F_COMM_DIAL     (SC_COMM * 256) + 5 + FNCT_BASE
#define ComDial(a,b)    \
	c_service(F_COMM_DIAL, (int) (a), (char far *) (b))

#define F_COMM_RESET    (SC_COMM * 256) + 6 + FNCT_BASE
#define ComReset(a,b)   \
	c_service(F_COMM_RESET, (int) (a), (int) (b))

#define F_COMM_HANGUP   (SC_COMM * 256) + 7 + FNCT_BASE
#define ComHangUp(a)    \
	c_service(F_COMM_HANGUP, (int) (a))

#define F_COMM_SNDBTS   (SC_COMM * 256) + 8 + FNCT_BASE
#define ComSendBytes(a,b,c,d)   \
	c_service(F_COMM_SNDBTS, (int) (a), (char far *) (b), (int) (c), (int far *) (d))

#define F_COMM_QRYTXQ   (SC_COMM * 256) + 9 + FNCT_BASE
#define ComQryTxQue(a,b,c)      \
	c_service(F_COMM_QRYTXQ, (int) (a), (int far *) (b), (int far *) (c))

#define F_COMM_QRYRXQ   (SC_COMM * 256) + 10 + FNCT_BASE
#define ComQryRxQue(a,b,c)      \
	c_service(F_COMM_QRYRXQ, (int) (a), (int far *) (b), (int far *) (c))

#define F_COMM_RCVBTS   (SC_COMM * 256) + 11 + FNCT_BASE
#define ComReceiveBytes(a,b,c)  \
	c_service(F_COMM_RCVBTS, (int) (a), (char far *) (b), (int far *) (c))

#define F_COMM_HAZCMD   (SC_COMM * 256) + 12 + FNCT_BASE
#define ComHayesCommand(a,b,c)  \
	c_service(F_COMM_HAZCMD, (int) (a), (char far *) (b), (int) (a))

#define F_COMM_COMAND   (SC_COMM * 256) + 13 + FNCT_BASE
#define ComCommand(a,b,c)       \
	c_service(F_COMM_COMAND, (int) (a), (char far *) (b), (int) (c))

#define F_COMM_BREAK    (SC_COMM * 256) + 14 + FNCT_BASE
#define ComBreak(a,b)           \
	c_service(F_COMM_BREAK, (int) (a), (int) (b))

#define F_COMM_FRCXON   (SC_COMM * 256) + 15 + FNCT_BASE
#define ComForceXon(a)          \
	c_service(F_COMM_FRCXON, (int) (a))

#define F_COMM_FRCXOF   (SC_COMM * 256) + 16 + FNCT_BASE
#define ComForceXoff(a)         \
	c_service(F_COMM_FRCXOF, (int) (a))

#define F_COMM_SETDTR   (SC_COMM * 256) + 17 + FNCT_BASE
#define ComSetDtr(a,b)          \
	c_service(F_COMM_SETDTR, (int) (a), (int) (b))

#define F_COMM_XMITNG   (SC_COMM * 256) + 18 + FNCT_BASE
#define ComXmitting(a)          \
	c_service(F_COMM_XMITNG, (int) (a))

#define F_COMM_STATUS   (SC_COMM * 256) + 19 + FNCT_BASE
#define ComStatus(a)            \
	c_service(F_COMM_STATUS, (int) (a))

#define F_COMM_SET      (SC_COMM * 256) + 20 + FNCT_BASE
#define ComSet(a,b)     \
	c_service(F_COMM_SET, (int) (a), (com_settings far *) (b))

#define F_COMM_GET      (SC_COMM * 256) + 21 + FNCT_BASE
#define ComGet(a,b)     \
	c_service(F_COMM_GET, (int) (a), (com_settings far *) (b))

#define F_COMM_CNFGUR   (SC_COMM * 256) + 22 + FNCT_BASE
#define ComConfigure(a,b,c,d,e) \
	c_service(F_COMM_CNFGUR, (int) (a), (int) (b), (int) (c), (int) (d), (int) (e))

#define F_COMM_QRYERR   (SC_COMM * 256) +23 + FNCT_BASE
#define ComQryErr(a)            \
	c_service(F_COMM_QRYERR, (int) a)

#define F_COMM_ACQUIRE  (SC_COMM * 256) +24 + FNCT_BASE
#define ComAcquire(a, b)        \
	c_service(F_COMM_ACQUIRE, (int) a, (int) b)

#define F_COMM_RELEASE  (SC_COMM * 256) +25 + FNCT_BASE
#define ComRelease(a)           \
	c_service(F_COMM_RELEASE, (int) a)

#define F_COMM_GETPARM  (SC_COMM * 256) +26 + FNCT_BASE
#define ComGetParameters(a, b)  \
	c_service(F_COMM_GETPARM, (int) a, (struct COM_SETTINGS far *) b)

#define F_COMM_SETPARM  (SC_COMM * 256) +27 + FNCT_BASE
#define ComSetParameters(a, b)  \
	c_service(F_COMM_SETPARM, (int) a, (struct COM_SETTINGS far *) b)

#define F_COMM_MDMCLR   (SC_COMM * 256) +28 + FNCT_BASE
#define ComMdmClr(a, b)         \
	c_service(F_COMM_MDMCLR, (int) a, (int) b)

#define F_COMM_MDMSET   (SC_COMM * 256) +29 + FNCT_BASE
#define ComMdmSet(a, b)         \
	c_service(F_COMM_MDMSET, (int) a, (int) b)

#define F_COMM_MDMSTAT   (SC_COMM * 256) +30 + FNCT_BASE
#define ComMdmStatus(a, b, c)     \
	c_service(F_COMM_MDMSTAT, (int) a, (int far *) b, (int far *) c)

#define F_COMM_ACQRCV    (SC_COMM * 256) + 31 + FNCT_BASE
#define ComAcqReceiveBytes(a, b, c)     \
	c_service(F_COMM_ACQRCV, (int)a, (char far *)b, (int far *)c, (int)d)

#define F_COMM_ACQSEND   (SC_COMM * 256) + 32 + FNCT_BASE
#define ComAcqSendBytes(a, b, c, d)     \
	c_service(F_COMM_ACQSEND, (int)a, (char far *)b, (int)c, (int far *)d)

#define F_COMM_POWER     (SC_COMM * 256) + 33 + FNCT_BASE
#define ComPower(a)      \
	c_service(F_COMM_POWER,(int)a)

/*      RESERVED         (SC_COMM * 256) + 34 + FNCT_BASE */


/*SC_MISC*/

#define F_M_ERRMSG      (SC_MISC * 256) + 0 + FNCT_BASE
#define m_errmsg(a,b,c,d)               \
	c_service(F_M_ERRMSG,a,(void far *)(b),c,(void far *)(d))

#define F_DRAWBOX       (SC_MISC * 256) + 1 + FNCT_BASE
#define drawbox(a)               \
	c_service(F_DRAWBOX,(void far *)(a))

#define F_SHOWNAME      (SC_MISC * 256) + 2 + FNCT_BASE
#define showname(a)              \
	c_service(F_SHOWNAME,(void far *)(a))

#define F_MESSAGE       (SC_MISC * 256) + 3 + FNCT_BASE
#define message(a,b,c,d)               \
	c_service(F_MESSAGE,(void far *)(a),b,(void far *)(c),d)

#define F_MSG_OFF       (SC_MISC * 256) + 4 + FNCT_BASE
#define msg_off()               \
	c_service(F_MSG_OFF)

#define F_M_COM_TIMER_ADDR  (SC_MISC * 256) + 5 + FNCT_BASE
#define com_timer_addr()                  \
	c_service_far_ptr(F_M_COM_TIMER_ADDR)

#define F_M_COM_TIMER_COUNT_ADDR  (SC_MISC * 256) + 6 + FNCT_BASE
#define com_timer_count_addr()            \
	c_service_far_ptr(F_M_COM_TIMER_COUNT_ADDR)


#define F_M_SYS_RSRC_ADDR (SC_MISC * 256) + 7 + FNCT_BASE
#define m_sys_rsrc_addr()               \
	c_service_far_ptr(F_M_SYS_RSRC_ADDR)


/* SC_MISC * 8 is reserved */

#define F_MESSAGE3      (SC_MISC * 256) + 9 + FNCT_BASE
#define message3(a,b,c,d,e,f)          \
	c_service(F_MESSAGE3,(void far *)(a),b,(void far *)(c),d,(void far *)(e),f)


#define F_M_DISABLE_MACROS (SC_MISC * 256) + 10 + FNCT_BASE
#define m_disable_macros()              \
	c_service(F_M_DISABLE_MACROS)


#define F_M_ENABLE_MACROS (SC_MISC * 256) + 11 + FNCT_BASE
#define m_enable_macros()               \
	c_service(F_M_ENABLE_MACROS)

#define F_M_FORM_FT (SC_MISC * 256) + 13 + FNCT_BASE
#define m_form_ft(a)            \
	c_service_far_ptr(F_M_FORM_FT,(void far *)(a))

#define F_M_RAM_IV_INFO (SC_MISC * 256) + 14 + FNCT_BASE
#define m_ram_iv_info(a)                \
	c_service(F_M_RAM_IV_INFO,(void far *)(a))

#define F_M_GET_SYSVER (SC_MISC * 256) + 15 + FNCT_BASE
#define m_get_sysver()          \
	c_service_far_ptr(F_M_GET_SYSVER)

#define F_M_START_RECORD (SC_MISC * 256) + 16 + FNCT_BASE
#define m_start_record(a)          \
	c_service_far_ptr(F_M_START_RECORD, a)

#define F_M_STOP_RECORD (SC_MISC * 256) + 17 + FNCT_BASE
#define m_stop_record()            \
	c_service_far_ptr(F_M_STOP_RECORD)

#define F_M_UPDATE_APNAMES (SC_MISC * 256) + 18 + FNCT_BASE
#define m_update_apname_list(a)	    \
	c_service_far_ptr(F_M_UPDATE_APNAMES,(int) a)

#define F_M_GET_TCB_SIZE (SC_MISC * 256) + 19 + FNCT_BASE
#define m_get_TCB_size()	    \
	c_service_far_ptr(F_M_GET_TCB_SIZE )

#define F_M_PLAYBACK_MACRO (SC_MISC * 256) + 20 + FNCT_BASE
#define m_playback_macro(a)	    \
	c_service(F_M_PLAYBACK_MACRO,(int) a)


/*SC_VIDVIR*/

#define F_M_VID_INIT            ((SC_VIDVIR * 256) + 0) + FNCT_BASE
#define m_vid_init()                    \
   c_service(F_M_VID_INIT)

#define F_M_DIRTY_SYNC          ((SC_VIDVIR * 256) + 5) + FNCT_BASE
#define m_dirty_sync()                  \
   c_service(F_M_DIRTY_SYNC)

/*SC_VIDPHY*/



/*SC_RSRC*/

#define F_MAP_RESOURCE_FILE   (SC_RSRC * 256) + 0 + FNCT_BASE
#define map_resource_file(a)             \
	c_service(F_MAP_RESOURCE_FILE,(void far *)(a))


#define F_GET_RESOURCE_PTR   (SC_RSRC * 256) + 1 + FNCT_BASE
#define get_resource_ptr(a)              \
	c_service_far_ptr(F_GET_RESOURCE_PTR,a)

#define F_GET_RSRC_TAB_PTR   (SC_RSRC * 256) + 2 + FNCT_BASE
#define get_rsrc_tab_ptr()      \
	c_service_far_ptr(F_GET_RSRC_TAB_PTR)

#define F_INIT_SYSGMR_RSRCS     (SC_RSRC * 256) + 3 + FNCT_BASE
#define init_sysmgr_resources() \
	c_service(F_INIT_SYSGMR_RSRCS)

#define F_M_INIT_KEYLABELS (SC_RSRC * 256) + 4 + FNCT_BASE
#define m_InitKeyLabels()          \
	c_service(F_M_INIT_KEYLABELS)

#define F_M_GET_SCANCODE (SC_RSRC * 256) + 5 + FNCT_BASE
#define m_GetScanCode(a)          \
	c_service(F_M_GET_SCANCODE,(void far *)(a))

#define F_M_GET_KEYLABEL (SC_RSRC * 256) + 6 + FNCT_BASE
#define m_GetKeyLabel(a,b)          \
	c_service(F_M_GET_KEYLABEL,(void far *)(a),b)




/* SC_HELP */


#define F_M_HELP_INIT         (SC_HELP * 256) + 0 + FNCT_BASE
#define m_help_init(a,b,c)    \
	c_service(F_M_HELP_INIT,(void far *)(a),(void far *)(b),c)

#define F_M_HELP_DISPLAY      (SC_HELP * 256) + 1 + FNCT_BASE
#define m_help_display(a)     \
	c_service(F_M_HELP_DISPLAY,(void far *)(a))

#define F_M_HELP_KEY          (SC_HELP * 256) + 2 + FNCT_BASE
#define m_help_key(a,b)       \
	c_service(F_M_HELP_KEY,(void far *)(a),b)

#define F_M_HELP_TERM         (SC_HELP * 256) + 3 + FNCT_BASE
#define m_help_term(a)        \
	c_service(F_M_HELP_TERM,(void far *)(a))

#define F_M_HELP2_INIT        (SC_HELP * 256) + 4 + FNCT_BASE
#define m_help2_init(a)       \
	c_service(F_M_HELP2_INIT,a)

#define F_M_HELP2_DISPLAY     (SC_HELP * 256) + 5 + FNCT_BASE
#define m_help2_display(a,b)  \
	c_service(F_M_HELP2_DISPLAY,a,b)

#define F_M_HELP2_KEY         (SC_HELP * 256) + 6 + FNCT_BASE
#define m_help2_key(a,b)      \
	c_service(F_M_HELP2_KEY,a,b)

#define F_M_HELP2_TERM        (SC_HELP * 256) + 7 + FNCT_BASE
#define m_help2_term(a)       \
	c_service(F_M_HELP2_TERM,a)

#define F_M_HELP2_GETMETRICS  (SC_HELP * 256) + 8 + FNCT_BASE
#define m_help2_getmetrics(a,b) \
	c_service(F_M_HELP2_GETMETRICS,a,b)


/*SC_COL*/

#define F_M_ColInit                     (SC_COL * 256) + 0 + FNCT_BASE
#define m_col_init()                            \
	c_service(F_M_ColInit)

#define F_M_ColCpStr            (SC_COL * 256) + 1 + FNCT_BASE
#define m_col_cpstr(a,b,c,d)    \
	c_service(F_M_ColCpStr,(void far *)(a),b,(void far *)(c),d)

#define F_M_ColLicsStr          (SC_COL * 256) + 2 + FNCT_BASE

#define F_M_ColLicsChar (SC_COL * 256) + 3 + FNCT_BASE

#define F_M_ColToLower          (SC_COL * 256) + 4 + FNCT_BASE
#define m_col_tolower(a,b)              \
	c_service(F_M_ColToLower,(void far *)(a),b)

#define F_M_ColCpSearch (SC_COL * 256) + 5 + FNCT_BASE
#define m_col_cpsearch(a,b,c,d,e)       \
	c_service(F_M_ColCpSearch,(void far *)(a),b,(void far *)(c),d,e)

#define F_M_ColToUpper          (SC_COL * 256) + 6 + FNCT_BASE
#define m_col_toupper(a,b)              \
	c_service(F_M_ColToUpper,(void far *)(a),b)

/* --------------------------------------------------------------------------
m_task_info() declarations 
-------------------------------------------------------------------------- */

#define     TI_TASK_HOT_KEY      0
#define     TI_TASK_STATE        1
#define     TI_TASK_EXTNAME      2
#define     TI_TASK_SP_STATE     3
#define     TI_TASK_SP_LINK      4

#define     m_task_hotkey(TaskNo,RetArea)       \
            m_task_info(TI_TASK_HOT_KEY, TaskNo, (void far *) RetArea)
#define     m_current_task_hotkey(RetArea)      \
            m_task_hotkey(-1, RetArea)            

#define     m_task_state(TaskNo,RetArea)        \
            m_task_info(TI_TASK_STATE, TaskNo, (void far *) RetArea)
#define     m_current_task_state(RetArea)       \
            m_task_state(-1, RetArea)            

#define     m_task_name(TaskNo,RetArea)         \
            m_task_info(TI_TASK_EXTNAME, TaskNo, (void far *) RetArea)
#define     m_current_task_name(RetArea)        \
            m_task_name(-1, RetArea)            

#define     m_task_sp_state(TaskNo,RetArea)    \
            m_task_info(TI_TASK_SP_STATE, TaskNo, (void far *) RetArea)
#define     m_current_task_sp_state(RetArea)   \
            m_task_sp_state(-1, RetArea)            

#define     m_task_sp_link(TaskNo,RetArea)      \
            m_task_info(TI_TASK_SP_LINK, TaskNo, (void far *) RetArea)
#define     m_current_task_sp_link(RetArea)     \
	         m_task_sp_link(-1, RetArea)


#ifdef XBS
#include <XBS_inter.h>
#endif

/* QP!AJG */
#define F_M_QUALIFY_PATH        (SC_QUALIFY * 256) + 0 + FNCT_BASE
#define m_qualify_path(a,b)              \
        c_service(F_M_QUALIFY_PATH,(void far *)(a),(void far *)(b))


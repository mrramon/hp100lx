/* $Header:   T:/HH/SYSMGR/VCS/TASK.H_V   1.9   29 Jan 1993 14:19:06   KIM  $ */

/****************************************************************
 * Morel Accessories Kernel
 *
 *	Copyright(c) 1990
 *	Lotus Development Corporation
 *	All Rights Reserved
 *
 * TASK.H
 ****************************************************************/

/* 
 * $Log:   T:/HH/SYSMGR/VCS/TASK.H_V  $
 * 
 *    Rev 1.9   29 Jan 1993 14:19:06   KIM
 * Out of sync with TASK.MAC: 1) TASKCOUNT was incorrect; 2) t_lock_state was
 * incorrect.
 * 
 *    Rev 1.8   11 Jan 1993 18:43:32   PETERR
 * Replaced based entries with new lock state and 3 spare bytes.
 * 
 *    Rev 1.7   16 Nov 1992 20:32:58   KIM
 * Synchronized TASKCOUNT and MaxRAMApps constants with task.mac.
 * 
 *    Rev 1.6   15 Oct 1992 10:15:24   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.ASM, HELPSTUB.ASM, and HELPDATA.INC have post-Build 19 changes.
 * 
 *    Rev 1.5   23 Jul 1992 19:37:52   KIM
 * System Manager will now use HP's low-level graphics routines for COUGAR.
 * Change IFDEFs for VIDEO related values.  
 * 
 *    Rev 1.4   23 Jul 1992 12:13:34   PETERR
 * Replaced TCB entry extra with one that indicates the rev of the app
 * 
 *    Rev 1.3   01 May 1992 14:14:04   PETERR
 * Added new field for far ptr 
 * 
 *    Rev 1.2   21 Apr 1992 17:27:42   KIM
 * Enhanced Sys Mgr to handle ROM image files larger than 1Mb.
 * Enhanced Sys Mgr to handle new Memory Bank information, for Bank Swapping,
 * in the ROM image vector.  
 * These changes were made across the RIB and SYSTEM MANAGER.
 * Also, IMAGEVEC.MAC moved to the common include directory (HH\INCLUDE).
 * 
 * 
 *    Rev 1.1   09 Mar 1992 11:27:06   PETERR
 * DOS mem & launch changes
 * 
 *    Rev 1.0   03 Dec 1991 16:21:20   DAVID
 * Initial revision.
 *
 */


/*
*********************	    NOTE	 *******************************

      See	task.mac for details concerning items

**********************************************************************
*/

#ifdef	 MOTOWN
#define TASKCOUNT	        24
#define MaxAppsFileSize	    1024
#define MaxRAMApps	        TASKCOUNT
#else
#define TASKCOUNT	        30
#define MaxAppsFileSize	    512
#define MaxRAMApps	        10
#endif


#define	ACC_NAME_LEN	12		/* sufficient to store filename */

typedef unsigned char UChar;
typedef unsigned int  UInt;


struct task {
	unsigned  t_sp;			    /* offset made low, mdb     */
	unsigned  t_ss;
	unsigned  t_off_image;		/* offset made low, mdb     */
	unsigned  t_seg_image;		
	unsigned  t_ds;
	unsigned  t_memseg;		    /* base of allocation block */
	unsigned  t_hotkey;		    /* copied from IMAGEVEC     */

    unsigned  t_phypage[2];     /* IMAGEVEC.wAS_PhyPage     */
    unsigned  t_logicalpage[2]; /* IMAGEVEC.wRS_LogicalPage */
    unsigned  t_pagecount[2];   /* IMAGEVEC.wRS_PageCount   */
	char	  t_chipsel[2];
	char	  t_state;
	char	  t_is123;
	unsigned  t_rsrc_seg;
	char	  t_nowait;
	char	  t_sys_ver;
	char	  t_extname[12];
	char      t_lock_state;
	char	  t_spare[3];
	unsigned  t_far_size;
	unsigned  t_far_off;
	unsigned  t_far_rsvrd;
	char	  t_sp_status;
	char	  t_sp_link;
	unsigned  t_ic_u_loc;
	unsigned  t_ic_o_loc;


};


struct rsrc_entry {
	char re_name[13];
	char re_pad;
	unsigned long re_romloc;
};


#ifdef XBS
  #ifdef EMUL
  #define ImagevecSeg  0x9000
  #else
  #define ImagevecSeg  0xE000
  #endif
#else
  #ifdef EMUL
  #define ImagevecSeg  0x9000
  #else
  #define ImagevecSeg  0xF000
  #endif
#endif

#define VIDEOGRSEG   0xb800
#define VIDEOGROFF   0x0000
#define VIDEOGRLINELEN 0x50
#define VIDEOBASE    ((void far *)0xB8000000)

#define ImagevecOff0 0x7

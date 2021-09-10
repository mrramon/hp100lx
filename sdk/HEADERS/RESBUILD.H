/* $Header:   T:/HH/INCLUDE/VCS/RESBUILD.H_V   1.10   21 Dec 1992 23:00:40   KIM  $ */
/**************************************************************************
 *
 *  Name:       C:\COUGAR\LSETUP\DLGPARSE.H
 *  Date:       Tuesday March 24, 1992
 *  Abstract:   Header file defining interface for DialogBuild and FKeyBuild
 *              functions that read the resource files to build dialog box
 *              and function key structures
 *
 * ------------------------------------------------------------------------
 *                   Copyright 1992 Lotus Development Corp.
 *                           All rights reserved.
 **************************************************************************
 *
 * $Log:   T:/HH/INCLUDE/VCS/RESBUILD.H_V  $
 * 
 *    Rev 1.10   21 Dec 1992 23:00:40   KIM
 * Error fix: forgot to put in #typedef for DISTANCE.
 * 
 *    Rev 1.9   21 Dec 1992 22:54:30   KIM
 * Added DISTANCE qualifier to prototypes.
 * 
 *    Rev 1.8   16 Dec 1992 23:54:20   DAVID
 * Added prototype for AtoI()
 * 
 *    Rev 1.7   15 Oct 1992 10:52:10   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.6   27 Jul 1992 16:00:40   DAVID
 * Expanded mask for LHWINDOW style field from 0xFFFF to 0xFFFFFFFF since
 * field was changed from int to long.
 * 
 *    Rev 1.5   23 Apr 1992 18:16:54   DAVID
 * Added DLG_NORESOURCE
 * 
 *    Rev 1.4   16 Apr 1992 02:48:28   DAVID
 * Simplified #defines for filling in window structure
 * 
 *    Rev 1.3   12 Apr 1992 23:00:24   DAVID
 * Fixed typo in new prototypes
 * 
 *    Rev 1.2   12 Apr 1992 22:34:08   DAVID
 * Changed prototypes for menu text functions
 * 
 *    Rev 1.1   12 Apr 1992 21:41:06   DAVID
 * Added prototype for menu parsing function.
 * 
 *    Rev 1.0   10 Apr 1992 19:34:10   DAVID
 * Initial revision.
 * 
 *    Rev 1.0   09 Apr 1992 22:52:18   DAVID
 * Initial revision.
 * 
 *    Rev 1.1   31 Mar 1992 23:50:58   DAVID
 * Now gets resource string for Title field
 * 
 *    Rev 1.0   27 Mar 1992 20:39:54   DAVID
 * Initial revision.
 *
 **************************************************************************/

#if !defined(RESBUILD_H)
#define RESBUILD_H

#if !defined(LHAPI_H)
#include <LHAPI.H>
#endif

/****************************************************************************
 * Each of the services are prefaced with the following constant in
 * order to allow for mixed-memory model applications.  A developer can
 * selectively set this constant to either "_near" or "_far", or leave it
 * as NULL in order to have all the calls use the same model as
 * that specified to the compiler.
 */
#if !defined(DISTANCE)
#define	DISTANCE
#endif


/**************************************************************************
 * Dialog Box
 */

/* Prototype for the DialogBuild
 *
 * The application must provide:
 *   
 *   - array of window structures
 *   - array of int with the same number of elements as array of windows 
 *     structures.
 *   - the number of elements each has. 
 *
 *  On return, the int array will contain the new indexes of each dialog 
 *  control.  For example, if "File Name"  control was originally
 *  awndDialog[3] (in the window structure array), aiIndex[3] will contain
 *  the new index for "File Open", so you'll need to call access it as
 *  awndDialog[aiIndex[3]].
 */

void DialogBuild(PLHWINDOW awndDialog, int *aiIndex, int n );

/* The following bit definitions define which elements of each control
 * are read from the resource file.  If resourced, (see DLG_ISRESOURCED
 * below), the coordinates are read at a minimum.  Setting the DLG_GETCLASS
 * bit or the GETATTRIBS bit will cause each of these to be read, as well.
 * (Note: The resource file is not allowed to define a Class as 0.  If
 * it attempts to do so, the hardcoded value remains.  This has two
 * corollaries: First, a class abbreviation of "NO" is defined as 0 so the
 * resource file can cause the Class to default to the value supplied by the
 * programmer.  Second, if a Class of 0 is to be allowed for a particular
 * window or control, this must be the default value.  (In other words, if
 * the resource file is give the privilege of defining a Class as 0 (which
 * is the base class?), it must accept full responsibility.  Fair, no?)
 */

#define DLG_GETCLASS    0x01
#define DLG_GETSTYLE    0x02
#define DLG_GETALL      (DLG_GETCLASS|DLG_GETSTYLE)

/* The third bit in the status field of the window structure is reserved
 * to indicate whether this control is to be read from the resource file
 */
#define DLG_ISRESOURCED    0x08


/* Setting x to the following will keep DialogBuild from getting the 
 * Title from the resource file.
 */
#define DLG_NORESOURCE    -1


/* This is the maximum number of controls that can be contained in a
 * dialog box:
 */
#define DLG_MAXELEMS 127        /* Maximum number of dialog items */


/* Style bits that are allowed to be set by the resource file */
#define DLG_ATTRIBMASK 0xFFFFFFFF   /* All attributes can be set for now */

/**************************************************************************
 * Function Keys
 */

/* Defines and structs for FKBUILD: Function Key/Resource file interface */

#define FK_LASTKEY   0,0,FKEY_LAST

struct FKEYRESOURCE{
      unsigned char far *msg;
      int iTxtHandle;
      int iDefHandle;
};

void DISTANCE FKeyBuild(PLHFKEYS astFKey, struct FKEYRESOURCE *astFKRes);

/* Prototype for function to parse menu text and return scan code. */
void DISTANCE GetMenuHotKeys(PLHMENU astMenu);
unsigned int DISTANCE MenuTextToScancode(char far *lszMenuText);

/* an atoi implementation for resource strings */
int DISTANCE AToI(unsigned char far *hlszString);

#endif

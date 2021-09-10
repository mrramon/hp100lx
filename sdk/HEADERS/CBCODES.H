/* $Header:   T:/HH/INCLUDE/VCS/CBCODES.H_V   1.2   15 Oct 1992 10:48:28   KIM  $ */
/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
cbcodes.h

Clipboard status codes; negative values used because the "get clipboard
data" routine needs to use positive values for partial completion status.

This file needs to be copied into \mdl\incl for use by accessories and
applications.

					Doug Knowles	8/85
****************************************************************/

/*
 * $Log:   T:/HH/INCLUDE/VCS/CBCODES.H_V  $
 * 
 *    Rev 1.2   15 Oct 1992 10:48:28   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.1   04 May 1992 19:26:58   DAVID
 * Changed includes to use angle brackets instead of double quotes
 * 
 *    Rev 1.0   03 Dec 1991 20:14:34   DAVID
 * Initial revision.
 *
 */


#ifndef	CBCODES

#define	CB_OK		0		/* All's well with the world */

#define	CB_LOCKED	-1		/* Clipboard access denied */
#define CB_NOT_OPEN	-2		/* Clipboard not formally open */
#define CB_ALLOC	-3		/* Heap allocation failure */
#define CB_EXISTS	-4		/* Representation already exists */
#define	CB_REP_OPEN	-5		/* A Representation is open */
#define	CB_NO_REP	-6		/* No representation open */
#define	CB_NO_SUCH	-7		/* No such representation */
#define	CB_BOUNDS	-8		/* Transfer request out of bounds */

/****************************************************************
Maximum size of a clipboard author or representation name
****************************************************************/
#define	MAX_CB_NAME	48

/****************************************************************
Declarations, of the strings only, of certain representation type names
****************************************************************/
#define	CB_REP_TEXT		"TEXT"
#define	CB_REP_KEYSTROKE	"KEYSTROKE"

/****************************************************************
Declare a macro which, when invoked, will define a string constant
identifying the standard TEXT data type.
****************************************************************/
#define DECLARE_TEXT	char TEXTTYPE[] = "TEXT"

/****************************************************************
For the benefit of non-kernel users, force a declaration of the
kernel service calls (to force far pointers into the calls
****************************************************************/
#include <m_calls.h>
#define	CBCODES
#endif

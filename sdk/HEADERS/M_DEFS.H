/* $Header:   T:/HH/INCLUDE/VCS/M_DEFS.H_V   1.1   15 Oct 1992 10:51:44   KIM  $ */
/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
m_defs.h

miscellaneous "C" definitions for accessories
****************************************************************/
/*
 * $Log:   T:/HH/INCLUDE/VCS/M_DEFS.H_V  $ 
 * 
 *    Rev 1.1   15 Oct 1992 10:51:44   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.0   03 Dec 1991 20:14:50   DAVID
 * Initial revision.
 *
 */



#ifndef	M_DEFS

#ifndef BOOLEAN
#define BOOLEAN int
#endif



#ifndef	TRUE
#define TRUE  1
#define FALSE 0
#endif



#define FILE_PATH_LEN 80		/* As per MS limit?   */
#define FILE_NAME_LEN 14                /* Include trailing 0 */

#define lobyte(x)       (((char *) &(x))[0])
#define hibyte(x)       (((char *) &(x))[1])
#define loword(x)       (((unsigned *) &(x))[0])
#define hiword(x)       (((unsigned *) &(x))[1])


#define MAXINT          32767
#ifndef NULL
#define NULL            0
#endif

#define strsiz(s)	strlen(s)

extern int strlen(char far *);

#define	M_DEFS
#endif

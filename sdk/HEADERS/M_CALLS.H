/* $Header:   T:/HH/INCLUDE/VCS/M_CALLS.H_V   1.3   30 Nov 1992 20:19:38   DAVID  $ */
/****************************************************************
 *
 *	Copyright(c) 1990  Lotus Development Corporation
 *	All Rights Reserved
 *
 ****************************************************************/

/****************************************************************
m_calls.h

Declarations for Mosaic calls with Type information
****************************************************************/
/*
 * $Log:   T:/HH/INCLUDE/VCS/M_CALLS.H_V  $ 
 * 
 *    Rev 1.3   30 Nov 1992 20:19:38   DAVID
 * Removed prototype for strlen()
 * 
 *    Rev 1.2   15 Oct 1992 10:51:42   KIM
 * * * * SERVER DISK CRASH RECOVERY * * *
 *  1) The VCS directory was restored with Build 18 backup tape.
 *  2) This revision is to update files to Build 19.
 *  3) HELP.H has post-Build 19 changes.
 * 
 *    Rev 1.1   07 May 1992 17:03:28   DAVID
 * Added prototype for strlen
 * 
 *    Rev 1.0   03 Dec 1991 20:14:50   DAVID
 * Initial revision.
 *
 */

#ifndef  M_CALLS


/* **************** */

void copyn(void far *, void far *, unsigned);
void rcopyn(void far *, void far *, unsigned);
unsigned index(char, char far *, unsigned);
void fillc(char, void far *, unsigned);
void fillw(unsigned, void far *, unsigned);
void segcopy( int, int, int );
int  format(char far *, unsigned, unsigned, int, char, char far *, unsigned);
int  nformat(char far *, unsigned, unsigned, int, char, unsigned);
int  lformat(char far *, unsigned, unsigned, int, char, unsigned long);

#define	M_CALLS
#endif

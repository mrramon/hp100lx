/* $Header:   T:/HH/INCLUDE/VCS/HELP.H_V   1.3   23 Nov 1992 22:32:02   KIM  $ */
/***************************************************************************
 *
 *  Name:       HELP.H
 *  Date:       09/01/92
 *  Abstract:   Include file for applications using HELP (help2 functions).
 *
 * -------------------------------------------------------------------------
 *                  Copyright 1992 Lotus Development Corp.
 *                          All rights reserved.
 ***************************************************************************
 *
 * $Log:   T:/HH/INCLUDE/VCS/HELP.H_V  $
 * 
 *    Rev 1.3   23 Nov 1992 22:32:02   KIM
 * Added pKeyTxt (for use by non-LHAPI applications).
 * 
 *    Rev 1.2   19 Oct 1992 11:55:12   KIM
 * Changed to use near * (to far *) for Help data area.
 * 
 *    Rev 1.1   18 Oct 1992 17:07:28   KIM
 * Updated for "common" help file facility.
 * 
 *    Rev 1.4   12 Oct 1992 15:48:40   KIM
 * Reduced size of HELPDATASIZE.
 * 
 *    Rev 1.3   09 Oct 1992 18:32:54   KIM
 * Added HELPMETRICS structure for m_help2_getmetrics().
 * 
 *    Rev 1.2   28 Sep 1992 12:38:50   KIM
 * Final checkin for first version of Sys Mgr HELP support.
 * 
 *    Rev 1.1   23 Sep 1992 16:13:44   KIM
 * Phase 2: Select components are functional.
 * 
 *    Rev 1.0   21 Sep 1992 13:41:44   KIM
 * Initial revision.
 *
 ***************************************************************************/
#if !defined(HELP_H)
#define HELP_H

/*
 * Special Help context codes, common across all applications
 *
 */
#define H_C_GETSTARTED  0xFFFF
#define H_C_HOWUSEHELP  0xFFFE

/*
 * Return codes
 *
 */
#define H_RC_OK         0x0000

#define H_RC_FILE_ERR   0x0001
#define H_RC_REV_ERR    0x0002

#define HELPDATASIZE    0x0A80     


typedef struct HELPSTRUCT {

    unsigned int    wSize;      /* Struct size: For compatibility check     */
    char *          pHelpFile;  /* Near ptr to path and name of Help file   */
    char *          pHelpData;  /* Near ptr to far ptr to Help data area    */
    unsigned int    wHelpRev;   /* Help file rev #: compatibility check     */
    char *          pKeyTxt;    /* Near ptr to Bottom Row key text          */

} HELPSTRUCT;


typedef struct HELPMETRICS {

    unsigned int    wSize;      /* Struct size: For compatibility check     */
    unsigned int    wHX;        /* Help Screen X coordinate                 */
    unsigned int    wHY;        /* Help Screen Y coordinate                 */
    unsigned int    wHWidth;    /* Help Screen width including shadow       */
    unsigned int    wHHeight;   /* Help Screen height including shadow      */
    unsigned int    wSWidth;    /* Help Screen shadow width                 */
    unsigned int    wSHeight;   /* Help Screen shadow height                */

} HELPMETRICS;


typedef HELPSTRUCT  _near *pHELPSTRUCT;
typedef HELPMETRICS _near *pHELPMETRICS;

#endif  /* #if !defined(HELP_H) at top of this file */

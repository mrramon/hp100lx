/*
 * $Header:   T:/HH/INCLUDE/VCS/COMIO.H_V   1.11   31 Jan 1993 00:59:16   RICHB  $
 *  Name:       C:\SRC\COUGCOM\COMIO.H
 *  Date:       Wednesday May 20, 1992
 *  Legal:      Copyright 1992 Lotus Development Corp.
 *              All rights reserved.
 *  Abstract:     This file is included by applications wanting to use the
 *              sysmgr communications services. It contains all the
 *              definitions necessary for successful integration.
 *
 *                Note that since we don't guarantee source compatibility
 *              with Jaguar, some data structure members are either renamed
 *              or used for other purposes.
 *
 *  History:    
 *      RPB 5/20/1992  Created from the bones of Jaguar's comio.h.
 * $Log:   T:/HH/INCLUDE/VCS/COMIO.H_V  $
 * 
 *    Rev 1.11   31 Jan 1993 00:59:16   RICHB
 * Added com port power flags.
 * 
 *    Rev 1.10   24 Jan 1993 20:48:04   RICHB
 * Fixed number of buffers for handset/emulator.
 * 
 *    Rev 1.9   21 Dec 1992 22:36:20   RICHB
 * Made COM_BR_AUTO unique.
 * 
 *    Rev 1.8   07 Dec 1992 16:06:12   RICHB
 * Added com_br_auto
 * 
 *    Rev 1.7   25 Oct 1992 17:20:14   RICHB
 * Added constant for easily testing for any flow control being enabled.
 * 
 *    Rev 1.5   05 Oct 1992 16:57:34   RICHB
 * Fixed name collision with TERM_SETTINGS.
 * 
 *    Rev 1.4   02 Oct 1992 22:41:18   DAVID
 * Added struct for common configuration file (originally from  Filer) and
 * some defines it required.
 * 
 *    Rev 1.3   08 Sep 1992 21:47:08   RICHB
 * I misspelled "D", and spelled it "R" by accident.
 * 
 *    Rev 1.2   08 Sep 1992 21:45:16   RICHB
 * Added modem status masks.
 * 
 *    Rev 1.1   18 Aug 1992 03:04:42   RICHB
 * Added COM_CTL_NORMAL
 * 
 *    Rev 1.0   26 Jul 1992 21:14:36   RICHB
 * Initial revision.
 */

/*
 *    Some ancient history held over until this file is completely cleaned
 *  up.
 *
 *    Rev 4.0   16 Oct 1991 23:58:30   ADMINISTRATOR
 * Creating JAGUAR Branch
 * 
 *    Rev 3.2   16 Jul 1991 18:47:10   richb
 * Added the "comm power on suppresion" flag.
 * 
 *    Rev 3.1   22 Mar 1991 15:42:34   richb
 * Added 38400.
 * 
 *    Rev 3.0   14 Mar 1991 11:33:02   GOLD1
 * No change.
 * 
 *    Rev 2.2   11 Feb 1991 20:30:24   richb
 * Added explicit flow control clearing options for ComReset.
 * 
 *    Rev 2.1   20 Dec 1990 02:32:42   richb
 * Made 300 bps into 56.7K for filer testing.
 * 
 *    Rev 2.0   13 Nov 1990 16:29:10   BETA
 * No change.
 * 
 *    Rev 1.4   16 Oct 1990 11:54:00   unknown
 * made an ifndef so this file is only included once.
 * 
 *    Rev 1.3   05 Sep 1990 15:17:28   GBAYLES
 * Fixed bad parity constants.
 * 
 *    Rev 1.2   31 Aug 1990 15:00:56   GBAYLES
 * Added constants for parity MARK and SPACE.
 * 
 *    Rev 1.1   25 Jul 1990 16:15:40   GBAYLES
 * Added 19200 baud constant, and ENQ/ACK constant.
 * 
 *    Rev 1.0   13 Jul 1990 15:35:08   peterb
 * Initial revision.
 * 
 *    Rev 1.2   11 Jul 1990 16:31:20   GBAYLES
 * Fixed $LOG$
 */

#if !defined(_COMIO_H_)
#define _COMIO_H_

/*
 *	Something to keep the compiler happy while we sort out error codes.
 */

#include "comerrs.h"
#define err_t int
#define handle_t int
#define com_handle int

/*
 *    COM_LINE_? is used in com driver calls to indicate the com port
 *  the user wants to manipulate. COM_LINE_1 == COM1, etc.
 *
 *    COM_LINE_IDX_? is the index value to use for COM_LINE_?. This index
 *  value is only present in SysMgr internals.
 */

#define COM_LINE_1      1
#define COM_LINE_2      2
#define COM_LINE_A      3       /* "alternate" port (for Cougar). */
#define COM_LINE_3      3
#define COM_LINE_4      4

/*
 *        This value had better match the MAX_PORT value defined in the
 *      asiports.equ file, or you can expect some trouble.
 *
 *        It defines the maximum number of hardware ports that may be present.
 *      However, it may *not* reflect the actual number of ports that can be
 *      in use simultaneously because of buffer constraints, etc.
 */

#if defined(COUGAR)
#define COM_MAX_PORT    COM_LINE_A
#else
#define COM_MAX_PORT    COM_LINE_4
#endif

/*
 *    These are the acquisition types defined.
 *
 *      COM_ACQUIRE_EXCLUSIVE - No one can seize port unless force is used.
 *      COM_ACQUIRE_NORMAL - An app can seize from another app without using
 *          any force.
 *
 *      COM_SEIZE_NORMAL - Don't use force when seizing a port.
 *      COM_SEIZE_UNCONDITIONAL - Rough 'em up (use force) when seizing.
 */

#define COM_ACQUIRE_NORMAL 1
#define COM_ACQUIRE_EXCLUSIVE 2
#define COM_SEIZE_NORMAL 4
#define COM_SEIZE_UNCONDITIONAL 8

/*
 *        This flag added so that the filer doesn't unconditionally power
 *      up the UART.
 */

#define COM_PWR_SUPPRESS 0x80

#define COM_ANS_WAIT 1          /* Wait till phone answered */
#define COM_ANS_NOWAIT 2        /* Put phone in answer mode */
#define COM_ANS_STOP 3          /* Turn off answer mode (default) */

#define COM_RESET_LINE 0x1      /* Reset line */
#define COM_RESET_TXB 0x2       /* Flush transmit buffer */
#define COM_RESET_RXB 0x4       /* Flush receive buffer */
#define COM_RESET_MODEM 0x8     /* Reset modem */
#define COM_RESET_RXFLOW 0x10   /* Reset receiver's ^S state, if set. */
#define COM_RESET_TXFLOW 0x20   /* Reset transmitter's ^S state, if set. */

#define COM_CTL_NORMAL 0x0      /* can send a partial buffer. */
#define COM_CTL_WHOLE 0x01      /* Must send whole buffer. */
#define COM_CTL_SETRCV 0x02     /* Turn rcvr on after sending. */

/*
 *    Since we are obliged to retain *binary* compatibility, we can change
 *  the shape of the ComSettings structure in source, and introduce a new
 *  structure which retains the shape of the Jaguar compatible structure.
 *  All new applications will (continue to) use the ComSettings structure,
 *  which contains new Cougar goodies. The compatibility code in the Sysmgr
 *  will use the old definitions when running a Jaguar binary.
 *
 *    For ease of design, the structures match in the elements through
 *  "UnusedEcho".
 *
 *    Yes, I could have done things with unions, but I think it would have
 *  been messier than this, since Jaguar compatibility should disappear
 *  sooner or later. Only time will tell, however...
 */

#pragma pack(1)
typedef struct JAG_COM_SETTINGS { /* Jaguar binary compatibility. */
        char    UnusedDial;     /* Jaguar compatibility. */
        int     Baud;           /* Line speed (really UART's BRG value). */
        char    Parity;         /* Line Parity. */
        char    Stop;           /* Number of stop bits. */
        char    Data;           /* Number of data bits. */
        char    Flow;           /* Flow control. */
        char    InfraRed;       /* Infrared mode selected. */
        char    UnusedDuplex;   /* Jaguar compatibility. */
        char    UnusedEcho;     /* Really a terminal emulator thingie. */
} JComSettings_t;

typedef struct COM_SETTINGS {   /* New cougar version. */
        char    UnusedDial;     /* Jaguar compatibility. */
        int     Baud;           /* Line speed (really UART's BRG value). */
        char    Parity;         /* Line Parity. */
        char    Stop;           /* Number of stop bits. */
        char    Data;           /* Number of data bits. */
        char    Flow;           /* Flow control. */
        char    InfraRed;       /* Infrared mode selected. */
        char    UnusedDuplex;   /* Jaguar compatibility. */
        char    UnusedEcho;     /* Really a terminal emulator thingie. */
/*
 *    Anything added after this point is new to Cougar. Don't change anything
 *  above this comment unless you really know what you are doing.
 */
} ComSettings_t;
#pragma pack()

#define com_settings ComSettings_t  /* backward compatibility. */

/*
 *  Values for Dial field in com_settings struct.
 */

#define COM_DIAL_TONE 'T'       /* Go into TONE dial mode */
#define COM_DIAL_PULSE 'P'      /* Go into PULSE dial mode */

/*
 *    Maximum size of a dialing string, including the "ATD{T|P}" and a
 *  trailing <CR><NUL>.
 */

#define MAX_DIAL_STRING 64

/*
 *    Values for Baud field in com_settings struct. Note that these really
 *  are constants used to program the UART's "baud-rate generator". As such,
 *  lower values represent higher speeds. Yes, this in nonsense, but we have
 *  to keep this for binary compatibilty.
 */

#define COM_BR_110 0x417
#define COM_BR_150 0x300
#define COM_BR_300 0x180
#define COM_BR_600 0xC0
#define COM_BR_1200 0x60
#define COM_BR_2400 0x30
#define COM_BR_AUTO 0x05    /* we'll rendezvous at 2400 bps, then decide. */
#define COM_BR_4800 0x18
#define COM_BR_9600 0xC
#define COM_BR_19200 0x6
#define COM_BR_38400 0x3
#define COM_BR_57600 0x2
#define COM_BR_115200 0x1

/* 
 *    These values define the line speeds allowable with Cougar IR. We run
 *  at 9600 in Cougar, and 2400 for backward compatibility with Jaguar.
 */

#define COM_BR_JAGUAR_IR 0x30
#define COM_BR_COUAR_IR 0xc

/*
 *    Values for Parity field in com_settings struct. Again, these are values
 *  that can be used to program the UART directly.
 */

#define COM_PTY_NO 0x0          /* No parity. */
#define COM_PTY_ODD 0x8         /* Odd parity. */
#define COM_PTY_EVEN 0x18       /* Even parity. */
#define COM_PTY_MARK 0x28       /* Mark parity. */
#define COM_PTY_SPACE 0x38      /* Space parity. */

/*
 *    Values for Stop field in com_settings struct. Once more, UART bit
 *  masks are used.
 */

#define COM_STOP_1 0x0          /* One stop bit. */
#define COM_STOP_2 0x4          /* Two stop bits. */

/*
 *    Values for Data field in com_settings struct. You'll never guess: these
 *  are UART masks, too.
 */

#define COM_DATA_7 0x2          /* Seven data bits. */
#define COM_DATA_8 0x3          /* Eight data bits. */

/*
 *    Values for Flow field in com_settings struct. At this point, RTS-CTS
 *  hardware flow control is mutually exclusive with XON-XOFF.
 *
 *    ENQ-ACK is a weird one, in that it is the host polling to see if the
 *  remote can accept more data. If this is selected in the Flow variable,
 *  then the driver will, in fact, do ENQ-ACK flow control (basically it
 *  answers "ACK" for an ENQ if there's enough buffer space. However, the
 *  terminal emulator probably doesn't want this to happen, especially
 *  given the problems we had in the Jaguar; the terminal emulator will
 *  definitely want to handle ENQ-ACK itself.
 *
 *    While RTS-CTS is mutually exclusive with XON-XOFF, ENQ-ACK can
 *  supplement either RTS-CTS or XON-XOFF.
 */

#define COM_XON_OFF 0x1         /* Moron: unused. Disable XOF/XON. */
#define COM_FLOW_NONE 0x1       /* A better definition. */
#define COM_XON_ON 0x2          /* XOF/XON selected. */
#define COM_XON_ANY 0x4     /* XOF/ANY selected (not doc'ed or impl'ed!). */
#define COM_ENQACK 0x8          /* ENQ/ACK for HP. */
#define COM_HARDWARE 0x10       /* RTS-CTS flow control. */

/*
 *   This constant eases testing for any flow control being enabled.
 */

#define COM_FLOW_CONTROLLED (COM_HARDWARE | COM_ENQACK | COM_XON_ON)

/*
 *    Values for InfraRed field in com_settings struct. This only applies
 *  to the built in com port on Cougar.
 */

#define COM_IR_OFF 1            /* Turn off infra-red/on modem/rs-232 */
#define COM_IR_ON  2            /* Turn on infra-red/off modem/rs-232 */

/*
 *      Values for Duplex field in com_settings struct (unused in cougar).
 */

#define COM_DUP_HALF 1
#define COM_DUP_FULL 2

/*
 *        Values for Echo field in com_settings struct. This is really a
 *      terminal emulator parameter, but we'll keep it here for compatibility
 *      for now.
 *
 *        Also, for the Jaguar, the sense of this was backwards. What it
 *      described was what the remote computer did (i.e. if it echoed or not),
 *      and it did *not* describe the local machines behavior.
 */

#define COM_ECHO 1
#define COM_REMOTE_ECHOS 1              /* a better definition. */
#define COM_NOECHO 2
#define COM_NEED_ECHO 2                 /* a better definition. */

/*
 *        Values for Compression field in com_settings struct. Jaguar stuff,
 *      why G3, eh, Smokey?
 */

#define COM_CMP_OFF 1           /* Turn compression off */
#define COM_CMP_G31D 2          /* Group 3, 1 dimensional (FAX uses this) */

/*
 *    These are the values to be used in the ComMdmClr and ComMdmSet
 *  function calls. Note that you can really screw things up if you don't
 *  know what you are doing, or if hardware flow control is in effect and
 *  and you diddle RTS.
 */

#define COM_MDM_DTR	0x1
#define COM_MDM_RTS 0x2
#define COM_MDM_OUT1 0x4
#define COM_MDM_OUT2 0x8
#define COM_MDM_LOOP 0x10
#define COM_MDM_MASK 0x1f       /* to isolate the above bits. */

/*
 *    These bits can be used to test the modem status returned by the
 *  ComMdmStatus call.
 *
 *    I wouldn't trust the "XXX changed" bits, since the modem control
 *  register may be read in the course of interrupt processing. The other
 *  bits will be okay, though.
 */

#define COM_MDM_DCTS 0x1        /* CTS changed. */
#define COM_MDM_DDSR 0x2        /* DSR changed. */
#define COM_MDM_TERI 0x4        /* Trailing edge RI. */
#define COM_MDM_DDCD 0x8        /* DCD changed. */

#define COM_MDM_CTS 0x10        /* current CTS. */
#define COM_MDM_DSR 0x20        /* current DSR. */
#define COM_MDM_RI  0x40        /* current RI. */
#define COM_MDM_DCD 0x80        /* current DCD. */


/*
 *    Buffer parameters for Jaguar. We permit unique sizes for the transmit
 *  and receive buffers, but know of no reason to take advantage of this
 *  feature.
 *
 *    Also, we must always have at least as many buffers as the maximum
 *  number of simultaneously open com ports (makes sense, no?).
 */

#if defined(TSR)
#define COM_MAX_BUFFERS 1
#elif defined(ELEKTRA)
#define COM_MAX_BUFFERS 1
#else                               /* handset/emulator. */
#define COM_MAX_BUFFERS 2
#endif

#define COM_RX_BUFFER_SIZE (4097 + 256)
#define COM_TX_BUFFER_SIZE (4097 + 256)

#define RX_LOW_WATER (COM_RX_BUFFER_SIZE / 4)
#define RX_HIGH_WATER (COM_RX_BUFFER_SIZE - RX_LOW_WATER)

/*
 * Common Configuration information - based on filer
 */

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


typedef struct {

	com_settings		/* COM driver settings (baud, parity, ...) */
		ComSettings;
   int   comm_port;                    /* comm port (com1 or com2)     */
	char	CreatedBy;                    /* who created these settings   */
	char	SettingsRev;                  /* version of the structure     */
	long	Magic1;                       /* Magic number for file typing */
	char	Number[MAX_DIAL_STRING];      /* Phone number. */
	char	Script[MAX_FILE_STRING+1];	   /* Script file (+even pad) */
	char	Ctf[MAX_FILE_STRING+1];       /* Conversion translation file */
	char	Emulation;	            /* Emulation - NONE, VT100, GLASS TTY, MAP */
	char	Wrap;	                        /* Screen wrap on or off. */
	char	Backspace;                    /* Backspace char */
	long	Magic2;

} COMMON_CONFIG;

#define MAGIC1_VALUE	0x61766961
#define MAGIC2_VALUE	0x6c657261

#define FILER_SETTINGS    2             /* created by filer (signature) */
#define TERMINAL_SETTINGS 3             /* created by filer (signature) */
#define RSERVER_SETTINGS  4             /* created by filer (signature) */
#define RCLIENT_SETTINGS  5             /* created by filer (signature) */

#define SETTINGS_REVISION 3             /* revision # for config file   */

/*
 *  Flags for power on and power off.
 */

#define COM_PORT_OFF    0
#define COM_PORT_ON     1

#endif


;
; SCCS info:   Module scktsrv.inc  Version 1.1  Date 09/22/92  Time 13:23:57
;
;****************************************************************************
;File: SCKTSRV.INC
;
;By:	slf
;
;Created:	12-09-91
;Updated:	12-09-91
;
;This include file contains constant definitions for the PCMCIA Socket
;Services Interface.  This is based on Release 1.01 of the specification.
;---------------------------------------------------------------------------*/

;----	Define socket services interrupt number.

scktSrvInt 	equ	1Ah

;----	Define the command numbers.
;
getNumberAdapters 	equ	080h	
regStatChngCall		equ	081h
regCardTechCall		equ	082h
getSSVersion		equ	083h
inquireAdapter		equ	084h
getAdapter		equ	085h
setAdapter		equ	086h
inquireWindow		equ	087h
getWindow		equ	088h
setWindow		equ	089h
getPage			equ	08Ah
setPage			equ	08Bh
inquireSocket		equ	08Ch
getSocket		equ	08Dh
setSocket		equ	08Eh
getCard			equ	08Fh
resetCard		equ	090h
readOne			equ	091h
writeOne		equ	092h
readMultiple		equ	093h
writeMultiple		equ	094h
inquireEDC		equ	095h
getEDC			equ	096h
setEDC			equ	097h
startEDC		equ	098h
pauseEDC		equ	099h
resumeEDC		equ	09Ah
stopEDC			equ	09Bh
readEDC			equ	09Ch

;----	Define the socket services error codes.

BAD_ADAPTER		equ	001h
BAD_ATTRIBUTE		equ	002h
BAD_BASE		equ	003h
BAD_EDC			equ	004h
BAD_INDICATOR		equ	005h
BAD_IRQ			equ	006h
BAD_OFFSET		equ	007h
BAD_PAGE		equ	008h
BAD_READ		equ	009h
BAD_SIZE		equ	00Ah
BAD_SOCKET		equ	00Bh
BAD_TECHNOLOGY		equ	00Ch
BAD_TYPE		equ	00Dh
BAD_VCC			equ	00Eh
BAD_VPP			equ	00Fh
BAD_WAIT		equ	010h
BAD_WINDOW		equ	011h
BAD_WRITE		equ	012h
NO_ADAPTERS		equ	013h
NO_CARD			equ	014h
UNSUPPORTED_FUNCTION	equ	015h

;----	Card status byte bit definitions (used in Status Change Callback,
;----	Get Card command, and low level Get Card Command).

CS_WP		equ	001h	;Write protected.
CS_BVD1		equ	010h	;Battery Voltage Detect 1 (1=dead).
CS_BVD2		equ	020h	;Battery Voltage Detect 2 (1=warning).
CS_rdy		equ	040h	;Ready/Busy state (1=ready).
CS_cardDetect	equ	080h	;Card Detected (1 = detected).

;----	Socket Status Change interrupt mask bit definitions.  (used in Status
;----	Change Callback, Get Socket, and Set Socket).

SCI_ejectReq	equ	004h	;Ejection request interrupt.
SCI_insertReq	equ	008h	;Insertion request interrupt.
SCI_batDead	equ	010h	;Battery Dead interrupt.
SCI_batWarn	equ	020h	;Battery low warning interrupt.
SCI_rdyChng	equ	040h	;Ready signal changed interrupt.
SCI_cardDetect	equ	080h	;Card change detected interrupt.

;----	Socket status bit definitions (used in Status Change Callback,
;----	Get Socket, and Set Socket.  

SS_locked	equ	002h	;lock card or card is locked
SS_ejectReq	equ	004h	;eject pending or reset eject pending
SS_insertReq	equ	008h	;insert pending or reset insert pending
SS_ejectCard	equ	010h	;eject card or eject complete
SS_insertCard	equ	020h	;insert card or insert complete
SS_cardChng	equ	080h	;card changed or reset card change

;----	Read/Write attributes bit definitions.  Used in Read One, Write One, 
;----	Read Multiple, Write Multiple, Card Technology Callback, and Low Level
;----	Socket Services Write Many, Write One, and Read One.
		
RW_memType	equ	001h	;Memory Type (0=common, 1=attribute)
RW_dataWidth	equ	002h	;Data width (0=byte, 1=word)
RW_evenBytes	equ	004h	;Even bytes only (0=even and odd, 1=even)
RW_packed	equ	008h	;Packed (1=true)

;----	Adapter capabilities bit definitions.  Used in Inquire Adapter.

ACAP_indicators	equ	001h	;1=indicators set for adapter.
ACAP_pwrMang	equ	002h	;Power management set for adapter.
ACAP_busWidth	equ	004h	;Bus width set for adapter.
ACAP_statChgInt	equ	008h	;Status Change interrupt available.
ACAP_SCISftShr	equ	010h	;Status change interrupt is software
				;shareable.
ACAP_SCIHwdShr	equ	020h	;Status change interrupt is hardware
				;shareable.

;----	Adapter attributes bit definitions.  Used in Get Adapter and Set
;----	Adapter.

AA_reducePwr	equ	001h	;reduced power mode or reduce power command
AA_presrvState	equ	002h	;preserve state in reduced power mode.
AA_enaSCInt	equ	004h	;Enable Status Change interrupt.
AA_sftShrSCI	equ	008h	;Software share status change interrupt.
AA_hwdShrSCI	equ	010h	;Hardware share status change interrupt.

;----	Window capabilities bit definitions.  Used in Inquire Window.

WCAP_commonMem	equ	001h	;Window can map common memory.
WCAP_attrMem	equ	002h	;Window can map attribute memory.
WCAP_IOSpace	equ	004h	;Window can map I/O
WCAP_EISAIO	equ	008h	;Window can map EISA I/O
WCAP_sepEnaEISA	equ	010h	;EISA mapping with separate enables.

;----	Window access speeds.  Used in Inquire Window, Get Window, and Set 
;----	Window.

WIN_WAIT	equ	001h	;Use card wait line.
WIN_100NS	equ	002h	;The reset of these generate wait states
WIN_150NS	equ	004h	;to satisfy a card of the indicated speed.
WIN_200NS	equ	008h
WIN_250NS	equ	010h
WIN_300NS	equ	020h
WIN_600NS	equ	040h

;----	Memory and I/O window characteristics table capabilities.  NOTE: I/O
;----	windows don't use the last 3 values.  Used in Inquire Window.

WCT_prgBaseAddr	equ	0001h	;has programmable base address
WCT_prgWinSize	equ	0002h	;has programmable window size
WCT_winDisEna	equ	0004h	;has disable/enable capability
WCT_8BitBus	equ	0008h	;8-bit bus supported
WCT_16BitBus	equ	0010h	;16-bit bus supported
WCT_baseAddrAlgn equ	0020h	;Requires base address alignment on
				;size bounrdy.
WCT_pwr2Size	equ	0040h	;Power of 2 size granularity.
WCT_cardAddrAlgn equ	0080h	;Requires card alignment on size boundary
WCT_pageHwdAvail equ	0100h	;Paging hardware available
WCT_pageHwdShared equ	0200h	;Paging hareware is shared
WCT_pageDisEna	equ	0400h	;page disable/enable supported

;----	Window attributes definitions.  Used in Get Window and Set Window.

WA_IOnotMEM	equ	001h	;1=I/O window, 0=memory window
WA_ATTRnotCOMMON equ	002h	;1=map attribute mem, 0=map common mem
WA_EISAnotIO	equ	002h	;1=map EISA I/O, 0=map I/O
WA_ENAnotDIS	equ	004h	;1=enabled, 0=disabled
WA_busWidth	equ	008h	;1=16 bit bus, 0=8 bit bus
WA_paged	equ	010h	;1=paged, 0=not paged
WA_EISAnonspecific equ	020h	;Non-Specific EISA access slot enable=1

;----	Page attributes definitions.  Use in Get Page and Set Page

PA_ENAnotDIS	equ	001h	;1=page enabled, 0=page disabled

;----	Socket capabilities bit definitions.  Used in Inquire Socket.

SCAP_cardChng	equ	001h	;1=Can report card change.
SCAP_cardLock	equ	002h	;1=can lock card in socket.
SCAP_cardInsert	equ	004h	;1=card insert supported
SCAP_cardEject	equ	008h	;1=card eject supported

;----	Socket hardware indicators bit definitions.  Used in Inquire Socket,
;----	Get Socket, and Set Socket.

SHI_busy	equ	001h		;1=Card is busy
SHI_WP		equ	002h		;1=Card is write protected
SHI_bat		equ	004h		;1=batter is low or dead
SHI_cardLock	equ	008h		;1=card is locked
SHI_XIP		equ	010h		;1=XIP is using the card

;----	Socket characteristics table bit definitions of Supported Card Types

SCT_memory	equ	001h	 	;Socket supports memory cards.
SCT_IO		equ	002h		;Socket supports I/O cards.

;----	Power management table entry power supply bit definitions.

PMT_VPP2	equ	020h		;Entry is possible on VPP2
PMT_VPP1	equ	040h		;Entry is possible on VPP1
PMT_VCC		equ	080h		;Entry is possible on VCC

;----	Define the socket characteristics table type.  The supportCardTypes
;----	field indicating the types of cards supported (SCT_memory and/or
;----	SCT_IO).  SteerableIRQ is a bit mask of interrupt requests that the
;----	socket can generate.  Bits 0 - 15 coorespond to IRQ through IRQ15.  
;----	The otherIRQ field is a bit mask of other interrupt sources.  Bit 0 is
;----	NMI, bit 17 is I/O Check, bit 18 is bus error, and bit 18 is vendor
;----	unique.

socketCharTab	STRUC

supportCardTypes DW	?
steerableIRQ	 DW	?
otherIRQ	 DW	?

socketCharTab	ENDS

;----	This structure defines the power management table.  The number of
;----	copies in the "aVoltSet" structure is in tabSize.  For each aVoltSet
;----	entry voltage is the voltage times 10 (example - 5 volts is enter as
;----	50) and pins is a bit map of the card pins this voltage is available
;----	on.  Use the defined value PMT_VPP2 for the VPP2 pin, PMT_VPP1 for
;----	the VPP1 pin, and PMT_VCC for the VCC pin.

volpin	STRUC

voltage	DB	?
pins	DB	?

volpin	ENDS

pwrMang	STRUC

tabSize		DW	?

pwrMang	ENDS

;----	Structures of type socketInfo are used to store the information 
;----	returned by the SSInqSocket function.  This information describes 
;----	the characteristics of a socket.

socketInfo	STRUC

socketcap	DB	?
hwdInd		DB	?
theSCT		DD	?
thePMT		DD	?

socketInfo	ENDS

;----	Structures of type scktConf are used to store the information returned
;----	by/given to the SSGetSocket/SSSetSocket functions.  This information
;----	gives/sets the current state of the socket.

scktConf	STRUC

statChngIEM	DB	?
vccLevel	DB	?
vpp1Level	DB	?
vpp2Level	DB	?
scktAttr	DB	?
scktInd		DB	?
cardType	DW	?
IRQLevel	DW	?

scktConf	ENDS

;----	This is a structure which is used to access a memory window 
;----	characteristics	table.  (see the socket services inquire window
;----	command).  NOTE: The winCap field bit definitions are defined by
;----	the WCT_ constants defined above.

memWinCharTab struc

mWinCap   dw ?		;Window Capabilities (bit map)
mMinBase  dw ?		;Minimum base address / 4K
mMaxBase  dw ?		;Maximum base address / 4K
mMinSize  dw ?		;Minimum size / 4K
mMmaxSize dw ?		;Maximum size / 4K
mWwinGran dw ?		;Window granularity / 4K
mBaseAlign dw ?		;Base alignment required / 4K
mCardAlign dw ?		;Card alignment required / 4K

memWinCharTab ends

;----	This is a structure which is used to access an I/O window
;----	characteristics	table.  (see the socket services inquire window
;----	command).  NOTE: The winCap field bit definitions are defined by
;----	the WCT_ constants defined above.

IOWinCharTab struc

iWinCap	dw ?		;window capabilities bit map
iMinBase dw ?		;Minimum Base Address (bytes)
iMaxBase dw ?		;Maximum Base Address (bytes)
iMinSize dw ?		;Minimum window size (bytes)
iMaxSize dw ?		;Maximum window size (bytes)
iWinGran dw ?		;window granularity (bytes)

IOWinCharTab ends

;----	Structures of type winInfo are used to store the information returned
;----	by the SSInqWindow function.  This information describes the 
;----	characteristics of a window.

winInfo	struc

aMWCT		dd	?
anIOWCT		dd	?
assgnSckts	dw	?
capabilities	db	?
EISAAddr	db	?
accessSpds	db	?

winInfo	ends

;----	Structures of type winConf are used to store the information returned
;----	by/given to the SSGetWindow/SSSetWindow functions.


winConf struc

winSize	   dw	?
winBase	   dw	?
cardOffset dw	?
socket	   db	?
attr	   db	?
accessSpd  db	?

winConf	ends


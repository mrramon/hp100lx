;
;****************************************************************************
; File: CBEXTDEF.INC - Card BIOS External Definitions
;
; ---------------------------------------------------------------------------
; This include file contains definitions for Card BIOS data structures,
; bit fields, and constants that are part of the external specification.
;----------------------------------------------------------------------------
;
;----------------------------------------------------------------------------
;
;----   Request packet structure for GetBIOSInfo function.

requestGetBIOSInfo      struct

rlength         db      ?
function        db      ?
signature       dw      ?
baseRAM         dw      ?
clientRAM       dw      ?
handleRAM       dw      ?
regionRAM       dw      ?
scktCount       dw      ?
attributes      dw      ?
vendorString    dd      ?
version         dw      ?

requestGetBIOSInfo      ends

;----   Bit definitions for attributes field of requestGetBIOSInfo structure.

BIOSInitialized equ     1h

;----   Request packet structure for InitializeBIOS function.

requestInit     struct

rlength         db      ?
function        db      ?
RAMSize         dw      ?
RAMPtr          dd      ?
maxHandles      dw      ?
maxRegions      dw      ?

requestInit     ends

;----   Request packet structure for UnitializeBIOS function.

requestUninit   struct

rlength         db      ?
function        db      ?

requestUninit   ends

;----   Request packet structure for RegisterClient function.

requestReg      struct

rlength                 db      ?
function                db      ?
clientStructPtr         dd	?

requestReg      ends

;----   Client registration structure passed from client during
;----   RegisterClient function.

clientStruct    struc

clientLink              dd      ?
clientEntry             dd      ?
clientEraseQueuePtr     dd      ?
clientEventMask         dw      ?
clientAttributes        dw      ?
                                        ; start of BIOS work area
clientArtEvents         db      ?       ; *** MUST BE FIRST ADDED ***
                                        ; (its offset is used in computation
                                        ;  of client RAM requirement)
clientBroadcastPtr	dd	?	; Far pointer passed in broadcast
					; request events.
scktEvents  dw      1 dup (?)           ; Holds the socket related pending
                                        ; events for this client.  One word
                                        ; per socket.  MUST BE LAST ITEM
                                        ; IN THIS STRUCTURE.
clientStruct    ends

;----   Bit definition for clientEventMask field of clientStruct.
;
;----   Note 1: For some bits the socket services defines are used,
;----   they are documented here for reference.
;
;----   Note 2: Bits 3, 9-11, and 13-15 are reserved and should be zero.

clientEvent_CardInsert  equ     0001h
; SS_locked             equ     0002h   ; ss socket status: lock card or card is locked
; SS_ejectReq           equ     0004h   ; ss socket status: eject pending or reset eject pending
; CS_BVD1               equ     0010h   ; ss card status: battery dead
; CS_BVD1               equ     0020h   ; ss card status: battery low
; CS-rdy                equ     0040h   ; ss card status: ready change
; SS_cardChng           equ     0080h   ; ss socket status: card change
eventAPMResume          equ     0100h	; client wants APM Resume events 
eventBroadcast		equ	1000h	; client wants broadcast events

;The following are definded for the socket data structure socket events
;field whose other bits are definded the same a clientEventMask.  (In
;CBINTDEF.INC)  We need to avoid conflicts with these.

;eventAPMResuming	equ	2000h	;About to process APM_RESUME on
					;this socket
;clientEvent_CardRemoval equ     4000h   ;Card removed event on this socket
;clientEvent_WPChange    equ     8000h   ;Write protect change on this socket

;----   Bit definitions for clientAttributes field of clientStruct.
;----   Note: Only bit zero is defined, bits 1..15 are reserved.

MemCardDD       equ     1h      ; Memory card client device driver.

;----   Bit definitions for clientArtificialEvents field of clientStruct.

registrationCompleteEvent equ   2
eraseCompleteEvent        equ   4
requestBroadcastEvent	  equ	8


;----   Erase Queue Header structure pointed to by the clientEraseQueuePtr
;----   in clientStruct.

eraseQueueHeaderStruct  struct

eraseQueueLink          DD      ?
eraseQueueEntryLen      DB      ?
eraseQueueEntryCnt      DB      ?

eraseQueueHeaderStruct  ends

;----   Erase Queue Entry structure.  eraseQueueEntryCnt of these immediately
;----   follow the Erase Queue Header in memory.

eraseQueueEntryStruct   struct

socket                  db      ?
memoryType              db      ?
memoryHandle            dw      ?
entryState              db      ?
eraseLen                db      ?
cardEraseOffset         dd      ?
QClient                 dd      ?   ;Pointer to client structure (first entry
                                    ;only).

eraseQueueEntryStruct   ends

;----   Request packet structure for Read and Write Memory functions.

requestRdWrt      struct

rlength  db     ?
function db     ?
socket   db     ?
memoryType db   ?
memoryHandle dw ?
tLength  dw     ?
cardOffset  dd  ?
bufferPtr   dd  ?

requestRdWrt      ends

;----   Request packet structure for CopyMemory function.

requestCopy      struct

rlength  db     ?
function db     ?
socket   db     ?
memoryHandle dw ?
tLength  dw     ?
srcOffset  dd   ?
destOffset  dd   ?

requestCopy      ends

;----   Bit definitions for memoryType field.

memoryCommon            equ     01h     ; origin is common memory
memoryAttribBoth        equ     02h     ; both even and odd attrib mem bytes

;----   Request packet structure for RequestEraseQueueService function.

requestErase    struct

rlength                 db      ?
function                db      ?
clientStructPtr dd      ?

requestErase    ends

;----   Request packet structure for GetStatus function.

requestStatus struct

rlength         db      ?
function        db      ?
socket          db      ?
rsv             db      ?
cardStatus      dw      ?
socketStatus    dw      ?

requestStatus ends

;----   Request packet structure for Get First/Next Region functions.

requestRegion struct

rlength         db      ?
function        db      ?
socket          db      ?
memoryType      db      ?
regionPtr       dd      ?
regionStructLen db      ?
                                        ; start of BIOS reserved area
                                        ;   (16 bytes total)

sysRegionPtr    dw      ?               ; OUTPUT: near ptr to the
                                        ; sysRegionStruct from which the
                                        ; region information was extracted
                                        ; (assuming function was successful)
                                        ; Used as an input by the get next
                                        ; function.

regionNumber    db      ?               ; from Microsoft BIOS, need???
reserved        db      13 dup (?)      ; available for BIOS expansion

requestRegion ends

;----   Region structure pointed to regionPtr field in requestFirstRegion
;----   and requestNextRegion.  Note that this definition lacks the
;----   access speed which is needed in a windowed interface.  For this
;----   reason, an internal region structure is defined in llcb.inc which
;----   includes this structure together with additional information.

regionStruct    struct

regionSize              dd      ?
effBlockSize            dd      ?
partMinimumSize         dd      ?
jedecId                 dw      ?
regionDevType           db      ?
regionAttributes        db      ?
memoryHandle            dw      ?
cardOffset              dd      ?

regionStruct    ends

;----   Equates for the regionDevType field in regionStruct.

DTYPE_NULL      equ     00h     ; No device, a hole in the address space.
DTYPE_ROM       equ     01h     ; Masked ROM.
DTYPE_OTPROM    equ     02h     ; One-Time Programmable ROM.
DTYPE_EPROM     equ     03h     ; UV EPROM.
DTYPE_EEPROM    equ     04h     ; EEPROM.
DTYPE_FLASH     equ     05h     ; Flash EPROM.
DTYPE_SRAM      equ     06h     ; Static RAM.
DTYPE_DRAM      equ     07h     ; Dynamic RAM.
DTYPE_IO        equ     0Dh     ; I/O devcie.
DTYPE_EXTEND    equ     0eh     ; Extended type.
DTYPE_ATA       equ     20h     ; ATA Interfaced card.
DTYPE_UNKNOWN   equ     0ffh    ; Device type cannot be determined.

;----   Bit definitions for the regionAttributes field in regionStruct.
;----   Bits 5..7 are reserved and must be 0.

writeProtectIgnored     equ     01h
CISused                 equ     02h
regionReadable          equ     04h
regionWritable          equ     08h
regionErasable          equ     10h

;----   Structure used by get First/Next tuple.  This is not part of
;----   Card BIOS; it is being retained from File Services because
;----   the plan is to extend Card BIOS to provide these functions.

requestTuple    struc

rlength         db      ?
function        db      ?
socket          dw      ?
flags           dw      ?
linkOffset      dd      ?
CISOffset       dd      ?
attributes      db      ?
desiredTuple    db      ?
bufferPtr       dd      ?
bufferLength    dw      ?
tupleOffset     dw      ?
bytesRetrieved  db      ?

requestTuple    ends

;----   Bit definitions for attributes of reqTuple.

returnLinkTuples equ    1
getNextSubtuple  equ    2

;----   Return codes used by the tuple routines.  Again, these are not
;----   part of standard Card BIOS but they are included here in
;----   anticipation of extending Card BIOS to provide these functions.

END_TUPLE       equ     1Ah
END_OF_TUPLE    equ     1Bh
NO_CIS          equ     1Ch
NO_TUPLE        equ     1Dh

;----	Request packet structure for setSocketBusyCallback function.

reqSetSBCall	struct

rlength		db	?
function	db	?
callbackPtr	dd	?	

reqSetSBCall	ends

;----   Request structure for getDriveParameters function.

requestGetDP    struct

rlength         db  ?
function        db  ?
memoryHandle    dw  ?
numSectPerTrack db  ?
numHeads        db  ?
numCylinders    dw  ?

requestGetDP    ends

;----   Request structure for broadcastRequest function.

requestBroadcast    struct

rlength         db  ?
function        db  ?
clientPtr	dd  ?
dataPtr		dd  ?

requestBroadcast    ends

requestSetRemovable	struct

rlength		db  ?
function	db  ?
socket		db  ?
removable	db  ?

requestSetRemovable	ends

;---- Card BIOS version, INT 1A AX identification code, and signature.

VERSION_MAJOR           equ     01h     ; '1'
VERSION_MINOR           equ     02h     ; '2'
CARD_BIOS_REQUEST       equ     0B000h
BIOS_SIGNATURE          equ     'CS'


;---- Card BIOS command codes.

getBIOSInfo                     equ     00h
initializeBIOS                  equ     01h
uninitializeBIOS                equ     02h
registerClient                  equ     03h
deregisterClient                equ     04h
getFirstRegion                  equ     05h
getNextRegion                   equ     06h
getStatus                       equ     07h
read                            equ     08h
write                           equ     09h
copy                            equ     0Ah
requestEraseQueueService        equ     0Bh
getFirstTuple                   equ     80h
getNextTuple                    equ     81h
getTupleData                    equ     82h
hookInt2F                       equ     83h
setBusyCallback                 equ     84h
getDriveParms                   equ     85h
broadcastRequest		equ	86h
setRemovable			equ	87h

; Card BIOS return codes.

MEDIA_MISSING                   equ     080h
MEDIA_WRITE_PROTECTED           equ     084h
MEDIA_NOT_ERASABLE              equ     086h
MEDIA_NOT_WRITEABLE             equ     087h
INVALID_CARD_SOCKET             equ     088h
INVALID_LENGTH                  equ     089h
INVALID_CARD_MEMORY_ADDRESS     equ     08Ah
READ_WRITE_ERASE_FAULT          equ     08Bh
NO_ATTRIB_MEMORY                equ     08Ch
NO_MORE_REGIONS                 equ     090h
BIOS_INVALID_FUNCTION           equ     0D0h
BIOS_UNINITIALIZED              equ     0D1h
BIOS_INVALID_HANDLE             equ     0D2h
BLOCK_ERASE_PASSED              equ     0E0h
BLOCK_ERASE_FAILED              equ     0E1h
GENERAL_FAILURE                 equ     0FFh

;---- Erase Queue States (other than return codes.)

QUEUED_FOR_ERASE                equ     000h
READY_FOR_ERASE                 equ     001h
ERASING_ENTRY                   equ     002h

; Card BIOS event codes.

BATTERY_DEAD                    equ     01h
BATTERY_LOW                     equ     02h
CARD_LOCK                       equ     03h
CARD_READY                      equ     04h
CARD_REMOVAL                    equ     05h
CARD_UNLOCK                     equ     06h
EJECTION_COMPLETE               equ     07h
EJECTION_REQUEST                equ     08h
INSERTION_COMPLETE              equ     09h
INSERTION_REQUEST               equ     0Ah
APM_RESUME                      equ     0Bh
APM_SUSPEND                     equ     0Ch
WRITE_PROTECT_TOGGLE            equ     0dh
APM_RESUMING			equ	10h
CARD_INSERTION                  equ     40h
ERASE_COMPLETE                  equ     81h
REGISTRATION_COMPLETE           equ     82h
BROADCAST_NOTIFY		equ	90h
BROADCAST_COMPLETE		equ	91h

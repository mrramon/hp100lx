;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;
; CICDEMO is an example "card installation client" (CIC) for the 
; HP 100LX or HP 200LX.  The typical job of a card installation 
; client is to recognize and configure a certain PCMCIA card or 
; certain type of PCMCIA cards.  As such a CIC must watch as cards
; come and go from the PCMCIA slot and be able to recognize when
; on of its cards has been inserted.  When one of its cards is
; present, CIC configures the card as necessary for use by higher
; level applications.
;
; A CIC is typically a TSR (either DOS or System Manager).
;
; CICDEMO does Megahertz XJ1144 modem card recognition and configuration
; because this card was available to the author.  CICDEMO duplicates 
; some of the functions of CIC100, which is the	modem card installer 
; provided with the 100LX and 200LX; consequently, CIC100 should not be 
; loaded when experimenting with CICDEMO since they will be fighting 
; over the Megahertz card.
;
; The main features of CICDEMO are:
;
; - It is a DOS TSR.  To show how to unregister, an uninstall capability
;   is included.  To uninstall, use command line: CICDEMO U.
;
; - It registers as a CardBIOS client in order to receive notification of
;   card insertions and removals.
;
; - It checks each newly inserted card for being its card by
;   examining the card's tuples (if any).
;
; - If it finds its card, the card is configured as COM2 and
;   two beeps are sounded.  When its card is removed one beep
;   is sounded.
;
; - If it finds a non-Megahertz card, one beep is sounded.
;
; To build the program use MASM 6.x with the command
;
;   ml cicdemo.asm
;
; See the HP Palmtop developer's guide for overview information.
;
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;

        .model	tiny
	.186

;----------------------------------------------------------------------------
; General program structure.
;
; CICDEMO is written as a .COM program so the basic situation is that
; DS = ES = SS = CS -> PSP.  The initialization code and data follow the
; TSR code and data.  This allows the initialization code and data to
; be jetisoned when the program becomes a TSR.
;
; The Multiplex interrupt (2Fh) is used as a mechanism for checking to
; see if the TSR is already loaded and finding its location if it is
; already loaded.  An attempt was made to use this in a standard way.
;

;----------------------------------------------------------------------------
;----	Includes.

;----	cbextdef.inc is the CardBIOS external definitions.  This file
;----	contains structure definitions and defines that are useful in
;----	calling CardBIOS functions.

	include cbextdef.inc

;----	scktsrv.inc is structure definitions and defines that are useful
;----	in calling Socket Services functions.

	include scktsrv.inc


;----------------------------------------------------------------------------
;----	Defines.


; Mask for determining for which events we'll receive notification from
; CardBIOS.  We ask for card insertion/removal & advanced power management.
; See the cardBIOScallback routine for comments on what events will be
; reported with this setting.

EVENT_MASK 	equ	clientEvent_CardInsert OR eventAPMResume

; Some 100LX specific timing constants.
; Number of times though "loop $" to guarantee a 1 millisecond delay.

NS_PER_CYCLE	EQU	125		; nano seconds per cycle (8mH clock)
CYCLES_PER_LOOP	EQU	16		; cycles for "loop $" on a 80186
NS_PER_US	EQU	1000		; nano seconds per micro second

CX_FOR_1MS	EQU	(1000 * NS_PER_US) / (NS_PER_CYCLE * CYCLES_PER_LOOP)

; Tuple definitions.

CISTPL_VERS_1	equ		15h

; Socket number.

SOCKET_NUM	equ	1	; 100LX only has one socket and its num is 1

; Max size of a tuple that we are interested in reading.  This is kept small
; here since this is a TSR after all.

MAX_TUPLE_SIZE	equ	32

; Install check function number for the multiplex interrupt.

INSTALLCHECK	EQU	00h

;----------------------------------------------------------------------------
        .code

        org     100h

entry_point:
        jmp	init_code

;----------------------------------------------------------------------------
;----	Data declarations.

;----	Request packet for registering as a Card Drive BIOS client.
;----	Initialize the packet length and the function number here.  The
;----	ptr to the client structure is initialized in the code.

registerPkt	requestReg { \
			TYPE requestReg,	; rlength is packet length
			registerClient,		; function is function number
			}			; clientStructPtr init'd in code

;----	Client data structure.

client	clientStruct { \
	      	,		; clientLink is set by CardBIOS
		,		; clientEntry is init'd in code
		0,		; clientEraseQueuePtr of 0 means we won't be erasing
		EVENT_MASK,	; clientEventMask indicated callback events of interest
		1 }		; clientAttributes (CardBIOS only knows about memory cards)

;----	Request packet for the Get First and Get Next Tuple functions.

rTuple		requestTuple { \
 	      		TYPE requestTuple }	; packet length

;----	Request packet for writing to the card through CardBIOS.

WritePkt	requestRdWrt { \
			TYPE requestRdWrt }	; packet length

;----	Buffer to hold tuple data.

tupleBuffer	db	MAX_TUPLE_SIZE dup (?)

;----	Value to be written to the Configuration option register to
;----	put the Megahertz card into IO mode.

cor_value	db	61h

;----	Flag to indicate whether one of our cards is currently inserted.

ourCardFlag	db	0	; 0 => not ours, 1 => ours

;----	Handle for use by Int 2F handler.

tsr_handle	db	0

;----	Storage for addr of previous multiplex interrupt handler.

oldint2f	dd	0


;----	TSR identification string

CICDemoId	db      "CIC_Demo_Id",0
CIC_DEMO_ID_LEN	equ	$-CICDemoId

;----------------------------------------------------------------------------
;----	TSR code which remains when we become a TSR.
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; This is the CardBIOS event callback handler.  Once registered, a client
; is notified after its initial registration and any time an enabled event
; occurs on any socket.  On the 100LX family, there is only one socket so
; this will always be socket 0.
;
; A callback handler must preserve all registers.  Any event can be
; rejected by returning with carry set; this will cause the event not to
; be done at this time and cause it to be retried on a subsequent timer
; tick.  To accept an event, return carry cleared.
;
; In general, a callback handler is called with
;
;	ax     event code
;	es:bx  far ptr to event struct, if relevant
;	cx     socket the event occurred on (ch will be zero)
;     	dx     return code, if relevant
;
; and returns
;
;    	Carry clear to accept event
;	Carry set to reject the event until a subsequent timer tick
;
; In our case the event mask is clientEvent_CardInsert OR eventAPMResume,
; which causes the  following events to be reported in the following
; situations:
;
; Case 1: Client registration with no card inserted.
;
;         Events:   REGISTRATION_COMPLETE  
;
; Case 2: Client registration with a card inserted.
;
;         Events:   REGISTRATION_COMPLETE  
;                   CARD_INSERTION
;
; Case 3: Hot card insertion. (Remember that this is a no-no on 100LX family.)
;
;         Events:   CARD_INSERTION
;
; Case 4: Hot card removal. (Remember that this is a no-no on 100LX family.)
;
;         Events:   CARD_REMOVAL
;
; Case 5: Off then On with no card inserted ever.
;
;         Events:   (none)
;
; Case 6: Off and then On with card contiuously inserted.
;
;         Events:   APM_SUSPEND (called before turning off)
;                   APM_RESUMING (called after turning back on)
;                   APM_RESUME (called after turning back on)
;
; Case 7: Off with card inserted, remove card, and then On.
;
;         Events:   APM_SUSPEND (called before turning off)
;                   APM_RESUMING (called after turning back on)
;                   CARD_REMOVAL (called after turning back on)
;
; Case 8: Off with no card inserted, insert card, and then On.
;
;         Events:   CARD_INSERTION (called after turning back on)
;

cardBIOScallback	proc far

;----	Preserve all registers that we might use.

	pusha
	push	ds
	push	es

;----	Setup DS

	push	cs
	pop	ds

;----	Check for a card insertion event.

	cmp	al, CARD_INSERTION
	jne	callback_not_card_insertion

;----	A new card is present.  Check for a modem card and, if found,
;----	configure for operation as COM2.

	call	new_card
	jmp	callback_exit

callback_not_card_insertion:

;----	Check for a card removal event.

	cmp	ax, CARD_REMOVAL
	jne	callback_not_card_removal

;----	A card has been removed.  If it was one of ours, unconfigure
;----	the socket so it is ready for the next customer.

	call	card_removed
	jmp	callback_exit

callback_not_card_removal:

	cmp	ax, APM_SUSPEND
	jne	callback_not_suspend

;----	About to go to sleep.  Take this opportunity to un-configure the
;----	host side.  The card is about to lose its configuration when it
;----	loses power and we will not get another chance to do this if there
;----	is a different card in the slot when we wake back up.

	call	disable_host_COM2
	jmp	callback_exit

callback_not_suspend:

	cmp	ax, APM_RESUME
	jne	callback_not_resume

;----	The only time we get a resume event is when there was a card present
;----	when we suspended and there is again a card present when we
;----	resumed (may not be the same card, of course).  We process this 
;----	just like a card insertion event since the card will have been 
;----	reset during power on.

	call	new_card
	jmp	callback_exit

callback_not_resume:
callback_exit:

;----	Restore registers and clear carry to accept the event.

	pop	es
	pop	ds
	popa

	clc

	ret

cardBIOScallback	endp

;----------------------------------------------------------------------------
; new_card is called to process an inserted card.  This may be directly
; as the result of getting a CARD_INSERTION event or as the result of
; getting an APM_RESUME when a card was present before suspend.
;
; new_card is card specific.  This one just looks for vendor name
; starting with MEGA to keep this routine very simple.  This demo
; was tested with a Megahertz modem card.  If not a "MEGA" card, then 
; new_card does nothing.
;
; Users will need to change this routine to recognize their card.
;
; If it is a "MEGA" card, new_card writes to the card's configuration
; register to put it in IO mode and uses Socket Services to create
; an appropriate IO window.

new_card	proc

;----	See if it is our card.

	call	identify_card
	or	ax,ax			; our card?
	jnz	new_card_exit		; if not

;----	Configure the card for IO mode, etc.

	call	configure_card

;----	Configure the host side for card being COM2.

	call	enable_host_COM2

;----	Set the flag indicating that we have configured a card.

	mov	ourCardFlag, 1

;----	Two beeps to indicate that we have one of ours.

	mov	ax, 2000
	mov	bx, 50
	call	sound_tone
	mov	ax, 50
	call	sound_delay
	mov	ax, 2000
	mov	bx, 50
	call	sound_tone

new_card_exit:
	ret

new_card	endp

;----------------------------------------------------------------------------
; card_removed is called to process a CARD_REMOVED event.

card_removed	proc

;----	If it was our card that was removed, clean up on the host side.

	cmp	ourCardFlag,0		; our card?
	je	@f			; if not, nothing to do

	call	disable_host_COM2

;----	Indicate that it is not our card any longer.

	mov	ourCardFlag,0

;----	Sound one tone.

	mov	ax, 1000
	mov	bx, 50
	call	sound_tone
@@:
	ret

card_removed	endp

;----------------------------------------------------------------------------
; identify_card examines the CIS to see if the newly inserted card
; is "our" card.
;
; Returns	AX == 0 if it is our card
;		AX != 0 if it is not
;
identify_card	proc

;----	Attempt to get vendor info tuple from card.

	call	getCardVendor
	or	ax,ax			; successful?
	jnz	identify_card_exit	; if not

;----	See if name starts with "MEGA".

	cmp	word ptr tupleBuffer, 'EM'
	jne	identify_card_not_ours
	cmp	word ptr tupleBuffer+2, 'AG'
	jne	identify_card_not_ours
	
	xor	ax,ax			; it's ours
	jmp	identify_card_exit

identify_card_not_ours:
	mov	ax,1

identify_card_exit:
	ret

identify_card	endp

;----------------------------------------------------------------------------
; configure_card writes to the card's configuration and status register
; to put it into IO mode, etc.
;
configure_card	proc

;----	Write a value to the configuration option register.  Note that the
;----	register is at offset 200h in attribute memory but since CardBIOS
;----	is only dealing with even bytes, this address must be divided
;----	by two on input to the write routine when writing to attribute
;----	memory.
	
    	mov	bx, OFFSET WritePkt
	assume	bx:PTR requestRdWrt
	mov	[bx].function, write 
    	mov	[bx].socket, SOCKET_NUM

    ; attribute mem using memory handle 0
    	mov	[bx].memoryType, 0
	mov	[bx].memoryHandle, 0

    ; length to write is sizeof value
	mov	[bx].tLength, 1

    ; specify card address in terms of even bytes
	mov	ax, 100h			; low word of address
	shr	ax, 1				; divide by 2 for CardBIOS
    	mov	WORD PTR [bx].cardOffset, ax
	mov	ax, 0				; high word of address
	shr	ax, 1				; divide by 2 for CardBIOS
    	mov	WORD PTR [bx].cardOffset+2, ax

    ; set ptr to byte to write
	mov	WORD PTR [bx].bufferPtr, offset cor_value
	mov	WORD PTR [bx].bufferPtr+2, cs

	mov	ax, CARD_BIOS_REQUEST
	int	1ah

	ret

configure_card	endp

;----------------------------------------------------------------------------
; enable_host_COM2 configures the host side of the PCMCIA socket in 
; three areas:
;
; 1. Socket is set for IO card operation with the interrupt enabled 
;    and steered to the COM2 interrupt IRQ3.
;
; 2. IO window is enabled for COM2.  Window is 8 bytes long and 
;    located at 2F8h.
;
; 3. BIOS data area for COM2 is filled in with 2F8h.
;
enable_host_COM2	proc

;----	Set socket for IO card operation using IRQ3 (COM2 interrupt).

	mov	ah,setSocket
	xor	al,al			; 0 is adapter number
	mov	bh,SCI_cardDetect	; enable card detect change
	mov	bl,SOCKET_NUM		; socket number is always 1
	mov	ch,01			; Vcc level is 5 volts
	mov	cl,11h			; Vpp levels are 5 volts
	mov	dh,0			; no socket attributes
	mov	dl,0			; no indicators 
	mov	si,2			; IO card
	mov	di,8003h		; enable IRQ3
	int	1ah			; call Socket Services

;----	Set up the first IO window to the card (standard COM2 port).

	mov	ah,setWindow
	xor	al,al			; 0 is adapter number
	mov	bh,9			; first IO window is number 9
	mov	bl,SOCKET_NUM		; socket number is always 1
	mov	cx,8			; IO window size in bytes
	mov	dh,5			; wind attr = IO and enabled
	mov	dl,1			; wait line monitoring
	mov	si,2f8h			; base addr of COM2
	int	1ah			; call Socket Services

;----	Fill in the BIOS data area entry for COM2.

	push	es
	mov	ax, 40h
	mov	es, ax
	mov	word ptr es:[2],2f8h
	pop	es

	ret

enable_host_COM2	endp

;----------------------------------------------------------------------------
; disable_host_COM2 resets the host side of the PCMCIA socket to
; effectively undo what was done by enable_host_COM2.  Specifically, 
;
; 1. Socket is set for memory card operation.
;
; 2. IO window is disabled.
;
; 3. BIOS data area for COM2 is zeroed out.
;
disable_host_COM2	proc

;----	Set socket for generic memory card operation.

	mov	ah,setSocket
	xor	al,al			; 0 is adapter number
	mov	bh,80h			; enable card detect change
	mov	bl,SOCKET_NUM		; socket number is always 1
	mov	ch,01			; Vcc level is 5 volts on
	mov	cl,11h			; Vpp levels are 5 volts
	mov	dh,0			; no socket attributes
	mov	dl,0			; no indicators 
	mov	si,1			; memory card
	int	1ah			; call Socket Services

;----	Disable the first IO window to the card.

	mov	ah,setWindow
	xor	al,al			; 0 is adapter number
	mov	bh,9			; first IO window is number 9
	mov	bl,SOCKET_NUM		; socket number is always 1
	mov	cx,8			; IO window size in bytes
	mov	dh,2			; wind attr = IO and disabled
	mov	dl,1			; wait line monitoring
	mov	si,2f8h			; base addr of COM2
	int	1ah			; call Socket Services

;----	Clear the BIOS data area entry for COM2.

	push	es
	mov	ax, 40h
	mov	es, ax
	mov	word ptr es:[2],0
	pop	es

	ret

disable_host_COM2	endp

;----------------------------------------------------------------------------
;
;	Get card vendor information.
;	Fetches the Level 1 Version/ Product Information Tuple
;	and returns level 1 info.
;
;	input:
;		socket	socket to check
;		vendor	ptr to existing buffer in which data is returned
;		len		length of vendor
;
;	returns:
;		0     success
;		non-0 error code defined by CardBIOS

getCardVendor	proc

;----	Attempt to find tuple containing manufacturer's ID.

    	mov	bx, OFFSET rTuple
	assume	bx:PTR requestTuple
	mov	[bx].function, getFirstTuple 
	mov	[bx].socket, SOCKET_NUM
	mov	[bx].attributes, 0
	mov	[bx].desiredTuple, CISTPL_VERS_1
	mov	ax, CARD_BIOS_REQUEST
	int	1ah
	or	ax, ax
	jnz	getCardVendor_exit	; if tuple not found 


;----	Get contents of tuple into buffer.  This request uses the same
;----	structure as above with a different function number and some
;----	additional information about where to put the data and how
;----	much data we can handle.

	mov	[bx].function, getTupleData
        mov	WORD PTR [bx].bufferPtr, offset tupleBuffer
 	mov	WORD PTR [bx].bufferPtr+2, cs
	mov	[bx].bufferLength, MAX_TUPLE_SIZE
	mov	[bx].tupleOffset, 4		; skip over code, link, & ver
	mov	ax, CARD_BIOS_REQUEST
	int	1ah

getCardVendor_exit:

	ret

getCardVendor	endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; This section just contains routines to support beeps.

sound_tone	proc

; Makes a tone of a given frequency and a given length.
;
; Input: AX = frequency, BX = length in milliseconds
;
	call	sound_frequency
	call	sound_toneset
	call	sound_toneon

	mov	ax, bx
	call	sound_delay

	call	sound_toneoff
	ret

sound_tone	endp

sound_toneon	proc

; Turns on timer2 and the speaker to produce a tone.

	in	al, 61h
	or	al, 3
	out	61h, al
	ret

sound_toneon	endp

sound_toneoff	proc

; Turns off the timer and the speaker.

	in	al, 61h
	and	al, not 3
	out	61h, al
	ret

sound_toneoff	endp

sound_toneset	proc

; Sets speaker frequncy by setting timer2.
;
; Input: AX is frequency

	out	42h, al
	mov	al, ah
	out	42h, al
	ret

sound_toneset	endp

sound_frequency	proc

; 
;
; Input: AX
;
; Output: AX

	mov	cx, ax
	mov	dx, 12h
	mov	ax, 34deh
	div	cx
	ret

sound_frequency	endp

sound_delay	proc

; Delay for specified number of milliseconds
;
; Input: AX is number of milliseconds to delay.

	mov	cx, ax

sound_delay_loop:			; outer loop on milliseconds
	push	cx

	mov	cx, CX_FOR_1MS
	loop	$

	pop	cx
	loop	sound_delay_loop

	ret

sound_delay	endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; This section contains our Int 2fh handler which helps out with TSR stuff.

;----------------------------------------------------------------------------
; If called with AH == our handle and AL == installation check, returns
; with CX:BX -> identification string.
  
newint2f	proc	far

	cmp	ah, cs:[tsr_handle]
	jne	@f
	cmp	al, INSTALLCHECK
	jne	@f

;----	Install check with our handle, return pointer to our id string
;----	which also tells our segment.

	mov	cx, cs
 	mov	bx, offset CICDemoId
        mov	al, 0ffh		; indicate in use

	iret

@@:
        jmp cs:[oldint2f]

newint2f	endp

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
;----	Initialization code which is discarded when we become a TSR.

;----	Since this is a .COM pgm, ds=es=ss=cs -> PSP.

init_code	proc

	LOCAL	tsr_seg:word

;----	Print startup message.

        mov	ah,9
        mov	dx, offset cicstartupmsg
        int	21h

;----	Just for safety sake, make sure we are on a 100LX family machine.

	mov	dx, offset not100lx_msg
	mov	ax,4dd4h
	int	15h
	cmp	bx,'HP'	       		; HP machine?
	jne	init_msg_exit		; if not
	cmp	ch,1			; palmtop?
	jne	init_msg_exit		; if not
	cmp	cl,2			; later than the 95LX?
	jb	init_msg_exit		; if not

;----	Check to see if we are already loaded and if not at least 
;----	find out a valid handle to use for our Int 2fh handler. 

	lea	ax,tsr_seg
	push	ax
	mov	ax,CIC_DEMO_ID_LEN
	push	ax
	push	cs
	mov	ax,offset CICDemoId
	push	ax
	call	FindTSR
	add	sp,8
	mov	tsr_handle,al

;----	Check the command line for a U indicating uninstall.

        mov     di,81h
init_SkipSpace:
        cmp     byte ptr [di],020h
        jne     init_DoneSkipping
	inc di
	jmp     init_SkipSpace

init_DoneSkipping:
        mov     al,byte ptr [di]
        cmp     al,'u'
        je      init_want_uninstall
        cmp     al,'U'
        je      init_want_uninstall
	jmp	init_want_load

init_want_uninstall:

;----	Uninstall requested, see if we are loaded.

	cmp	tsr_seg,0		; was TSR found?
	jne	@f			; if yes

	mov	dx,offset tsr_not_loaded_msg
	jmp	init_msg_exit
@@:

;----	Is it OK to uninstall or has something else come along and hooked
;----	into the multiplex interrupt?

        mov     ax,352fh                ; Get old Mux
        int     21h
	mov	ax,es			; current segment
	cmp	ax,tsr_seg		; same as our TSR?
	je	@f			; if yes, OK to uninstall

	mov	dx,offset tsr_trapped_msg
	jmp	init_msg_exit

@@:

;----	Proceed with uninstall.  Note that at this point ES points to
;----	the resident TSR code and data.

;----	If one of our cards is currently installed, refuse to uninstall.

	mov	dx,offset tsr_has_card_msg
	cmp	es:[ourCardFlag], 0	; TSR have a card?
	jne	init_msg_exit		; if yes

;----	Initialize the client ptr in the requestPkt struct to point to
;----	the client structure that is resident in the TSR.

    	mov	bx, OFFSET registerPkt
	assume	bx:PTR requestReg
	mov	WORD PTR [bx].clientStructPtr, OFFSET client
	mov	WORD PTR [bx].clientStructPtr+2, es	; NOTE ES!

;----	Set the function and deregister the client taking care that
;----	ES points to us again for a while.

	push	es
	mov	ax,cs
	mov	es,ax
	mov	BYTE PTR [bx].function,deregisterClient
	mov	ax, CARD_BIOS_REQUEST
	int	1ah
	pop	es
	mov	dx,offset deregistration_fail_msg
	jc	init_msg_exit

	mov	dx,offset deregistration_success_msg
        mov	ah,9
        int	21h

;----	We should now be free of callbacks so we can safely uninstall.

	push	ds
        mov	dx,word ptr es:oldint2f[0]      ; Restore the Last Mux interrupt
        mov	ds,word ptr es:oldint2f[2]
        mov	ax,252fh
        int	21h
	pop	ds

        push	es                 ; Save resident segment
        mov	es,es:[2ch]        ; Free the resident environment
        mov	ah,49h
        int	21h
        pop	es

        mov	ah,49h              ; ES is resident segment--what we're freeing
        int	21h

	mov	dx,offset tsr_uninstalled_msg
	jmp	init_msg_exit

;----------------------------------------------------------------------------

init_want_load:

;----	Load requested, check that we are not already loaded.

	mov	dx, offset already_loaded_msg
	cmp	tsr_seg, 0		; already loaded?
	jne	init_msg_exit		; if yes

;====	Register as a Card Drive BIOS client.	

;----	Initialize the client ptr in the requestPkt struct.

    	mov	bx, OFFSET registerPkt
	assume	bx:PTR requestReg
	mov	WORD PTR [bx].clientStructPtr, OFFSET client
	mov	WORD PTR [bx].clientStructPtr+2, ds

;----	Initialize the call back ptr in the client struct.

    	mov	bx, OFFSET client
	assume	bx:PTR clientStruct
	mov	WORD PTR [bx].clientEntry, OFFSET cardBIOScallback
	mov	WORD PTR [bx].clientEntry+2, cs

;----	Do the registration.
    	
	mov	bx, offset registerPkt	; es:bx -> request pkt
	assume	bx:PTR requestReg
	mov	ax, CARD_BIOS_REQUEST
	int	1ah
	mov	dx,offset registration_fail_msg
	jc	init_msg_exit

	mov	dx,offset registration_success_msg
	mov	ah,9
	int	21h

;----	Install our multiplex interrupt hook.

	push	es
        mov     ax,352fh                ; Get old Mux
        int     21h

        mov     word ptr oldint2f,bx	; and save it
        mov     word ptr oldint2f+2,es
	pop	es

        mov     dx,offset newint2f	; replace with new mux
        mov     ah,25h
        int     21h

;----	Become a TSR.

	mov	dx, offset init_code	; don't keep the init code & data
	add	dx, 15			; convert to paras, rounding up
	shr	dx, 1
	shr	dx, 1
	shr	dx, 1
	shr	dx, 1
	mov	ax, 3100h		; go TSR
	int	21h

init_msg_exit:

        mov	ah,9			; print error message
        int	21h

	mov	ax,4c00h		; exit to DOS
	int	21h

init_code	endp

;----------------------------------------------------------------------------
; Data used only during Install/Uninstall and hence does not need to be
; kept in the TSR portion.

tsr_seg		dw	0

cicstartupmsg	db	'CICDemo v1.0 08/08/94',13,10,'$'
not100lx_msg	db	'CICDemo - Needs a 100LX family machine to run.',13,10,'$'
tsr_not_loaded_msg db	'CICDemo - Not loaded so can''t uninstall.',13,10,'$'
tsr_trapped_msg	db	'CICDemo - Can''t unhook int 2fh so can''t uninstall.',13,10,'$'
tsr_uninstalled_msg db	'CICDemo - TSR successfully uninstalled.',13,10,'$'
already_loaded_msg db	'CICDemo - TSR already loaded.',13,10,'$'
registration_fail_msg db 'CICDemo - CardBIOS registration failed.',13,10,'$'
registration_success_msg db 'CICDemo - CardBIOS registration successful.',13,10,'$'
deregistration_fail_msg db 'CICDemo - CardBIOS deregistration failed.',13,10,'$'
tsr_has_card_msg db	'CICDemo - Card active, can''t uninstall.',13,10,'$'
deregistration_success_msg db 'CICDemo - CardBIOS deregistration successful.',13,10,'$'

;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; This section just contains routines to support TSR stuff.


;----------------------------------------------------------------------------
; unsigned int FindTSR(char far * IdString, unsigned int Len, int *pTSRseg);
;
; FindTSR searches for a copy of ourselves being already loaded.  If found,
; it returns with the handle in AX.
;
; Entry:	Far ptr to IdString
;		Length of IdString
;		Near pointer to place to store TSRseg
;
; Exit:		If TSR is already loaded
;			TSRseg == TSR segment
;			AX = int 2fh handle and
;		else (not loaded and a handle is available)
;			TSRseg = 0
;			AX = available handle and
;		else // not loaded and no handle avail
;			TSRseg = 0
;			AX = 0
; 
;----------------------------------------------------------------------------  

FindTSR	proc C uses si di es pIdString:FAR PTR, Len:WORD, pTSRseg:NEAR PTR

        LOCAL   handle:byte
	LOCAL	avail_handle:byte
	LOCAL	handle_seg:word

        mov     handle,0c0h		; initialize handle
	mov	avail_handle,0		; initialize available handle
	mov	handle_seg,0
 
FindTSR_loop:
       	mov	ah,handle		; handle to try
        mov     al,INSTALLCHECK
        int     2fh

        cmp     al,0ffh			; this handle in use?
        je	@f			; if yes

	cmp	avail_handle,0		; have we recorded an avail handle?
	jne	@f			; if yes

	mov	ah,handle      		; else, record this one
	mov	avail_handle,ah

@@:
        mov     es,cx
        mov     di,bx			; es:di -> possible signature
	mov	handle_seg,cx		; possible TSR segment

	push	ds
	lds	si,pIdString
	ASSUME	ds:NOTHING
        mov     si,word ptr pIdString	; ds:si -> desired signature 
        mov     cx,Len	  		; length of desired signature

        repe    cmpsb                   ; strings match ?
	pop	ds
	ASSUME	ds:DGROUP
        je	FindTSR_exit		; if yes, we found it
					; else, keep looking

	inc     handle                  ; next handle, does it wrap to 0?
        jnz     FindTSR_loop		; if not, keep looking else fail

FindTSR_notFound:
        mov     handle_seg,0		; indicate not found
	mov	al,avail_handle		; set up to return avail handle (if any)
	mov	handle,al

FindTSR_exit:
	mov	ax,handle_seg
	mov	bx,pTSRseg
	mov	word ptr [bx],ax 	; return seg/loaded flag

	xor	ah,ah
	mov	al,handle		; return handle in AX
        ret

FindTSR endp



	END	entry_point

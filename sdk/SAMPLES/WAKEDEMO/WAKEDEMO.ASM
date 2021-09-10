;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;
; WAKEDEMO shows how to configure an HP 100LX or 200LX so that a plug-in
; card can cause a wakeup.
;
; The idea is that the card has an independent power supply.  This is
; needed since the card will get no power from the palmtop when the
; palmtop is turned off.
;
; To produce wakeup, the card should hold the BVD1 (STSCHG) signal low
; until it wants to cause a wakeup at which time it should drive the 
; signal high.
;
; The BVD1 signal comes into the palmtop INVERTED on GPIO pin 6.  So, 
; the palmtop should be configured so that GPIO 6 will cause an interrupt
; on a low level.  This configuration is done within an Int 6h handler
; which performs the configuration on the way into deep sleep.
;
; To see that this works, insert a RAM card and run the TSR.  Now try to
; turn the unit off.  It should wake right back up again.  Now
; uninstall the TSR (WAKEDEMO U) and it will turn off again ok.
;
; To build the program use MASM 6.x with the command
;
;   ml wakedemo.asm
;
; See the HP Palmtop developer's guide for overview information.
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
;

        .model	tiny
	.186

;----------------------------------------------------------------------------
; General program structure.
;
; WAKEDEMO is written as a .COM program so the basic situation is that
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

; Install check function number for the multiplex interrupt.

INSTALLCHECK	EQU	00h

;----------------------------------------------------------------------------
        .code

        org     100h

entry_point:
        jmp	init_code

;----------------------------------------------------------------------------
;----	Data declarations.

;----	Original Int 6 handler.

origInt6	dd	0

;----	Handle for use by Int 2F handler.

tsr_handle	db	0

;----	Storage for addr of previous multiplex interrupt handler.

oldint2f	dd	0


;----	TSR identification string

WakeDemoId	db      "Wake_Demo_Id",0
WAKE_DEMO_ID_LEN	equ	$-WakeDemoId

;----------------------------------------------------------------------------
;----	TSR code which remains when we become a TSR.
;----------------------------------------------------------------------------

;----------------------------------------------------------------------------
; This is the Int 6h handler which checks to see if the unit is about to
; go into deep sleep and, if it is, it enables plug-in card wakeup on
; the palmtop.  In a real application, this routine would only enable
; wakeup if the correct card was in the slot and it might also perform
; wakeup configuration of the card.
;
; There is no need to undo the wakeup configuration of the palmtop since
; the BIOS will restore the usual non-wakeup enabled state on power up.
;

newint6 proc far

;----	Preserve all registers that we might use.

	pusha
	push	ds
	push	es

;----	Setup DS.

	push	cs
	pop	ds

;----	If entering deep sleep, enable wakeup on card and palmtop

	or	ah,ah			; entering deep sleep (ah==0) ?
	jnz	newint6_exit		; if not

;----	About to enter deep sleep.

;----	A real application would now check to make sure that their card
;----	is installed.  This might be to just check a RAM variable that
;----	is set/cleared by the card installation client for the card.
;----	See the CICDEMO on this subject.

;----	Here we just pretend that it is our card.

;----	A real application would now probably do something to put the
;----	card into wakeup mode.  This would cause the card to drive the
;----	BVD1 signal low and be ready to drive it high when a wakeup is
;----	desired.

;----	Here we just pretend that this has been done.

;----	Enable wakeup on the palmtop.

    	call	enablePalmtopWakeup

newint6_exit:

;----	Restore registers.

	pop	es
	pop	ds
	popa

;----	Transfer control down int 6 chain.

	jmp	cs:[origInt6]

newint6	endp


;----------------------------------------------------------------------------
;
; Configure GPIO 6 to interrupt on a low level.  This corresponds to
; the BVD1 card signal being driven high by the card and is the
; mechanism by which a card can cause the palmtop to wake up.

enablePalmtopWakeup	proc

	push	ax

	mov	al,043h		; configure GPIO6 as input w/ pullup
	out	022h,al
	in	al,023h
	and	al,0f0h
	or	al,004h
	out	023h,al

	mov	al,055h		; set GPIO6 to interrupt on low level
	out	022h,al
	in	al,023h
	or	al,040h
	out	023h,al

	mov	al,056h		; clear GPIO6 source bit
	out	022h,al
	mov	al,0bfh
	out	023h,al

	mov	al,054h		; enable GPIO6 interrupts
	out	022h,al
	in	al,023h
	or	al,040h
	out	023h,al

	pop	ax

	ret

enablePalmtopWakeup	endp

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
 	mov	bx, offset WakeDemoId
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
        mov	dx, offset wakestartupmsg
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
	mov	ax,WAKE_DEMO_ID_LEN
	push	ax
	push	cs
	mov	ax,offset WakeDemoId
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
;----	into the multiplex or int6 interrupts?

        mov     ax,352fh                ; Get old Mux
        int     21h
	mov	ax,es			; current segment
	cmp	ax,tsr_seg		; same as our TSR?
	je	@f			; if yes, OK so far

	mov	dx,offset tsr_trapped_msg
	jmp	init_msg_exit

@@:

        mov     ax,3506h                ; Get old int 6
        int     21h
	mov	ax,es			; current segment
	cmp	ax,tsr_seg		; same as our TSR?
	je	@f			; if yes, OK to uninstall

	mov	dx,offset tsr_trapped_msg
	jmp	init_msg_exit

@@:

;----	Proceed with uninstall.  Note that at this point ES points to
;----	the resident TSR code and data.

	push	ds
        mov	dx,word ptr es:oldint2f[0]      ; Restore the Last Mux interrupt
        mov	ds,word ptr es:oldint2f[2]
        mov	ax,252fh
        int	21h
	pop	ds

	push	ds
        mov	dx,word ptr es:origInt6[0]      ; Restore the Last int 6 interrupt
        mov	ds,word ptr es:origInt6[2]
        mov	ax,2506h
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

;----	Install our int 6 interrupt hook.

	push	es
        mov     ax,3506h                ; Get old int 6
        int     21h

        mov     word ptr origInt6,bx	; and save it
        mov     word ptr origInt6+2,es
	pop	es

        mov     dx,offset newint6	; replace with new mux
        mov     ah,25h
        int     21h

;----	Print success message.

	mov	dx,offset installation_success_msg
	mov	ah,9
	int	21h

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

wakestartupmsg	db	'WakeDemo v1.0 09/02/94',13,10,'$'
not100lx_msg	db	'WakeDemo - Needs a 100LX family machine to run.',13,10,'$'
tsr_not_loaded_msg db	'WakeDemo - Not loaded so can''t uninstall.',13,10,'$'
tsr_trapped_msg	db	'WakeDemo - Can''t unhook int 2fh so can''t uninstall.',13,10,'$'
tsr_uninstalled_msg db	'WakeDemo - TSR successfully uninstalled.',13,10,'$'
already_loaded_msg db	'WakeDemo - TSR already loaded.',13,10,'$'
installation_success_msg db 'WakeDemo - TSR installation successful.',13,10,'$'

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

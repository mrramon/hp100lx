
COMMENT @
****************************************************************************
*
*	The following is an example of a service handler (or special TSR).
*	The timer tick rupt (int 8) is chained into. This simulates an
*	external triggering event.  The handler invokes a service request
*	for the linked application. The application communicates with
*	the service handler via a common buffer (and optionally through
*	the user entry far pointer).
*
*	The service handler is initially loaded by the linked application
*	via a system manager call ( _m_load_TSR( *filespec) ).
*
* NOTE: service handlers may also be loaded during system manager startup
*
*
*	The service handler must process commands to:
*
*
*		link	-  Associate app with service
*	       unlink	-  App no longer linked to service
*    request complete	-  App has completed service request
*		hook	-  Hook service into system (capture rupts)
*	      unhook	-  Unhook service from system (free rupts)
*	      unload	-  Unhook service and release memory
*
****************************************************************************
END COMMENT @





	include spectsr.MAC
	include interfac.mac
	include event.mac
	include sysdefs.hhh


BREAKPOINT	MACRO
	INT	3
	ENDM


_TEXT	SEGMENT WORD PUBLIC 'CODE'	; Code segment
	ASSUME	cs:_TEXT, ds:_TEXT, ss:_TEXT

start:
	ORG	0			;All special service handlers must
					;be org'd at 0
;Only field to be inited is the offset to the initialization routine
spec_tsr_hdr	S_spec_TSR_hdr<init_code,,,,>




PUBLIC service_ptr
service_ptr	dd	?		;Far ptr to allocated service entry

PUBLIC global_request
global_request	dd	?		;Far ptr to sysmgr request flag



PUBLIC	service_hooked
service_hooked	db	0		;Timer rupt hooked indicator


PUBLIC	 call_count
call_count	dw	0		;Number of times safe call point


PUBLIC timer_rupt
timer_rupt	dd	?		;Original timer rupt vect


PUBLIC timer_count
timer_count	db	0		;Count of ticks since last service


PERIOD		=	10		;Ticks per service request



;******   SAfe call output (not used in this example)	***********
			db	' APP LOADED BACKGROUND TASK    '
TEXT_SIZE	EQU	$ -offset call_count-2



PUBLIC	test_buffer
test_buffer	db	80h dup('?')		;App - service shared buffer



init_code:
	push	cs
	pop	ds
	push	ds
	pop	es

	BREAKPOINT
	push	ds
	mov	ax,offset global_request	;Get service entry & address
	push	ax				;to global request flag

	push	ds
	mov	ax,offset service_ptr		;Location to store far ptr to
	push	ax				;service entry.

	mov	cx,F_M_GET_SERVICE_ENTRY	;Get a service entry from
	push	cx				;sysmgr.
	call	_c_service
	add	sp,10
	cmp	ax,0				;Error?
	jne	error_logic


	les	si,[service_ptr]		;Get pointer to service table

;Set up service structure

;*********  Note in this example, the safe call point is not used ****
				      ;Sysmgr safe callback
	mov	word ptr es:[si].SVC_safe_entry,offset safe_call_point
	mov	word ptr es:[si].SVC_safe_entry+2,cs

;**********************************************************************


				      ;Application entry point
	mov	word ptr es:[si].SVC_user_entry,offset user_entry_point
	mov	word ptr es:[si].SVC_user_entry+2,cs


	mov	es:[si].SVC_user_id  ,'T'	;Give service a name
	mov	es:[si].SVC_user_id+1,'S'
	mov	es:[si].SVC_user_id+2,'R'
	mov	es:[si].SVC_user_id+3,0

	mov	al,ds:[bx].TSR_hdr_parent	;Get parent value.
	mov	es:[si].SVC_app_index,al	;Valid if invoked thru app

	retf					;Return to sysmgr





timer_hook:
	cmp	cs:[timer_count],0		;Have we turned off checks?
	je	@f				;Yes, skip period check
	inc	cs:[timer_count]		;Increment period counter
	cmp	cs:[timer_count],PERIOD 	;Have we reached period?
	jz	time_to_set_sema		;Yes, request service
@@:
	jmp	cs:[timer_rupt] 		;Chain to original rupt

time_to_set_sema:

	push	ds
	push	si

	mov	cs:[timer_count],0		;Turn off timer countdown

	lds	si,cs:[service_ptr]		;Get service entry address
	mov	[si].SVC_request,08		;Set request priority

	;Params passed to app thru event structure
	mov	word ptr [si].SVC_user_ptr,offset test_buffer
	mov	word ptr [si].SVC_user_ptr+2,cs
	xor	[si].SVC_user_type,1		;Set CMD to app

	lds	si,cs:[global_request]		;Get sysmgr global rqst flag
	mov	byte ptr ds:[si],1		;Set rqst in effect

	pop	si
	pop	ds
	jmp	cs:[timer_rupt] 		;Chain to original rupt



error_logic:
	BREAKPOINT



;**************   This example does not use this routine  **************
safe_call_point:

	BREAKPOINT

	push	cs
	pop	ds


	inc	[call_count]
	mov	[timer_count],1

	mov	cx,TEXT_SIZE
	mov	bx,1
	mov	dx,offset call_count+2
	mov	ah,40h
	int	21h
	retf

;**********************************************************************




COMMENT @
**********************************************************************
*********
*********	 Application entry point to special service handler
*********
**********************************************************************
*********

NOTE:
     The commands serviced are required for proper system manager operation


END COMMENT  @





POINTER_PARAM_START	equ	bp+6

user_entry_point:
	BREAKPOINT

	push	bp
	mov     bp,sp
	sub	sp,4


	push	ds
	push	es
	push	si
	push	bx


	les	bx,[POINTER_PARAM_START]
	mov	al,es:[bx]			;Get TCB #

	cmp	al,CMD_SERVICE_COMPLETE		;Service complete?
	jne	@f

	mov	cs:[timer_count],1		;Start another cycle
	jmp	user_exit

@@:
	cmp	al,CMD_SERVICE_LINK		;New app to be linked?
	jne	@f

	mov	al,es:[bx+1]			;Get new TCB to link with
	lds	si,cs:[service_ptr]
	mov	[si].SVC_app_index,al		;Set new link
	mov	cs:[timer_count],1		;Start the cycle
	jmp	user_exit

@@:
	cmp	al,CMD_SERVICE_UNLINK		;New app to be linked?
	jne	@f

	mov	al,es:[bx+1]			;Get new TCB to link with
	lds	si,cs:[service_ptr]
	mov	es:[si].SVC_app_index,0		;Set no link
	mov	cs:[timer_count],0		;Turn off processing
	jmp	user_exit

@@:
	cmp	al,CMD_SERVICE_HOOK		;Establish control?
	jne	@f

	call	hook_timer
	jc	user_bad_result
	jmp	user_exit

@@:
	cmp	al,CMD_SERVICE_UNHOOK		;Remove rupts?
	jne	@f

	call	unhook_timer
	jc	user_bad_result
	jmp	user_exit


@@:
	cmp	al,CMD_SERVICE_UNLOAD		;Remove self completely?
	jne	user_bad_result

	call	unhook_timer			;Another prog chained in!
	jc	user_bad_result

						;Free service entry
	push	cs
	mov	ax,offset service_ptr
	push	ax

	mov	cx,F_M_FREE_SERVICE_ENTRY
	push	cx
	call	_c_service
	add	sp,6
	cmp	ax,0


	push	cs
	pop	ds				;Set ds to self

	mov	ax,offset spec_tsr_hdr.TSR_hdr_init ;Pass ptr to dword
	push	ax
	mov	cx,F_M_FREE_STATIC_BLOCK
	push	cx

extrn	_c_service:near
	call	_c_service
	add	sp,4
	cmp	ax,0
	jne	user_bad_result
	jmp	user_exit


user_bad_result:
	BREAKPOINT
	mov	ax,0ffffh			;Error return
	jmp	user_exitf

user_exit:
	mov	ax,0				;Set ok return
user_exitf:
	pop	bx
	pop	si
	pop	es
	pop	ds

	add	sp,4			; clear auto data
	pop     bp

	retf





;Hook into the timer interrupt vector
PUBLIC	hook_timer
hook_timer	proc
	push	es

	cmp	cs:[service_hooked],1
	je	@f				;Already hooked

	mov	cs:[service_hooked],1


	mov	ax,0
	mov	es,ax

	mov	si,4*8			;Offset to timer rupt

	mov	ax,es:[si]		;Get original rupt
	mov	word ptr cs:[timer_rupt],ax
	mov	ax,es:[si+2]		;Get original rupt
	mov	word ptr cs:[timer_rupt+2],ax

	cli
	mov	es:[si],offset timer_hook
	mov	es:[si+2],cs
	sti

@@:	clc				;Good result
	pop	es
	ret
hook_timer	endp


PUBLIC	unhook_timer
unhook_timer	proc
;Unhook timer interrupt vector
	push	es

	cmp	cs:[service_hooked],0		;Has it been hooked?
	je	get_out				;No

	mov	ax,0
	mov	es,ax

	mov	si,4*8			;Offset to timer rupt

	mov	ax,word ptr cs:[timer_rupt]
	mov	dx,word ptr cs:[timer_rupt+2]


;Check for other prog hooked after we loaded!
	cmp	es:[si],offset timer_hook
	jne	cant_do

	mov	cx,cs
	cmp	es:[si+2],cx
	je	@f

cant_do:
	stc				;Need to unhook other first!
	jmp	unhook_xit

@@:
	cli
	mov	es:[si],ax		;Restore vects
	mov	es:[si+2],dx
	sti

	mov	cs:[service_hooked],0	;Set unhooked

get_out:
	clc				;Set success


unhook_xit:
	pop	es
	ret
unhook_timer	endp







_TEXT	ENDS

	END

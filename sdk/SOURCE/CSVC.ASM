; $Header:   T:/HH/LIB/CSVC/VCS/CSVC.ASV   1.2   09 Jul 1992 17:51:22   KIM  $
;
; $Log:   T:/HH/LIB/CSVC/VCS/CSVC.ASV  $
;  
;     Rev 1.2   09 Jul 1992 17:51:22   KIM
;  Added c_service2... routines for new API scheme (API2).
;  
;     Rev 1.1   28 May 1992 15:35:42   KIM
;  Synchronize with copy in \HH\INCLUDE.
;  
;     Rev 1.3   28 May 1992 14:53:58   KIM
;  .DATA statement added to put IP savearea in Data Seg.
;  
;     Rev 1.2   28 May 1992 14:49:22   KIM
;  Fix a checkin error.
;  
;     Rev 1.1   28 May 1992 14:37:04   KIM
;  Got latest working copy from Peter R., and then added changes for
;  Medium Model support.  * * * NOTE * * * This routine does not
;  support recursive calls.  The current IP saving mechanism should be
;  enhanced to work like a "stack" or we should increase the parameter area
;  offset by a word (to account for CS in Medium Model).
;  
;     Rev 1.0   03 Dec 1991 20:14:36   DAVID
;  Initial revision.
;

IFDEF MMODEL
        .MODEL medium
ELSE
        .MODEL small
ENDIF

	include services.mac

IFDEF MOTOWN
	extrn	_sysmgr_patch:PROC
ENDIF

	.DATA

ARG_ADDR        equ     4
API2_ARG_ADDR   equ     6

wSavePtr        dw      0               ; Save IP stack pointer
wSaveIP         dw      0               ; Save IP stack

	.CODE
;*
;* The following entry points (which are not under IFDEF API2) are
;* for compatibility with old code and call INT 60.  These can be
;* deleted once everyone has converted over to the new INT 7E mechanism.
;*

	PUBLIC _c_service
	PUBLIC _c_service_near_ptr
	PUBLIC _c_service_far_ptr

	PUBLIC _c_service_based_ptr
	PUBLIC _c_service_reg_based
	PUBLIC _c_service_far_mem
	PUBLIC _c_service_long_based
	PUBLIC _c_service_long_far
	PUBLIC _c_service_reg_far
	PUBLIC _c_service_get_avail_mem
	PUBLIC _c_service_free_static
	PUBLIC _c_service_free_static


       IFDEF API2

	PUBLIC _c_service2
	PUBLIC _c_service2_near_ptr
	PUBLIC _c_service2_far_ptr

	PUBLIC _c_service2_based_ptr
	PUBLIC _c_service2_reg_based
	PUBLIC _c_service2_far_mem
	PUBLIC _c_service2_long_based
	PUBLIC _c_service2_long_far
	PUBLIC _c_service2_reg_far
	PUBLIC _c_service2_get_avail_mem
	PUBLIC _c_service2_free_static
	PUBLIC _c_service2_free_static

       ENDIF

;------------------------------------------------------------------------
; c_service (old Jaguar scheme. Uses INT 60)
;
; generic interface for C functions to call certain services (which
; must follow C calling conventions. This OBJ file should be linked with
; all the C programs.
;
; Input:	normal C call with argument list starting at SP+2
; 		Format of argument list:
;
;		[sp]   caller's return address
;		[sp+2] service class in high byte, service number in low byte
;		[sp+4] first byte for the real argument list
;
; An INT is executed to invoke the ServiceDispatch routine in kernel
;
;------------------------------------------------------------------------
_c_service_based_ptr     LABEL PROC
_c_service_reg_based     LABEL PROC
_c_service_reg_far       LABEL PROC
_c_service_near_ptr      LABEL PROC
_c_service_far_ptr       LABEL PROC
_c_service_far_mem       LABEL PROC
_c_service_long_based    LABEL PROC
_c_service_long_far      LABEL PROC
_c_service_get_avail_mem LABEL PROC
_c_service_free_static   LABEL PROC
_c_service               PROC

       IFDEF MMODEL
        pop     wSaveIP                 ; Pop caller's IP off the stack
       ENDIF

	push	bp		        ; Standard stack frame
	mov	bp,sp		        ; ...
	xchg	di,[bp+ARG_ADDR]        ; Swap DI and Class/Func value
	pop	bp		        ; Restore bp

;*------------------------------------------------------------------------
;*	System_Service_Call	        ; ServicesDispatch
;*------------------------------------------------------------------------
       IFDEF MOTOWN
	call	_sysmgr_call
       ELSE
        int     60h                     ; Hardcode the interrupt number
       ENDIF ;MOTOWN

       IFDEF MMODEL
        push    wSaveIP                 ; Push caller's IP back on the stack
       ENDIF

	ret
_c_service ENDP


;------------------------------------------------------------------------
; c_service2 (uses INT 7E)
;
; generic interface for C functions to call certain services (which
; must follow C calling conventions. This OBJ file should be linked with
; all the C programs.
;
; Input:	normal C call with argument list starting at SP+2
; 		Format of argument list:
;
;		[sp]   caller's return address 
;		[sp+2] service class in high byte, service number in low byte
;		[sp+4] first byte for the real argument list
;
; An INT is executed to invoke the ServiceDispatch routine in kernel
;
;------------------------------------------------------------------------
_c_service2_based_ptr     LABEL PROC
_c_service2_reg_based     LABEL PROC
_c_service2_reg_far       LABEL PROC
_c_service2_near_ptr      LABEL PROC
_c_service2_far_ptr       LABEL PROC
_c_service2_far_mem       LABEL PROC
_c_service2_long_based    LABEL PROC
_c_service2_long_far      LABEL PROC
_c_service2_get_avail_mem LABEL PROC
_c_service2_free_static   LABEL PROC
_c_service2               PROC


       IFNDEF MMODEL
        sub     sp,2                    ; In small model, adjust stack
       ENDIF

	push	bp		        ; Save BP
	mov	bp,sp		        ; BP -> current stack offset
	xchg	di,[bp+API2_ARG_ADDR]   ; Swap DI and Class/Func value
        xchg    ax,di                   ; Put Class/Func value in AX
	pop	bp		        ; Restore bp
	System_Service_Call	        ; ServicesDispatch

        push    bp                      ; Save BP
	mov	bp,sp		        ; BP -> current stack offset
	xchg	di,[bp+API2_ARG_ADDR]   ; Restore DI
	pop	bp		        ; Restore bp

       IFNDEF MMODEL
        add     sp,2                    ; In small model, adjust stack
       ENDIF

	ret
_c_service2 ENDP

;------------------------------------------------------------------------
; patch_service_interrupt
;
; This routine should be called by the startup assembly code of each
; application to patch in the correct system manager interrupt service
; number.
;
; input:	dl = interrupt service number
;------------------------------------------------------------------------
	PUBLIC patch_service_interrupt
patch_service_interrupt PROC
IFDEF MOTOWN
	call	_sysmgr_patch
ENDIF
	ret
patch_service_interrupt ENDP

	PUBLIC _break_point
_break_point PROC
IFDEF DEBUG
	push	ax
	int	12h
	pop	ax
ENDIF
	ret
_break_point ENDP


end

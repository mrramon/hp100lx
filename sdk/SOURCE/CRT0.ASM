	title	crt0 - C start up routine

;--------------------------------------------------------------------------
; Morel Accessories Startup Code
; Lotus Development Corp, @ 1990
;
; Adapted from Microsoft code
;
; CRT0.ASM -
;
;	*************  IMPORTANT  *****************************************
;
;	The "DOSSEG" directive in this module must not be removed or else
;	the user will have to link with the "/DOSSEG" linker switch in
;	order to get proper segment ordering.
;
;	See the C documentation for more information about the /DOSSEG switch.
;
;	All assembler modules must be assembled with the /Mx switch, i.e.
;
;		masm -Mx crt0,;
;
;--------------------------------------------------------------------------



;*******************************;*
	DOSSEG			;* specifies DOS SEGment ordering *
;*******************************;*

	.model small


extrn	_main:near

public	__acrtused 		; trick to force in startup
	__acrtused = 9876h	; funny value not easily matched in SYMDEB


	.stack	2048		; should be overridden at link time

	.data?


segpad	db	15 dup (?)	; unused, makes sure that there's space
				; between DATA and _BSS.




	.code

_astart proc near
	jmp	_main		; nothing to do but start.
_astart endp

	end	_astart		

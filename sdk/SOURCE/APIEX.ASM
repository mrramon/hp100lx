;The following is code snippets cut from the source code for KEYBEZ which
;"hooks" into the System Manager dispatch table, so that it can replace
;and/or modify some of the functions built into the System Manager.  Lines
;beginning with ;! (semi-colon and exclamation point) were commented out
;in order to get this code snippet to assemble (even though it SHOULD NOT be
;executed), and were actually NOT commented out in the original KEYBEZ
;source.
;
;This code is provided as-is, in a TOTALLY unsupported fashion.  Do not call
;HP if you have questions, because you won't get any answers (primarily because
;we don't understand how this works either! <g>)  If you don't read Assembly
;Language, we're sorry.  C'est la vie.
;
;(In order to get this to assemble, you'll need to use the /Cp option if using
;the Microsoft Assembler, so that everything won't be uppercased.  For Borland
;TASM, you're on your own (just kidding...use the /ml option).
;
;NOTICE: this took about 20 minutes to rip out of KEYBEZ and "document". There
;is a REAL good chance that something's missing, something's not clear,
;something's wrong, something stinks in Hoboken,.......sorry about that.
;Have fun.

;o
; \/____________________________________________________________________________
; /\
;o
        .model small
        .data
        .186


pwHandle        word    ?                       ; SysMgr Print handle address
pbStatus	word	?			; serial port status
wDummy          word    ?                       ; dummy word used in WPrint
PageMap         byte    19 dup (?)              ; buffer used to save the Page Map

Fth             struct                          ; API2 Function Table Header
  _Num		db	?			; Class number
  _StartAbs	db	?			; Constant start position
  _NextTbl	dd	?			; far pointer to next table
  _CS		dw	?			; code segment of functions in table
  _RamPg	db	?			; ram page
  _RomPg	db	?			; rom page
  _RomDev	db	?			; rom device id
  _Pages	db	?			; pages to map
  _Start	db	?			; first function number in table
  _End		db	?			; last function number in table
Fth		ends				;
FTH		equ	(Fth ptr [si])



fthPrn          Fth     <4Ch,0,0,,80h,0,0,0,3,5>
		word	3 dup (Dispatch_Prn)	; 3 Functions to replace
fthKey          Fth     <53h,0,0,,80h,0,0,0,5,6>
                word    2 dup (Dispatch_Key)    ; 2 Functions to replace
fthCol          Fth     <56h,0,0,,80h,0,0,0,2,7>
                word    6 dup (Dispatch_Col)    ; 6 Functions to replace




        .code

; PrintHandle ()                                ; Locate SysMgr's printer handle
; return:	none				;
; destroy:	all				;
; -----------------------------------------------
PrintHandle	proc				;
                mov     ax, 0100h               ;
                push    cs                      ; Save Page Map
		pop	es			;
		lea	di, PageMap		;
		int	63h			;

		mov	ax, 4001h		;
		int	7eh			;
		mov	ds, dx			; dx:ax -> Function tables
		mov	si, ax			;
		lds	si, [si+4ch*4]		; ds:si -> SC_PRINT Function Table
		mov	ax, ds			;
		cmp	ax, 0f000h		; deal only with the ROM handler
		jb	short Exit		; no way to dig into someone else's code
		xor	ax, ax			;
		mov	bx, ax			;
		mov	cx, ax			;
		mov	dx, ax			;
		mov	al, FTH._RamPg		; load SC_PRINT routines in e800
		mov	bl, FTH._RomPg		;
		mov	cl, FTH._Pages		;
		mov	dl, FTH._RomDev 	;

		test	al, 80h 		; needs Bank switching?
		jnz	@F			;
		int	63h			;
		cmp	ah, 0			; ah must be zero on return
		jne	short Exit		;
	@@:
		mov	ax, FTH._CS		;
		mov	si, [si+ sizeof Fth + 4]; M_WRITE_PRINTER offset
		mov	ds, ax			;
		cmp	word ptr [si+60h], 7ecdh; must be "int 7eh"
		jne	short Exit		;
		mov	ax, [si+58h]		; from	  "lea ax, Handle"
		mov	cs:pwHandle, ax		;
		mov	ax, [si+73h]		; from	  "cmp [status], 0"
		mov	cs:pbStatus, ax		;
		mov	cs:fthPrn._Start, 3	; say we will handle F_M_WRITE_PRINTER
	Exit:
		mov	ax, 0101h		; Restore Page Map
		push	cs			;
		pop	ds			;
		lea	si, PageMap		;
		int	63h			;
		ret				;
PrintHandle	endp				;




page
; HookSysMgr ()					; Hook/UnHook SysMgr's API
; return:	none				; the proc is called from the TimerTick code
; destroy:	all				; and from the Install code
; ----------------------------------------------;
HookSysMgr	proc	far			; Remove and Insert Keybez Function tables
						; ----------------------------------------
		mov	ax, 4003h		; m_remove_fnct_tbl
		push	cs			; harmless if tables were not installed
		call	@F			;

		mov	ax, 4002h		; m_insert_fnct_tbl
	@@:
		lea	si, fthCol		;
		mov	di, ax			;
		mov	dx, 56h 		; Class SC_COL
		mov	bp, sp			;
		push	cs			; bp-2
		push	si			; bp-4
		push	dx			; bp-6
		push	ax			; bp-8
		push	ax			; call far
		push	ax			;
		int	7eh			;
		mov	ax, di			; m_insert/remove_fnct_tbl
	@@:	mov	word ptr [bp-4], offset fthKey
		mov	byte ptr [bp-6], 53h	; Class SC_KEY
		int	7eh			;
		mov	ax, di			; m_insert/remove_fnct_tbl
		mov	word ptr [bp-4], offset fthPrn
		mov	byte ptr [bp-6], 4Ch	; Class SC_PRINT
		int	7eh			;
	Exit:
		mov	sp, bp			;
		ret				;
HookSysMgr	endp				;





page
Dispatch_Prn	proc	far			; 18 16 14 12 10  8  6	4
		pusha				; ax,cx,dx,bx,sp,bp,si,di
		push	ds			;
		push	es			; bp.0
		mov	bp, sp			;
		cmp	al, 04h			;
		ja	short XPrint		; 4c05h
		je	short IPrint		; 4c04h
WPrint: 					; 4c03h
		mov	cx, [bp+46]		; iLen
	Write:	jcxz	Stop			;
		push	cx			;
		mov	cs:wDummy, 1		;
		lea	ax, wDummy		;
		push	cs			; int far *datalen
		push	ax			;
		sub	ax, ax			;
		push	ax			; int option = COM_CTL_WHOLE
		push	[bp+44] 		; char far *data = lpString
		push	[bp+42] 		;
		mov	bx, cs:pwHandle 	; Printer Handle address
		int	7fh			;
		push	[bx]			;
		mov	ax, 4e09h		; ComSendBytes
		mov	di, ax			;
		push	ax			;
		sub	sp, 4			;
		int	7eh			;
		add	sp, 18			;
		pop	cx			;
		cmp	cs:wDummy, 0		; error ?
		je	@F			;
		inc	word ptr [bp+42]	;
		dec	cx			;
	@@:	mov	bx, cs:pbStatus 	;
		cmp	byte ptr [bx], 0	;
		je	short Write		;
	Stop:	mov	ax, [bp+46]		;
		sub	ax, cx			; return bytes actually written
		jmp	short DispatchExit	;
XPrint:
		les	di, [bp+44]		; Destination buffer
		mov	ax, [bp+42]		; al has byte to convert
		stosb				;
		mov	ax, 1			;
		jmp	short DispatchExit	;
IPrint:
		mov	ax, 4004h		; m_disable_fnct_tbl
		mov	dx, 4c04h		; Class SCPRINT, Fcn m_init_printer
		push	dx			;
		push	ax			;
		push	ax			; call far
		push	ax			;
		int	7eh			;
		add	sp, 8			;

		mov	ax, 4c04h		; m_init_printer
		mov	di, ax			;
		push	ax			;
		push	ax			; call far
		push	ax			;
		int	7eh			;
		add	sp, 6			;
		push	ax			; save return value

		mov	ax, 4005h		; m_enable_fnct_tbl
		mov	dx, 4c04h		; Class SCPRINT, Fcn m_init_printer
		push	dx			;
		push	ax			;
		push	ax			; call far
		push	ax			;
		int	7eh			;
		add	sp, 8			;
		pop	ax			;
	DispatchExit::				;
		mov	[bp+18], ax		; return ax
		pop	es			;
		pop	ds			;
		popa				;
		ret				;
Dispatch_Prn	endp				;



Dispatch_Key    proc    far                     ;
;!               mov     bx, 128                 ;
;!               cmp     di, 5305h               ; Pc_Lics : 128 last bytes
;!               je      @F                      ;
;!               xor     bx, bx                  ; Lics_Pc : 256 bytes table
;!       @@:     mov     es, cs:pbLics           ; es:bx will point table upon return
;!               ret                             ;
Dispatch_Key	endp				;



page
Dispatch_Col	proc	far			; 18 16 14 12 10  8  6	4
		pusha				; ax,cx,dx,bx,sp,bp,si,di
		push	ds			;
		push	es			; bp.0
		mov	bp, sp			;
		xchg	ax, di			;
		cmp	al, 03h 		; 2 SC_COL functions get parameters in registers
		je	short LicsStr		; 5603h
		cmp	al, 04h 		;
		je	short LicsChar		; 5604h
		lds	si, [bp+42]		; (void far *) lpStr1
		mov	cx, [bp+46]		; int	       iLen1
		les	di, [bp+48]		; (void far *) lpStr2
		mov	dx, [bp+52]		; int	       iLen2
		cmp	al, 02h 		;
		je	short CpStr		; 5602h
		cmp	al, 06h 		;
		jb	short ToLower		; 5605h
		je	short CpSearch		; 5606h
		ja	short ToUpper		; 5607h
LicsChar:
		mov	ax, cx			; Char1 in ah, Char2 in al
;!              call    Weight                  ;
		jb	short More		;
		je	short Same		;
		jmp	short Less		;
LicsStr:
		push	es			;
		pop	ds			; ds:si -> lpStr1
		mov	es, di			; cx	   iLen1
		mov	di, dx			; es:di -> lpStr2
		mov	dx, bx			; bx	   iLen2
CpStr:
		mov	ax, cx			; if both strings empty
		or	ax, dx			;     return Same
		je	short Same		; if String1 empty
		jcxz	Less			;     return Less
		or	dx, dx			; if String2 empty
		je	short More		;     return More
		lodsb				; Char1 in al
		dec	cx			;
		mov	ah, es:[di]		; Char2 in ah
		inc	di			;
		dec	dx			;
		or	ax, ax			; EndOfString ?
		je	short Same		;
		or	al, al			;
		je	short Less		;
		or	ah, ah			;
		je	short More		;
;!              call    Weight                  ;
		cmp	al, ah			;
		je	short CpStr		; loop while same weight
		ja	short More		;
	Less:	mov	ax, +1			; lpStr1 weight Less than lpStr2
		jmp	short Exit		;
	NoMatch:
	Same:	xor	ax, ax			; lpStr1 weight Same than lpStr2
		jmp	short Exit		;
	More:	mov	ax, -1			; lpStr1 weight More than lpStr2
		jmp	short Exit		;
ToUpper:
;!              mov     es, cs:pbToUp           ;
		jmp	short Case		;
ToLower:
;!              mov     es, cs:pbToLo           ;
	Case:
		jcxz	Exit			;
		mov	bh, 0			;
	@@:	mov	bl, [si]		;
		mov	bl, es:[bx]		; get lower or upper case
		mov	[si], bl		;
		inc	si			;
		loop	@B			;
		jmp	short Exit		;
CpSearch:					; int CpSearch (ds:si lpSearch, cx iSearch,
		jcxz	NoMatch			;		es:di lpTarget, dx iTarget, bDir)
		xchg	cx, dx			;
		sub	cx, dx			; lpTarget must be larger than lpSearch
		jb	short NoMatch		; dx	= iSearch
		inc	cx			; cx	= chars of lpTarget to scan for lpSearch[0]

		lds	si, [bp+48]		; ds:si = lpTarget
		mov	bx, [bp+54]		; bx	= bDir	(-1 or +1)

		cmp	bx, -1			;
		je	@F			;
		add	si, cx			; if Backward, adjust si so
		dec	si			; ds:si -> last possible candidate of lpTarget
		std				;
	@@:
		lodsb				; try to find 1st char of lpSearch in lpTarget
		les	di, [bp+42]		; es:di -> lpSearch[0]
		mov	ah, es:[di]		;
;!              call    Weight                  ;
		cmp	al, ah			;
		je	short FirstMatch	;
	Next:	loop	@B			;
		jmp	short NoMatch		;
	FirstMatch:				;
		push	cx			;
		push	si			;
		add	si, bx			; compensate the last Backward lodsb:
		inc	si			; si += 2 if Backward (bx = +1)
		mov	cx, dx			;
		dec	cx			; cx = chars to match
		je	short Stop		; only one char in lpStr1
	@@:	mov	al, [si]		; we don't use lodsb because of Direction Flag
		inc	si			;
		inc	di			; compare upward other chars of lpStr1
		mov	ah, es:[di]		;
;!              call    Weight                  ;
		cmp	al, ah			;
		loope	@B			;
	Stop:	pop	si			;
		pop	cx			;
		jne	short Next		;
		add	si, bx			; compensate last lodsb
		mov	ax, si			; return index of first/last match
	Exit:					;
		jmp	near ptr DispatchExit	;
Dispatch_Col	endp				;




Begin           proc near

                lea     si, fthPrn              ;
		mov	FTH._Num, 4Ch		; Class number
		mov	FTH._CS, ss		; code segment of functions in table
		mov	FTH._RamPg, 80h 	; no bank switching required
		mov	FTH._Start, 4		; first function number in table
		mov	FTH._End, 5		; last function number in table
		lea	si, fthKey		;
		mov	FTH._Num, 53h		; Class number
		mov	FTH._CS, ss		; code segment of functions in table
		mov	FTH._RamPg, 80h 	; no bank switching required
		mov	FTH._Start, 5		; first function number in table
		mov	FTH._End, 6		; last function number in table
		lea	si, fthCol		;
		mov	FTH._Num, 56h		; Class number
		mov	FTH._CS, ss		; code segment of functions in table
		mov	FTH._RamPg, 80h 	; no bank switching required
		mov	FTH._Start, 2		; first function number in table
		mov	FTH._End, 7		; last function number in table

                lea     ax, Dispatch_Prn        ;
		mov	di, offset fthPrn + sizeof Fth
		stosw				;
		stosw				;
		stosw				;
                lea     ax, Dispatch_Key        ;
                mov     di, offset fthKey + sizeof Fth
                stosw                           ;
                stosw                           ;
                lea     ax, Dispatch_Col        ;
                mov     di, offset fthCol + sizeof Fth
                mov     cx, 6                   ;
                rep     stosw                   ;

                call    HookSysMgr

;                .
;                . more code here to taste, simmer until done
;                .

                mov ax,4c00h
                int 21h

Begin           endp

                end Begin

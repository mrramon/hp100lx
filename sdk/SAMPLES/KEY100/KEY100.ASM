       .MODEL tiny
       .CODE
;       locals @@
       ORG 100h
Begin:
       jmp Main

OldKey  dd ?            ; Int 09h -- keyboard hook
OldMux  dd ?            ; Int 2fh -- multiplex interrupt

LastKey db ?
;=======================================================================

NewMux proc far
       cmp ax,'A'+256*'p'
       jne @@DoOld
       cmp bx,'p'+256*'K'
       jne @@DoOld

       mov bx,'e'+256*'y'
       push cs
       pop es
       iret

@@DoOld:
       jmp cs:[OldMux]
NewMux endp


;=======================================================================

OutputKey proc near
       push ax                 ; Save scancode
       push bx
       push cx
       mov bx,dx               ; Key address
       mov cl,[bx]             ; Get count
       xor ch,ch
       jcxz @@NoMore
@@Loop:
       inc bx
       mov al,[bx]             ; Get next key
       cmp al,80h              ; Is it FN?
       jne @@NotFn
       push es
       mov ax,40h              ; BIOS data area
       mov es,ax
       or byte ptr es:[0F1h],16   ; Set BIOS flag to indicate next key is FN
       or byte ptr es:[0FAh],16   ; Set Keyboard flag for FN active
       and byte ptr es:[0FAh],0dfh ; And clear the FN Clear flag
       mov byte ptr es:[0F8h],79h   ; Put the last key pressed to "Fn"
       pop es
       jmp short @@Continue
@@NotFn:
       out 60h,al              ; "Press" it
       pushf
       call cs:[OldKey]        ; Fake interrupt to old int 9h
@@Continue:
       loop @@Loop
@@NoMore:

       pop cx
       pop bx
       pop ax
       ret
OutputKey endp


;=======================================================================


NewKey proc far
       push ax
       push ds
       push bx
       push cx
       push dx

; First things first--see if the FN key was pressed before coming here--
; if so, make the "last key pressed" the FN key
       mov ax,40h
       mov ds,ax
       mov bx,0f1h
       test byte ptr [bx],16      ; Keyboard Scan Flags (bit 4 is Fn active)
       jz @@NotFnKey

       mov cs:LastKey,80h           ; Fake out Last Key by saying it was Fn
                                    ; (which it was, but not as viewed by int 9)

@@NotFnKey:
       mov ax,cs
       mov ds,ax

       in al,60h

       xor bx,bx
@@StartLoop:
       mov cx,word ptr KeyTable[bx]     ; "From"
       lea dx,KeyTable[bx]+2   ; Address of "to" (starting with count)

       jcxz @@DoneLoop
       add bx,3
       add bl,KeyTable[bx]-1   ; Skip past count
       adc bh,0

       cmp al,ch         ; Is the scan key the one we're looking for?
       jne @@StartLoop
       or cl,cl          ; If the preceeding code is 0, we'll accept any previous key
       jz @@FoundKey
       cmp cl,LastKey    ; Else, previous key must be the same too.
       jne @@StartLoop

@@FoundKey:
       call OutputKey
       push ax
       in al,61h         ; Reset keyboard scan
       or al,80h
       out 61h,al
       and al,7fh
       out 61h,al
       mov al,20h         ; Send EOI to interrupt controller
       out 20h,al
       pop ax
       jmp short @@Exit

@@DoneLoop:
       pushf             ; Can't find the key, so send straight through
       call cs:[OldKey]
@@Exit:
       mov LastKey,al   ; Save the last key pressed (to keep track of Shift/Alt/Ctrl)

       pop dx
       pop cx
       pop bx
       pop ds
       pop ax
       iret
NewKey endp




EndFile label byte      ; Where TSR gets truncated--all above is resident code

;=======================================================================
;
;=======================================================================
;
;=======================================================================


KeyTable label byte     ; This table overwrites the Logon, and Help messages
                        ; during creation.  The size of it is added to EndFile,
                        ; so that it won't be truncated by making a TSR.
;
; EXAMPLE FORMAT OF KEYTABLE ENTRIES
;; cc:mail    ->  #   Ctrl-Dn Phone-Dn   Phone-Up   Ctrl-Up
;db 00h,073h,    4,  1dh,    075h,      0f5h,      9dh,
;
;; Ctrl+Phone ->      Ctrl-Up cc:mail-Dn cc:mail-Up Ctrl-Dn
;db 1dh,075h,    4,  9dh,    073h,      0f3h,      1dh,
;
;; Memo       ->      Ctrl-Dn Memo-Dn    Memo-Up    Ctrl-Up
;db 00h,076h,    4,  1dh,    076h,      0f6h,      9dh,
;
;; Ctrl+Memo  ->      Ctrl-Up Memo-Dn    Memo-Up    Ctrl-Dn
;db 1dh,076h,    4,  9dh,    076h,      0f6h,      1dh,
;
;; End of Table
;db 0,0,0,0


Logon db 13,10
      db 'KEY100 v 1.01 -- HP100LX special keyboard remapper.',13,10
      db '$'

Help  db 13,10
      db 'Usage: KEY100 <filename>.   KEY100 reads macros from <filename>.  Valid names:',13,10
      db 'ÚÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄÂÄÄÄÄ¿',13,10
      db '³Esc ³\   ³F1  ³F2  ³F3  ³F4  ³F5  ³F6  ³F7  ³F8  ³F9  ³F10 ³Up  ³On  ³',13,10
      db 'ÃÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄ´',13,10
      db '³Tab ³File³cc:m³Appt³Phon³Memo³123 ³Calc³More³Back³Del ³Left³Down³Righ³',13,10
      db 'ÃÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÂÄÅÄÄÂÄÅÄÄÄÄÅÄÄÄÄ´',13,10
      db '³ Q  ³ W  ³ E  ³ R  ³ T  ³ Y  ³ U  ³ I  ³ O  ³ P  ³7 ³[³8 ³]³  9 ³  / ³',13,10
      db 'ÀÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÁÄÄÂÄÅÄÄÅÄÅÄÄÁÄÅÄÄÂÄÅÄÄÄÄ´',13,10
      db '   ³ A  ³ S  ³ D  ³ F  ³ G  ³ H  ³ J  ³ K  ³ L  ³ ³4 ³;³  5 ³6 ³',39,'³  * ³',13,10
      db 'ÚÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÂÄÄÁÄÄÄÄÁÄÅÄÄÁÄÅÄÄÄÄÅÄÄÁÄÅÄÄÄÄ´',13,10
      db '³Ctrl³ Z  ³ X  ³ C  ³ V  ³ B  ³ N  ³ M  ³Ente     ³  1 ³  2 ³  3 ³  - ³',13,10
      db 'ÃÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÁÄÄÄÄÁÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÂÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄÅÄÄÄÄ´',13,10
      db '³Shif³ Alt³ Fn ³ Spac         ³ ,  ³ .  ³Menu³RShi³  0 ³  R.³  = ³  + ³',13,10
      db 'ÀÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÁÄÄÄÄÙ',13,10
      db 'AND Scrl,Ins,Home,PgUp,PgDn,End,Prts,Cut,Copy,Past,Date,Time,Zoom,`,Caps.',13,10
      db '    The first 4 letters must be entered, but more may be entered for clarity.',13,10
      db 'The format of each line is:  [opt. LeadingKey] DefineKey : key1 key2 .. keyN',13,10
      db 'Since KEY100 functions at a very low level, all key presses AND releases must',13,10
      db 'be included (preceed a key release with ^).  This includes characters that are',13,10
      db 'normally shifted on a PC--e.g. a : would be Shift ; ^Shift.  Example key file:',13,10
      db '    Ctrl Memo  : ^Ctrl Ctrl ^Ctrl Memo ^Memo        ! Can use comments',13,10
      db '    Memo       : Ctrl Memo ^Memo ^Ctrl              ! after !',13,10
      db 'This swaps Memo & Notetaker keys (keeping Ctrl states in sync).  Put double',13,10
      db 'key macros (like Ctrl Memo) before single.  Should not be loaded before KEYBEZ.$'

Filename  db 64 dup (0)


EndOfKeyMaps label byte

AbortMsg  db 'Bad or missing filename.',13,10,'$'
Deinstall db 'Deinstalled.',13,10,'$'
Installed db 13,10,'Installed.',13,10,'$'

NameTable db 001h,'ESC '
          db 02bh,'\   '
          db 03bh,'F1  '
          db 03ch,'F2  '
          db 03dh,'F3  '
          db 03eh,'F4  '
          db 03fh,'F5  '
          db 040h,'F6  '
          db 041h,'F7  '
          db 042h,'F8  '
          db 043h,'F9  '
          db 044h,'F10 '
          db 048h,'UP  '
          db 070h,'ON  '
          db 00fh,'TAB '
          db 072h,'FILE'
          db 073h,'CC:M'
          db 074h,'APPT'
          db 075h,'PHON'
          db 076h,'MEMO'
          db 077h,'123 '
          db 078h,'CALC'
          db 071h,'MORE'
          db 00eh,'BACK'
          db 053h,'DEL '
          db 04bh,'LEFT'
          db 050h,'DOWN'
          db 04dh,'RIGH'
          db 010h,'Q   '
          db 011h,'W   '
          db 012h,'E   '
          db 013h,'R   '
          db 014h,'T   '
          db 015h,'Y   '
          db 016h,'U   '
          db 017h,'I   '
          db 018h,'O   '
          db 019h,'P   '
          db 008h,'7   '
          db 009h,'8   '
          db 00ah,'9   '
          db 035h,'/   '
          db 01eh,'A   '
          db 01fh,'S   '
          db 020h,'D   '
          db 021h,'F   '
          db 022h,'G   '
          db 023h,'H   '
          db 024h,'J   '
          db 025h,'K   '
          db 026h,'L   '
          db 005h,'4   '
          db 006h,'5   '
          db 007h,'6   '
          db 009h,'*   '
          db 01dh,'CTRL'
          db 02ch,'Z   '
          db 02dh,'X   '
          db 02eh,'C   '
          db 02fh,'V   '
          db 030h,'B   '
          db 031h,'N   '
          db 032h,'M   '
          db 01ch,'ENTE'
          db 002h,'1   '
          db 003h,'2   '
          db 004h,'3   '
          db 00ch,'-   '
          db 02ah,'SHIF'
          db 038h,'ALT '
          db 080h,'FN  '             ; Special key--toggle 40:F1, bit 4
          db 039h,'SPAC'
          db 033h,',   '
          db 034h,'.   '
          db 07ah,'MENU'
          db 000h,'RSHI'
          db 00Bh,'0   '
          db 034h,'R.  '
          db 00dh,'=   '
          db 046h,'SCRL'
          db 052h,'INS '
          db 047h,'HOME'
          db 049h,'PGUP'
          db 051h,'PGDN'
          db 04fh,'END '
          db 037h,'PRTS'
          db 06dh,'CUT '
          db 06eh,'COPY'
          db 06fh,'PAST'
          db 06bh,'DATE'
          db 06ah,'TIME'
          db 069h,'ZOOM'
          db 027h,';   '
          db 01ah,'[   '
          db 01bh,']   '
          db 028h,39,'   '
          db 029h,'`   '
          db 03ah,'CAPS'
          db 000h,':   '
          db 000h,0,0,0,0            ; End of Table
; Other Ideas
; -----------
; Delay
; Recursive Key (or underlying key)
; Screen Off/On
; Beep
; IF Beep
; WaitForParticularKey
; IF OnScreenText
; Goto Label
; Label
; If Not
; Define
; Undefine
; Clipboard paste (keys)

;=======================================================================
LinePrefix db 'Definition $'
DefNumber dw 0
DefLine   db ' being processed.',13,'$'
BytesLeft db ' bytes left for macro definitions.',13,10,'$'
KeyBuffer db 0
BadKey    db 10
KeyName   db 4 dup (' ')
          db ' key name is invalid.',13,10,'$'
KeyShort  db 10,'Line must be of format  [KEY] KEY : KEY1..KEYn',13,10,'$'
KeyLong   db 10,'Line has exceeded maximum definition length.',13,10,'$'
NoRoom    db 10,'Exceeded macro space limits.',13,10,'$'
NumBuffer db '0000$'

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Main proc near
    mov ah,9
    mov dx,offset Logon
    int 21h

    mov ax,'A'+256*'p'
    mov bx,'p'+256*'K'
    int 2fh              ; ES gets set to our old code segment.

    cmp bx,'e'+256*'y'
    jne @@Install

;--------------------------------------

@@Deinstall:                ; Remember: ES is pointing to our old code seg
    mov dx,offset Deinstall
    mov ah,9
    int 21h

    mov dx,word ptr es:OldKey[0]  ; First restore the old key vector
    mov ds,word ptr es:OldKey[2]  ; by grabbing it out of our resident CS
    mov ax,2509h
    int 21h

    mov dx,word ptr es:OldMux[0]  ; restore the old mux vector
    mov ds,word ptr es:OldMux[2]  ; by grabbing it out of our resident CS
    mov ax,252fh
    int 21h

    mov ah,49h           ; es is our old code--that's the segment we free
    int 21h

@@Exit:
    mov ax,4c00h
    int 21h

;-------------------------------------

@@Install:
    mov es,es:[2Ch]      ; Grab our environment segment from the PSP
    mov ah,49h
    int 21h              ; Free that darn environment

    cmp byte ptr cs:[80h],0   ; If there is nothing on cmd line, they don't
    jne @@SkipHelp            ; know how to use it.

    mov dx,offset Help
    mov ah,9
    int 21h

    jmp @@Exit

@@SkipHelp:
    call ParseCmdLine           ; Returns in SI the last byte used by program.

    mov dx,offset Installed      ; Print installed message
    mov ah,9
    int 21h

    mov ax,352fh  ; Get Multiplex -- we hook this to communicate with our resident part during deinstall
    int 21h

    mov word ptr OldMux,bx
    mov word ptr OldMux+2,es

    mov dx,offset NewMux
    mov ah,25h
    int 21h

    mov ax,3509h  ; Get keyboard
    int 21h

    mov word ptr OldKey,bx
    mov word ptr OldKey+2,es

    mov dx,offset NewKey
    mov ah,25h
    int 21h

    mov dx,si
    add dx,15  ; want to round up
    shr dx,1
    shr dx,1
    shr dx,1
    shr dx,1   ; Divide by 16
    mov ah,31h       ; Terminate and stay resident
    int 21h
Main endp



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ParseCmdLine proc near
    mov si,81h          ; ds:si = Start looking at command line (skip the len byte)

@@StripLeading:         ; Remove leading spaces from cmdline
    lodsb
    cmp al,' '
    je @@StripLeading

    dec si              ; back up one so we don't miss the first char

; GET THE FILENAME

    xor bx,bx           ; Start at beginning of filename
@@GetUsername:
    lodsb
    cmp al,' '          ; Space is the separator
    je @@DoneUser

    cmp al,13           ; Are we at end of cmd line?
    je @@DoneUser

@@ContUser:
    cmp bx,64                ; Is file name too long?
    jne @@ShortUser
    jmp @@AbortError

@@ShortUser:
    mov Filename[bx],al     ; Store the char and loop for more
    inc bx
    jmp @@GetUsername

@@DoneUser:
    or bx,bx                ; Was there any file name?
    jnz @@OkUser
    jmp @@AbortError

@@OkUser:
    mov Filename[bx],0      ; Store an EOS after the user name

    mov ax,3d00h            ; Open disk file, read only
    mov dx,offset Filename
    int 21h
    jc @@AbortError

    mov bx,ax               ; Save the handle in bx
    call ParseLines

    mov ah,3eh              ; Close the file!
    int 21h
    ret

@@AbortError:
    mov dx,offset AbortMsg
Abort equ $
    mov ah,9
    int 21h
    mov ax,4c01h
    int 21h
ParseCmdLine endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Returns ax=-1 for bad key,-2 for end of file,-3 for end of line
;  else al=scan code of key.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
GetKey proc near
    push si
    push di
    xor si,si
    xor di,di           ; si is index into KeyName
    mov ax,2020h        ; two spaces

    mov word ptr KeyName,ax   ; Fill KeyName with 4 blanks
    mov word ptr KeyName+2,ax

@@NextChar:
    mov ah,3fh                  ; Read one byte from the file
    mov cx,1
    mov dx,offset KeyBuffer
    int 21h
    or ax,ax
    jz @@NoMore

    mov al,KeyBuffer
    cmp al,'!'           ; Comment
    jne @@NotComment

@@IgnoreComment:
    mov ah,3fh                  ; Read one byte from the file
    mov cx,1
    mov dx,offset KeyBuffer
    int 21h
    or ax,ax
    jz @@NoMore

    cmp KeyBuffer,13
    je @@NextChar
    jmp @@IgnoreComment

@@NotComment:
    cmp al,'^'           ; Break code
    je @@BreakCode
    cmp al,' '           ; Whitespace
    je @@Space
    cmp al,9             ;     "
    je @@Space
    cmp al,13            ;     "
    je @@Space
    cmp al,10            ; End of line
    je @@NewLine

    cmp al,'a'           ; real key
    jb @@NotSpace
    cmp al,'z'
    ja @@NotSpace
    sub al,20h
    jmp short @@NotSpace

@@BreakCode:
    mov si,80h
    jmp @@NextChar
@@Space:
    or di,di            ; If we have a spacing char and no chars yet, keep chugging
    jz @@NextChar
    jmp @@FindKey       ; We hit a spacing char, and we do have some--let's see if it's valid.
@@NotSpace:
    cmp di,4
    je @@NextChar
    mov KeyName[di],al           ; Put the char in KeyName
    inc di
    jmp @@NextChar

@@FindKey:
    mov ax,word ptr KeyName
    mov cx,word ptr KeyName+2
    mov di,offset NameTable-5
@@LookatNext:
    add di,5
    cmp word ptr [di]+1,0       ; Hit end of table and haven't recognized yet
    je @@BadKey
    cmp ax,[di]+1
    jne @@LookatNext
    cmp cx,[di]+3
    jne @@LookatNext

    xor ah,ah
    mov al,[di]
    or ax,si            ; Or in the Break code (80h) if necessary
    jmp @@Exit

@@NoMore:
    mov ax,-2
    jmp short @@Exit
@@BadKey:
    mov ax,-1
    jmp short @@Exit
@@NewLine:
    mov ax,-3
    jmp short @@Exit
@@Exit:
    pop di
    pop si
    ret
GetKey endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Comes in with BX = file handle.
; Returns with SI = pointer to last byte used by the program.
; Trashes ax,bx,cx,dx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; Input:
;       AX = number to convert
;       DS:BX = pointer to end of string to store text in
;       CX = number of digits to convert
;
; Output: None
;
; Registers destroyed: AX, DX
;
PrintAX proc near
        push bx
        push cx
        push si
        mov cx,4                           ;Number of digits to produce
        mov bx,offset NumBuffer+3

        mov     si,10                      ;used to divide by 10
@@ConvertLoop:
        sub     dx,dx                      ;convert AX to doubleword in DX:AX
        div     si                         ;divide number by 10. Remainder is in
                                           ; DX--this is a one-digit decimal
                                           ; number. Number/10 is in AX
        add     dl,'0'                     ;convert remainder to a text
        cmp     cx,4
        je @@DoDigit                       ; Always do at least one digit
        cmp     dl,'0'                     ; (but if a zero, replace with space)
        jnz @@DoDigit
        mov     dl,' '                     ; Use leading spaces
@@DoDigit:
        mov     [bx],dl                    ;put this digit in the string
        dec     bx                         ;point to the location for the
                                           ; next most-significant digit
        loop    @@ConvertLoop                ;do the next digit, if any

        mov ah,9
        mov dx,offset NumBuffer            ; Print the sucker
        int 21h

        pop si
        pop cx
        pop bx
        ret
PrintAX endp



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Comes in with BX = file handle.
; Returns with SI = pointer to last byte used by the program.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ParseLines proc
    mov si,offset KeyTable
@@GetNext:
    mov dx,offset LinePrefix
    mov ah,9
    int 21h

    inc DefNumber
    mov ax,DefNumber
    call PrintAx

    mov dx,offset DefLine
    mov ah,9
    int 21h

@@SkipComment:
    call GetKey
    cmp ax,0            ; ':'
    je @@LineShort
    cmp ax,-1           ; bad key
    je @@ErrorName
    cmp ax,-2           ; end of file
    je @@AllDone
    cmp ax,-3           ; end of line
    je @@SkipComment

@@KeyEntry:
    mov byte ptr [si],0
    mov byte ptr [si]+1,al
    call GetKey
    cmp ax,0           ; ':'
    je @@GetContents
    cmp ax,-1          ; bad key
    je @@ErrorName
    cmp ax,-2          ; end of file
    je @@AllDone
    cmp ax,-3          ; end of line
    je @@AllDone

    mov ah,[si]+1      ; Put the other key first
    mov [si],ah
    mov [si+1],al

    call GetKey        ; The next *must* be a colon.
    cmp ax,0
    jne @@LineShort

@@GetContents:
    add si,2
    mov di,si          ; Save the byte counter in di
    inc si             ; and point to the first defined key
    mov byte ptr [di],0  ; Say that we don't have any keys at first

@@KeyLoop:
    call GetKey
    cmp ax,0           ; ':'
    je @@ErrorName
    cmp ax,-1          ; Bad key
    je @@ErrorName
    cmp ax,-2          ; End of file
    je @@AllDone
    cmp ax,-3          ; End of Line
    je @@GetNext

    mov [si],al
    inc si
    cmp si,offset EndOfKeyMaps
    jae @@OutOfSpace

    inc byte ptr [di]
    cmp byte ptr [di],0
    je @@TooLong
    jmp @@KeyLoop

@@CloseAndAbort:
    mov ah,3eh        ; Close the File!
    int 21h
    jmp Abort
@@OutOfSpace:
    mov dx,offset NoRoom
    jmp @@CloseAndAbort
@@ErrorName:
    mov dx,offset BadKey
    jmp @@CloseAndAbort
@@LineShort:
    mov dx,offset KeyShort
    jmp @@CloseAndAbort
@@TooLong:
    mov dx,offset KeyLong
    jmp @@CloseAndAbort
@@AllDone:
    xor ax,ax          ; The table is ended by 4 bytes of 0
    mov [si],ax
    mov [si]+2,ax
    add si,4           ; Leave SI pointing past the table

    mov ax,offset EndOfKeyMaps
    sub ax,si
    call PrintAX
    mov ah,9
    mov dx,offset BytesLeft
    int 21h
    ret
ParseLines endp




    end Begin

char badstr[]="!!!!!!! NO TSRs !!!!!!!!\n$";

void CheckTSRs(void)
{
  _asm {
        push ds

	xor	ax,ax
	mov	ds,ax

	les	di,ds:[17ch]		; get INT 5F graphics vector

	mov	ax,es
	or	ax,di			; == 0?
        jz      ChkTsr_fail                    ; jif yes, fail

	cmp	byte ptr es:[di],0cfh	; IRET instruction?
        jz      ChkTsr_fail                    ; jif yes, fail

	les	di,ds:[180h]		; get INT 60 sysmgr vector

	mov	ax,es
	or	ax,di
        jz      ChkTsr_fail

	cmp	byte ptr es:[di],0cfh	; IRET?
        jz      ChkTsr_fail

        pop ds
        jmp short ChkTsr_done

ChkTsr_fail:
        pop ds

        mov     dx,offset badstr
	mov	ah,9
	int	21h

	mov	ax,4c01h		; signal bad
	int	21h

ChkTsr_done:
  }
}


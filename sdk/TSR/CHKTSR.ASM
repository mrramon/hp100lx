

cseg	segment para public 'code'
	assume	cs:cseg, ds:cseg
	org	100h
start:
	xor	ax,ax
	mov	ds,ax

	les	di,ds:[17ch]		; get INT 5F graphics vector

	mov	ax,es
	or	ax,di			; == 0?
	jz	fail			; jif yes, fail

	cmp	byte ptr es:[di],0cfh	; IRET instruction?
	jz	fail			; jif yes, fail

	les	di,ds:[180h]		; get INT 60 sysmgr vector

	mov	ax,es
	or	ax,di
	jz	fail

	cmp	byte ptr es:[di],0cfh	; IRET?
	jz	fail

	push	cs
	pop	ds

	lea	dx,okstr
	mov	ah,9
	int	21h

	mov	ax,4c00h		; signal ok
	int	21h

fail:
	push	cs
	pop	ds

	lea	dx,badstr
	mov	ah,9
	int	21h

	mov	ax,4c01h		; signal bad
	int	21h

okstr	db	"TSRs loaded",13,10,'$'
badstr	db	"!!!!!!! NO TSRs !!!!!!!!",13,10,'$'

cseg	ends
	end	start

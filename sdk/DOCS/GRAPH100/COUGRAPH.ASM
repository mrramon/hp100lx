; SCCS Information -- Module %M%  Version %I%  Date %G%  Time %U% */
;************************************************************************
;************************************************************************
;ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
;л Graphics interface module for calling Jaguar graphics from C programs л
;л Copyright 1990 Hewlett Packard Company.  All rights reserved.	 л
;л Author: Everett Kaser  August 14, 1990.				 л
;ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

	.MODEL	LARGE,C

	.CODE
	assume	ds:nothing

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Mode(int BiosVideoMode);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Mode		PROC	BiosVideoMode:word
	mov	ax,BiosVideoMode
	xor	ah,ah
	int	5fh
	ret
G_Mode		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Defaults();
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Defaults	PROC
	mov	ah,14h
	int	5fh
	ret
G_Defaults	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_FillMask(maskptr);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_FillMask	PROC	uses ES DI, maskptr:dword
	les	di,maskptr
	mov	ah,1
	int	5fh
	ret
G_FillMask	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_GetInfo(G_INFO *gp);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_GetInfo	PROC	uses ES DI, gp:dword
	les	di,gp
	mov	ah,2
	int	5fh
	ret
G_GetInfo	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_SetInfo(G_INFO *gp);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_SetInfo	PROC	uses ES DI, gp:dword
	les	di,gp
	mov	ah,13h
	int	5fh
	ret
G_SetInfo	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_LorgA(int x, int y);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_LorgA		PROC	x:word, y:word
	mov	cx,x
	mov	dx,y
	mov	ah,3
	int	5fh
	ret
G_LorgA		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_ClipL(int xmin, int ymin, int xmax, int ymax);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_ClipL		PROC	uses SI DI, xmin:word, ymin:word, xmax:word, ymax:word
	mov	cx,xmin
	mov	dx,ymin
	mov	si,xmax
	mov	di,ymax
	mov	ah,4
	int	5fh
	ret
G_ClipL		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Rect(int x, int y, int fillflag);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Rect		PROC	x:word, y:word, fill:word
	mov	cx,x
	mov	dx,y
	mov	ax,fill
	mov	ah,5
	int	5fh
	ret
G_Rect		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Draw(int x, int y);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Draw		PROC	x:word, y:word
	mov	cx,x
	mov	dx,y
	mov	ah,6
	int	5fh
	ret
G_Draw		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Point(int x, int y);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Point		PROC	x:word, y:word
	mov	cx,x
	mov	dx,y
	mov	ah,7
	int	5fh
	ret
G_Point		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Move(int x, int y);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Move		PROC	x:word, y:word
	mov	cx,x
	mov	dx,y
	mov	ah,8
	int	5fh
	ret
G_Move		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_ColorSel(int color);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_ColorSel	PROC	color:word
	mov	ax,color
	mov	ah,9
	int	5fh
	ret
G_ColorSel	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_RepRule(int rrule);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_RepRule	PROC	rrule:word
	mov	ax,rrule
	mov	ah,0ah
	int	5fh
	ret
G_RepRule	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_LineType(int ltype);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_LineType	PROC	ltype:word
	mov	cx,ltype
	mov	ah,0bh
	int	5fh
	ret
G_LineType	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_PointRead(int x, int y);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_PointRead	PROC	x:word, y:word
	mov	cx,x
	mov	dx,y
	mov	ah,0ch
	int	5fh
	ret
G_PointRead	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_ImageGet(int x1, int y1, int x2, int y2, char far *image);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_ImageGet	PROC	uses ES SI DI, x1:word, y1:word, x2:word, y2:word, image:dword
	mov	cx,x1
	mov	dx,y1
	mov	si,x2
	les	di,image
	mov	bp,y2
	mov	ah,0dh
	int	5fh
	ret
G_ImageGet	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_ImagePut(int x, int y, char far *image, int replacerule);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_ImagePut	PROC	uses ES DI, x:word, y:word, image:dword, reprule:word
	mov	cx,x
	mov	dx,y
	les	di,image
	mov	ax,reprule
	mov	ah,0eh
	int	5fh
	ret
G_ImagePut	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Text(int x, int y, char far *string, rotflag);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Text		PROC	uses DS ES SI DI, x:word, y:word, string:dword, rotflag:word
	mov	ax,rotflag
	mov	cx,x
	mov	dx,y
	les	di,string
	mov	ah,0fh
	int	5fh
	ret
G_Text		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_GetFont(int fontsize);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_GetFont	PROC	fontsize:word
	mov	cx,fontsize
	mov	ah,10h
	int	5fh
	ret
G_GetFont	endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Font(void far *fontptr);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Font		PROC	uses ES DI, fontptr:dword
	les	di,fontptr
	mov	ah,11h
	int	5fh
	ret
G_Font		endp

;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
; G_Scroll(int x, int y, int dir, int distance, int fillcolor);
;лллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

G_Scroll	PROC	uses SI, x:word, y:word, dir:word, distance:word, fillcolor:word
	mov	ax,dir
	mov	di,distance
	mov	cx,x
	mov	dx,y
	mov	si,fillcolor
	mov	ah,12h
	int	5fh
	ret
G_Scroll	endp

@CurSeg ends
	end

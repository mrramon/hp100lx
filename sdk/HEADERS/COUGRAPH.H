

/*€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€
  € Definitions.							€
  €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€*/

#define	G_RESTORE	-1
#define	G_ALPHA		0x07
#define	G_GRAPHICS	0x20	/* HP 95LX compatible graphics mode */
#define	G_95GRAPH	0x20	/* HP 95LX compatible graphics mode */
#define	G_CGAGRAPH	0x06	/* HP 100LX 640x200 graphics mode   */

#define	G_FORCE		0
#define	G_AND		1
#define	G_OR		2
#define	G_XOR		3
#define	G_NOTFORCE	4
#define	G_NOTAND	5
#define	G_NOTOR		6
#define	G_NOTXOR	7
#define	G_FORCETEXT	8

#define	G_OUTLINE	0
#define	G_SOLIDFILL	1
#define	G_PATTERNFILL	2

#define	MINCOLOR	0
#define	MAXCOLOR	1

/*€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€
  € Structures.								€
  €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€*/

typedef	struct g_info {
	unsigned char	vidmode;
	unsigned char	defmode;
	unsigned int	xpixels;
	unsigned int	ypixels;
	int		xloc;
	int		yloc;
	unsigned int	linetype;
	int		rrule;
	unsigned int	color;
	int		xclipmin;
	int		yclipmin;
	int		xclipmax;
	int		yclipmax;
	int		xlorg;
	int		ylorg;
	unsigned char	fillmask[8];
	} G_INFO;

/*€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€
  € Graphics library function definitions.  All x,y locations are rel-	€
  €   ative to the current logical origin unless specified otherwise.	€
  €€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€*/

/**** G_MODE(mode): mode = {G_RESTORE | G_TEXT | G_GRAPHICS}
	changes the display mode to text or graphics		*/
void far cdecl G_Mode(int);

/**** G_DEFAULTS():
	sets PenColor=1, RepRule=G_FORCE, LineType=0xFFFF, FillMask=solid */
void far cdecl G_Defaults();

/**** G_GETINFO(GraphInfoPtr): GraphInfoPtr is a far pointer to a buffer of
	the programmers choosing where the graphics information will be copied.
	See the typedef for the G_INFO structure in this file for the contents
	of the buffer.	*/
G_INFO far * cdecl G_GetInfo(G_INFO far *);

/**** G_SETINFO(GraphInfoPtr): GraphInfoPtr is a far pointer to a buffer of
	the programmers choosing from which the graphics information will be
	copied. See the typedef for the G_INFO structure in this file for the
	contents of the buffer.	*/
void far cdecl G_SetInfo(G_INFO far *);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_COLORSEL(color): color = 0 or 1
	sets the current pen to "color"				*/
void far cdecl G_ColorSel(int);

/**** G_REPRULE(reprule): reprule = {G_FORCE | G_AND | G_OR | G_XOR}
	sets the current replacement rule for all other drawing (except for
	G_ImagePut, which specifies its own replacement rule). */
void far cdecl G_RepRule(unsigned int);

/**** G_LINETYPE(linetype): linetype = a 16-bit image that is repeated while
	drawing lines and G_OUTLINE'd rectangles.  Bits that are 1 cause the
	current pen color to be plotted using the current replacement rule.
	Bits that are 0 are not plotted and leave the display un-modified. */
void far cdecl G_LineType(unsigned int);

/**** G_FILLMASK(buffer): buffer is a far pointer to an 8-byte array which
	specifies the fillmask to use when doing drawings of rectangles
	with a fillflag of G_PATTERNFILL.  Every bit that is a 1 will
	cause a point of the current color to be plotted.  Every bit that is
	a 0 will cause that "point" of the display to be undisturbed. */
void far cdecl G_FillMask(unsigned char far *);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_LORGA(x,y): x,y = -32768 to +32767
	sets the logical origin (0,0) to be located at the absolute screen
	coordinate specified by x,y. */
void far cdecl G_LorgA(int, int);

/**** G_CLIPL(x1, y1, x2, y2): x1,y1,x2,y2 = -32768 to +32767
	sets the current clip boundary to the rectangle whose diagonally
	opposite corners are specified by the absolute screen coordinates
	equal to the x1,y1 and x2,y2 offsets from the current logical origin.*/
void far cdecl G_ClipL(int, int, int, int);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_MOVE(x, y): x,y = -32768 to +32767
	causes the logical pen to be moved to "x,y"		*/
void far cdecl G_Move(int, int);

/**** G_POINT(x,y): x,y = -32768 to +32767
	plots a point of the current pen color with the current replacement
	rule at "x,y" unless "x,y" is outside the current clip limits	*/
void far cdecl G_Point(int, int);

/**** G_POINTREAD(x,y): x,y = -32768 to +32767
	reads the color of the point located at x,y and returns that as the
	value of the function. */
int far cdecl G_PointRead(int, int);

/**** G_DRAW(x,y): x,y = -32768 to +32767
	draws a line of the current pen color with the current replacement rule
	and the current linetype from the current pen location to "x,y";
	only those points lying within the clip limits are actually plotted. */
void far cdecl G_Draw(int, int);

/**** G_RECT(x,y,fillflag): x,y = -32768 to +32767
	fillflag = {G_OUTLINE | G_SOLIDFILL | G_PATTERNFILL}
	draws a rectangle with diagonally opposite corners at the current pen
	location and at "x,y", using the current pen color and replacement
	rule.  The "type" of rectangle drawn is determined by fillflag.
	if fillflag==G_OUTLINE, the outline of a rectangle is drawn, using the
		current linetype.
	if fillflag==G_SOLIDFILL, a solid, filled rectangle of the current
		color is drawn.
	if fillflag==G_PATTERNFILL, a pattern filled rectangle of the current
		color is drawn, using the current fill mask.	*/
void far cdecl G_Rect(int, int, int);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_IMAGEGET(x1, y1, x2, y2, buffer):  x1,y1,x2,y2 = -32768 to +32767
	If both points x1,y1 and x2,y2 are within the current clip boundary,
	the display image bounded by the rectangle whose diagonally opposite
	corners are x1,y1 and x2,y2 is read into the bytes pointed to by
	"buffer". */
void far cdecl G_ImageGet(int, int, int, int, char far *);

/**** NOTE: for G_ImageGet(), the required size of 'buffer' is (on Jaguar):

	8 + ((x2-x1+8)/8) * (y2-y1+1) bytes

 ****/

/**** G_IMAGEPUT(x, y, buffer, reprule): x,y = -32768 to +32767
	reprule = {G_FORCE | G_AND | G_OR | G_XOR | G_NOTFORCE | G_NOTAND |
			G_NOTOR | G_NOTXOR}
	if x,y and the un-specified bottom-right corner of the image are
	within the clip boundary, the image from "buffer" is drawn on the
	display using reprule as the replacement rule.  (For the "NOT" replace-
	ment rules, the image is color-inverted first, then placed on the
	display using the "rest" of the replacement rule.) */
void far cdecl G_ImagePut(int, int, char far *, int);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_GetFont(fontsize): fontsize is an unsigned int specifying the width
	and height of the desired font.  ie, G_GetFont(0x0608) would try to
	return a far ptr to a font with characters 6 pixels wide by 8 high.
	Returns NULL if not found. */
void far * far cdecl G_GetFont(unsigned int);

/**** G_Font(void far *fontptr): sets the current font to the font pointed to
	by fontptr, for use by future G_TEXT calls. */
void far cdecl G_Font(void far *fontptr);

/**** G_TEXT(x,y,buffer,rotflag): x,y = -32768 to +32767
	draws the string pointed to by "buffer" on the display at the specified
	location x,y using the current font. If "rotflag" is 0, the string is
	drawn horizontally, else it's drawn vertically, 90 deg count-clock */
void far cdecl G_Text(int, int, char far *, int);

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

/**** G_Scroll(int x, int y, int dir, int distance, int fillcolor):
	Scrolls the rectangle specified by the current pen location and X,Y
	in the direction specified by DIR, for the number of pixels specified
	by DISTANCE, filling the "emptied" area with pixels of FILLCOLOR */
void far cdecl G_Scroll(int, int, int, int, int);

#define	G_LEFT	0
#define	G_RIGHT	1
#define	G_UP	2
#define	G_DOWN	3

/*‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹*/

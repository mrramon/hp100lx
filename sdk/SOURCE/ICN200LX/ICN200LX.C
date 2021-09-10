#include <ctype.h>
#include <io.h>
#include <dos.h>
#include "cougraph.h"


#define width 44
#define height 32
#define pixw 7
#define pixh 5
#define ileft 5
#define itop 15
#define menuleft 53
#define smallleft 345
#define smalltop 20

int curx=width/2,cury=height/2;
int command=0;
int rectx,recty;
int clipw,cliph;


char bits[width][height];
char clipboard[width][height];

char header[8]={1,0,1,0,44,0,32,0};

char *menu[]={"Freeware 200LX Icon Editor",
              "--------------------------",
              "Q = Quit",
              "L = Load",
              "S = Save",
              "B = Blank Icon",
              "F = Fill Rectangle",
              "E = Erase Rectangle",
              "C = Copy Rectangle",
              "P = Paste Rectangle",
              "I = Invert Rectangle",
              "Use arrows and space to edit",
              "When selecting a rectangle, ",
              "Enter completes, Esc aborts"};


int MY_getch(void)
{
  int ch;

  ch = getch();
  G_Defaults();
  G_Font(G_GetFont(0x0808));

  return ch;
}

void CheckFilename(char *filename)
{
  int i,p=0;

  for (i=0; filename[i]; i++)  if (filename[i]=='.')  p=1;

  if (!p)  strcat(filename,".ICN");
}


void LoadIconFile(char *filename)
{
  int h,i,j;
  unsigned int size;
  char temp[8];

  CheckFilename(filename);

  if (_dos_open(filename,0,&h)==0) {
        _dos_read(h,temp,8,&size);

	/* Verify Header */
	for (i=0; i<8; i++) {
	  if (temp[i]!=header[i])  {
                _dos_close(h);
		putch(7);
		return;
		}
	  }

	for (i=0; i<width; i++)
	  for (j=0; j<height; j++)
		bits[i][j]=0;

	for (j=0; j<height; j++) {
          _dos_read(h,temp,6,&size);

	  for (i=0; i<width; i++) {
		if (temp[i>>3] & (1 << (7-(i&7))))  bits[i][j]=1;
		}
	  }

        _dos_close(h);
	}
  else
	putch(7);
}


void SaveIconFile(char *filename)
{
  int h,i,j;
  char temp[6];
  unsigned size;

  CheckFilename(filename);

  if (_dos_creat(filename,0,&h)==0) {
        _dos_write(h,header,8,&size);

	for (j=0; j<height; j++) {
	  for (i=0; i<6; i++)  temp[i]=0;

	  for (i=0; i<width; i++) {
		if (bits[i][j])  temp[i>>3] |= 1 << (7-(i&7));
		}

          _dos_write(h,temp,6,&size);
	  }

        _dos_close(h);
	}
  else
	putch(7);
}


void ShowMenu(void)
{
  int i,y;

  for (y=0,i=0; i<(sizeof(menu)/sizeof(menu[0])); i++) {
    G_Text((menuleft-1)*8,y,menu[i],0);
    y+=8;
    }
}


void GetRectParms(int *x1,int *y1,int *x2,int *y2)
{
  int t;

  *x1=curx;
  *y1=cury;
  *x2=rectx;
  *y2=recty;

  if (*x1>*x2) {
	t = *x1;
	*x1 = *x2;
	*x2 = t;
	}

  if (*y1>*y2) {
	t = *y1;
	*y1 = *y2;
	*y2 = t;
	}
}



void ShowRect(void)
{
  int x1,y1,x2,y2;

  GetRectParms(&x1,&y1,&x2,&y2);

  G_RepRule(G_XOR);
  G_LineType(0x5555);

  G_ColorSel(1);
  G_Move(ileft+x1*pixw,itop+y1*pixh);
  G_Rect(ileft+x2*pixw+pixw-1,itop+y2*pixh+pixh-1,G_OUTLINE);

  G_LineType(0xFFFF);
  G_RepRule(G_FORCETEXT);
}



void Cursor(int on)
{
  int x=ileft+curx*pixw,y=itop+cury*pixh;
  int x2=x+pixw-1,y2=y+pixh-1;
  int c=bits[curx][cury];

  if (command&&!on)  ShowRect();

  G_ColorSel(c);
  G_Move(x,y);
  G_Rect(x2,y2,G_SOLIDFILL);

  G_Point(smallleft+curx,smalltop+cury);

  G_ColorSel(c ^ on);
  G_Move(x+1,y+1);
  G_Rect(x2-1,y2-1,G_OUTLINE);

  if (command&&on)  ShowRect();
}


void ShowIcon(int x,int y,int pw,int ph,int border)
{
  int i,j,x1,y1;

  G_ColorSel(1);
  G_Move(x-1-border*2,y-1-border);
  G_Rect(x+width*pw+border*2,y+height*ph+border,G_OUTLINE);

  for (i=0; i<width; i++) {
	for (j=0; j<height; j++) {
          G_ColorSel(bits[i][j]);
	  x1 = x+i*pw;
	  y1 = y+j*ph;
          G_Move(x1,y1);
          G_Rect(x1+pw-1,y1+ph-1,G_SOLIDFILL);
	  }
	}
}


void ShowBoth(void)
{
  ShowIcon(ileft,itop,pixw,pixh,0);
  ShowIcon(smallleft,smalltop,1,1,1);
  Cursor(1);
}


void ClearIcon(void)
{
  int i,j;

  for (i=0; i<width; i++)
	for (j=0; j<height; j++)
	  bits[i][j]=0;


  ShowBoth();
}



void CopyRegion(void)
{
  int i,j;
  int x1,y1,x2,y2;

  GetRectParms(&x1,&y1,&x2,&y2);

  clipw = (x2-x1)+1;
  cliph = (y2-y1)+1;

  for (i=0; i<clipw; i++)
	for (j=0; j<cliph; j++)
	  clipboard[i][j] = bits[x1+i][y1+j];

  command=0;
}


void PasteRegion(void)
{
  int i,j;

  if (clipw && cliph) {
	for (i=0; i<clipw; i++)
	  for (j=0; j<cliph; j++)
		bits[(curx+i)%width][(cury+j)%height] = clipboard[i][j];
	}

  ShowBoth();
}



int AskName(char *prompt,char *buffer)
{
  int ret=1;
  int done=0;
  int ch,l;
  char display[80];
  int x,y;

  strcpy(display,prompt);
  strcat(display,"?");
  strcat(display,buffer);
  strcat(display,"_");

  G_RepRule(G_FORCETEXT);
  G_ColorSel(1);
  x = (menuleft-1)*8;
  y = 17*8;
  G_Text(x,y,display,0);
  x+=8*strlen(display);

  do {
        ch = MY_getch();
	if (ch) {
	  switch (ch) {
		case 27:
                  ret=0; done=1;
		  break;
		case 13:
		  done=1; break;
		case 8:
		  l = strlen(buffer);
		  if (l) {
			buffer[--l]=0;
                        G_Text(x-8*2,y,"_ ",0);
                        x-=8;
			}
		  break;
		default:
		  l = strlen(buffer);
		  if (l<21) {
                        buffer[l]=ch;
                        buffer[l+1]=0;
                        G_Text(x-8,y,buffer+l,0);
                        G_Text(x,y,"_",0);
                        x+=8;
                        l++;
			}
		  break;
		}
	  }
	else
          ch = MY_getch();
  } while (!done);

  x = (menuleft-1)*8;
  y = 17*8;
  G_Text(x,y,"                           ",0);

  return ret;
}




void FillRegion(char c)
{
  int i,j;
  int x1,y1,x2,y2;

  command=0;
  GetRectParms(&x1,&y1,&x2,&y2);

  for (i=x1; i<=x2; i++)
	for (j=y1; j<=y2; j++)
          if (c==2) bits[i][j] ^= 1;
               else bits[i][j] = c;

  ShowBoth();
}



int main(int argc, char *argv[])
{
  int done=0;
  int ch;
  char buffer[80];

  if (argc>1)  {
	strcpy(buffer,argv[1]);
	LoadIconFile(buffer);
	}
  else
	buffer[0]=0;

  G_Mode(G_CGAGRAPH);
  G_Font(G_GetFont(0x0808));

  ShowMenu();
  ShowBoth();

  do {
        ch = MY_getch();
	ch = toupper(ch);

	switch (ch) {
	  case 27:
		Cursor(0);
		command=0;
		Cursor(1);
		break;
	  case 13:
		Cursor(0);
		switch (command) {
		  case 'F':
			FillRegion(1);
			break;
		  case 'C':
			CopyRegion();
			break;
		  case 'E':
			FillRegion(0);
			break;
                  case 'I':
                        FillRegion(2);
                        break;
		  }
		Cursor(1);
		break;

	  case 'L':
		if (AskName("Load",buffer)) {
		  LoadIconFile(buffer);
		  ShowBoth();
		  }
		break;
	  case 'S':
		if (AskName("Save",buffer)) SaveIconFile(buffer);
		break;
	  case 'B':
		ClearIcon();
		break;
	  case 'P':
		PasteRegion();
		break;

          case '7':
                goto HOME;
          case '8':
                goto UP;
          case '9':
                goto PGUP;
          case '4':
                goto LEFT;
          case '6':
                goto RIGHT;
          case '1':
                goto END;
          case '2':
                goto DOWN;
          case '3':
                goto PGDN;

	  case 'F':
	  case 'E':
          case 'I':
	  case 'C':
		Cursor(0);
		rectx=curx; recty=cury; command=ch;
		Cursor(1);
		break;
	  case ' ':
		Cursor(0);
		bits[curx][cury] ^= 1;
		Cursor(1);
		break;
	  case 0:
                ch=MY_getch();
		switch (ch) {
UP:
		  case 72:
			Cursor(0);
			if (--cury<0)  cury=height-1;
			Cursor(1);
			break;
LEFT:
		  case 75:
			Cursor(0);
			if (--curx<0)  curx=width-1;
			Cursor(1);
			break;
RIGHT:
		  case 77:
			Cursor(0);
			if (++curx>=width)   curx=0;
			Cursor(1);
			break;
DOWN:
		  case 80:
			Cursor(0);
			if (++cury>=height)  cury=0;
			Cursor(1);
			break;
HOME:
		  case 71:
			Cursor(0);
			if (--cury<0)  cury=height-1;
			if (--curx<0)  curx=width-1;
			Cursor(1);
			break;
PGUP:
		  case 73:
			Cursor(0);
			if (--cury<0)  cury=height-1;
			if (++curx>=width)   curx=0;
			Cursor(1);
			break;
END:
		  case 79:
			Cursor(0);
			if (++cury>=height)  cury=0;
			if (--curx<0)  curx=width-1;
			Cursor(1);
			break;
PGDN:
		  case 81:
			Cursor(0);
			if (++cury>=height)  cury=0;
			if (++curx>=width)   curx=0;
			Cursor(1);
			break;
		}
		break;

	  case 'Q':
		done=1;
	  }

  } while (!done);

  G_Mode(G_ALPHA);

  return 0;
}

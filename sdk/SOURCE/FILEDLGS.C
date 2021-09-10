/* SCCS info - Module filedlgs.c  Version 1.6  Date 04/13/93  Time 09:34:14 */
/****************************************************************************
 **                 R E A D   T H I S   N O T E ! ! !
 **==========================================================================
 **  This file must be #included into one of your source files.
 **
 ** BEFORE THIS FILE IS INCLUDED:
 **
 **  * Optionally #define FILECOUNT and DIRCOUNT to the maximum number of
 **    files and directories that will show up in your windows.  If you do
 **    not provide numbers, resonable defaults will be assumed.
 **
 **  * Optionally #define FILEWILDCARD to the wildcard specification that
 **    your program uses (e.g. "*.PBK").  If no specification is provided,
 **    the wildcard defaults to "*.*".
 **
 **  * Create a buffer called CurrentFilename which holds the name of the file
 **    that is currently open.  These routines don't modify this buffer,
 **    just look at it, so you will need to set the buffer on opening
 **    a new file.
 **
 **  * To link any of these dialogs with menu items or function keys,
 **    keys, the following functions create the windows:
 **      CreateFileOpenDialog() -- Pulls up F/O dialog, calls LoadFile with the file
 **      CreateFileSaveAsDialog() -- same as FileCopy, but different msgs.
 **      CreateFileNewDialog() -- Pulls up window prompting for new filename--asks
 **                               to overwrite existing files, calls NewFile if
 **                               user OKs.
 **
 ** AFTER THIS FILE IS INCLUDED:
 **
 **  * Call InitializeFileDialogs() in your startup initialization.
 **
 **  * Provide a means to fix up the far pointers to messages.  These
 **    are listed immediately after this comment block.
 **
 **  * Provide the following functions; the functions in this file will call
 **    routines that you have created to process the user input.
 **   -- void far LoadFile(char *filename, char *password) that will open a
 **                 given file in the format your program uses.  NULL is passed
 **                 as a password if none is required; else the password
 **                 entered by the user is passed (this happens only if
 **                 CreateFileAskPasswordDialog has been called).
 **   -- void far SaveAsFile(char *filename) that will save your current file
 **                 into a given file.
 **   -- void far NewFile(char *filename) that creates an empty file with
 **                 the given filename.
 ****************************************************************************/


/* You will need to fix up these far pointers!!! */
char far *msgFileOpenDBox0  ="Open a File";
char far *msgFileOpenDBox1  ="File to &Open:";
char far *msgFileOpenDBox2  ="&Directory";
char far *msgFileOpenDBox3  ="&Files";
char far *msgFileOpenDBox4  ="&Directories";

char far *msgFileSaveDBox0  ="File Save As";
char far *msgFileSaveDBox1  ="Save to";

char far *msgFileNewDBox0   ="Create a New File";
char far *msgFileNewDBox1   ="New File Name";

char far *msgOK             ="OK";
char far *msgCancel         ="Cancel";

char far *msgReplaceFileDlg0="";
char far *msgReplaceFileDlg1="Replace existing file?";
char far *msgReplaceFileDlg2="OK";
char far *msgReplaceFileDlg3="Cancel";

char far *fkeyCancel         ="Cancel";
char far *fkeyOK             ="OK";








/* Include headers */

#include	"interfac.h"
#include        "lstring.h"





#define CMD_F10KEY (CMD_USER)

#define ERR_NOFILE  1234
#define ERR_NOTOPEN 1235






#define FILELENGTH 13
#define DIRLENGTH 13

#ifndef FILECOUNT
#define FILECOUNT 100
#endif

#ifndef DIRCOUNT
#define DIRCOUNT 50
#endif

#ifndef FILEWILDCARD
#define FILEWILDCARD "*.*"
#endif

#define PW STYLE_PUSHB_WIDTH

char FileSpec[2*MAXNAM];        /* Buffer for File path window */
char Directory[MAXNAM];       /* Buffer for Directory window */
char BackupFileName[2*MAXNAM];  /* Buffer for file to open if LoadFile fails */

int FileOpenDestroyMode;

char *FileWildCard = FILEWILDCARD;

char *FileList[FILECOUNT];
char FileSpace[FILECOUNT*FILELENGTH];

char *DirList[DIRCOUNT];
char DirSpace[DIRCOUNT*DIRLENGTH];

extern LHAPIBLOCK LhapiBlock;


/*************** Prototypes for user provided functions ******************/
int  far LoadFile(char *filename);
void far SaveAsFile(char *filename);
void far NewFile(char *filename);
/*************************************************************************/



/* Functions executed on F10 keypress */
void far DoOpen(void);
void far DoSaveAs(void);
BOOL AddExtension(void);



typedef void _far REPFUNC (char *);






/**** Function key structure for all Dialog softkeys ****/
#define PATCH_F10 1

FKEY FileDlgFKeys[] = {
 { &fkeyCancel,(PFUNC)CMD_ESC, 9, FKEY_SENDMSG },
 { &fkeyOK,(PFUNC)CMD_F10KEY, 10+LAST_FKEY, FKEY_SENDMSG },
};

FKEY FileDlg2FKeys[] = {
 { &fkeyCancel,(PFUNC)CMD_ESC, 9, FKEY_SENDMSG },
 { &fkeyOK, DoOpen,    10+LAST_FKEY,0 }
};


PFUNC DoitFunction;
REPFUNC *ReplacementFunction;



/***************************************************************************
 ** Open a file                                                           **
 ***************************************************************************/

#ifndef OneTrueFileDlgHandler
int far OneTrueFileDlgHandler(PLHWINDOW Window, WORD Message, WORD Data, WORD Extra);
#endif


/*** FileOpenDBox Dialog Array ***/
WINDOW FileOpenDBoxArray[]={
  {Edit,34,33,27,1,			/* 1 = File to &Open */
   &msgFileOpenDBox1,FileSpec,
   64,STYLE_WHCHAR|EDIT_INSERT,NULL,PARENT_FKEYS,NO_MENU,NO_HELP},
  {PathText,34,55,31,1,			/* 2 = Directory */
   &msgFileOpenDBox2,Directory,
   64,STYLE_NOBORDER|STYLE_WHCHAR,NULL,PARENT_FKEYS,NO_MENU,NO_HELP},
  {FileListBox,34,74,17,7,			/* 3 = &Files */
   &msgFileOpenDBox3,(PSTR)FileList,
   countof(FileList),0|STYLE_WHCHAR,NULL,PARENT_FKEYS,(PMENU)countof(FileList),NO_HELP},
  {DirListBox,257,74,17,7,			/* 4 = &Directories */
   &msgFileOpenDBox4,(PSTR)DirList,
   countof(DirList),0|STYLE_WHCHAR,NULL,PARENT_FKEYS,(PMENU)countof(DirList),NO_HELP},
  {PushButton,482,37,40,20,			/* 5 = OK */
   &msgOK,(PSTR)CMD_DONE,ENTERKEY,1|PUSHB_SENDMSG|STYLE_PUSHBUTTON,0,0,0,NO_HELP},
  {PushButton,482,77,80,20,			/* 6 = Cancel */
   &msgCancel,(PSTR)CMD_ESC,0,0|PUSHB_SENDMSG|STYLE_PUSHBUTTON,0,0,0,NO_HELP}
};

/*** FileOpenDBox Dialog Window ***/
WINDOW FileOpenDBox={OneTrueFileDlgHandler,
       20,16,569,165,
       &msgFileOpenDBox0,(PSTR)FileOpenDBoxArray,
       countof(FileOpenDBoxArray),STYLE_PUSHB_WIDTH|FILEOPEN_NODEFAULT,
       NULL,FileDlgFKeys,NO_MENU,NO_HELP};




extern FKEY FileDlg2FKeys[];

/*** ReplaceFileDlg Dialog Array ***/
WINDOW ReplaceFileDlgArray[]={
  {StaticText,198,78,1,1,			/* 1 = Replace existing file? */
   &msgReplaceFileDlg1,NULL,
   1,STYLE_NOBORDER|STYLE_WHCHAR,NULL,PARENT_FKEYS,NO_MENU,NO_HELP},
  {PushButton,210,106,40,20,			/* 2 = OK */
   &msgReplaceFileDlg2,(PSTR) CMD_DONE,0,PUSHBUTTON_DEFAULT|PUSHB_SENDMSG|STYLE_PUSHBUTTON,0,0,0,NO_HELP},
  {PushButton,328,106,80,20,			/* 3 = Cancel */
   &msgReplaceFileDlg3,(PSTR) CMD_ESC,0,0|PUSHB_SENDMSG|STYLE_PUSHBUTTON,0,0,0,NO_HELP}
};

/*** ReplaceFileDlg Dialog Window ***/
WINDOW ReplaceFileDlg={DialogBox,
       160,50,306,100,
       &msgReplaceFileDlg0,(PSTR)ReplaceFileDlgArray,
       countof(ReplaceFileDlgArray),STYLE_PUSHB_WIDTH,
       NULL,FileDlg2FKeys,NO_MENU,NO_HELP};



BOOL far Exists(char *Name)
{
  _asm {
  mov ah,03dh   ; Open a file
  mov dx,Name   ; Get path name
  mov al,0      ; Read mode
  int 21h
  jc NoFile
File:
  mov bx,ax
  mov ax,04400h     ; Do IOCTL on the handle--fail if a device
  int 21h
  test dl,80h
  jz Exists_NoDev

  mov ah,03Eh   ; Close the file
  int 21h
  jmp short NoFile    ; Set flag to FALSE--Exists will fail for device

Exists_NoDev:
  mov ah,03Eh   ; Close the file
  int 21h
  mov ax,1      ; Set flag to TRUE --Exists for file is TRUE
  jmp short DoneExists

NoFile:
  xor ax,ax     ; Set flag to FALSE
DoneExists:
  }
}



/***** Functions linked to F10 keypresses *****/


void far LoadFileShell(void)
{
  (void far *)ReplacementFunction = (void far *)LoadFileShell;
  switch (LoadFile(FileSpec)) {
  case (-1):
    LoadFile(BackupFileName);
    /* FALL THROUGH */
  case 0:
    SendAllMsg(DRAW,DRAW_ALL,0);
    SendFocusMsg(NOTIFY, NOTIFY_FILEOPEN, 0);
    return;
  }
}


void far DoOpen(void)
{
  if (!Exists(FileSpec)) {
    SetError(ERR_NOFILE);
    return;
  }
  SendMsg(LhapiData.CurrentDialog, DESTROY, 1, 0);
  LoadFileShell();
}

void far DoReplacement(void)
{
  PLHWINDOW Wnd = ReplaceFileDlg.Parent;

  ReplaceFileDlg.Parent = NULL;
  SendMsg(&ReplaceFileDlg, DESTROY, 1, 0);
  LhapiData.FocusWnd = LhapiData.CurrentDialog = Wnd;
  SendMsg(LhapiData.CurrentDialog, DESTROY, FileOpenDestroyMode, 0);
  
  ReplacementFunction(FileSpec);
  if (FileOpenDestroyMode)
    SendAllMsg(DRAW,DRAW_ALL,0);
}


void DontOverwrite(REPFUNC *WhatToDo)
{
  if (lstrcmp(FileSpec, CurrentFilename) == 0) {
    SetError(ERR_NOTOPEN);
    return;
  }
  if (Exists(FileSpec)) {
    ReplacementFunction = WhatToDo;
    FileDlg2FKeys[PATCH_F10].Handler = DoReplacement;
    SendMsg(&ReplaceFileDlg, CREATE, CREATE_FOCUS, 0);
    return;
  }
  
  SendMsg(LhapiData.CurrentDialog, DESTROY, FileOpenDestroyMode, 0);
  WhatToDo(FileSpec);
  if (FileOpenDestroyMode)
    SendAllMsg(DRAW,DRAW_ALL,0);
}


void far DoSaveAs(void)
{
  DontOverwrite(SaveAsFile);
}

void far DoNew(void)
{
  DontOverwrite(NewFile);
}


void VariationOnFileOpen(PRES DialogTitle,PRES EditTitle,PFUNC Doit,int Help,int NormalDestroy)
{
  int i;

  FileOpenDestroyMode = !NormalDestroy;
  FileOpenDBox.Title = DialogTitle;
  FileOpenDBox.Help = Help;
  FileOpenDBoxArray[0].Title = EditTitle;
  (char *)FileOpenDBoxArray[0].Menu = FileWildCard;
  i = lstrcpy(Directory, CurrentFilename);
  while (i) {
    if (Directory[i] == '\\')
      break;
    i--;
  }
  if (i == 2)
    i++;
  Directory[i] = 0;

  DoitFunction = Doit;
  FileSpec[0] = 0;
  SendMsg(&FileOpenDBox, CREATE, CREATE_FOCUS, 0);
}



/**** Functions called by outside code ****/



void far CreateFileOpenDialog(void)
{
  VariationOnFileOpen(&msgFileOpenDBox0,&msgFileOpenDBox1,DoOpen,NO_HELP, 0);
}

void far CreateFileSaveAsDialog(void)
{
  VariationOnFileOpen(&msgFileSaveDBox0,&msgFileSaveDBox1,DoSaveAs,NO_HELP, 1);
}

void far CreateFileNewDialog(void)
{
//  FileSpec[0] = 0;   /* Wipe out filename */
  VariationOnFileOpen(&msgFileNewDBox0,&msgFileNewDBox1,DoNew,NO_HELP,0);
}



void InitializeFileDialogs(void)
{
  int i;

  /* Set up FileListBox and DirListBox:
   *  Their data area points to a list of pointers--
   *  Split the block of data for FileSpace and DirSpace into little
   *  string areas, and set the pointers in their lists to point to each
   *  block.
   */
  for (i=0; i<FILECOUNT; i++)  FileList[i] = FileSpace+i*FILELENGTH;
  for (i=0; i<DIRCOUNT; i++)   DirList[i]  = DirSpace+i*DIRLENGTH;
}



/************************************************************************
 ************************************************************************
  Handlers and handler support functions.
 ************************************************************************
 ************************************************************************/


BOOL AddExtension()
{
  int i=0;
  char buf[200];

/*----------------------------------------------------------------------
* tack on extension if needed
*----------------------------------------------------------------------*/
  i = lstrlen(FileSpec)-1;
  while( FileSpec[i]!='.' && FileSpec[i]!='\\' && FileSpec[i]!=':' && i ) 
    i--;
  if( FileSpec[i]!='.' ) 
    lstrcat(FileSpec, FileWildCard+1);

/*----------------------------------------------------------------------
* tack on path if needed.
* put directory in buf, add '\' to end if missing, then concatenate 
* file name and check for its existence. If everything hunky dory and
* file exists, go ahead and put into FileSpec.
*----------------------------------------------------------------------*/
  if (FileSpec[1] != ':') {
    if (buf[(i = lstrcpy(buf, Directory)) - 1] != '\\')
      *(int *)(buf+i) = '\\'; 
    lstrcat(buf, FileSpec);
    lstrcpy(FileSpec, buf);
  }
  m_qualify_path(buf, FileSpec);
  lstrcpy(FileSpec, buf);
  return TRUE;
}

int far OneTrueFileDlgHandler(PLHWINDOW Wnd,WORD Message,WORD Data,WORD Extra)
{
  if ((Message == COMMAND) && (Data == CMD_F10KEY)) 
    return SubclassMsg(FileOpenDialogBox, Wnd, KEYSTROKE, ENTERKEY,0);
  if ((Message == COMMAND) && (Data == CMD_DONE)) {
    AddExtension();
    SendMsg((PLHWINDOW)Wnd->Data, DRAW, DRAW_CLIENT, 0);
    DoitFunction();
    return TRUE;
  }

  return SubclassMsg(FileOpenDialogBox, Wnd, Message, Data, Extra);
}

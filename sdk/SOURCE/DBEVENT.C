/* Sample C Code of a Database Event Loop */

/* Note: this file is provided only as learning tool and is
   not complete or correct for any given program. As a typing
   aid the programmer will have to make numerous substitutions,
   definitions, and changes. */

/* Some documentation appears in the last part of DB.TXT in DBDT.ZIP
   in the \DOCS directory */

/**************************************************************************
***************************************************************************
*                                                                    
*   M     M      A     IIIII   N     N
*   MM   MM     A A      I     NN    N
*   M M M M    A   A     I     N N   N
*   M  M  M   AAAAAAA    I     N  N  N
*   M     M   A     A    I     N   N N
*   M     M   A     A    I     N    NN
*   M     M   A     A  IIIII   N     N
*
***************************************************************************
***************************************************************************/

/*****************************************************************************
******************************************************************************
*
* Global Data
*
******************************************************************************
******************************************************************************/
EVENT app_event;          /* System manager event struct */
CAPBLOCK CapData;         /* CAP application data block */

/*----------------------------------------------------------------------
* far ptrs to be managed by sysmgr in event that memory moves. They must
* be registered with the sysmgr during initialization. The first 
* MAX_CARD_WNDS are used to handle local titles for card windows, while the
* remainder are for database engine, note, multiline edit in general filter,
* and other miscellaneous uses that involve allocating memory block.
*----------------------------------------------------------------------*/
char far *CardWndTitlePtrs[MAX_CARD_WNDS + 27]; /* sysmgr farptrs */

/*----------------------------------------------------------------------
* Multitudinous other data declarations specific to your program would 
* normally appear here, but are excluded from this example.
*----------------------------------------------------------------------*/

/*****************************************************************************
******************************************************************************
*
* Main entry point and event loop.
*
* Main contains init calls, event loop, error check, and uninit calls.
* This event loop is the ONLY one in the entire application.
*
******************************************************************************
*****************************************************************************/
void main(int argc, char *argv[])
{
  int hotkey, appnum, err;

/*-----------------------------------------------------------
* determine which app is being run
*-----------------------------------------------------------*/
  AppFlavor = PHONE;
  IndexBlock.Done = FALSE;        /* Set terminate flag to FALSE before init */
  Initialize();

/*-----------------------------------------------------------
* loop repeatedly for events until Done goes TRUE.
*-----------------------------------------------------------*/
 eventloopstart:
  while (!IndexBlock.Done) {

/*----------------------------------------------------------------------
* if there is stuff that could be lost from buffered disk i/o, send it
* on through before asking for keystroke.
*----------------------------------------------------------------------*/
    if (Db.flags & DB_SHOULDFLUSH) {   /* protect against warmstart */
      if (DB_FlushFile(&Db) == DB_CLOSE)
	SetError(ERR_WRITE);
    }

/*----------------------------------------------------------------------
* Create error dlg if any errors reported during last event (or init).
* if db is closed and nofilewnd (DummyWnd) has not yet been launched, 
* do it before creating error dlg. If no errors, cruise on by.
*----------------------------------------------------------------------*/
    if (err = GetError()) {                  /* report errors if any */
      SendMsg(&ErrorDlg,CREATE,CREATE_FOCUS,0);
    }

/*----------------------------------------------------------------------
* Get an event (a keystroke or null(timer) event are the usuals.
*----------------------------------------------------------------------*/
  maction:
    app_event.do_event = DO_EVENT;
    m_action (&app_event);         /* Grab system manager event */

    if (DB_CardChanged(&Db)<0)
      goto restart;

    switch (app_event.kind) {      /* Branch on SysMgr event */

    case E_KEY:                    /* users, the source of all troubles */
      if (GetFocus())
	SendFocusMsg(KEYSTROKE, 
		Fix101Key(app_event.data,app_event.scan), 
		app_event.scan);
      else
	goto restart;
      break;
      
    case E_REFRESH:                /* redraw after sysmgr mucks up screen */
      SendAllMsg(DRAW, DRAW_ALL, 0);
      break;

    case E_ACTIV:                  /* reactivate on rtn from other app */
      HourglassOn(-1,-1);
      if (DB_Reactivate(&Db) != DB_OK) {  /* some file badness, restart app */
      restart:
	m_fini();                  /* but just in case, */
	return;                    /* bail out cuz windows can't be operated */
      }
      ReactivateLHAPI(&CapData);   /* things are OK, Hello CAP */
      SyncLHAPIKeyState();
      break;
      
    case E_DEACT:                  /* switch to another app */
      DB_Deactivate(&Db);
      DeactivateLHAPI();
      goto maction;
      break;
      
    case E_TERM:                   /* enforced bailout */
      IndexBlock.Done = TRUE;
      break;
      
    case E_NONE:
      SendMsg(GetFocus(), NULLEVENT, app_event.shifts, app_event.scan);
      HandleClock();                 /* show ticking clock (if enabled) */
      break;
      
    }                     /* end of switch on event type */
  }                       /* end of event loop */

/*-----------------------------------------------------------
* Proof that miracles exist: no reboot this time.
*-----------------------------------------------------------*/
  HourglassOn(-1,-1);

  CloseDbFile(&Db);       /* close current database */
  m_fini();               /* Tell System Manager goodbye */
}

/*****************************************************************************
******************************************************************************
*
*  Initialize CAP, SysMgr, Database, and data structures
*
******************************************************************************
******************************************************************************/
void Initialize(void)
{
  int i, size, err = 0;
  char *titleptr;
  char *bufptr;
  char *pstuffptr;

/*----------------------------------------------------------------------
* Greet all the important local personalities
*----------------------------------------------------------------------*/
  m_init_app(SYSTEM_MANAGER_VERSION);   /* Hello SysMgr */
  InitializeCAP(&CapData);              /* Hello CAP */

/*----------------------------------------------------------------------
* setup far internal far pointers to be maintained by sysmgr. Initialize
* the first block for use by card window titles.
*----------------------------------------------------------------------*/
  HourglassOn(-1,-1);

  titleptr = CardWndTitles;
  m_reg_far(&CardWndTitlePtrs, countof(CardWndTitlePtrs), 4); 
  for (i=0; i<MAX_CARD_WNDS; i++) {
    CardWndTitlePtrs[i] = titleptr;
    titleptr += DB_FIELDNAMELENGTH + 1;
    CardWindow[i].Title = &CardWndTitlePtrs[i];
  }
  
  MultiBuffer = FilterBuffer;
  EscBuffer = FileSpace;
  LineStarts = Scratch2;

/*----------------------------------------------------------------------
* Set fonts, map in the resource file and init resource ptrs.
*----------------------------------------------------------------------*/
  RegisterFont(FONT_NORMAL);       /* be as normal as possible */
  SetMenuFont(FONT_NORMAL);
  SetDefaultFont(FONT_NORMAL);

  map_resource_file("CDBAPP.RI");  /* map in resourced msgs */
  Init_Db_Strings();               /* create access to those msgs */

/*-----------------------------------------------------------
* App specific initialization 
*
* Must set EnvFileName, DfltDataFileName, ResourceFileName,
* DfltSmartClipName1/2, DfltCategory.
*-----------------------------------------------------------*/
  InitAppPhone();
/*----------------------------------------------------------------------
* register the name of the application
* with the system manager for use in case low memory prompting is 
* needed.
*----------------------------------------------------------------------*/
  m_reg_app_name(*IndexBlock.AppName); /* register appname */

/*----------------------------------------------------------------------
* Init file dlgs, empty index window, and no-file window.
*----------------------------------------------------------------------*/
  InitializeFileDialogs(); /* Hello File Dialogs */
  *BackupFileName = 0;     /* No 2nd chance if loadfile fails */

  SendMsg(&TopWnd,CREATE,CREATE_FOCUS,0); /* Create main application window */
  SendMsg(&DummyWnd, CREATE, CREATE_FOCUS, 0);
  HourglassOn(-1,-1);      /* put hourglass up again now that windows up */

/*----------------------------------------------------------------------
* Get name of current data file from environment file, use default file
* name if no environment file exists.
*----------------------------------------------------------------------*/
  if (_dos_open(EnvFileName, O_RDWR, &EnvHandle))
    lstrcpy(FileSpec, DfltDataFileName);
  else {
    _dos_lseek(EnvHandle, 0, SEEK_SET);
    _dos_read(EnvHandle, FileSpec, sizeof(Db.filename), &size);

/*----------------------------------------------------------------------
* get printer setup information. dflts are already in place if read fails
*----------------------------------------------------------------------*/
    i = sizeof(INDEXPRINTBLOCK);
    bufptr = Record;
    _dos_read(EnvHandle, bufptr, i, &size);    /* read print setup info */
    if (size == i) {
      pstuffptr = (char *)&PStuff;
      for (;i;i--) *(pstuffptr++) = *(bufptr++);
    }
  close:
    _dos_close(EnvHandle);
  }
/*----------------------------------------------------------------------
* init the db engine
*----------------------------------------------------------------------*/
  err = DB_Init();
  if (err || GetError()) {
    SetError(ERR_DBINIT);
    HourglassOff();
    return;
  }
 
/*----------------------------------------------------------------------
* if file is not default and does not exist, create no file state 
* instead
*----------------------------------------------------------------------*/
  if ((lstrcmp(FileSpec, DfltDataFileName)) && (!(Exists(FileSpec)))) {
    SetError(ERR_OPEN);
    HourglassOff();
    return;
  }
/*----------------------------------------------------------------------
* open up the datafile
*----------------------------------------------------------------------*/
  LoadFileShell(NULL);
}

/*****************************************************************************
******************************************************************************
*
* Launches app; closes current app and launches again - last ditch
* recovery system when datafile deleted underneath. 
*
******************************************************************************
******************************************************************************/
int RestartApp(int HotKey)
{
  struct task far *TCB = m_get_TCB();
  int i;

  for (i=0; i<TASKCOUNT; i++, TCB++)
    if (HotKey==TCB->t_hotkey) goto launch;
  return -1;
  
launch:
  app_event.e_launch_task = i;                      /* Index of the app we want */
  app_event.e_launch_type = LT_CLOSE_THEN_LAUNCH;   /* Close us out first */
  app_event.do_event = DO_LAUNCH;
  m_action(&app_event);              /* Launch our app */
  return 1;
}

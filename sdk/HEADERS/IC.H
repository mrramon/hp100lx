/***********************************************************************/
/** IC.H  -- Headers for "pretty" interface to intertasking by SysMgr **/
/** Note that only one IC link should be open for any given app.      **/
/***********************************************************************/





/* Gets index of task control block of the passed application. */
/* Returns -1 if the task cannot be found */
int GetTaskNumber(struct task far *TCB_ptr, int HotKey);


/****** APPLICATION SIDE IC ******/

/* Launches the specified application; returns 1 if successful, 0 otherwise */
int ICAppLaunch(int HotKey);

/* Passes a handle to an allocated far memory block to the slave app */
int ICAppPassInfoBlock(void far **handle);

/* Closes down the slave application */
int ICAppClose(void);


/****** SLAVE SIDE IC ******/

/* Returns 1 if app is being launched as a slave, 0 otherwise */
int ICSlaveStatus(void);

/* Links with IC channel, and returns handle to passed block */
void far ** ICSlaveInit(void);

/* Closes slave link */
int ICSlaveClose(void);

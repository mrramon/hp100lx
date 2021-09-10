/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
/****************************************************************
 ****************************************************************
 ****************************************************************
 ***                                                          ***
 ***       iii                                                ***
 ***       iii                                                ***
 ***                                                          ***
 ***                                                          ***
 ***     iiiii       ccccccccc                   ccccccccc    ***
 ***     iiiii       ccccccccc                   ccccccccc    ***
 ***       iii     cccc     cccc               cccc     cccc  ***
 ***       iii     cccc                        cccc           ***
 ***       iii     cccc                        cccc           ***
 ***       iii     cccc     cccc     ...       cccc     cccc  ***
 ***       iii     cccc     cccc     ...       cccc     cccc  ***
 ***     iiiiiii     ccccccccc       ...         ccccccccc    ***
 ***     iiiiiii     ccccccccc       ...         ccccccccc    ***
 ***                                                          ***
 ***                                                          ***
 ****************************************************************
 ****************************************************************
 ****************************************************************/

#undef DISTANCE
#define DISTANCE _far

#include "interfac.h"
#include "event.h"
#include "task.h"
#include "sysdefs.h"

int slaveindex;
EVENT ICEvent;

#ifdef DEBUGON
void event_monitor(EVENT *event, char tag);
void alarm_monitor(int time, char tag);
#endif

int GetTaskNumber(struct task far *TCB_ptr, int HotKey)
{
  int i;

  for (i=0; i<TASKCOUNT; i++, TCB_ptr++) {
    if (HotKey == TCB_ptr->t_hotkey)  
      return i;
  }

  return -1;
}


/* Launches app; returns 0 on failure, 1 on sucess */
int ICAppLaunch(int HotKey)
{
  struct task far *TCB_ptr = m_get_TCB();
  int error;

  /* Try to find our slave */
  slaveindex = GetTaskNumber(TCB_ptr, HotKey);   

  if (slaveindex==-1)  return 0;  /* Nope; can't find it */

  /* If the application isn't loaded, go ahead and launch it. */

  if (TCB_ptr[slaveindex].t_state == PS_CLOSED)  {
    ICEvent.e_launch_task = slaveindex;   /* Index of the app we want */
    ICEvent.e_launch_type = LT_LAUNCH_THEN_RETURN;
    ICEvent.do_event = DO_LAUNCH;
    m_action(&ICEvent);              /* Launch our slave */

#ifdef DEBUGON
event_monitor(&ICEvent, 'D');
#endif

    /* Check to see if the launch has failed or not--any error will do*/
    if (ICEvent.return_status)  return 0;

    /* Now make sure we have returned from slave with a E_SWAP */
    while (ICEvent.kind != E_SWAP) {
      error = ICEvent.kind;
      ICEvent.do_event = DO_EVENT;
      m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'E');
#endif

    }
  }
  return 1;
}


int ICAppPassInfoBlock(void far **handle)
{

  /* Our slave is up and running, now time to exchange info with it */
  ICEvent.e_icdata_ptr = handle;
  ICEvent.e_link_task  = slaveindex;
  ICEvent.do_event = DO_IC_INIT;
  m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'F');
#endif

/* control moves to the slave app until it returns */
  return 1;
}


int ICAppClose(void)
{

  /* Close down the slave app */
  ICEvent.e_link_task = slaveindex;
  ICEvent.do_event = DO_IC_CLOSE;
  m_action( &ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'G');
#endif

  return 1;
}




int ICSlaveStatus(void)
{
  static unsigned char app_status=0xFF;

  /* Only use m_get_run_mode to get the application's status the first time*/
  /* we're called--on subsequent calls, get the first value we returned. */
  /* (The mode changes after initial activation of the slave app.) */

  if (app_status==0xFF)
    app_status = (unsigned char)(m_get_run_mode());

  /* Have we been invoked in background? status will be TASK_STS_RETURN if so */
  return (app_status==TASK_STS_RETURN);
}


void far ** ICSlaveInit(void)
{

  /* Return back to launching app with the SWAP */
  ICEvent.do_event = DO_SWAP;
  m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'H');
#endif

  /* Wait for ic init event -- other messages are thrown away. */
  while (ICEvent.kind != E_IC_INIT) {
    ICEvent.do_event = DO_EVENT;
    m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'I');
#endif

    }

  /* return local far pointer entry to caller--this is the passed memory*/
  /*  block from the calling app */
  return ICEvent.e_slave_offset;
}



int ICSlaveClose(void)
{

  /* Return to Owner */
  ICEvent.do_event = DO_SWAP;
  m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'J');
#endif

  if (ICEvent.kind==E_IC_CLOSE) {
    ICEvent.do_event = DO_FINI;
    m_action(&ICEvent);

#ifdef DEBUGON
event_monitor(&ICEvent, 'K');
#endif

    }
  return 1;
}

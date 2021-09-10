#include "cbcodes.h"
#include "string.h"

/************************************************************************
 Function to get the contents of the clipboard (a paste operation).

 Parameters:

   repname:
     Clipboard representation you wish to get.
     "BINARY" = An application-specific binary chunk of data.
     "TEXT"   = ASCII text.  Applications should try to support this
                type, if no other.  Multiple lines have no CR/LF, only
                a LineFeed.
     "NOTE"   = A database note record.
   buffer:
     Pointer to a buffer to accept the clipboard contents.
   bufsize:
     Size of the buffer.
   size_got:
     Number of bytes retrieved from the clipboard.

 Returns:
   0 if successful; !=0 if error (see "cbcodes.h" for error details).

 Call Example:
   char buffer[200];
   int bytes_got;

   GetClipboard("BINARY",buffer,sizeof(buffer),&bytes_got);
*************************************************************************/


int GetClipboard(char *repname,char *buffer,unsigned bufsize,int *size_got)
{
  int index;
  unsigned int length;
  int err;

  *size_got = 0;

  m_lock();
  err = m_open_cb();

  if (err!=CB_OK) {
     m_unlock();
     return err;
  }

  /* Clipboard is open */
  err = m_rep_index(repname, &index, &length);

   if (err < 0) {
     /* Can't paste here -- CB item is from another app or another context,
         (as in attempting to paste ascii text), or clipboard is empty. */
     m_thud();
     m_close_cb();
     m_unlock();
     return err;
  }


  if (length>bufsize)
    err=-2;
  else
    err = m_cb_read(index, 0, (char far *)buffer, length);


  if (err==CB_OK) *size_got = length;

  m_close_cb();
  m_unlock();
  return err;
}




/************************************************************************
 Function to put something into the clipboard (a copy/cut operation).

 Parameters:

   owner:
     Name of the application that is putting the text.
   repname:
     Clipboard representation you wish to put.  If you only want to
     put a text representation, this pointer can be null.
   buffer:
     Pointer to a buffer of stuff to be put to the clipboard.
   bufsize:
     Number of bytes in the buffer to write.
   textbuffer:
     Pointer to the text representation of the stuff to copy.

   size_got:
     Number of bytes retrieved from the clipboard.

 Returns:
   0 if successful; !=0 if error (see "cbcodes.h" for error details).

 Call Example:
   long int SillyExample=4;

   PutClipboard("HEXCALC","BINARY",&SillyExample,sizeof(SillyExample),"4");
*************************************************************************/

int PutClipboard(char *owner,char *repname,char *buffer,int bufsize,
                 char *textbuffer)
{
  int i,size;
  int err;

  m_lock();
  err = m_open_cb();

  if (err!=CB_OK) {
     m_unlock();
     return err;
  }

  /* Clipboard is now open */
  m_reset_cb(owner);

  if (repname) {
    m_new_rep(repname);

    /* Although m_cb_write is supposed accept a far pointer, a bug */
    /* causes it to not always reliably work.  Use a NEAR buffer to be safe!*/
    err = m_cb_write(buffer,bufsize);

    if (err < 0) {
     m_reset_cb(owner);
     m_close_cb();
     m_unlock();
     return err;
     }
    }

  m_new_rep("TEXT");

  err = m_cb_write(textbuffer, strlen(textbuffer)+1);

  m_fini_rep();
  m_close_cb();
  m_unlock();

  return err;
}



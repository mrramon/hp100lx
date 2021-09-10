
/*
 * Communications errors definition file.
 */


#if !defined(_COMERRS_H_)
#define _COMERRS_H_


#include        <sysdefs.h>     /* system error definitions (for our range) */


/*
 * Note: all communications errors must be unique within the system
 * manager's definitions, as defined in sysdefs.h. The range for
 * communications is between ERROR_COMMS_MIN and ERROR_COMMS_MAX, a
 * total of 64 values. Keep in mind these are negative values.
 */



#define ESUCCESS 0

/*
 *      ECONNECTED - indicates that the modem dialing function received 
 *              carrier detect.
 *
 *      This is *not* an error condition!
 */

#define ECONNECTED 0

/*
 * The following set of errors are used by the Filer's Remote Procedure
 * Calls. They are defined here to keep them unique within the system.
 * The values here are ncessary to communicate with old Jaguars - do
 * NOT change them (the RPC errors that).
 */

#define RPC_HARDWARE_FAILURE (ERROR_COMMS_MIN) /* hardware problem */
#define RPC_TIMEOUT          (ERROR_COMMS_MIN + 1) /* request timed out */
#define RPC_SERVER_ERROR     (ERROR_COMMS_MIN + 2) /* no reply/bad init on remote. */
#define RPC_NOT_SERVER       (ERROR_COMMS_MIN + 3) /* not in server mode. */
#define RPC_SEQUENCE_ERROR   (ERROR_COMMS_MIN + 4) /* RPC not in sequence. */
#define RPC_INTERNAL_NO_FCBS (ERROR_COMMS_MIN + 5) /* server ran out of FCBs. */
#define RPC_PROC_MISMATCH    (ERROR_COMMS_MIN + 6) /* no such remote procedure found. */
#define RPC_SERVER_RESET     (ERROR_COMMS_MIN + 7) /* already in server mode. */

/*
 *      EBUSY - port is exclusively owned by another app -- the app receiving
 *              this error is expected to gracefully recover from this error,
 *              possibly by notifying the user, or simply by ignoring the
 *              error for now, and retrying later (e.g. this would be done in
 *              a background app. (ComAcquire error).
 */

#define EBUSY           (ERROR_COMMS_MIN + 8)

/*
 *      EINVALID - a bad parameter was passed to a function. This is most
 *              likely a programming error, and no runtime error recovery
 *              is typically possible. Rewrite your code and/or validity
 *              check user inputs more carefully.
 */

#define EINVALID        (ERROR_COMMS_MIN + 9)

/*
 *      ESEIZED - your app played Robin Hood and seized the port from the
 *              some other port-rich application, and gave it to a port-poor
 *              application (yours). Remember, it could happen to you, too.
 */

#define ESEIZED         (ERROR_COMMS_MIN + 10)

/*
 *      ENOTYOURS - you are trying to perform an operation on the port, but
 *              you haven't acquired it. This is probably a programming
 *              error - you *had* acquired the port, but lost it to another
 *              app when you switched out, and forgot to reacquire it.
 */

#define ENOTYOURS       (ERROR_COMMS_MIN + 11)

/* #define ENOACCESS    (ERROR_COMMS_MIN + 12)

/*
 *      ENONE - error code from ComPortGetChar indicating that no characters
 *              are available for reading. Non-fatal, and caller recovery is
 *              expected.
 */

#define ENONE           (ERROR_COMMS_MIN + 13)

/*
 *      ERESOURCE - not enough resources available to complete the call. This
 *              error is returned by ComOpen. The application may want to
 *              display and error to the user, and let the user close down
 *              some apps with an open com port.
 */

#define ERESOURCE       (ERROR_COMMS_MIN + 14)

/*
 *      EBADPORT - an application opened up a com port for which there was
 *              no configuration information. This is a configuration problem,
 *              and should not happen. Application recovery is not possible -
 *              though fixing the apcomm.lst may help.
 */

#define EBADPORT        (ERROR_COMMS_MIN + 15)

/*
 *      ENOFIT - a com send bytes call was made trying to transmit more bytes
 *              than the transmit buffer could hold, even if empty. This is a
 *              programming error, and runtime error recovery shouldn't be
 *              necessary (although it could be provided).
 */

#define ENOFIT          (ERROR_COMMS_MIN + 16)

/*
 *      ETIMEOUT - status indicating that some timeout period for the
 *              function expired.
 */

#define ETIMEOUT        (ERROR_COMMS_MIN + 17)

/*
 *      EBADSTATE - an error returned by the modem dialing functions if
 *              the functions weren't initialized before calling the
 *              MdmConnectPoll() function.
 */

#define EBADSTATE       (ERROR_COMMS_MIN + 18)

/*
 *      EAGAIN - status indicating that the "polled" call should be reissued
 *              since the function isn't complete yet. For instance, dialing
 *              a modem using the modem library functions will receive this
 *              status.
 *
 *      This is *not* an error condition!
 */

#define EAGAIN          (ERROR_COMMS_MIN + 19)

/*
 *      ESHORT - status error for ComAcqReceiveBytes indicating that the
 *              caller specified minimum number of bytes is not present at
 *              this time.
 */

#define ESHORT          (ERROR_COMMS_MIN + 20)

/*
 *      EMISSPD - Script file error: missing character (digit) after the
 *              "#" sign which requires 2 hex digits.
 */

#define EMISSPD         (ERROR_COMMS_MIN + 21)

/*
 *      EBADHEX - Script file error: bad hex digit found after "#".
 */

#define EBADHEX         (ERROR_COMMS_MIN + 22)

/*
 *      EMISSCTRL - Script file error: missing alpha character after "^".
 */

#define EMISSCTRL       (ERROR_COMMS_MIN + 23)

/*
 *      EBADCTRL - Script file error: non-alpha after "^".
 */

#define EBADCTRL        (ERROR_COMMS_MIN + 24)

/*
 *      ENOREMOTE - Script file error: no echo received after the user
 *              requested echo pacing.
 */

#define ENOREMOTE       (ERROR_COMMS_MIN + 25)

/*
 *      EMISSOPENFNAME - Script file error: didn't find an opening "["
 *              delimeter after a "@f" in the script file.
 */

#define EMISSOPENFNAME  (ERROR_COMMS_MIN + 26)

/*
 *      EMISSCLSFNAME - Script file error: didn't find a closing "]" after
 *              the filename and before the end of the script file, or the
 *              specified filename exceeded the internal buffer capacity.
 */

#define EMISSCLSFNAME   (ERROR_COMMS_MIN + 27)

/*
 *      EBADFNAME - Script file error: couldn't open the file specified in
 *              in the "@f[<pathname>]" construct.
 */

#define EBADFNAME       (ERROR_COMMS_MIN + 28)

/*
 *      ESCRFILEIO - Script file error: unspecified error during script file
 *              reading.
 */

#define ESCRFILEIO      (ERROR_COMMS_MIN + 29)

/*
 *      EMISSCLSBRK - Script file error: the end of file was encountered
 *              while looking for the closing brace (i.e. "}") in an
 *              "expect" string.
 */

#define EMISSCLSBRK     (ERROR_COMMS_MIN + 30)

/*
 *      ENOSTRING - Script file error: the expect string never arrived
 *              from the remote host.
 */

#define ENOSTRING       (ERROR_COMMS_MIN + 31)

/*
 *      EBADOP - CTF file error: invalid operator word (e.g. RTCONV)
 *              found.
 */

#define EBADOP          (ERROR_COMMS_MIN + 32)

/*
 *      EBADINT - CTF file error: invalid integer specified to an operator.
 */

#define EBADINT         (ERROR_COMMS_MIN + 33)

/*
 *      EBADRADIX - CTF file error: invalid radix specified.
 */

#define EBADRADIX       (ERROR_COMMS_MIN + 34)

/*
 *      ELINELONG - CTF file error: the line was too long.
 */

#define ELINELONG       (ERROR_COMMS_MIN + 35)

/*
 *      EXLTFILEIO - CTF file error: unspecified I/O error reading file.
 */

#define EXLTFILEIO      (ERROR_COMMS_MIN + 36)

/*
 *      EARGCOUNT - CTF file error: too many/too few arguments specified.
 */

#define EARGCOUNT       (ERROR_COMMS_MIN + 37)

/*
 *      EMULTIKEY - CTF file error: too many multikeys defined.
 */

#define EMULTIKEY       (ERROR_COMMS_MIN + 38)

/*
 *      ENOPHONE - Connect attempted, but no phone number was defined.
 */

#define ENOPHONE        (ERROR_COMMS_MIN + 39)

/*
 *      EKREFUSED - Kermit refused file.
 */

#define EKREFUSED       (ERROR_COMMS_MIN + 40)

/*
 *      EKEYBRDABORT - Keyboard abort occurred during a file transfer.
 */

#define EKEYBRDABORT    (ERROR_COMMS_MIN + 41)


/*
 *      EHELPFILE - Can't find help file
 */
#define EHELPFILE       (ERROR_COMMS_MIN + 42)

/*
 *      EHELPREV - mismatch help revision
 */
#define EHELPREV       (ERROR_COMMS_MIN + 43)

/*
 *      EFILENOTEXIST - file does not exist
 */
#define EFILENOTEXIST       (ERROR_COMMS_MIN + 44)

/*
 *      ENOMULFILES - can't send multiple files
 */
#define ENOMULFILES       (ERROR_COMMS_MIN + 45)

/*
 *      ETOOMANYRETRIES - too many retries during file transfer.
 *
 * This is not a programmer recoverable error, but an error to be reported
 * to the user. The file transfer protocol tried too many times to get a
 * file transferred, and finally gave up. The user can recover by retrying
 * the transfer with a better connection (phone line).
 */

#define ETOOMANYRETRIES   (ERROR_COMMS_MIN + 46)

/*
 *      ETOOMANYFILES - too many files for a wildcard send/get.
 *
 * This is not a programmer recoverable error (unless the file limit is set
 * too low). We are doing a wildcard send or the remote is doing a wildcard
 * receive, and we were unable to hold all the files in the *.* expansion.
 */

#define ETOOMANYFILES   (ERROR_COMMS_MIN + 47)

/*
 *      EREMOTECANCEL - remote cancelled the file transfer.
 *
 * The remote aborted the file transfer for some reason. This isn't
 * programmer recoverable, and the UI designers can determine if they
 * want a message displayed.
 */

#define EREMOTECANCEL   (ERROR_COMMS_MIN + 48)

/*
 *      EXFERFIO - file i/o error during file xfer
 *
 * File I/O error during file xfer.  This could be any kind of error eg.
 * Disk full, Drive write protected etc.  We lost detail info because
 * protocol modified original error code.
 */

#define EXFERFIO   (ERROR_COMMS_MIN + 49)

/*
 *      EXFERFOPEN - file open error for XMODEM
 *
 * File open error during file xfer.  This could be any kind of error eg.
 * Disk full, Drive write protected etc.  We lost detail info because
 * protocol modified original error code.
 */

#define EXFERFOPEN   (ERROR_COMMS_MIN + 50)

/*
 *      EXFERINVALID - invalid file xfer request
 */

#define EXFERINVALID   (ERROR_COMMS_MIN + 51)

/*
 *      EUNKNOWN - unknwon error
 */

#define EUNKNOWN   (ERROR_COMMS_MIN + 52)

/*
 *      ENAK - operation not acknowledged or refused
 */

#define ENAK  (ERROR_COMMS_MIN + 53)


/*
 *		ECALLNOTEST - call not established
 */

#define ECALLNOTEST (ERROR_COMMS_MIN + 54)

/*
 *		EINVALIDCONFIG - invalid configuration file
 *      error from settings open dialog box
 */

#define EINVALIDCONFIG (ERROR_COMMS_MIN + 55)

/*
 *		ENOCONFIG - invalid or missing configuration file
 */

#define ENOCONFIG (ERROR_COMMS_MIN + 56)

/*
 *      ECOM3NOTDEF - The CommIOAddr and/or CommINT values have not
 *      been defined via the Setup application.
 */

#define ECOM3NOTDEF (ERROR_COMMS_MIN + 57)

/*
 *      EINTCONFLICT - The CommINT value (specified via the Setup
 *      application) conflicts with the Interrupt being used by
 *      another open port.
 */

#define EINTCONFLICT (ERROR_COMMS_MIN + 58)

#endif

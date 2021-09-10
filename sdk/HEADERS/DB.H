/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
/******************************************************************************
 *
 *  Prototypes and structures for CAP database engine.
 *
 ******************************************************************************/
#pragma pack(1)


#include "db_sm.h"


#include "datetime.h"
#ifndef NULL
#define NULL 0
#endif

/******************************************************************************
 *
 * DB Defines and Error codes
 *
 ******************************************************************************/

#define DB_VER 0x102           /* DB Version 1.02 -- This number should update */
                               /*   only on a structural change that could */
                               /*   affect reading existing files. */
#define DB_TYPES 32            /* Count of database record types */
#define DB_FIELDS 32           /* Maximum # of database field types */

#define DB_SORTFIELDS 3        /* Maximum number of fields for sort keys */
#define DB_MAXVIEWPTS 16       /* Maximum number of viewpoint definitions/tables */

#define DB_MAXDIGITS  16       /* Maximum digits used in numerical comparisons*/

#define DB_MAXPASSWORD 16      /* Maximum size of password */

#define DB_MAXSTRING 256       /* Size of max search string for DB_FindRecord and DB_FindDataRecord */

#define DB_MAXFINDFIELDS 64    /* Max fields supported by DB_FindDataRecord */

#define DB_FIELDNAMELENGTH 20

#define DB_FINDSTRINGLENGTH 64  /* Max size of a search string passed to the Find routines */

#define DB_VIEWPTNAMELENGTH 31

#define DB_LINKNAMELENGTH (DB_VIEWPTNAMELENGTH)


#define DB_CATEGORYSEPARATOR ';'

/***
 *** ERROR VALUES RETURNED BY ALL DB FUNCTIONS
 ***/


enum DB_Errors {
DB_OK=0,        /* No error (any positive number is no error */
                /* because some calls return a record number) */
DB_NOTFOUND=-1,    /* 0xFFFF: Requested record not found */
DB_DELETED=-2,     /* 0xFFFE: Operation in question does not support deleted records */
DB_OPEN=-3,        /* 0xFFFD: Can't open file (file does not exist) */
DB_CLOSE=-4,       /* 0xFFFC: Can't close file */
DB_READ=-5,        /* 0xFFFB: Can't read record (disk error during read)*/
DB_WRITE=-6,       /* 0xFFFA: Can't write record (disk is full) */
DB_SIGNATURE=-7,   /* 0xFFF9: File signature incorrect (non-database file) */
DB_CREATE=-8,      /* 0xFFF8: Can't create file (too many files on disk)*/
DB_SEEK=-9,        /* 0xFFF7: Error seeking (file shorter than expected)*/

DB_SYNTAX=-10,     /* 0xFFF6: Syntax error in FISL string */
DB_OVERFLOW=-11,   /* 0xFFF5: Expression larger than allocated buffer */
DB_COMPLEX=-12,    /* 0xFFF4: Expression too complex/large to be parsed*/
DB_TYPEMISMATCH=
              -13, /* 0xFFF3: FISL type mismatch (e.g. number with string field)*/

DB_NOFIELD=-14,    /* 0xFFF2: Field name not in database definition or field deleted */
DB_NOCALLBACK=-15, /* 0xFFF1: User field encountered, but no callback was supplied */
DB_LARGEVIEWPT=-16,/* 0xFFF0: Viewpoint/lookup table too large--exceeded maximum record count */
DB_VPABSENT=-17,   /* 0xFFEF: Warning: current record lost from viewpoint */

DB_CANTUNDO=-18,   /* 0xFFEE: Further undos/redos are not available */

DB_MEMORY=-19,     /* 0xFFED: Not enough RAM to complete the operation */

DB_PASSWORDFAILED=
             -20,  /* 0xFFEC: File requires valid password to be opened */
DB_MAYBEINUSE=-21, /* 0xFFEB: Warning: file appears to be currently in use */
DB_RELEASE=-22,    /* 0xFFEA: Release ver. of datafile is after that of engine */
DB_FILETYPE=-23,   /* 0xFFE9: Database file is that of a different type */

DB_ILLEGAL=-24,    /* 0xFFE8: Error in requested operation */
DB_USERABORT=-25,  /* 0xFFE7: Operation aborted by user (Find operations only can be interrupted by keypress) */

DB_CORRUPT=-26,    /* 0xFFE6: A disk media error occurred during garbage collection--  */
                   /*         The file may or may not have survived intact. */
DB_SORTCHANGE=-27, /* 0xFFE5: The system sort order has changed.  The database */
                   /*         should be reopened to revalidate the viewpt tables. */
                   /*         (Ver 1.02+) */
DB_PROTECTED=-28   /* 0xFFE4: The action failed because the media is write-protected. */
};



/***
 ***  Allowable actions when passing a record to DB_ModifyViewpt()
 ***/



enum DB_ModifyViewptTypes {
MVP_DELETE,     /* Specified DATA record has been deleted */
MVP_MODIFY,     /* Specified DATA record has been modified */
MVP_ADD         /* Specified DATA record has been added */
};



/***
 ***  Action codes returned by DB_Undo
 ***/

enum DB_UndoActions {
UNDO_DELETED,   /* Specified record number has been deleted */
UNDO_MODIFIED,  /* Specified record number has been modified */
UNDO_ADDED      /* Specified record number has been added */
};



/***
 *** Bits returned by DB_UndoStatus()
 *** and other database flags
 ***/

#define DB_US_UNDO 1        /* Undo is possible */

#define DB_VIEWPTTOUCHED 2  /* Current viewpt has been touched since last read*/

#define DB_OPENDATABASE  4  /* Set when DB is opened, cleared when closed */

#define DB_OPENHANDLE    8  /* Set when database's dos handle is open, cleared when closed */

#define DB_SHOULDFLUSH   16 /* Set for actions that could leave the db vulnerable */
                            /* if a Ctrl-Alt-Del happens; mostly DB_WriteRecord and */
                            /* DB_DeleteRecord.  DB_FlushFile clears the bit */

#define DB_BEINGCREATED  32  /* Bit set during DB_Create--used in viewpoint */
                             /* creation to avoid creation of a ROM viewpoint */
                             /* from scratch.  NOTE that this optimization */
                             /* assumes that any ROM record database provides */
                             /* the records in sorted order of the default */
                             /* viewpoint AND that all the ROM records meet */
                             /* the default filter.  A regular DB_Open will, */
                             /* however create the viewpoint from scratch (only */
                             /* if missing) since RAM records may be present. */

#define DB_NEEDSGC       64  /* Set if the database contains garbage */

#define DB_WASTRANSLATED 128 /* Set during DB_Open if the file is a Lion file. */
                             /* If this flag is set, the database engine has */
                             /* translated all record types it knows about */
                             /* from Windows to Code Page 850.  It will not, */
                             /* however, touch TYPE_USER or above records. */
                             /* This is a signal to the application to apply */
                             /* the DB_WinStrTo850 function to all user defined */
                             /* strings in records other than TYPE_CATEGORY, */
                             /* TYPE_FIELDDEF, TYPE_VIEWPTDEF, TYPE_NOTE */
                             /* TYPE_DATA.  TYPE_LINKDEF isn't translated */
                             /* because Lion doesn't use SmartClip specs. */
/***
 *** Bit flags accepted by DB_FindRecord and DB_FindDataRecord
 ***/
#define DB_FIND_AUTOGOTO 0x8000
    /* Find only will match a record if the string */
    /* is found at the field passed in, and only if */
    /* it matches at the first character. Used for */
    /* AutoGoto searches. Only applies for DB_FindDataRecord. */

#define DB_FIND_DIRECT 0x4000
    /* begin and end refer to data records directly; */
    /* if not set, refers to viewpt entries.  This is */
    /* only applicable for data records.  Only applies
    /* for DB_FindDataRecord. */

#define DB_FIND_NOTES 0x2000
    /* Include notes in the search.  If not set, notes*/
    /* are not searched.  Only applies for FindDataRecord. */

#define DB_FIND_CASE 0x1000
    /* Match is case sensitive.  Default with bit off */
    /* is case insensitive match. */

#define DB_FIND_FIRST 0x0800
    /* Used on the first find in a sequence--starts the find from the */
    /* beginning of the record; resets the field and position values passed */
    /* in the pos (and field) pointers.  If DB_FINDFIRST is not */
    /* set, the search resumes at the byte after the one passed in the */
    /* pos pointer (and field pointer). */


/***
 *** Status bits in DBHEADER.filestatus
 ***/
#define STATUS_DBACTIVE 1      /* Set when database is opened */
#define STATUS_MODIFIED 2      /* Set if any records have been modified */



/***
 *** Status bits in RECORDHEADER status
 ***/

#define STATUS_GARBAGE 1
#define STATUS_MODIFIED 2      /* Used in reconciliation of databases */





/***
 *** Status bits in LOOKUPENTRY flags
 ***/

#define STATUS_DELETED   128    // Set when lookup entry is deleted.
#define STATUS_WASDELETED 64    // Set for undo so lookup table is restored
                                //   properly.
#define STATUS_SELECTED   32    // Set if the entry is CUA selected to
                                //   track multiple selections.


/******************************************************************************
 *
 *  Field type definitions.
 *
 *  Note that since the field type is also used to reconstruct windows, there
 *  is duplication of several field types so that window classes can be
 *  distinguished.
 ******************************************************************************/


enum FieldTypes {
BYTEBOOL_FIELD,      /* Checkbox: points to a bit in a byte */
WORDBOOL_FIELD,      /*           points to a bit in a word */
STRING_FIELD,        /* dataoffset points to ASCIIZ string */
PHONE_FIELD,         /* Treated as a String by the database */

NUMBER_FIELD,        /* Numbers are stored as ASCIIZ strings, but compare as numbers */
CURRENCY_FIELD,      /* Treated as a number by the database */

CATEGORY_FIELD,      /* Stored as an ASCIIZ string with semicolons separating each selection*/

TIME_FIELD,          /* int: Minutes since midnight */
DATE_FIELD,          /* points to char array[3]; array[0] is Year: 0 starts at 1900 */
                     /*                          array[1] is Month from 0..11 */
                     /*                          array[2] is Day from 0..30 */

RADIO_FIELD,         /* dataoffset points to actual offset of radio button byte */

NOTE_FIELD,          /* The field is a note (which resides in a separate
                      * record); dataoffset points to the record number
                      * of the note.  If no note is attached, the record number
                      * pointed to should be  -1.
                      */
GROUP_FIELD,         /* Used for Groupboxes; no data is attached */
STATIC_FIELD,        /* Used for Static Text; no data is attached */
MULTILINE_FIELD,     /* Same as a string, but the field can have CR/LFs */
LIST_FIELD,          /* Doubles as a STATIC_FIELD */
COMBO_FIELD,         /* Doubles as a STRING_FIELD */
USER_FIELD,          /* The applications can define their own field types starting at USER_FIELD
                      * NOTE: The FIELDDEF for user defined fields MUST have the status bit
                      * set to FIELDDEF_CALLBACK and a FieldCallback function provided.
                      */
};



/* FieldType status bits */

#define FIELDDEF_NODATA   128       // The fielddef does not have any associated data
#define FIELDDEF_RESERVED  64       // The fielddef record is reserved (don't use)
#define FIELDDEF_DELETED (128+64)   // This fielddef should not be accessed (either
                                    // has no data or is reserved).
#define FIELDDEF_RELATIVE  32  // The dataoffset does not point to the actual data, but
                               // points to a word.  This word is the offset to the data.
#define FIELDDEF_NULLTITLE 16  // The window related to the fielddef has no title


/* Viewpoint flag bits */
#define VIEWPT_UNNAMED  1    /* Viewpoint has not yet been named by user */
#define VIEWPT_NODELETE 2    /* Viewpoint cannot be deleted by user */
#define VIEWPT_GENERAL  4    /* Viewpoint was entered as a FISL string, not a Card filter */


/* Linkdef flag bits */
#define LINK_NODELETE 2      /* Linkdef cannot be deleted by user */



/******************************************************************************
 *
 *  Built-in record type definitions.  Individual apps can add
 *  their own.
 *
 ******************************************************************************/

enum DB_RecordTypes {
TYPE_DBHEADER,
TYPE_PASSWORD,
TYPE_DEFAULT_DATA,
TYPE_CONFIGREC,
TYPE_CARDDEF,
TYPE_CATEGORY,
TYPE_FIELDDEF,
TYPE_VIEWPTDEF,
TYPE_PRINTDEF,
TYPE_NOTE,
TYPE_VIEWPTTABLE,
TYPE_DATA,
TYPE_LINKDEF,
TYPE_CARDPAGEDEF,
TYPE_USER,              /* Start of user defined types */

TYPE_LOOKUPTABLE=DB_TYPES-1,       /* End of user defined types */
};



/******************************************************************************
 *
 *  Database record structures.  Each app supplements these with
 *  its own data and other record structs.  The data record struct
 *  contained here has only the mandatory record fields needed
 *  for the engine to manage the database.
 *
 ******************************************************************************/

typedef int RECORDNUM;

typedef long int RECORDOFFSET;


/* This structure is used in memory--it needs the undo information, and */
/* is structured for efficient manipulation (i.e. record offsets are longs) */
typedef struct {
  unsigned int size;     /* = 0 for deleted AND committed lookup entries */
                         /* Size includes recordheader, so 0 length user recs*/
                         /* will have size 6. */
  unsigned int filters;
  char flags;  /* bit 7 for deleted entries, bit 6 for modified, */
               /* bit 5 for selected, bits 4-0 reserved */
  RECORDOFFSET offset;
  RECORDOFFSET undooffset;
  } LOOKUPENTRY;



/* This structure is the one actually saved to disk.  It has the undo ofs */
/* removed, and the record offset is stored in 3 bytes (plenty reasonable) */
typedef struct {
  unsigned int size;
  unsigned int filters; /* No undo offset stored (can't undo after close) */
  char flags;
  char offset[3];       /* Only 3 bytes of offset stored (0..16 meg) */
  } PACKEDLOOKUPENTRY;



typedef char FILEID[4];   /* This "record" is not represented anywhere in */
                          /* the DB tables; it is only referenced during  */
                          /* DB_Open for verification. */




typedef struct {         /* On disk, this record heads all record types */
  char type;
  char status;           /* Record status : garbage, needs reconciliation, etc. */
  unsigned int length;
  RECORDNUM record;   /* Relative record index--each rec type starts at 0 */
  } RECORDHEADER;





typedef struct {
  RECORDHEADER rhdr;     /* Only included in this record because it is read directly */
  int releasenum;
  char filetype;
  char filestatus;
  RECORDNUM curr_viewpt;   /* Index of currently active filter; 0 is default */
  RECORDNUM numrecords;
  RECORDOFFSET lookupseek;
  DB_TIMESTAMP lastreconciled;  /* Timestamp that database was last reconciled */

  int magicviewptsort;
  /* "Magic" value generated from system settings: Country, CodePage, Sort, Keyboard, and Language */
  /* Used to keep viewpt tables from going out of date if system sort order changes. */
  /* If on DB_Open this number mismatches, all viewpoint tables are flagged as invalid. */
  /* If on DB_Reactivate, this number mismatches, the Reactivate returns a DB_SORTCHANGE, */
  /*   and the file must be reopened.  */
  /* (Ver 1.02+ only.  On Ver 1.01, this field is a reserved int, and may */
  /*  contain random values.) */
  } DBHEADER;


typedef struct {
  FILEID signature;
  DBHEADER header;
  } DBFILEPREFIX;



/*******************************************************************************************/
/* THE REST OF THE RECORDS DO NOT HAVE THE RECORDHEADER EMBEDDED IN THEM.  THIS IS BECAUSE */
/* THE RECORDS ARE READ INTO MEMORY, AND THE RECORD HEADERS ARE STRIPPED BY THE DATABASE   */
/* ENGINE UPON READING THEM.  THE RECORDHEADERS ARE PRESENT ON THE DISK.                   */
/*******************************************************************************************/



/* Gives size of given FieldDef, not including trailing garbage bytes */
#define FIELDDEFSIZEOF(f) (sizeof(FIELDDEF)-DB_FIELDNAMELENGTH+lstrlen((f).name))

typedef struct {
  char fieldtype;
  char fieldID;       /* Used to identify unique fields by applications--not used by DB */
  int dataoffset;
  char flags;         /*  See FieldType status bits*/
  int reserved;      /* Used for some fields */
                         /* BYTEBOOL_FIELD,WORDBOOL_FIELD : bitmask of bit to check */
                         /* RADIO_FIELD : value to stuff into data byte */
                         /* CATEGORY_FIELD : record number of associated CATEGORY_TYPE rec */
  char name[DB_FIELDNAMELENGTH+1];   /* With room for trailing 0 */
  } FIELDDEF;



/* Column Arrangement */

typedef struct {
  char fieldnumber;      /* The first FieldNumber == -1 means "no more columns" */
  char width;            /* Column width in characters */
  } COLUMNINFO;

#define DB_NUMBERCOLUMNS 20



typedef struct {
  int tokenlen;
  int stringlen;                  /* don't include trailing zero */

  int flags;
  char reserved;           /* Possibly used for a column count */

  char name[DB_VIEWPTNAMELENGTH+1];

/* Sort information */
  int sortfield[DB_SORTFIELDS];   /* field indicies, first -1 means "no more fields" */
  int ascending[DB_SORTFIELDS];   /* 1 = ascending, 0 = descending */

/* Column arrangement */
  COLUMNINFO arrange[DB_NUMBERCOLUMNS];

/* Filter information */
/* -----------------------------------  */
/* FOLLOWED ON DISK/IN MEMORY BY:       */
/*    char tokens[tokenlen];            */
/*    char FISLstring[stringlen];       */
/*                                      */
  } VIEWPTDEF;



typedef struct {
  int stringlen;
  int flags;
  char name[DB_LINKNAMELENGTH+1];

/* Link information */
/* -----------------------------------  */
/* FOLLOWED ON DISK/IN MEMORY BY:       */
/*    char Linkstring[stringlen];       */
/*                                      */
  } LINKDEF;



/* DBBLOCK is a memory structure only--it doesn't exist on disk */

typedef struct {
  char filename[80];       /* filename of db file opened */
  int handle;              /* dos handle of db file */
  DBHEADER header;         /* header info for db (see record above) */
  unsigned int typefirst[DB_TYPES];     /* index of first record of each type */
                                        /*    into lookup table */
  unsigned int typecount[DB_TYPES];     /* Number of records of each type */
  LOOKUPENTRY far * APP_NEAR *lookup;     /* ptr to lookup table for all records */
  int far * APP_NEAR *viewpt;             /* ptr to array of record numbers for TYPE_DATA */
                                /*    records in current viewpt */
  int viewptcount;         /* number of records in current viewpt */
  int lookupmax,viewptmax; /* max number or lookup/viewpt entries before*/
                           /*   expansion needed */
  FIELDDEF far * APP_NEAR *fielddefs;  /* ptr to field definitions for current db */
  int (far *FieldCallBack)();  /* ptr to code to translate field formats */
                               /* See below. */
  unsigned int flags;      /* database flags */
  VIEWPTDEF far * APP_NEAR *viewptdef; /* ptr to current viewpt definition filter */
  int cardchanges;
  int reserved;
  char password[DB_MAXPASSWORD+1];  /* encrypted version of the current password*/
  int (far *ROMCallBack)();  /* ptr to code to read ROM data records (or */
                             /* other calculated records */
  int ROMCount;
} DBBLOCK;
 

/*************************************************************************
 ** FieldCallBack
 *************************************************************************
 FieldCallBack is the method used by the DB Engine to support field types
 not natively used by the database.  To do this, the application creates 
 fields with their own field types (starting at USER_FIELD).  Those 
 fields are automatically registered as callback fields.  The application
 additionally must provide a function on opening/creating the database 
 that knows how to deal with the additional types.  This function is the 
 FieldCallBack.

 The FieldCallBack function MUST have the following prototype:
   int far CallBack(DBBLOCK *dblk,int action, int buffersize,
                    int intarg, void far *ptrarg1, void far *ptrarg2);

 Although the DBBLOCK is passed to the callback, it should be used for 
 reading database structures only.  The CallBack function is not allowed 
 to make any calls to the database engine. The DB Engine will use the 
 callback with the following calling sequences :

 *  CallBack(dblk, USERTYPE_TOSTRING, buffersize,
             userfieldtype, userptr, buffer);
        Used to convert the user defined fields into a string.  
        userfieldtype identifies the field type, userptr is a pointer to 
        the user field in a data record, buffer points to a ram buffer 
        allocated by the Engine to hold the ASCIIZ string, and 
        buffersize is the maximum size of the buffer.  Under no 
        circumstances should the callback copy more than buffersize 
        bytes into the buffer; if the string is too large, it should be 
        truncated. The return value is ignored.

 *  CallBack(dblk,USERTYPE_FROMSTRING, userptrbufsize,
             (int near *)size, buffer, userptr);
        Used to convert a string into some user defined field type.
        buffer points to the ASCIIZ string to convert, and userptr
        points to the buffer to place the converted type.  userptrbufsize
        is the size of the buffer at userptr: if the string cannot be
        converted, the callback should return DB_SYNTAX.  If the 
        callback cannot fit the converted result into the buffer at 
        userptr, the callback should return DB_MEMORY.  Otherwise,
        the function should return the type of the data that was 
        converted, and place the size of the converted type in the int 
        pointed at by size.  Under no circumstances should the callback
        copy more than userptrbufsize bytes into the buffer.

 *  CallBack(dblk,USERTYPE_COMPARE, 0,
             userfieldtype, userptr1, userptr2);
        Used to compare two user defined fields. userfieldtype is used 
        to identify the field type for both user-defined fields. 
        userptr1 and userptr2 point to the those fields in (perhaps) two 
        different data records. The value returned should be 1 if 
        *userptr1>*userptr2, -1 if *userptr1<*userptr2, and 0 if 
        *userptr1==*userptr2 (just like strcmp).

 *  CallBack(dblk,USERTYPE_CONTAINS, 0,
             userfieldtype, userptr, buffer);
        Used to see if a string is contained in the user type.  This is
        provided in case the user field has some special meaning for the
        CONTAINS relation (e.g. the user type is a set).  The number returned
        should be 0 if the string at buffer is not contained in userptr,
        else the returned value should be >0.

        A return value of DB_COMPLEX indicates to the DB Engine that the
        CallBack does not provide the CONTAINS relation:  in this case,
        the DB Engine simply uses USERTYPE_TOSTRING to convert the
        type to a string and does a string search.  Note that since
        the Database uses a limited size buffer to convert the string,
        very large fields should support their own CONTAINS.
 *************************************************************************/
enum CallBackActions {
USERTYPE_TOSTRING,
USERTYPE_FROMSTRING,
USERTYPE_COMPARE,
USERTYPE_CONTAINS,
USERTYPE_FROMSTRINGNORESET,
};


/*************************************************************************
 ** ROM database callback.
 *************************************************************************

 ROMCallBack is another method of expanding the database, by providing
 data records that do not exist in the file.  The ROM method can be
 applicable to other types of not-saved or calcuated records with the
 following limitations:
   * The ROM records must be contiguous at the beginning of the file.
       Viewpointing may still resort the records as necessary.
   * The number of ROM records must be known before the file is opened,
       and cannot be changed dynamically.
   * The ROM records cannot be deleted, but can be overwritten.  The
       database acts if deleted records below the ROM record count
       are "showing" through to the ROM records; If a record is added
       below the ROM record count, it blocks the ROM record from showing.

  The prototype for the ROM callback function must look like this:

   int far ROMCallBack(DBBLOCK *dblk, int n, char far *dest, int bufsize,
                       int *sizeread);
 *************************************************************************/






//typedef struct {
/* -----------------------------------         */
/* FOLLOWED ON DISK BY:                        */
/*    PACKEDLOOKUPENTRY entry[numberrecords];  */
/*    unsigned int typefirst[DB_TYPES];        */
/*                                             */
/*  The actual lookup table directly follows this structure that  */
/*  contains only the record header.  It is not defined in the  */
/*  structure because it is of variable size.  The first/count  */
/*  list for the lookup entries follows.  */
//  } LOOKUPTABLE;





//typedef struct {
/* -----------------------------------                            */
/* FOLLOWED ON DISK BY:                                           */
/*    RECORDNUM entry[keysize];                                   */
/*                                                                */
/*  The actual viewpt table directly follows this structure that  */
/*  contains only the record header.  It is not defined in the    */
/*  structure because it is of variable size.                     */
//} VIEWPTTABLE;





//typedef struct {

/* -----------------------------------                              */
/* FOLLOWED ON DISK BY:                                             */
/*    char category[size];                                          */
/* The category record is a single ASCIIZ string with the individual */
/* categories separated by semicolons.  Each CATEGORY field will    */
/* have its own category record.                                    */
//  } CATEGORY;



/****************************************************************************
 * Undo return structure--used to track record changes as a result of an Undo
 ****************************************************************************/


typedef struct {
  char Action;           /* What was done to get the Undone record */
  char Type;             /* Type of the undone record */
  RECORDNUM Record;      /* Record number */
} UNDORETURN;





/******************************************************************************
 ******************************************************************************
 ******************************************************************************
 *
 *  Function prototypes for database engine.
 *
 ******************************************************************************
 ******************************************************************************
 ******************************************************************************/


#include "db_pvt.h"


/***** Essential functions *****/



int far _DB_Init(DBSMPREFIX0);
#define DB_Init()  \
            (DB_DIRECT(2)? \
            _DB_Init(DBSMCALL0):  \
            c_service(F_DB_INIT))



int far _DB_Create(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char APP_NEAR *filename, char filetype,
                   int (far *FieldCallBack)(),
                   int ROMCount, int (far *ROMCallBack)());
#define DB_Create(dblk,filename,filetype,FieldCallBack,ROMCount,ROMCallBack) \
            (DB_DIRECT(1)? \
            _DB_Create(DBSMCALL1 (dblk),(filename),(filetype),(FieldCallBack), (ROMCount),(ROMCallBack)): \
            c_service(F_DB_CREATE,(void near *)(dblk),(void near *)(filename),(filetype),\
                       (void far *)(FieldCallBack),(ROMCount),(void far *)(ROMCallBack)))


int far _DB_Open(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char APP_NEAR *filename, char filetype,
                  int (far *FieldCallBack)(), char APP_NEAR *password,
                  int ROMCount, int (far *ROMCallBack)());
#define DB_Open(dblk,filename,filetype,FieldCallBack,password,ROMCount,ROMCallBack) \
        (DB_DIRECT(1)? \
         _DB_Open(DBSMCALL1 (dblk), (filename), (filetype), (void far *)(FieldCallBack), (password), (ROMCount), (void far *)(ROMCallBack)): \
         c_service(F_DB_OPEN,(void *)(dblk),(void *)(filename),(filetype), \
                   (void far *)(FieldCallBack),(password),(ROMCount),(void far *)(ROMCallBack)))



int far _DB_Deactivate(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);  /* Used before DEACTIVATING to flush file */
#define DB_Deactivate(dblk) \
        (DB_DIRECT(2)? \
         _DB_Deactivate(DBSMCALL1 (dblk)):  \
         c_service(F_DB_DEACTIVATE,(void *)(dblk)))



int far _DB_Reactivate(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);  /* Used upon REACTIVATING to reopen file */
#define DB_Reactivate(dblk) \
        (DB_DIRECT(2)? \
        _DB_Reactivate(DBSMCALL1 (dblk)): \
        c_service(F_DB_REACTIVATE,(void *)(dblk)))



int far _DB_Close(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_Close(dblk) \
        (DB_DIRECT(1)? \
        _DB_Close(DBSMCALL1 (dblk)): \
        c_service(F_DB_CLOSE,(void *)(dblk)))



int far _DB_ShutDown(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_ShutDown(dblk) \
        (DB_DIRECT(1)? \
        _DB_ShutDown(DBSMCALL1 (dblk)): \
        c_service(F_DB_SHUTDOWN,(void *)(dblk)))



int far _DB_ReadRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum, void far *dest, unsigned int bufsize,unsigned int APP_NEAR *sizeread);
#define DB_ReadRecord(dblk,recordtype,recordnum,dest,bufsize,sizeread) \
        (DB_DIRECT((1*0))? \
        _DB_ReadRecord(DBSMCALL1 (dblk),(recordtype),(recordnum),(void far *)(dest),(bufsize),(sizeread)): \
        c_service(F_DB_READRECORD,(void *)(dblk),(recordtype),(recordnum),  \
           (void far *)(dest),(bufsize),(void *)(sizeread)))



int far _DB_WriteRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum, void far *src, unsigned int recsize, int APP_NEAR *viewptindex);
#define DB_WriteRecord(dblk,recordtype,recordnum,src,recsize,viewptindex) \
        (DB_DIRECT(1)? \
        _DB_WriteRecord(DBSMCALL1 (dblk),(recordtype),(recordnum),(void far *)(src),(recsize),(viewptindex)): \
        c_service(F_DB_WRITERECORD,(void *)(dblk),(recordtype),(recordnum),  \
           (void far *)(src),(recsize),(void *)(viewptindex)))



int far _DB_WriteInPlace(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum, unsigned int recoffset, void far *src, unsigned int size);
#define DB_WriteInPlace(dblk,recordtype,recordnum,recoffset,src,recsize) \
        (DB_DIRECT((0*2))? \
        _DB_WriteInPlace(DBSMCALL1 (dblk),(recordtype),(recordnum),(recoffset),(void far *)(src),(recsize)): \
        c_service(F_DB_WRITEINPLACE,(void *)(dblk),(recordtype),(recordnum),  \
           (recoffset),(void far *)(src),(recsize)))



int far _DB_DeleteRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum, int APP_NEAR *viewptindex);
#define DB_DeleteRecord(dblk,recordtype,recordnum,viewptindex) \
        (DB_DIRECT(1)? \
        _DB_DeleteRecord(DBSMCALL1 (dblk),(recordtype),(recordnum),(viewptindex)): \
        c_service(F_DB_DELETERECORD,(void *)(dblk),(recordtype),(recordnum),  \
           (void *)(viewptindex)))



int far _DB_CompressDelete(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum);
#define DB_CompressDelete(dblk,recordtype,recordnum) \
        (DB_DIRECT(2)? \
        _DB_CompressDelete(DBSMCALL1 (dblk),(recordtype),(recordnum)): \
        c_service(F_DB_COMPRESSDELETE,(void *)(dblk),(recordtype),(recordnum)))




/*  DB_RecordSize returns a negative number on error, otherwise a record size*/
int far _DB_RecordSize(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, RECORDNUM recordnum);
#define DB_RecordSize(dblk,recordtype,recordnum) \
        (DB_DIRECT(1)? \
        _DB_RecordSize(DBSMCALL1 (dblk),(recordtype),(recordnum)): \
        c_service(F_DB_RECORDSIZE,(void *)(dblk),(recordtype),(recordnum)))



int far _DB_NextRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype);
#define DB_NextRecord(dblk,recordtype) \
        (DB_DIRECT(1)? \
        _DB_NextRecord(DBSMCALL1 (dblk),(recordtype)): \
        c_service(F_DB_NEXTRECORD,(void *)(dblk),(recordtype)))



int far _DB_NextNextRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype);
#define DB_NextNextRecord(dblk,recordtype) \
        (DB_DIRECT(2)? \
        _DB_NextNextRecord(DBSMCALL1 (dblk),(recordtype)): \
        c_service(F_DB_NEXTNEXTRECORD,(void *)(dblk),(recordtype)))




/*
int far _DB_AddRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, void far *src,
                 unsigned int recsize, int APP_NEAR *viewptindex);

RECORDNUM far _DB_CountRecords(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype);
*/

int far _DB_CopyRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dbto,DBBLOCK APP_NEAR *dblk, char recordtype, int recordnum);
#define DB_CopyRecord(dbto,dblk,recordtype,recordnum) \
        (DB_DIRECT(2)? \
        _DB_CopyRecord(DBSMCALL1 (dbto),(dblk),(recordtype),(recordnum)): \
        c_service(F_DB_COPYRECORD,(void *)(dbto),(void *)(dblk),(recordtype),(recordnum)))



/***** Maintenance functions *****/


/* These get called automatically if no lookup table(s) exists */

int far _DB_CreateLookupTable(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_CreateLookupTable(dblk) \
        (DB_DIRECT(1)? \
        _DB_CreateLookupTable(DBSMCALL1 (dblk)): \
        c_service(F_DB_CREATELOOKUPTABLE,(void *)(dblk)))



int far _DB_CreateViewptTable(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_CreateViewptTable(dblk) \
        (DB_DIRECT(1)? \
        _DB_CreateViewptTable(DBSMCALL1 (dblk)): \
        c_service(F_DB_CREATEVIEWPTTABLE,(void *)(dblk)))

int far _DB_CreateViewptInMemory(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_CreateViewptInMemory(dblk) \
        (DB_DIRECT(2)? \
        _DB_CreateViewptInMemory(DBSMCALL1 (dblk)): \
        c_service(F_DB_CREATEVIEWPTINMEMORY,(void *)(dblk)))

/* Gets called automatically when db is closed and on SetUndoPoint */
int far _DB_CollectGarbage(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_CollectGarbage(dblk) \
        (DB_DIRECT(2)? \
        _DB_CollectGarbage(DBSMCALL1 (dblk)): \
        c_service(F_DB_COLLECTGARBAGE,(void *)(dblk)))


int far _DB_FlushFile(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_FlushFile(dblk) \
        (DB_DIRECT(1)? \
        _DB_FlushFile(DBSMCALL1 (dblk)): \
        c_service(F_DB_FLUSHFILE,(void *)(dblk)))


/***** Selection functions *****/

int far _DB_Select(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM recordnum, int status);
#define DB_Select(dblk,recordnum,status) \
        (DB_DIRECT(2)? \
        _DB_Select(DBSMCALL1 (dblk),(recordnum),(status)): \
        c_service(F_DB_SELECT,(void *)(dblk),(recordnum),(status)))


int far _DB_IsSelected(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM recordnum);
#define DB_IsSelected(dblk,recordnum) \
        (DB_DIRECT(2)? \
        _DB_IsSelected(DBSMCALL1 (dblk),(recordnum)): \
        c_service(F_DB_ISSELECTED,(void *)(dblk),(recordnum)))



/*****  Syncronization/Password *****/

int far _DB_Reset(DBSMPREFIX1  char APP_NEAR *filename);
#define DB_Reset(filename) \
        (DB_DIRECT(2)? \
        _DB_Reset(DBSMCALL1 (filename)): \
        c_service(F_DB_RESET,(void *)(filename)))


int far _DB_ChangePassword(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char APP_NEAR *oldpassword, char APP_NEAR *newpassword);
#define DB_ChangePassword(dblk,oldpassword,newpassword) \
        (DB_DIRECT(2)? \
        _DB_ChangePassword(DBSMCALL1 (dblk),(oldpassword),(newpassword)): \
        c_service(F_DB_CHANGEPASSWORD,(void *)(dblk),(void *)(oldpassword), \
                  (void *)(newpassword)))


/* Returns a DB_OK if the password succeeded, DB_PASSWORDFAILED if the password failed. */
int far _DB_VerifyPassword(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char APP_NEAR *password);
#define DB_VerifyPassword(dblk,password)\
        (DB_DIRECT(2)? \
        _DB_VerifyPassword(DBSMCALL1 (dblk),(password)): \
        c_service(F_DB_VERIFYPASSWORD,(void *)(dblk),(void *)(password)))


/***** Undo functions *****/

/*
int far _DB_SetUndoPoint(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
*/

/* Normally, an application will call DB_SetUndoPoint above, which will */
/* also collect garbage in the file. If however, the application needs to */
/* set an undo point but not collect garbage, a "hidden" function is provided */
/* for this; DB_SetUndoPoint_() */
int far _DB_SetUndoPoint_(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_SetUndoPoint_(dblk) \
        (DB_DIRECT(2)? \
        _DB_SetUndoPoint_(DBSMCALL1 (dblk)) : \
        c_service(F_DB_SETUNDOPOINT_,(void *)(dblk)))


int far _DB_Undo(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk,int APP_NEAR *undocount,UNDORETURN far * APP_NEAR * APP_NEAR *undoret);
#define DB_Undo(dblk,undocount,undoret) \
        (DB_DIRECT(2)? \
        _DB_Undo(DBSMCALL1 (dblk),(undocount),(undoret)):\
        c_service(F_DB_UNDO,(void *)(dblk),(void *)(undocount),(void *)(undoret)))


/*
int far _DB_UndoStatus(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);  // Returns flag word with bits:
                                   //  DB_US_UNDO set if undo possible
*/





/***** Field retrieval functions *****/

int far _DB_LoadFieldDefs(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_LoadFieldDefs(dblk) \
        (DB_DIRECT(1)? \
        _DB_LoadFieldDefs(DBSMCALL1 (dblk)): \
        c_service(F_DB_LOADFIELDDEFS,(void *)(dblk)))



int far _DB_GetField(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, void far *srcrec,int index, char far *string, int bufsize);
#define DB_GetField(dblk,srcrec,index,string,bufsize) \
        (DB_DIRECT(1)? \
        _DB_GetField(DBSMCALL1 (dblk),(void far *)(srcrec),(index),(void far *)(string),(bufsize)): \
        c_service(F_DB_GETFIELD,(void *)(dblk),(void far *)(srcrec), \
                 (index),(void far *)(string),(bufsize)))

/*
int far _DB_GetNamedField(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, void far *srcrec, char far *name,
                     char far *string, int bufsize);
*/


int far _DB_GetFieldStrings(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, void far *srcrec,int far indextable[], char far * far *ptrtable, int indexcount, char far *buffer, int bufsize);
#define DB_GetFieldStrings(dblk,srcrec,indextable,ptrtable,indexcount,buffer,bufsize) \
        (DB_DIRECT(1)? \
        _DB_GetFieldStrings(DBSMCALL1 (dblk),(void far *)(srcrec),(void far *)(indextable),(void far *)(ptrtable),(indexcount),(void far *)(buffer),(bufsize)): \
        c_service(F_DB_GETFIELDSTRINGS,(void *)(dblk),(void far *)(srcrec), \
                 (void far *)(indextable),(void far *)(ptrtable),(indexcount),\
                 (void far *)(buffer),(bufsize)))



int far _DB_GetFieldName(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, int index,char far *name);
#define DB_GetFieldName(dblk,index,name) \
        (DB_DIRECT(1)? \
        _DB_GetFieldName(DBSMCALL1 (dblk),(index),(void far *)(name)): \
        c_service(F_DB_GETFIELDNAME,(void *)(dblk),index,(void far *)(name)))


/* Only returns index of fields that contain real data-- */
/* fielddefs for labels, etc. get returned as DB_NOFIELD */
int far _DB_GetFieldIndex(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk,char far *name);
#define DB_GetFieldIndex(dblk,name) \
        (DB_DIRECT(1)? \
        _DB_GetFieldIndex(DBSMCALL1 (dblk),(void far *)(name)) : \
        c_service(F_DB_GETFIELDINDEX,(void *)(dblk),(void far *)(name)))


/* DB_Order returns -1 if rec1<rec2, 1 if rec1>rec2, and 0 if rec1=rec2 */
int far _DB_Order(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM recnum1,char far *rec1,RECORDNUM recnum2, char far *rec2);
#define DB_Order(dblk,recnum1,rec1,recnum2,rec2) \
        (DB_DIRECT(1)? \
        _DB_Order(DBSMCALL1 (dblk),(recnum1),(void far *)(rec1),(recnum2),(void far *)(rec2)): \
        c_service(F_DB_ORDER,(void *)(dblk),(recnum1), \
                  (void far *)(rec1),(recnum2),(void far *)(rec2)))



/* DB_DefaultCallBack is the function the database engine uses itself */
/* for internal field types.  It functions exactly the same way that */
/* a user-defined FieldCallBack function would, i.e. it is called instead */
/* of the user callback if the field is a native type. */
int far _DB_DefaultCallBack(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, int action, int buffersize, int intarg, void far *arg1, void far *arg2);
#define DB_DefaultCallBack(dblk,action,buffersize,intarg,arg1,arg2) \
        ((DB_DIRECT((1*0)))? \
        _DB_DefaultCallBack(DBSMCALL1 (dblk),(action),(buffersize),(intarg),(void far *)(arg1),(void far *)(arg2)): \
        c_service(F_DB_DEFAULTCALLBACK,(void *)(dblk),(action), \
                  (buffersize),(intarg),(void far *)(arg1),(void far *)(arg2)))




/***** Searching / Filtering functions *****/

/* DB_ParseFISL returns the index of last correctly interpreted character
   in OutExpLen if the string is in error (ie. returns DB_COMPLEX, DB_OVERFLOW,
   or DB_SYNTAX);  otherwise, the length of the token string is returned.
   MAKE SURE that DB_Init has been called sometime (just once in main() is needed)
   before calling this function, or you might get strange results. */
int far _DB_ParseFISL(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char far *InFISL, char far *OutTOKEN,int OutBufSize,int APP_NEAR *OutExpLen);
#define DB_ParseFISL(dblk,InFISL,OutTOKEN,OutBufSize,OutExpLen) \
        (DB_DIRECT(2)? \
        _DB_ParseFISL(DBSMCALL1 (dblk),(void far *)(InFISL),(void far *)(OutTOKEN),(OutBufSize),(OutExpLen)): \
        c_service(F_DB_PARSEFISL,(void *)(dblk),(void far *)(InFISL), \
                  (void far *)(OutTOKEN),(OutBufSize),(void *)(OutExpLen)))


/* Tests record against given viewpt: returns 1 if in, 0 if not */
int far _DB_InViewpt(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, void far *src, VIEWPTDEF far * APP_NEAR *viewptdef);
#define DB_InViewpt(dblk,src,viewptdef) \
        (DB_DIRECT(1)? \
        _DB_InViewpt(DBSMCALL1 (dblk),(void far *)(src),(viewptdef)): \
        c_service(F_DB_INVIEWPT,(void *)(dblk),(void far *)(src), \
                  (void *)(viewptdef)))




/* Tests and modifies current viewpt table according to record at src and
     action code (action is a ModifyViewptType) */
int far _DB_ModifyViewpt(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM recordnum, void far *src, int action);
#define DB_ModifyViewpt(dblk,recordnum,src,action) \
        (DB_DIRECT(1)? \
        _DB_ModifyViewpt(DBSMCALL1 (dblk),(recordnum),(void far *)(src),(action)): \
        c_service(F_DB_MODIFYVIEWPT,(void *)(dblk),(recordnum),(void far *)(src), \
                  (action)))



/*** SetCurrentViewpt reads the specified viewpt definition, viewpt table,
 *** and sort definition records from disk ***/

int far _DB_SetCurrentViewpt(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM viewptnum);
#define DB_SetCurrentViewpt(dblk,viewptnum) \
        (DB_DIRECT(1)? \
        _DB_SetCurrentViewpt(DBSMCALL1 (dblk),(viewptnum)): \
        c_service(F_DB_SETCURRENTVIEWPT,(void *)(dblk),(viewptnum)))



int far _DB_StoreViewpt(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk,RECORDNUM viewptnum,VIEWPTDEF far *viewptdef);
#define DB_StoreViewpt(dblk,viewptnum,viewptdef) \
        (DB_DIRECT(1)? \
        _DB_StoreViewpt(DBSMCALL1 (dblk),(viewptnum),(void far *)(viewptdef)): \
        c_service(F_DB_STOREVIEWPT,(void *)(dblk),(viewptnum), \
                  (void far *)viewptdef))

/*
int far _DB_CancelViewpt(DBSMPREFIX1  DBBLOCK *dblk);       // Removes viewpoint filter
*/



int far _DB_FindRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char recordtype, char far *searchstring, RECORDNUM begin, RECORDNUM end, int flags, unsigned int far *pos);
#define DB_FindRecord(dblk,recordtype,searchstring,begin,end,flags,pos) \
        (DB_DIRECT(1)? \
        _DB_FindRecord(DBSMCALL1 (dblk),(recordtype),(void far *)(searchstring),(begin),(end),(flags),(void far *)(pos)): \
        c_service(F_DB_FINDRECORD,(void *)(dblk),(recordtype), \
                  (void far *)(searchstring),(begin),(end),(flags),(void far *)(pos)))



int far _DB_FindMemoryRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char far *buffer, char far *searchstring,int flags, int far *field, unsigned int far *pos);
#define DB_FindMemoryRecord(dblk,buffer,searchstring,flags,field,pos) \
        (DB_DIRECT(1)? \
        _DB_FindMemoryRecord(DBSMCALL1 (dblk),(void far *)(buffer),(void far *)(searchstring),(flags),(void far *)(field),(void far *)(pos)): \
        c_service(F_DB_FINDMEMORYRECORD,(void *)(dblk), (void far *)(buffer), \
                  (void far *)(searchstring),(flags),(void far *)(field),(void far *)(pos)))



int far _DB_FindDataRecord(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char far *searchstring, RECORDNUM begin, RECORDNUM end, int flags, int far *field, unsigned int far *pos);
#define DB_FindDataRecord(dblk,searchstring,begin,end,flags,field,pos) \
        (DB_DIRECT(1)? \
        _DB_FindDataRecord(DBSMCALL1 (dblk),(void far *)(searchstring),(begin),(end),(flags),(void far *)(field),(void far *)(pos)): \
        c_service(F_DB_FINDDATARECORD,(void *)(dblk), \
                  (void far *)(searchstring),(begin),(end),(flags),(void far *)(field),(void far *)(pos)))



int far _DB_AutoGoto(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char far *searchstring, RECORDNUM begin, RECORDNUM end);
#define DB_AutoGoto(dblk,searchstring,begin,end ) \
        (DB_DIRECT(2)? \
        _DB_AutoGoto(DBSMCALL1 (dblk),(void far *)(searchstring),(begin),(end )): \
        c_service(F_DB_AUTOGOTO,(void *)(dblk),(void far *)(searchstring), \
                  (begin),(end)))


/* Category functions */
/*** CheckCategory adds category entries to the specified category record if any entries
 *** are missing from the string.  Categories are separated by semicolons in the
 *** string, e.g. "Personal;Business" has two categegories.
 ***/
int far _DB_CheckCategory(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, RECORDNUM categoryrecord, char far *CategoryString);
#define DB_CheckCategory(dblk,categoryrecord,CategoryString) \
        (DB_DIRECT(2)? \
        _DB_CheckCategory(DBSMCALL1 (dblk),(categoryrecord),(CategoryString)): \
        c_service(F_DB_CHECKCATEGORY,(void *)(dblk),(categoryrecord), \
                  (void far *)(CategoryString)))



/***** "EZ" card filter translation functions ******/


int far _DB_CardToFISL(DBSMPREFIX1  DBBLOCK APP_NEAR *db, int field, char far *fielddata, char far *fislbuffer, int fislbuffersize);
#define DB_CardToFISL(db,field,fielddata,fislbuffer,fislbuffersize) \
        (DB_DIRECT(2)? \
        _DB_CardToFISL(DBSMCALL1 (db),(field),(void far *)(fielddata),(void far *)(fislbuffer),(fislbuffersize)): \
        c_service(F_DB_CARDTOFISL,(void *)(db),(field), \
                  (void far *)(fielddata), (void far *)(fislbuffer), (fislbuffersize)))



int far _DB_FISLToCard(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, int index, char far *fislbuffer, char far *fielddata,int fielddatasize);
#define DB_FISLToCard(db,index,fislbuffer,fielddata,fielddatasize) \
        (DB_DIRECT(2)? \
        _DB_FISLToCard(DBSMCALL1 (db),(index),(void far *)(fislbuffer),(void far *)(fielddata),(fielddatasize)): \
        c_service(F_DB_FISLTOCARD,(void *)(db),(index), \
                  (void far *)(fislbuffer), (void far *)(fielddata), (fielddatasize)))



int far _DB_FieldInFISL(DBSMPREFIX1  char far *InTOKEN, int tokenlen, int fieldcount, int far *fieldnumbers);
#define DB_FieldInFISL(InTOKEN,tokenlen, fieldcount,fieldnumbers) \
        (DB_DIRECT(2)? \
        _DB_FieldInFISL(DBSMCALL1 (void far *)(InTOKEN),(tokenlen),(fieldcount),(void far *)(fieldnumbers)): \
        c_service(F_DB_FIELDINFISL,(void far *)(InTOKEN),(tokenlen),(fieldcount), \
                  (void far *)(fieldnumbers)))

int far _DB_FISLFieldTranslate(DBSMPREFIX1  char far *InTOKEN, int tokenlen, int fieldcount, int far *oldfieldnumbers, int far *newfieldnumbers);
#define DB_FISLFieldTranslate(InTOKEN,tokenlen, fieldcount,oldfieldnumbers,newfieldnumbers) \
        (DB_DIRECT(2)? \
        _DB_FISLFieldTranslate(DBSMCALL1 (InTOKEN),(tokenlen),(fieldcount),(void far *)(oldfieldnumbers),(void far *)(newfieldnumbers)): \
        c_service(F_DB_FISLFIELDTRANSLATE,(void far *)(InTOKEN),(tokenlen),(fieldcount), \
                  (void far *)(oldfieldnumbers),(void far *)(newfieldnumbers)))


int far _DB_UnparseFISL(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk, char far *InTOKEN, int tokenlen, char far *OutFISL,int OutBufSize);
#define DB_UnparseFISL(dblk,InTOKEN,tokenlen, OutFISL,OutBufSize) \
        (DB_DIRECT(2)? \
        _DB_UnparseFISL(DBSMCALL1 (dblk),(void far *)(InTOKEN),(tokenlen),(void far *)(OutFISL),(OutBufSize)): \
        c_service(F_DB_UNPARSEFISL,(void *)(dblk),(void far *)(InTOKEN), (tokenlen),\
                  (void far *)(OutFISL),(OutBufSize)))

int far _DB_CardChanged(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_CardChanged(dblk) \
        (DB_DIRECT(2)? \
        _DB_CardChanged(DBSMCALL1 (dblk)): \
        c_service(F_DB_CARDCHANGED,(void *)(dblk)))

int far _DB_WinStrTo850(DBSMPREFIX1  char far *str);
#define DB_WinStrTo850(str) \
        (DB_DIRECT(2)? \
        _DB_WinStrTo850(DBSMCALL1 (str)): \
        c_service(F_DB_WINSTRTO850,(void far *)(str)))

int far _DB_WinGledTo850(DBSMPREFIX1  char far *str,int stringlength);
#define DB_WinGledTo850(str,stringlength) \
        (DB_DIRECT(2)? \
        _DB_WinGledTo850(DBSMCALL1 (str),(stringlength)): \
        c_service(F_DB_WINGLEDTO850,(void far *)(str),stringlength))


int far _DB_WinTranslate(DBSMPREFIX1  DBBLOCK APP_NEAR *dblk);
#define DB_WinTranslate(dblk) \
        (DB_DIRECT(2)? \
        _DB_WinTranslate(DBSMCALL1 (dblk)): \
        c_service(F_DB_WINTRANSLATE,(void *)(dblk)))


int far _DB_AdvanceFISL(DBSMPREFIX1  char far *fisl);
#define DB_AdvanceFISL(fisl) \
        (DB_DIRECT(2)? \
        _DB_AdvanceFISL(DBSMCALL1 (fisl)): \
        c_service(F_DB_ADVANCEFISL,(void far *)(fisl)))






/***** "Alias" functions *****/

#define DB_CancelViewpt(db)           DB_SetCurrentViewpt(db, 0)
#define DB_AddRecord(db,rt,sc,rs,vpi) DB_WriteRecord(db,rt,32767,sc,rs,vpi)
#define DB_CountRecords(db,rt)        ((db)->typecount[rt])
#define DB_UndoStatus(db)             ((db)->flags & DB_US_UNDO)
#define DB_GetNamedField(db,sr,nm,str,bs)  DB_GetField(db,sr,\
                                           DB_GetFieldIndex(db,nm),str,bs)
#define DB_SetUndoPoint(db)           DB_CollectGarbage(db)






/******************************************************************
 ** By default, the db engine allocates functions to SM_OVL5 if
 ** they are not declared in the pragma below.  ANY FUNCTIONS ADDED
 ** TO THE 2ND OVERLAY MUST APPEAR IN THIS PRAGMA.
 *****************************************************************/

#ifdef DB_INTERNAL

#pragma alloc_text(SM_OVL6,_DB_Init,_DB_Reset,_DB_Deactivate,_DB_Reactivate,\
                   _DB_IsSelected,_DB_Select,MoveBlock,SortLookup,_DB_CollectGarbage,\
                   _DB_Undo,_DB_SetUndoPoint_,EmitToken,CheckField,GetToken,\
                   Match,val,range,bool,notexp,andboolexp,expr,_DB_ParseFISL,\
                   TransformPassword,_DB_ChangePassword,_DB_VerifyPassword,\
                   _DB_AutoGoto,slstrcat,_DB_CardToFISL,_DB_FISLToCard,\
                   _DB_CompressDelete,_DB_WriteInPlace,_DB_NextNextRecord,\
                   _DB_CopyRecord,_DB_CheckCategory,_DB_FieldInFISL, \
                   _DB_FISLFieldTranslate, _DB_UnparseFISL, AdvanceFISL, \
                   PopString, PushString,comparecategory2,contains2,\
                   KeyAddr,qvc_Compare,VptQuickSort,_DB_CreateViewptInMemory,\
                   qvc_Swap,LoadViewptDef2,_DB_CardChanged,qvc_Random,\
                   _DB_WinStrTo850,_DB_WinGledTo850,_DB_AdvanceFISL,CompareAutoGoto,\
                   JollyGr,LittleUndoHeaderUpdateFunction)

#endif

#pragma pack()

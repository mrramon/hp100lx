/*  Module:  chtype.h  Version 1.6  */

#define BEGNAME		0x1	/* can be 1st character of variable name */
#define INNAME          0x2   	/* can be non-1st character in var name */
//#define LOWER           0x4     /* lower case a-z */
//#define UPPER           0x8     /* upper case A-Z */
#define DIGIT	       0x10	/* digit [0-9] */
#define SPACE	       0x20	/* space, tab, return, '\0' */
#define FISLNAME       0x40     /* characters allowable in fisl field names */
#define USALPHA      0x80     /* Is A-Z or a-z */

#define IsBeginName(c)  ( chtype[(unsigned char)c] & BEGNAME )
#define IsInName(c)     ( chtype[(unsigned char)c] & INNAME )
//#define IsLower(c)      ( chtype[(unsigned char)c] & LOWER )
//#define IsUpper(c)      ( chtype[(unsigned char)c] & UPPER )
#define IsDigit(c)      ( chtype[(unsigned char)c] & DIGIT )
#define IsSpace(c)      ( chtype[(unsigned char)c] & SPACE )
#define IsFISLName(c)   ( chtype[(unsigned char)c] & FISLNAME )
#define IsUSAlpha(c)    ( chtype[(unsigned char)c] & USALPHA)

unsigned int far toupper_(unsigned int c);
unsigned int far tolower_(unsigned int c);

#define ToUpper(c)      (toupper_(c))
#define ToLower(c)      (tolower_(c))

extern unsigned char chtype[];


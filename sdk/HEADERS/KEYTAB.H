/* SCCS info - Module keytab.h  Version 1.2  Date 03/03/93  Time 08:52:21 */
/* Version 1.6 */
/* keytab.h - keytables include file */

/***********************************************************************
* 
* #defines for keycodes. To accomodate both PC and breadboard keyboards,
* both F11 and Menu are used for the menu key, and O and On/C are 
* used for the On/C fcn. 
*
***********************************************************************/
#define ONEKEY		0x0031
#define TWOKEY		0x0032
#define THREEKEY	0x0033
#define FOURKEY		0x0034
#define FIVEKEY		0x0035
#define SIXKEY		0x0036
#define SEVENKEY	0x0037
#define EIGHTKEY	0x0038
#define NINEKEY		0x0039
#define ZEROKEY		0x0030
#define PERIODKEY	0x002e
#define COMMAKEY        0x002C
#define EKEY		0x0065
#define LPARENKEY	0x0028
#define RPARENKEY	0x0029
#define CHSKEY		0x006C
#define ENTERKEY	0x000d
#define EQUALKEY	0x003D
#define PERCENTKEY      0x0025
#define PLUSKEY		0x002b
#define MINUSKEY	0x002d
#define MULTIPLYKEY	0x002a
#define SHIFTPLUSKEY	0x0025
#define SHIFTMINUSKEY	0x005E
#define UNDERSCOREKEY   0x005F
#define SHIFTMULTKEY	0x0022
#define DIVIDEKEY 	0x002f
#define POWERKEY	0x005e
#define INVKEY		0x0078
#define SQRTKEY		0x007A
#define SWAPKEY		0x0063
#define RDOWNKEY	0x0076
#define STOKEY		0x006e
#define RCLKEY		0x006d
#define LASTKEY		0x0062
#define UPKEY		0x4800
#define DOWNKEY		0x5000
#define LEFTKEY		0x4b00
#define RIGHTKEY	0x4d00
#define PAGEUPKEY	0x4900
#define PAGEDOWNKEY	0x5100
#define HOMEKEY		0x4700
#define ENDKEY		0x4f00
#define OKEY		0x006F
#define ESCKEY		0x001b
#define TABKEY		0x0009
#define SPACEKEY	0x0020
#define BACKSPACEKEY    0x0008
#define DELKEY		0x5300
#define F1KEY		0x3b00
#define F2KEY		0x3c00
#define F3KEY		0x3d00
#define F4KEY		0x3e00
#define F5KEY		0x3f00
#define F6KEY		0x4000
#define F7KEY		0x4100
#define F8KEY		0x4200
#define F9KEY		0x4300
#define F10KEY          0x4400
#define SHIFTF1KEY      0x5400
#define SHIFTF2KEY      0x5500
#define SHIFTF3KEY      0x5600
#define SHIFTF4KEY      0x5700
#define SHIFTF5KEY      0x5800
#define SHIFTF6KEY      0x5900
#define SHIFTF7KEY      0x5A00
#define SHIFTF8KEY      0x5B00
#define SHIFTF9KEY      0x5C00
#define SHIFTF10KEY     0x5D00
#define MENUKEY         0xC800
#define SHIFTMENUKEY    0xC900
#define F11KEY		0x8500
#define QKEY		0x0071
/* defines the shifted Fkey of N in TVM */
#define SHIFTN		0x5900
#define GKEY		0x0067
#define CLIPKEY		0x0063

#ifdef MOTOWN
#define WKEY            0x0077
#endif

#define XKEY		0x0078
#define YKEY		0x0079
#define ATKEY		0x0040
#define SHIFTRPARENKEY  0x005c
#define INSERTKEY       0x5200
#define SHIFTTABKEY     0x0F00
#define CTRLF2KEY       0x5F00
#define CTRLF3KEY       0x6000
#define CTRLF4KEY       0x6100
#define CTRLHOMEKEY     0x7700
#define CTRLENDKEY      0x7500

#define ALTDOWNKEY	0xa000
#define ALTUPKEY	0x9800
#define BACKTABKEY	SHIFTTABKEY

#define CTRLUPKEY	0x8d00
#define CTRLDOWNKEY	0x9100
#define CTRLLEFTKEY	0x7300
#define CTRLRIGHTKEY	0x7400
#define CTRLENTERKEY    0x1C0A
#define ZOOMKEY         0xd000
#define DATEKEY         0xd100
#define TIMEKEY         0xd200
#define CTRLCKEY        0x0003
#define CTRLVKEY        0x0016
#define CTRLXKEY        0x0018
#define CTRLZKEY        0x001A
#define CUTKEY          0xD400
#define COPYKEY         0xD500
#define PASTEKEY        0xD600
#define SHIFTMAKEKEY    0x7E33
#define SHIFTBREAKKEY   0xFE33

/***********************************************************************
* 
* key tables
*
***********************************************************************/
extern int     StdKeyCodeTable[];
extern void    (*StdFcnTable[])(void);
extern int     SolveNeutralKeyTab[];
extern void    (*SolveNeutralFcnTable[])(void);
extern int     NumKeyCodeTable[];



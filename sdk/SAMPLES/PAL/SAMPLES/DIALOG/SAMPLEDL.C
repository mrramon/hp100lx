/* DIALOG RESOURCE FILE */

#include <stdlib.h>

#include "pal.h"
#include "sampledlg.h"

/*[PDE:SampleDlg]*/

IHCBINIT CBCBPALMTOP = {
{"HP200LX",  20, MEDIUM_FONT },
"HP95LX|HP100LX|HP200LX|HP400LX"
};


IHRAINIT RARABOTH = {
MEDIUM_FONT, 1
};


DLGITEM SampleDlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{  9, 10,152, 14,   0L, "&Name:",     NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 19, 50,182, 14,   0L, "&Palmtop:",  &CBCBPALMTOP,IhCombo , sizeof(IHCOMBO ), NULL },
{ 19, 70, 15, 15,   0L, "&Turbo'ed",  NULL,       IhCheck , sizeof(IHCHECK ), NULL },
{ 11, 30,372,104,   0L, "Palmtop &Info", NULL,       IhGroup , sizeof(IHGROUP ), NULL },
{269, 63,  0,  0,   1L, "&Dos",       NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{269, 80,  0,  0,   1L, "&Pim",       NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{269, 96,  0,  0,   1L, "&Both",      &RARABOTH,  IhRadio , sizeof(IHRADIO ), NULL },
{259, 41,114, 86,   0L, "Usage",     NULL,       IhGroup , sizeof(IHGROUP ), NULL },
{ 19,100, 60, 25,   0L, "&Help",      NULL,       IhButton, sizeof(IHBUTTON), NULL },
{342,  5, 40, 25, IF_RET, "&OK",        NULL,       IhButton, sizeof(IHBUTTON), NULL }
};

DIALOG SampleDlg = {
394, 154,
0L,
0,
10,
SampleDlgItems,
DhStandard,
NULL,
NULL,
0
};


/* Test file for sampledl.c resource. */

#include "pal.h"
#include "sampledl.h"

void main(void)
{
   int n;
   char Name[40];
   int  Turboed;
   int  Palmtop;
   int  Dos;
   int  Result;

   if(!PalInit(1)) FatalExit("Init failed - CGAGRAPH not loaded ?", 1);

   InitDialog(&SampleDlg);                       /* initialize dialog */
   ShowDialog(&SampleDlg, 50, 20, "Palmtop Poll"); /* display it */
   Result = HandleDialog(&SampleDlg, 0);         /* handle keys */

   /* now retrieve some values */
   GetDlgItem(&SampleDlg, EDNAME,    EDGI_TXT,   Name);
   GetDlgItem(&SampleDlg, CKTURBO,   CKGI_STATE, &Turboed);
   GetDlgItem(&SampleDlg, CBPALMTOP, CBGI_IDX,   &Palmtop);
   GetDlgItem(&SampleDlg, RADOS,     RAGI_STATE, &Dos);

   CloseDialog(&SampleDlg);
   PalDeInit(1);

   if(Result == DN_CANCEL) printf("You hit F9 or ESC\n");
   if(Result == DN_OK)     printf("You hit F10\n");
   if(Result == BNOK)      printf("You selected OK (or RETURN)\n");
   if(Result == BNHELP)    printf("You selected HELP\n");
   printf("Name entered: <%s>\n", Name);
   printf("Your palmtop is %sturboed\n", Turboed ? "" : "not ");
   printf("The index of your palmtop selection is %d\n", Palmtop);
   printf("You are %susing it for DOS programs\n", Dos ? "" : "not ");
}

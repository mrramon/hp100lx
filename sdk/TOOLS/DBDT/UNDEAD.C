/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
#include <stdio.h>
#include "\cougar\build\headers\interfac.h"
#include "db.h"
#include "db_pvt.h"
#include "\cougar\headers\4build\dosfile.h"

char StartupMsg[]="Undead Ver 1.0\nReconstructing database from DDT output.\n\n";

FILE *input;
FILE *output;

enum Errors {
  ERR_BADFILE,
  ERR_TOOMANYFILES,
};

char *errortext[]={
"Invalid or missing filename.",
"Too many filenames.",
};



void error(int n)
{
  printf("\n\nERROR: %s\n\n",errortext[n]);
  exit(1);
}


void ParseBuffer(char *buffer)
{
  int i;
  char ch;
  unsigned char hex;

  do {
    ch = *buffer;
    if (ch>='a' && ch<='f')  ch-=0x20;

    if ((ch>='0' && ch<='9') ||
       (ch>='A' && ch<='F')) {

       ch-='0';
       if (ch>9)  ch -= 7;
       hex = ch;

       ch = buffer[1];
       if (ch>='a' && ch<='f')  ch-=0x20;

       ch-='0';
       if (ch>9)  ch -= 7;
       hex = hex*16 + ch;

       fputc(hex, output);
       buffer += 3;
       }
     else
       return;
  } while (1);
}


void main(int argc, char *argv[])
{
  char buffer[256];
  char *neof;

  printf(StartupMsg);

  if (argc<3)  error(ERR_BADFILE);
  if (argc>3)  error(ERR_TOOMANYFILES);

  printf("Reading DDT Dump file %s\n",argv[1]);
  printf("Writing database file %s\n\n",argv[2]);

  input = fopen(argv[1],"rb");
  if (!input)   error(ERR_BADFILE);

  output = fopen(argv[2],"w+b");
  if (!output)  error(ERR_BADFILE);

  do {
    neof = fgets(buffer,sizeof(buffer),input);

    if (neof && strlen(buffer)>6 &&
        buffer[6]==':' && buffer[0]=='0') {   /* Data line */
      buffer[56] = 0;          /* Truncate stuff on end */
      ParseBuffer(buffer+8);   /* Pass in after the address */
      }
  } while (neof);

  fclose(output);
  fclose(input);

  printf("File ok.\n");
}

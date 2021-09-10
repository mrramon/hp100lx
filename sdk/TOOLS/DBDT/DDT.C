/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
#include <stdio.h>
#include <stdlib.h>
#include "\cougar\build\headers\interfac.h"
#include "db.h"
#include "db_pvt.h"
#include "\cougar\headers\4build\dosfile.h"

char StartupMsg[]="D.D.T. Ver 1.5\nDatabase Diagnostic Tool\n\n";

#define intat(x) (*((int *)(bigbuf+x)))

char bigbuf[32768];

enum Errors {
  ERR_BADFILE,
  ERR_TOOMANYFILES,
  ERR_BADSIG,
  ERR_APPT,
  ERR_SINGLE,
};

char *errortext[]={
"Invalid or missing filename.",
"Too many filenames.",
"File signature bad; file may not be database file.",
"Not enough memory to allocate Appointment Book Validation Structure.",
"Not enough memory to allocate Single Occurrence Structure.",
};

char *DayOfWeek[]={
"Mon","Tue","Wed","Thu","Fri","Sat","Sun"
};

char *MonthOfYear[]={
"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

enum Warnings {
WARN_UNKNOWNBITS,
WARN_TOOLONG,
WARN_TOOBIG,
WARN_TOOSHORT,
};

char *warningtext[]={
"Unrecognized bits.",
"File longer than expected (file header was not updated properly).",
"Record larger than 32K.",
"File shorter than expected (may be ROM record file).",
};


char *recname[]={
"TYPE_DBHEADER",
"TYPE_PASSWORD",
"TYPE_DEFAULT_DATA",
"TYPE_CONFIGREC",
"TYPE_CARDDEF",
"TYPE_CATEGORY",
"TYPE_FIELDDEF",
"TYPE_VIEWPTDEF",
"TYPE_PRINTDEF",
"TYPE_NOTE",
"TYPE_VIEWPTTABLE",
"TYPE_DATA",
"TYPE_LINKDEF",
"TYPE_CARDPAGEDEF",
"TYPE_USER"
};

int handle;
int junk;
long int fileseek;

char FileType;

typedef struct {
  int prev,next;
  } Links;

#pragma pack(1)
typedef struct {
  char date[3];
  int record;
  } SingleOccur;
#pragma pack()

Links       *ApLink;       /* Allocate 5041 which is max lookup size */
SingleOccur *ApSingle;

int MultiChain;
int SingleItems;


#define AddStatus(z,x)   if ((z) & (x))  {                \
    (z) &= ~(x);   if (buffer[0])  strcat(buffer," | ");  \
    strcat(buffer,"x");  }


void error(int n)
{
  printf("\n\nERROR: %s\n\n",errortext[n]);
  exit(1);
}

void warning(int n)
{
  printf("\nWARNING: %s\n\n",warningtext[n]);
}


int DumpRead(unsigned char *buffer,unsigned int bufsize)
{
  unsigned int i,x,j;

  if (bufsize>sizeof(bigbuf)) {
    warning(WARN_TOOBIG);
    DumpRead(buffer,sizeof(bigbuf));
    bufsize-=bigbuf;
    DumpRead(buffer,bufsize);
    return 1;
    }

  if (_dos_read(handle, buffer, bufsize, &junk)==-1)  {
    warning(WARN_TOOSHORT);
    return 0;
    }

  for (x=0,i=0; i<junk; i++) {
    if (x==0) printf("%06x: ",fileseek);
    printf("%02x ",buffer[x]);  x++;
    if (x>=16 || i==junk-1) {
      printf("%*s",52-x*3,"");
      for (j=0; j<x; j++)  printf("%c",(buffer[j]<' '?'.':buffer[j]));
      buffer+= x;
      fileseek+=x;
      x = 0;
      printf("\n");
      }
    }

  printf("\n");
  if (junk!=bufsize)  {
    warning(WARN_TOOSHORT);
    return 0;
    }
  return 1;
}


void LabelSection(char *secname,int sep)
{
  if (sep)  printf("=====================================================\n");
  printf("%s>>\n",secname);
}


void DumpHeader(RECORDHEADER *rh)
{
  char buffer[17];
  char *str;

  if (rh->type==DB_TYPES-1) {
    strcpy(buffer,"TYPE_LOOKUPTABLE");
    str = buffer;
    }
  else if (rh->type>TYPE_USER) {
    sprintf(buffer,"TYPE_USER+%d",rh->type-TYPE_USER);
    str = buffer;
    }
  else {
    str = recname[rh->type];
    }

  printf("  Type: %d (%s)   Status: %02x    Length: %d (%04x)   Record #: %d\n",
         rh->type,str,rh->status,rh->length,rh->length,rh->record);
  printf("  Status =");
  if (rh->status & STATUS_GARBAGE)  printf(" DELETED");
  if (rh->status & STATUS_MODIFIED)  printf(" Modified");
  printf("\n\n");
}



void CheckSig(void)
{
  unsigned char sig[4];

  LabelSection("SIGNATURE",1);
  DumpRead(sig,sizeof(sig));
  if (sig[0]!='h' || sig[1]!='c' || sig[2]!='D' || sig[3]!=0)  error(ERR_BADSIG);
}


void DumpDataRecord(void)
{


}


void DumpApptBookRecord(int n)
{
  int type;
  int i,x,y;
  char *str,*str2;

  type = (bigbuf[14]&0x10);          /* 1 = ToDo, 0 = Appt */
  switch (bigbuf[26]) {
    case 1:
      str2="Non-Repeating ";
      str="";
      break;
    case 2:
      str2="Daily Repeating ";
      str = "Days";
      break;
    case 4:
      str2="Weekly Repeating ";
      str = "Weeks";
      break;
    case 8:
      str2="Monthly Repeating ";
      str = "Months";
      break;
    case 16:
      str2="Yearly Repeating ";
      str = "Years";
      break;
    case 32:
      str2="Custom Repeating ";
      str = "(Custom)";
      break;
    default:
      str2="UNKNOWN REPEAT TYPE";
      str = "??";
    }
  if (type)  printf("---------%s ToDo--------------\n",str2);
       else  printf("---------%s Appointment-------\n",str2);

  printf("THIS RECORD [%d]    PREVIOUS RECORD [%d]    NEXT RECORD [%d]\n",n,intat(10),intat(12));

  printf("Length: %d   ", intat(0) );
  printf("Description: \"%s\"   Location: \"%s\"\n",bigbuf+27,bigbuf + intat(4));
  printf("Note Record: %d\n",intat(8));

  ApLink[n].prev = intat(10);
  ApLink[n].next = intat(12);

  printf("Flags: ");
  if (!type) {
    if (bigbuf[14]&1)  printf("[Alarm Enabled]   ");
    if (bigbuf[14]&2)  printf("[Month View]   ");
    if (bigbuf[14]&4)  printf("[Week View]   ");
    if (bigbuf[14]&128)  printf("[Appointment]   ");
    }
  else {
    if (bigbuf[14]&2)  printf("[Checked off]  ");
    if (bigbuf[14]&4)  printf("[Carry Forward]  ");
    if (bigbuf[14]&16)  printf("[To Do]  ");
    if (bigbuf[14]&64)  printf("[Stub]  ");
    }
  printf("\n");

  printf("Start Date: %d/%d/%d   ",bigbuf[16]+1,bigbuf[17]+1,bigbuf[15]+1900);
  if (type) {
    printf("Priority: %c%c",bigbuf[18],bigbuf[19]);
    printf("  # Days:%d",intat(20));

    /* Its a repeating ToDo, and it isn't a stub */
    if ((bigbuf[26])!=1 && !(bigbuf[14]&64))
        printf("  FIRST STUB: [%d]\n",intat(22));
     else
        printf("\n");
   }
  else {
    printf("Start Time: %d:%d\n",intat(18)/60,intat(18)%60);
    printf("# Days:%d   End Time: %d:%d   Lead Time: %d\n",intat(20),intat(22)/60,intat(22)%60,intat(24));
   }

 if (bigbuf[26]!=1) {
   printf("Repeat Factor : Every %d %s,    ",bigbuf[intat(6)],str);
   x = bigbuf[intat(6)+1];
   if (x&128)  {
     for (i=0; i<=6; i++) {
       if (x&(1<<i))  printf("%s ",DayOfWeek[i]);
       }
     }
   else
     printf("Day # %d  ",x);

   y = intat(intat(6)+2);
   if (y==0)
     printf("All Months");
   else {
     for (i=0; i<=11; i++) {
       if (y&(1<<i))  printf("%s ",MonthOfYear[i]);
       }
     }

   x = intat(6)+5;
   printf("\nDuration : %d/%d/%d  to  %d/%d/%d ",bigbuf[x+1]+1,bigbuf[x+2]+1,bigbuf[x]+1900,
                                               bigbuf[x+4]+1,bigbuf[x+5]+1,bigbuf[x+3]+1900);
   x += 6;
   y = bigbuf[x];
   if ((bigbuf[14]&64)) y = 0;
   printf("      %d Deleted Occurences\n",y);
   x++;
   for (i=0; i<y; i++) {
     printf("  %d/%d/%d  %s\n",bigbuf[x+1]+1,bigbuf[x+2]+1,bigbuf[x]+1900,bigbuf[x+3]?"DELETED":"STUB");
     x+=4;
     }

   if (type && (bigbuf[14]&64)) {
     printf("  ** STUB ** Previous Record:[%d]  Next Record:[%d]  Spawner:[%d]\n",
            intat(x),intat(x+2),intat(x+4));
     }
   }
  printf("\n\n");
}


void FollowChain(int x)
{
  int y,z=0;

  if (ApLink[x].prev != -1)  printf("(NO HEAD!)");
  do {
    y = ApLink[x].next;
    if (y!=-1) {
      if (ApLink[y].prev != x)  printf("(Y IN LINK!)");
      }
    printf("%d",x);
    if (++z>20)  {
      z=0;  printf("\n   ");
      }
    ApLink[x].prev = -1;  ApLink[x].next = -1;
    x = y;
  } while (y>=0 && y<=5041);

  if (y!=-1)  printf("(NO TAIL!)");
}


void DumpApptInfo(void)
{
  printf("*** Appointment Book Info Structure ***\n");
  printf("Today: %d/%d/%d   Settings: 0x%x   BeepSettings: %x\n",
         bigbuf[1]+1,bigbuf[2]+1,bigbuf[0]+1900,intat(3),intat(5));
  printf("Composite View: %x   First Hour: %d   Duration: %d\n",intat(7),intat(9)/60,intat(11));
  printf("Lead Time: %d   Priority: %c%c",intat(13),bigbuf[15],bigbuf[16]);
  printf("\n");
  printf("Event Lines: %d  Single Occur. Days: %d\n",intat(21),intat(19));
  printf("Multi Occurrence Chain Start : (%d)\n",intat(17));
  MultiChain = intat(17);
  printf("\n");
}


void DumpSingleTable(int n)
{
  printf("*** Single Occurence Table ***\n");

  SingleItems = n;

  if (n) {
    ApSingle=malloc(n*sizeof(SingleOccur));
    if (!ApSingle)  error(ERR_SINGLE);
    }

  memcpy(ApSingle,bigbuf,5*n);
}


void FollowSingleChain(void)
{
  int i,z;

  for (i=0; i<SingleItems; i++) {
    z = ApSingle[i].record;
    printf("  Day %d/%d/%d   Chain (%d) START",ApSingle[i].date[1]+1,ApSingle[i].date[2]+1,ApSingle[i].date[0]+1900,z);
    FollowChain(z);
    printf("\n");
    }
}



int CheckHeader(void)
{
  DBHEADER header;
  char buffer[80];
  int i;

  LabelSection("HEADER",1);
  DumpRead((char *)&header,sizeof(header));

  LabelSection("RECORD HEADER",0);
  DumpHeader(&header.rhdr);

  printf("  Release number:%03x   File type:'%c'  File status:%02x\n",
         header.releasenum,header.filetype,header.filestatus);
  FileType = header.filetype;
  switch (FileType) {
    case '2':
       ApLink = malloc(5041*sizeof(Links));
       if (!ApLink)  error(ERR_APPT);
       for (i=0; i<5041; i++) {
         ApLink[i].prev = -1;
         ApLink[i].next = -1;
         }
       printf("  ************ Appointment Book File *************\n");
       break;
    case 'W':  printf("  ************ World Time File **************\n"); break;
    case 'D':  printf("  ************ General Database/Phone Book File ************\n"); break;
    case 'N':  printf("  ************ Notetaker File *************\n"); break;
    }

  buffer[0] = 0;
  AddStatus(header.filestatus,STATUS_DBACTIVE);
  AddStatus(header.filestatus,STATUS_MODIFIED);

  if (header.filestatus)  warning(WARN_UNKNOWNBITS);
  if (buffer[0])  printf("  (File status = %s)\n",buffer);

  printf("  Current viewpt:%d   Number records:%d   Lookup seek address:%06x\n",
         header.curr_viewpt,header.numrecords,header.lookupseek);

  printf("  Last reconciled on: %d/%d/%d  %d:%02d\n",
         header.lastreconciled.date[1]+1,header.lastreconciled.date[2]+1,header.lastreconciled.date[0],
         header.lastreconciled.time/60,header.lastreconciled.time%60);

  printf("  Magic Viewpt Sort Key:0x%04x\n\n",header.magicviewptsort);

  return header.numrecords;
}


int CheckRecord(void)
{
  RECORDHEADER rh;

  LabelSection("RECORD HEADER",1);
  if (!DumpRead((char *)&rh,sizeof(rh)))  return 0;

  DumpHeader(&rh);

  switch (rh.type) {
//    case TYPE_PASSWORD:
//    case TYPE_DEFAULT_DATA:
//    case TYPE_CONFIGREC:
//    case TYPE_CARDDEF:
//    case TYPE_CATEGORY:
//    case TYPE_PRINTDEF:
//    case TYPE_VIEWPTTABLE:
//    case TYPE_FIELDDEF:
//    case TYPE_VIEWPTDEF:
//      break;
    case TYPE_DATA:
      LabelSection("RECORD",0);
      if (DumpRead(bigbuf,rh.length-sizeof(rh))) {
        switch (FileType) {
          case '2':
            DumpApptBookRecord(rh.record);
            break;
          default:
            DumpDataRecord();
            break;
          }
        }
      break;
    case TYPE_LOOKUPTABLE:
      LabelSection("RECORD",0);
      DumpRead(bigbuf,rh.length-sizeof(rh));
      LabelSection("TYPE FIRST INFORMATION",0);
      DumpRead(bigbuf,sizeof(int)*DB_TYPES);
      break;
    case TYPE_USER:
      if (FileType=='2') {
        LabelSection("RECORD",0);
        DumpRead(bigbuf,rh.length-sizeof(rh));
        if (rh.record)  DumpSingleTable((rh.length-sizeof(rh))/5);
                  else  DumpApptInfo();
        break;
        }
/* ELSE FALL THROUGH TO DEFAULT CASE */
    default:
      LabelSection("RECORD",0);
      DumpRead(bigbuf,rh.length-sizeof(rh));
   }

  return 1;
}


void main(int argc, char *argv[])
{
  int i,n;
  long int pos;
  long int size;

  printf(StartupMsg);

  if (argc<2)  error(ERR_BADFILE);
  if (argc>2)  error(ERR_TOOMANYFILES);

  printf("Reading Database file %s\n\n",argv[1]);

  if (_dos_open(argv[1],O_RDONLY,&handle)==-1)  error(ERR_BADFILE);

  size = _dos_lseek(handle, 0L, SEEK_END);
  _dos_lseek(handle, 0L, SEEK_SET);

  fileseek = 0L;
  CheckSig();
  n=CheckHeader();

  for (i=0; i<n-1; i++)  {
    if (!CheckRecord())  goto AllDone;
    }

  do {
    pos = _dos_lseek(handle, 0L, SEEK_CUR);
    if (pos!=size) {
      warning(WARN_TOOLONG);
      CheckRecord();
      }
  } while (pos<size);

AllDone:
  _dos_close(handle);

  if (FileType=='2') {
    printf("\n\nMulti Occurrence Chain\n");
    printf("START");
    FollowChain(MultiChain);

    printf("\n\nSingle Occurrence Chain\n");
    printf("START");
    FollowSingleChain();

    printf("\n\n");

    for (n=0,i=0; i<5041; i++) {
      if (ApLink[i].prev!=-1 || ApLink[i].next!=-1)  {
        n=1;
        printf("Record %d ABSENT FROM ANY CHAIN!\n",i);
        }
      }
    }

  printf("File ok.\n");
}

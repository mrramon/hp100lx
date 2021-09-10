/*
   Add note about what things this is not likely to fix (passwords, etc.)
   & should merge afterwards

   Potentially recover damaged data records

*/


/* SCCS info - Module %M%  Version %I%  Date %G%  Time %U% */
#include <stdio.h>
#include <stdlib.h>
#include "\cougar\build\headers\interfac.h"
#include "db.h"
#include "db_pvt.h"
#include "\cougar\headers\4build\dosfile.h"

char StartupMsg[]="Garlic Ver 1.2\nDamaged Database Reconstructor\n";

#define intat(x) (*((int *)(bigbuf+x+6)))

char bigbuf[32768+6];

char signature[4]="hcD";

enum Errors {
  ERR_BADFILE,
  ERR_TOOMANYFILES,
  ERR_BADSIG,
  ERR_APPT,
  ERR_SINGLE,
  ERR_VLOOKUP,
  ERR_TOOBIG,
  ERR_BUMMER,
  ERR_READ,
  ERR_WRITE,
  ERR_MULTI,
  ERR_NOTE,
};

char *errortext[]={
"Invalid or missing filename.\n\nSyntax is: GARLIC [/D] [/V] BADFILE.?DB FIXDFILE.?DB\nThe /D switch controls full debugging output.\nThe /V switch removes all subsets.\n",
"Too many filenames.",
"File signature bad; file may not be database file.",
"Not enough memory to allocate Appointment Book Validation Structure.",
"Not enough memory to allocate Single Occurrence patch matrix.",
"Not enough memory to allocate virtual lookup table.",
"Virtual lookup table exceeded max allocation.",
"File is too damaged to patch virtual lookup table.",
"Cannot read section of damaged file.",
"Cannot write to section of reconstructed file.",
"Not enough memory to allocate Multiple Occurrence patch matrix.",
"Not enough memory to allocate Note record table.",
};


char DefaultApptInfo[29]={
0x0e,0x02,0x1d,0x00,0x00,0x00,
0x00,0x00,0x00,0xe4,0x0f,0x14,0x00,0x06,0x06,0xe0,0x01,0x3c,0x00,0x05,0x00,0x31,
0x00,0xff,0xff,0x00,0x00,0x03,0x00,
};



char DefaultViewptInfo[]={
0x07,0x02,0x62,0x00,0x00,0x00,

0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x41,0x6c,0x6c,0x20,0x44,0x61,0x74,0x61,0x62,
0x61,0x73,0x65,0x20,0x49,0x74,0x65,0x6d,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x19,0x01,0x19,0x02,0x0e,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

0x0a,0x02,0x08,0x00,0x00,0x00,
0xFF,0xFF,
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
"TYPE_USER",
"TYPE_USER+1",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"ERROR!",
"TYPE_LOOKUP"
};

int handle,handle2;
int junk;
long int fileseek;
long int filesize;
int Debug;
int ZapViewpts;

char FileType;


#pragma pack(1)
typedef struct {
  char date[3];
  int record;
  } SingleOccur;
#pragma pack()

typedef struct {
  long int offset;
  int record;
  unsigned int size;
  char type;
  char status;
  } VLOOKUP;


SingleOccur *single;
int scount;

int *multi;
int mcount;

int *noterec;
int ncount;
int noteoffset;

int vsize;
VLOOKUP     *vlookup;      /* Virtual lookup table used in initial pass */


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

int ReadBuf(unsigned char *buffer,unsigned int bufsize)
{
  unsigned int i,x,j;

  if (bufsize>sizeof(bigbuf)) {
    warning(WARN_TOOBIG);
    ReadBuf(buffer,sizeof(bigbuf));
    bufsize-=bigbuf;
    ReadBuf(buffer,bufsize);
    return 1;
    }

  if (_dos_read(handle, buffer, bufsize, &junk)==-1)  {
    warning(WARN_TOOSHORT);
    return 0;
    }

  fileseek += junk;

  if (junk!=bufsize)  {
    warning(WARN_TOOSHORT);
    return 0;
    }
  return 1;
}




void CheckSig(void)
{
  unsigned char sig[4];

  ReadBuf(sig,sizeof(sig));
  if (sig[0]!='h' || sig[1]!='c' || sig[2]!='D' || sig[3]!=0)  error(ERR_BADSIG);
}



int CheckHeader(void)
{
  DBHEADER header;
  char buffer[80];
  int i;

  ReadBuf((char *)&header,sizeof(header));
  FileType = header.filetype;

  if (Debug) {
    printf("  Release number:%03x   File type:'%c'  File status:%02x\n",
           header.releasenum,FileType,header.filestatus);
    switch (FileType) {
      case '2':  printf("  ************ Appointment Book File *************\n"); break;
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
    }

  return header.numrecords;
}



int PotentialRH(RECORDHEADER *rh)
{
  if (rh->record>5041 || rh->record<0)  return 0;
  if (rh->status<0 || rh->status>3)  return 0;
  if (rh->length>=32767+sizeof(RECORDHEADER))  return 0;
  if (rh->type>TYPE_USER+1 && rh->type<TYPE_LOOKUPTABLE) return 0;
  if (rh->type>TYPE_LOOKUPTABLE || rh->type<0)  return 0;
  if (rh->type!=TYPE_NOTE && rh->length==0)  return 0;
  switch (rh->type) {
    case TYPE_USER:
      if (rh->record>1) return 0;
      break;
    case TYPE_USER+1:
    case TYPE_LINKDEF:
      if (rh->record>100) return 0;    /* Arbitrary--just to weed out bad stuff */
      break;
    case TYPE_DBHEADER:
      if (fileseek!=4)  return 0;
      break;
    case TYPE_PASSWORD:
      if (rh->length != 17+6)  return 0;
      if (rh->record!=0)  return 0;
      break;
    case TYPE_CATEGORY:
      if (rh->record!=0)  return 0;
      if (rh->length>256+6)  return 0;
      break;
    case TYPE_CARDDEF:
    case TYPE_CARDPAGEDEF:
      if (rh->record!=0)  return 0;
      break;
    case TYPE_LOOKUPTABLE:
      if (rh->record!=0)  return 0;
      if ((rh->length-6)%8)  return 0;
      rh->length += 64;                /* Account for type-first info */
      break;
    case TYPE_VIEWPTTABLE:
      if (rh->record<0 || rh->record>16)  return 0;
      if (rh->length & 1  ||
          rh->length > 5041*2+6)  return 0;
      break;
    case TYPE_VIEWPTDEF:
      if (rh->record<0 || rh->record>16)  return 0;
      if (rh->length>1024+6)  return 0;
      break;
    case TYPE_FIELDDEF:
      if (rh->record<0 || rh->record>100) return 0;
      if (rh->length<8+6 || rh->length>28+6)  return 0;
      break;
    case TYPE_DATA:
      if (rh->length>2048+6)  return 0;
      break;
    case TYPE_DEFAULT_DATA:
    case TYPE_CONFIGREC:
    case TYPE_PRINTDEF:
      return 0;
    }

  return 1;
}


int AddPotentialRH(RECORDHEADER *rh)
{
  vlookup[vsize].offset = fileseek;
  vlookup[vsize].record = rh->record;
  vlookup[vsize].type   = rh->type;
  vlookup[vsize].size   = rh->length;
  vlookup[vsize].status = rh->status;
  if (++vsize==5042)  error(ERR_TOOBIG);
}


void SpitRH(RECORDHEADER *rh)
{
  if (Debug)
    printf("%06lx: %s (%d) Len=%d\n",fileseek,recname[rh->type],rh->record,rh->length);
}



ConstructVirtualLookup(void)
{
  RECORDHEADER rh;
  long int x;

  vsize=0;
  vlookup=malloc(5041*sizeof(VLOOKUP));
  if (!vlookup)  error(ERR_VLOOKUP);

  _dos_lseek(handle, 0L, SEEK_SET);
  fileseek = 0L;

  CheckSig();
  CheckHeader();

  printf("Scanning possible record headers...\n");
  for (x=4; x<filesize-sizeof(RECORDHEADER); x++) {
    _dos_lseek(handle, x,  SEEK_SET);
    ReadBuf((char *)&rh,sizeof(rh));
    fileseek = x;
    if (PotentialRH(&rh))  {
      AddPotentialRH(&rh);
      SpitRH(&rh);
      }
    }

}

void DeleteVLE(int x)
{
  int i;

  for (i=x; i<vsize; i++)
    vlookup[i] = vlookup[i+1];

  vsize--;
}

void PatchVirtualLookup(void)
{
  int t,b,i,j,x;
  int cns_nfds;         /* Number of consecutive non-fielddef records */

  printf("Patching virtual lookup...\n");

  /* Remove any records embedded in the TYPE_FIELDDEF records... */
  cns_nfds=t=b=0;

  /* The 4 below is an arbitrary constant designed to prevent throwing
     away tons of data between real fielddefs and a fake-id'ed fielddef
     way into the file.  Only 4 garbage records can be between fielddef
     records before we give up. */
  for (i=0; i<vsize && cns_nfds<4; i++) {
    if (vlookup[i].type==TYPE_FIELDDEF)  {
      if (!t)  t=i;      /* Get first fielddef */
      b=i;               /* Get last fielddef */
      cns_nfds=0;        /* Reset counter of non-fielddef records */
      }
    else
      cns_nfds++;      /* If it isn't a TYPE_FIELDDEF, increment counter */
    }

  if (!b || !t)  error(ERR_BUMMER);

  i=t;
  while (i<=b) {
    if (vlookup[i].type!=TYPE_FIELDDEF) {
      if (Debug)
        printf("At %06lx: removing %s embedded in fielddefs.\n",vlookup[i].offset,recname[vlookup[i].type]);

      DeleteVLE(i);
      b--;
      }
    else
      i++;
    }

  fileseek = 4L;

  for (i=0; i<vsize; i++) {
    if (fileseek<vlookup[i].offset)  {
      if (Debug)
         printf("At %06lx: %ld garbage bytes\n",fileseek,vlookup[i].offset-fileseek);

      fileseek = vlookup[i].offset+vlookup[i].size;;
      }
    else if (fileseek>vlookup[i].offset) {
      if (Debug)
         printf("At %06lx: %ld byte overrun",fileseek,fileseek-vlookup[i].offset);

      x=0;
      for (j=1; i+j<vsize; j++) if (fileseek==vlookup[i+j].offset) {
        x=j; break;
        }
      if (fileseek==filesize)  x = vsize-i;

      if (x) {
        if (Debug)
           printf(" (corrected by deleting %d virtual item(s)).\n",x);

        while (x--) DeleteVLE(i);
        i--;               /* Don't skip this next item */
        }
      else {
        if (Debug)  printf(".\n");
        }
      }
    else {
      fileseek+=vlookup[i].size;
      }
    }

  for (i=0; i<vsize; i++) {
    if (vlookup[i].status & STATUS_GARBAGE) {
      if (Debug)
         printf("At %06lx: %ld byte garbage record\n",vlookup[i].offset);

      DeleteVLE(i);
      i--;
      }
    }

  printf("Virtual lookup table patched.\n");

  if (Debug) {
    for (i=0; i<vsize; i++) {
      printf("%06lx: %s (%d) Len=%d\n",vlookup[i].offset,recname[vlookup[i].type],vlookup[i].record,vlookup[i].size);
      }
    }
}

void ElimDups(void)
{
  int i,j;

  printf("Eliminating duplicate record id's...\n");

  for (i=0; i<vsize; i++) {
    for (j=i+1; j<vsize; j++) {
      if (vlookup[i].record==vlookup[j].record &&
          vlookup[i].type==vlookup[j].type) {
         if (Debug)
             printf("Records %d (at %06lx) and %d (at %06lx--to be deleted) have identical record numbers.\n",
                    i,vlookup[i].offset,j,vlookup[j].offset);

         DeleteVLE(j);
         j--;           
         }
      }
    }

}




int IsMultiItem(void)
{
  int todo = bigbuf[14+6]&0x10;         /* 1 = todo, 0 = appt */

  if (!todo && bigbuf[26+6]==1)  return 0;

  return 1;
}


void ReconstructMulti(void)
{
  int i,mfound=0;

  printf("Reconstructing multiple occurrence chain...\n");

  for (i=0; i<vsize; i++) {
    if (vlookup[i].type==TYPE_USER && vlookup[i].record==0)  {
      mfound=1;
      break;
      }
    }

  if (!mfound) {
    printf("No appointment book info structure; one being added.\n");
    vlookup[vsize].type=TYPE_USER;
    vlookup[vsize].record=0;
    vlookup[vsize].offset=0;
    vsize++;
    }

  multi = malloc(5041*sizeof(int));
  if (!multi)  error(ERR_MULTI);
  mcount=0;

  for (i=0; i<vsize; i++) {
    if (vlookup[i].type==TYPE_DATA) {
      _dos_lseek(handle, vlookup[i].offset, SEEK_SET);
      if (_dos_read(handle, bigbuf, vlookup[i].size, &junk)==-1 || junk!=vlookup[i].size)
        error(ERR_READ);

      if (IsMultiItem()) multi[mcount++] = vlookup[i].record;
      }
    }

  /* Print out multi-occurrence table */
  if (Debug) {
    for (i=0; i<mcount; i++)
      printf("Record:%d\n",multi[i]);
    }
}


int DateLess(int x,int y)
{
  if (single[x].date[0]<single[y].date[0])
    return 1;
  else if (single[x].date[0]==single[y].date[0]) {
    if (single[x].date[1]<single[y].date[1])
      return 1;
    else if (single[x].date[1]==single[y].date[1]) {
      if (single[x].date[2]<single[y].date[2])  return 1;
      }
    }
  return 0;
}

int DateEqual(int x,int y)
{
  if (single[x].date[0]==single[y].date[0] &&
      single[x].date[1]==single[y].date[1] &&
      single[x].date[2]==single[y].date[2])  return 1;

  return 0;
}

/* Takes item at bottom of single occurrence table and swaps up until it
   is sorted in place */
void sort_in_single(void)
{
  int n;
  SingleOccur temp;

  n = scount-1;

  while (n>0 && DateLess(n,n-1)) {
    temp = single[n-1];
    single[n-1] = single[n];
    single[n] = temp;
    n--;
    }
}



void ReconstructSingle(void)
{
  int i;

  printf("Reconstructing single occurrence table...\n");

  for (i=0; i<vsize; i++) {
    if (vlookup[i].type==TYPE_USER && vlookup[i].record>=1)  {
      printf("Removing old single occurrence table.\n");
      DeleteVLE(i);
      i--;
      break;
      }
    }


  single = malloc(5041*sizeof(SingleOccur));
  if (!single)  error(ERR_SINGLE);
  scount=0;

  for (i=0; i<vsize; i++) {
    if (vlookup[i].type==TYPE_DATA) {
      _dos_lseek(handle, vlookup[i].offset, SEEK_SET);
      if (_dos_read(handle, bigbuf, vlookup[i].size, &junk)==-1 || junk!=vlookup[i].size)
        error(ERR_READ);

      if (!IsMultiItem()) {
        single[scount].record = vlookup[i].record;
        single[scount].date[0] = bigbuf[15+6];
        single[scount].date[1] = bigbuf[16+6];
        single[scount].date[2] = bigbuf[17+6];
        scount++;
        sort_in_single();
        }
      }
    }

  /* Print out single occurrence table */
  if (Debug) {
    for (i=0; i<scount; i++) {
      printf("Date:%d/%d/%d  Record:%d\n",single[i].date[1]+1,single[i].date[2]+1,single[i].date[0]+1900,single[i].record);
      }
    }

  /* Reinstate single occurrence table */

  vlookup[vsize].type=TYPE_USER;
  vlookup[vsize].record=1;
  vlookup[vsize].offset=0;
  vsize++;
}

int FindNote(int n)
{
  int i;

  for (i=0; i<ncount; i++)  if (noterec[i]==n)  return i;

  return -1;
}

void PatchNotes(void)
{
  int i,j;
  int n;
  int bytes;

  noteoffset=-1;

  printf("Creating note table...\n");

  ncount=0;

  /* Get note address in data record from field defs */
  for (i=0; i<vsize; i++) if (vlookup[i].type==TYPE_FIELDDEF) {
    bytes = vlookup[i].size;
    if (bytes && vlookup[i].offset) {
      _dos_lseek(handle, vlookup[i].offset, SEEK_SET);
      if (_dos_read(handle, bigbuf, bytes, &junk)==-1 || junk!=bytes)
        error(ERR_READ);

      if (bigbuf[0+6]==NOTE_FIELD)  noteoffset = intat(2);
      }
    }

  if (noteoffset==-1)  {
    printf("No note record.\n");
    return;
    }

  noterec = malloc(5041*sizeof(int));
  if (!noterec)  error(ERR_NOTE);

  /* Set up noterec array */
  for (i=0; i<vsize; i++) if (vlookup[i].type==TYPE_NOTE) {
    noterec[ncount++] = vlookup[i].record;
    }

  /* Look through data records to find any unmatched notes */
  for (i=0; i<vsize; i++) if (vlookup[i].type==TYPE_DATA) {
    bytes = vlookup[i].size;
    if (bytes && vlookup[i].offset) {
      _dos_lseek(handle, vlookup[i].offset, SEEK_SET);
      if (_dos_read(handle, bigbuf, bytes, &junk)==-1 || junk!=bytes)
        error(ERR_READ);

      /* Get note record */
      n = intat(noteoffset);

      if (Debug) printf("Data record %d has note %d\n",vlookup[i].record,n);

      if (n!=-1) {
        if (FindNote(n)==-1) {
          if (Debug)  printf("NOTE %d DOES NOT EXIST!\n",n);
          }
        else
          noterec[FindNote(n)] = -1;     /* Zap that note out of the table */
        }
      }
    }

  /* Any notes left in the table are unattached notes--print and delete them */
  for (i=0; i<ncount; i++) if (noterec[i]!=-1) {
    for (n=0; n<vsize; n++)  if (vlookup[n].type==TYPE_NOTE && vlookup[n].record==noterec[i])  break;

    if (n<vsize) {
      if (Debug) {
        printf("Note %d is unattached; contents are :\n",noterec[i]);
        bytes = vlookup[n].size;
        if (bytes && vlookup[n].offset && bytes<32767) {
          _dos_lseek(handle, vlookup[n].offset, SEEK_SET);
          if (_dos_read(handle, bigbuf, bytes, &junk)==-1 || junk!=bytes)
            error(ERR_READ);

          for (j=0; j<bytes; j++) putch(bigbuf[j]);
          }
        printf("\n");
        }

      DeleteVLE(n);
      }
    }

  /* Restore noterec array to new notes without unattached */
  for (ncount=0,i=0; i<vsize; i++) if (vlookup[i].type==TYPE_NOTE) {
    noterec[ncount++] = vlookup[i].record;
    }
}



int FindInMultiTable(int n)
{
  int i;

  for (i=0; i<mcount; i++)  if (multi[i]==n)  return i;

  return -1;
}

int FindInSingleTable(int n)
{
  int i;

  for (i=0; i<scount; i++)  if (single[i].record==n)  return i;

  return -1;
}

void PatchApptRecord(int n)
{
  int x,t,b;

  if (IsMultiItem()) {
    x = FindInMultiTable(n);
    if (x==-1)  {
      printf("Cannot find record %d in multi occurrence table.\n",n);
      error(ERR_BUMMER);
      }

    if (x==0) intat(10) = -1;
         else intat(10) = multi[x-1];

    if (x==mcount-1)  intat(12) = -1;
                 else intat(12) = multi[x+1];
    }
  else {
    x = FindInSingleTable(n);
    if (x==-1)  {
      printf("Cannot find record %d in single occurrence table.\n",n);
      error(ERR_BUMMER);
      }

    t=b=x;
    /* Find first and last with same date */
    while (t>=0 && DateEqual(t,x)) t--;
    if (!DateEqual(t,x)) t++;

    while (b<scount && DateEqual(b,x)) b++;
    if (!DateEqual(b,x)) b--;


    if (x==t)  intat(10) = -1;
          else intat(10) = single[x-1].record;

    if (x==b)  intat(12) = -1;
          else intat(12) = single[x+1].record;
    }
}

void PatchSingleOccurrence(int countonly,int *bytes)
{
  int x=-1;
  int z=0;

  if (!countonly) {
    bigbuf[0] = TYPE_USER;
    bigbuf[1] = STATUS_MODIFIED;
    bigbuf[4] = 1;
    bigbuf[5] = 0;
    }

  *bytes = 6;

  if (!scount)  return;

  for (z=0; z<scount; z++) {
    if (x==-1 || !DateEqual(x,z)) {
      if (!countonly) *(SingleOccur *)(bigbuf+*bytes) = single[z];
      x = z;
      *bytes += sizeof(SingleOccur);
      }
    }
}


void CopyDefaultApptInfo(int *bytes)
{
  int i;

  for (i=0; i<sizeof(DefaultApptInfo); i++)
    bigbuf[i] = DefaultApptInfo[i];

  *bytes = sizeof(DefaultApptInfo);
}


void PatchApptInfo(int singleoccurrencedays)
{
  if (mcount)
    intat(17) = multi[0];
  else
    intat(17) = -1;

  intat(19) = singleoccurrencedays;
}



void OutputRecords(void)
{
  unsigned int bytes;
  int i,x;

  printf("Creating output file...\n");

  _dos_lseek(handle2, 0L, SEEK_END);
  _dos_write(handle2, signature, 4, &bytes);
  if (bytes!=4)  error(ERR_WRITE);

  for (i=0; i<vsize; i++) {
    bytes = vlookup[i].size;
    fileseek = _dos_lseek(handle2, 0L, SEEK_END);

    if (bytes && vlookup[i].offset) {
      _dos_lseek(handle, vlookup[i].offset, SEEK_SET);
      if (_dos_read(handle, bigbuf, bytes, &junk)==-1 || junk!=bytes)
        error(ERR_READ);
      }

    switch (vlookup[i].type) {
      case TYPE_DBHEADER:
        if (ZapViewpts) {
          bigbuf[10]=bigbuf[11]=0;                  // Set current viewpt to 0
          }
        bigbuf[14]=bigbuf[15]=bigbuf[16]=bigbuf[17]=0;   // Zap lookup table
        break;
      case TYPE_LOOKUPTABLE:
        bytes=0;
        break;
      case TYPE_VIEWPTTABLE:
        if (ZapViewpts)
          bytes=0;
        else {
          bytes=2+6;
          bigbuf[0+6]=bigbuf[1+6]=0xFF;   /* Write out a single -1 */
          }
        break;
      case TYPE_VIEWPTDEF:
        if (ZapViewpts) bytes=0;
        break;
      case TYPE_DATA:
        if (FileType=='2')  PatchApptRecord(vlookup[i].record);

        /* If can't find the attached note, set it to -1 */
        if (noteoffset!=-1) {
          x = intat(noteoffset);
          if (FindNote(x)==-1) intat(noteoffset)=-1;
          }
        break;
      case TYPE_USER:
        if (vlookup[i].record)
          PatchSingleOccurrence(0,&bytes);
        else {
          if (!vlookup[i].offset)  CopyDefaultApptInfo(&bytes);
          PatchSingleOccurrence(1,&x);
          PatchApptInfo((x-6)/sizeof(SingleOccur));
          }
        break;
      }

    bigbuf[2] = bytes&0xFF;
    bigbuf[3] = bytes>>8;
    if (Debug)
      printf("%06lx: %s (%d) Len=%d\n",fileseek,recname[vlookup[i].type],vlookup[i].record,bytes);

    if (bytes) {
      if (_dos_write(handle2, bigbuf, bytes, &junk)==-1 || junk!=bytes)
        error(ERR_WRITE);
      }
    }
}


void main(int argc, char *argv[])
{
  int i;

  printf(StartupMsg);

  if (argc<3)  error(ERR_BADFILE);
  if (argc>4)  error(ERR_TOOMANYFILES);

  Debug=0;
  ZapViewpts=0;


  for (i=1; i<=argc; i++) {
    if (argv[i][0]=='/' || argv[i][0]=='-') {
      if (argv[i][1]=='D' || argv[i][1]=='d') Debug=1;
      if (argv[i][1]=='V' || argv[i][1]=='v') ZapViewpts=1;
      }
    }

  printf("Reading Database file %s\nOutputting to Reconstructed Database %s\n\n",argv[ZapViewpts+Debug+1],argv[ZapViewpts+Debug+2]);

  if (ZapViewpts)  printf("REMOVING ALL SUBSETS (VIEWPTDEFS/VIEWPTTABLES)\n\n");

  if (_dos_open(argv[ZapViewpts+Debug+1],O_RDONLY,&handle)==-1)  error(ERR_BADFILE);
  if (_dos_creat(argv[ZapViewpts+Debug+2],0,&handle2)==-1)  error(ERR_BADFILE);

  filesize = _dos_lseek(handle, 0L, SEEK_END);

  ConstructVirtualLookup();

  PatchVirtualLookup();

  ElimDups();

  if (FileType=='2') {
    ReconstructMulti();
    ReconstructSingle();
    }

  PatchNotes();

  OutputRecords();

  if (ZapViewpts) {
    printf("Adding default viewpoint definition and table...\n");
    _dos_write(handle2,DefaultViewptInfo,sizeof(DefaultViewptInfo),&i);
    if (i!=sizeof(DefaultViewptInfo)) error(ERR_WRITE);
    }


  _dos_close(handle);
  _dos_close(handle2);
}

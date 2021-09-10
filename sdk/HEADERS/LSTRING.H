#ifndef LPSTR
typedef char far *LPSTR;
#endif

int far lstrlen(LPSTR String);
int far lstrcpy(LPSTR Dest, LPSTR Source);
int far lstrcpyto(LPSTR Dest, LPSTR Source, int maxsize);
void far lstrcat(LPSTR Dest, LPSTR Source);
void far lstrcatto(LPSTR Dest, LPSTR Source, int maxsize);
void far lstrdel(LPSTR String, int pos, int count);
void far lstrinsch(LPSTR String, int pos, char ch);
void far lstrpad(LPSTR String, int len);
int far lstrcmp(LPSTR String1, LPSTR String2);
int far lstrcmpi(LPSTR String1, LPSTR String2);
void far lmemset(void far *dest, char c, unsigned int size);
int far lstrcontains(LPSTR string,LPSTR test);
int far lstrstrip(LPSTR dest,LPSTR src);
//int far lstrstripto(LPSTR Dest, LPSTR Source, int maxsize);
int far lstrcpy_nonull(LPSTR Dest, LPSTR Source);
void far lstrcenter(LPSTR Dest, LPSTR Source, int destsize);

/**** _dos_creat type CONSTANTS--COPIED OUT OF MS HEADER FILE dos.h ****/

#define _A_NORMAL       0x00    /* Normal file - No read/write restrictions */
#define _A_RDONLY	0x01	/* Read only file */
#define _A_HIDDEN	0x02	/* Hidden file */
#define _A_SYSTEM	0x04	/* System file */
#define _A_VOLID	0x08	/* Volume ID file */
#define _A_SUBDIR	0x10	/* Subdirectory */
#define _A_ARCH         0x20    /* Archive file */


/**** _dos_open mode CONSTANTS--COPIED OUT OF fcntl.h ****/

#define O_RDONLY        0x0000  /* open for reading only */
#define O_WRONLY	0x0001	/* open for writing only */
#define O_RDWR		0x0002	/* open for reading and writing */


/**** _dos_lseek CONSTANTS--COPIED OUT OF stdio.h ****/
#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0


#ifndef DB_INTERNAL

long int far _dos_lseek(int handle, long int pos, int mode);
int far _dos_creat(char far *filename, int type, int far *handle);
int far _dos_open(char far *filename, int mode, int far *handle);
int far _dos_read(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
int far _dos_write(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
int far _dos_close(int handle);

int far _dos_copy(char *tofile, char *fromfile);
int far _dos_erase(char *filename);
int far _dos_dup(int handle);
int far _dos_qualifypath(char far *topath, char far *frompath);
int far _dos_rename(char far *newname, char far *oldname);
int far _dos_chmod(char far *filename, unsigned int fileattribs);

#else



long int far _dos_lseek1(int handle, long int pos, int mode);
long int far _dos_lseek2(int handle, long int pos, int mode);
#define _dos_lseek(handle, pos, mode) \
        (DB_OVL==1 ? _dos_lseek1((handle),(pos),(mode)):\
                     _dos_lseek2((handle),(pos),(mode)))

int far _dos_creat1(char far *filename, int type, int far *handle);
int far _dos_creat2(char far *filename, int type, int far *handle);
#define _dos_creat(filename, type, handle) \
        (DB_OVL==1 ? _dos_creat1((filename),(type),(handle)):\
                     _dos_creat2((filename),(type),(handle)))

int far _dos_open1(char far *filename, int mode, int far *handle);
int far _dos_open2(char far *filename, int mode, int far *handle);
#define _dos_open(filename, mode, handle) \
        (DB_OVL==1 ? _dos_open1((filename),(mode),(handle)):\
                     _dos_open2((filename),(mode),(handle)))

int far _dos_read1(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
int far _dos_read2(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
#define _dos_read(handle, buffer, size, retsize) \
        (DB_OVL==1 ? _dos_read1((handle),(buffer),(size),(retsize)):\
                     _dos_read2((handle),(buffer),(size),(retsize)))

int far _dos_write1(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
int far _dos_write2(int handle, void far *buffer, unsigned int size, unsigned int far *retsize);
#define _dos_write(handle, buffer, size, retsize) \
        (DB_OVL==1 ? _dos_write1((handle),(buffer),(size),(retsize)):\
                     _dos_write2((handle),(buffer),(size),(retsize)))

int far _dos_close1(int handle);
int far _dos_close2(int handle);
#define _dos_close(handle) \
        (DB_OVL==1 ? _dos_close1(handle):_dos_close2(handle))

int far _dos_dup1(int handle);
int far _dos_dup2(int handle);
#define _dos_dup(handle) \
        (DB_OVL==1 ? _dos_dup1(handle):_dos_dup2(handle))

int far _dos_qualifypath1(char far *topath, char far *frompath);
#define _dos_qualifypath(topath, frompath) \
        (DB_OVL==1 ? _dos_qualifypath1((topath),(frompath)) : \
                     printf("YOU CANNOT USE _DOS_QUALIFY_PATH FROM OVL 2"))


#endif

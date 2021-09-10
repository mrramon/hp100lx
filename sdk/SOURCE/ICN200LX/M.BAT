call clearobj icn200lx.obj

cl /c /Gs1 icn200lx.c            >icn200lx.log
link icn200lx.obj+cougraph.obj; >>icn200lx.log
pklite icn200lx.exe             >>icn200lx.log

call examine icn200lx.log
call update icn200lx.exe ..\..\build\all\romdisk\bin

#********************************************************************#
#                                                                    #
#------------------- COUGAR GENERAL MAKE FILE -----------------------#
#                                                                    #
#********************************************************************#

#********************************************************************
#
# This file remains the same for all executables produced for Cougar.
# Before this file is included into a specific make file, the equates
# APP_LIBS and EXEFILE must be defined that specify the applications
# libraries and executeable.
#
# This make file contains the general make rules, and the different
# flags, etc. for each different type of build.
#
# To specify a special build, type  "nmk OPT=name" and the
# build "name" will be created (the available build names are
# tko, cvo, emul, semul, real and official).  Running nmk with no parameter
# builds the default executeable for the PC platform.
#
# The equate BUILD_TOOLS if set will build the tool libraries (eg.
# the system object and CAP TSR libraries) for system building.
# For normal development this macro should not be set.
#
#
# APP_LIBS:  The library containing the main executeable MUST be the
#            first library on the line.
#
# EXEFILE:   The file that the make process will produce.
#
# Optional defines:
#
# MASMOPTS:  Additional command line options for assembly code.
#
# COPTS:     Additional command line options for C code.
#
# LOPTS:     Additional linker options.
#
# NOCAP is set to 1 if you don't want the CAP.LIB to be linked
#
# LIBTYPE is set to a three letter extension that can be used to
# derive different libs for different versions.
#
# WARNING_LEVEL defaults to 1 if not set before cougar.mak is included
#
# STACK_SIZE defaults to 0x1000 (4K) if not set.
#
#********************************************************************

DEBUG   =1
.SUFFIXES:
.SUFFIXES:  .c .asm .h .inc .mac .obj

#*******************************************************************
#
# Set up paths -- If an official build, use no paths, and get all
#   files from the current directory (default);  if a developer's
#   build (DEVELOPER set to 1) use the variables LIB_PATH and
#   INC_PATH to get libraries and include files.  If LIB_PATH or
#   INC_PATH aren't set, they are set to "$(BUILD_PATH)\libs" and
#   "$(BUILD_PATH)\headers" respectively.  If the paths are set,
#   they must not end with a backslash.
#
#*******************************************************************
!IFNDEF BUILD_PATH
BUILD_PATH=C:\COUGAR\BUILD
!ENDIF


!IFDEF DEVELOPER

!IFNDEF LIB_PATH
LIB_PATH=$(BUILD_PATH)\libs
!ENDIF

!IFNDEF INC_PATH
INC_PATH=$(BUILD_PATH)\headers
!ENDIF

!ELSE

LIB_PATH=.
INC_PATH=.

!ENDIF


!IFNDEF WARNING_LEVEL
WARNING_LEVEL=1
!ENDIF

!IFNDEF STACK_SIZE
STACK_SIZE=0x1000
!ENDIF

#*******************************************************************
#
# tko - TKernel Optimized version, no CodeView info
#
#*******************************************************************
!IF "$(OPT)"=="tko"
AFLAGS   = /ml /DPCKEYBOARD /DTKERNEL /DAPI2 $(MASMOPTS)
MLFLAGS  = /Cp /c /DPCKEYBOARD /DTKERNEL /DAPI2 $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /DPCKEYBOARD /DTKERNEL /DAPI2 /G1 /Oswl /Gs $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /SE:0x80 /ST:$(STACK_SIZE) $(LOPTS)
CAP_NAME = captko.lib
LIBTYPE  = tko
SYS_LIBS = $(LIB_PATH)^\tsr.lib $(LIB_PATH)^\mlhstub.lib
!ENDIF

#*******************************************************************
#
# cvo - CodeView enabled TKernel Optimized version 
#
#*******************************************************************
!IF "$(OPT)"=="cvo"
AFLAGS   = /ml /Zi /Zd /DPCKEYBOARD /DTKERNEL /DAPI2 $(MASMOPTS)
MLFLAGS  = /Cp /c /Zi /Zd /DPCKEYBOARD /DTKERNEL /DAPI2 $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /DPCKEYBOARD /DTKERNEL /DAPI2 /Oswl /G1 /Gs /Zi /Zd $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /SE:0x80 /ST:$(STACK_SIZE) /CO $(LOPTS)
CAP_NAME = captko.lib
LIBTYPE  = cvo
SYS_LIBS = $(LIB_PATH)^\tsr.lib $(LIB_PATH)^\mlhstub.lib
!ENDIF

#*******************************************************************
#
# emul - Emulator version, optimized
#
#*******************************************************************
!IF "$(OPT)"=="emul"
AFLAGS   = /ml /DPCKEYBOARD /DEMULATOR /DNDEBUG /DAPI2 $(MASMOPTS)
MLFLAGS  = /Cp /c /DPCKEYBOARD /DEMULATOR /DNDEBUG /DAPI2 $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /Oswl /Gs /G1 /DNDEBUG /DEMULATOR /DPCKEYBOARD /DAPI2 $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /NOD /MAP /SE:0x80 /ST:$(STACK_SIZE) $(LOPTS)
CAP_NAME = capemu.lib
LIBTYPE  = emu
SYS_LIBS = $(LIB_PATH)^\bb.lib $(LIB_PATH)^\mlhstub.lib
!ENDIF

#*******************************************************************
#
# semul - Emulator version, optimized with symbolic info
#
#*******************************************************************
!IF "$(OPT)"=="semul"
AFLAGS   = /ml /DPCKEYBOARD /DEMULATOR /DAPI2 $(MASMOPTS)
MLFLAGS  = /Cp /c /DPCKEYBOARD /DEMULATOR /DAPI2 $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /Oswl /Gs /G1 /Zi /Zd /DEMULATOR /DPCKEYBOARD /DAPI2 $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /NOD /MAP /CO /LI /SE:0x80 /ST:$(STACK_SIZE) $(LOPTS)
CAP_NAME = capemu.lib
LIBTYPE  = sem
SYS_LIBS = $(LIB_PATH)^\bb.lib $(LIB_PATH)^\mlhstub.lib
!ENDIF


#*******************************************************************
#
# real (and official) - Real Cougar hardware version
#
# official build will create the tool libraries
#*******************************************************************
!IF "$(OPT)"=="real" || "$(OPT)"=="official"
AFLAGS   = /ml /DAPI2 /DNDEBUG /DOFFICIAL $(MASMOPTS)
MLFLAGS  = /Cp /c /DAPI2 /DNDEBUG /DOFFICIAL $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /Oswl /Gs /G1 /DNDEBUG /DAPI2 /DOFFICIAL $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /NOD /MAP /SE:0x80 /ST:$(STACK_SIZE) $(LOPTS)
CAP_NAME = caprea.lib
LIBTYPE  = rea
SYS_LIBS = $(LIB_PATH)^\bb.lib $(LIB_PATH)^\mlhstub.lib
!IF "$(OPT)"=="official"
BUILD_TOOLS = 1
!ENDIF
!ENDIF

#*******************************************************************
#
# default load type - non-optimized tkernel PC load w/codeview info
#
#*******************************************************************
!IFNDEF SYS_LIBS
AFLAGS   = /ml /DPCKEYBOARD /DTKERNEL /DAPI2
MLFLAGS  = /Zi /Cp /c /DPCKEYBOARD /DTKERNEL /DAPI2 $(MASMOPTS)
CFLAGS   = /AS /W$(WARNING_LEVEL) /Ze /Zi /Od /Gs /G1 /DPCKEYBOARD /DTKERNEL /DAPI2 $(COPTS)
LFLAGS   = /CP:0xffff /NOI /NOE /SE:0x80 /ST:$(STACK_SIZE) /CO $(LOPTS)
CAP_NAME = captko.lib
LIBTYPE  = tko
SYS_LIBS = $(LIB_PATH)^\tsr.lib $(LIB_PATH)^\mlhstub.lib
!ENDIF

#*******************************************************************
#
# INCFILES is a list of all .inc files to be created from existing
#          .h files--the .h files must be in the current directory.
#          The .inc files supplied by the application are always
#          built if needed; if DEVELOPER is not defined, additional
#          system .inc files are also built.
#*******************************************************************

!IFNDEF DEVELOPER
INCFILES = $(INCFILES) lhapi.inc
!ELSE
!IFNDEF INCFILES
INCFILES =
!ENDIF
!ENDIF

##*******************************************************************##
##*******************************************************************##
## Build of the Actual EXE                                           ##
## -----------------------                                           ##
## Here are the build commands. Official builds should compile all   ##
## files for safety, so make sure all objs are deleted before        ##
## running.  tsr.lib contains lotus supplied startup code.           ##
##                                                                   ##
##*******************************************************************##
#********************************************************************##

!IFDEF NOCAP
$(EXEFILE): $(INCFILES) $(APP_LIBS) $(SYS_LIBS)
        link $(LFLAGS) @<<
$(APP_LIBS),$(EXEFILE),,$(SYS_LIBS);
<<NOKEEP
!ELSE
$(EXEFILE): $(INCFILES) $(APP_LIBS) $(SYS_LIBS) $(LIB_PATH)\$(CAP_NAME)
        link $(LFLAGS) @<<
$(APP_LIBS),$(EXEFILE),,$(SYS_LIBS) $(LIB_PATH)\$(CAP_NAME);
<<NOKEEP
!ENDIF

# ALTERNATIVE VERSION OF ABOVE--USED WHEN LIB LINE GROWS TOO LONG
#$(LIB_PATH)\$(CAP_NAME)
#$(EXEFILE)
#
#$(APP_LIBS: =+^
#)+
#$(SYS_LIBS: =+^
#)
#
#<<NOKEEP

#*******************************************************************
#
# Here are the make rules for the various types of source.
# Individual overrides to obtain different options occur after
# make rules for app libraries.
#
#********************************************************************
.c.obj:
        cl /c /I$(INC_PATH) $(CFLAGS) $*.c
.asm.obj:
        ml /I$(INC_PATH) $(MLFLAGS) $*.asm
.h.inc:
        h2inc /C $*.h

#.obj.exe:
#        link $(LFLAGS) @<<
#$**, $@,, $(APP_LIBS) $(SYS_LIBS) $(LIB_PATH)^\$(CAP_NAME);
#<<NOKEEP

#.obj.lib:
#        del $@
#        lib @<<
#$* /NOI +$(**: =&^
#+);
#<<NOKEEP



#*******************************************************************
#
# Overrides of individual make rules.
#
#********************************************************************

!IFNDEF NOCAP

#csvc.obj: csvc.asm
#        masm $(AFLAGS) /Mx $*.asm;

crt0.obj: crt0.asm
        masm $(AFLAGS) /Mx $*.asm;

crt0dat.obj: crt0dat.asm
        masm /Mx $*.asm;

card.obj: card.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

#pushb.obj: pushb.c
#        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

caperror.obj: caperror.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

#fileopen.obj: fileopen.c
#        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

disp.obj: disp.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

exists.obj: exists.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

#_combobx.obj: _combobx.c
#        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

#_listbox.obj: _listbox.c
#        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

cathand.obj: cathand.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

d_edit.obj: d_edit.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

t_edit.obj: t_edit.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

n_edit.obj: n_edit.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

atoi.obj: atoi.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

index.obj: index.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

caputil.obj: caputil.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

lstring.obj: lstring.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

lstringe.obj: lstringe.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

note.obj: note.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

find.obj: find.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

arrange.obj: arrange.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

viewpt.obj: viewpt.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

link.obj: link.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

graychck.obj: graychck.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

grayrad.obj: grayrad.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c

showhour.obj: showhour.c
        cl /c /I$(INC_PATH) $(CFLAGS) /NTCAPSEG $*.c


!ENDIF


#********************************************************************
#
# Make for the system-wide libraries
#
#  CAP.LIB : linked with all CAP compliant programs
#  TSR.LIB : linked with emulator versions of program on PC
#  BB.LIB  : linked with real builds on target hardware
#
#********************************************************************
# Note: The "magic macro" $(**: =+) in the link line works like this:
#   $** returns all the dependent filenames.
#   $(<macroname>:<findstring>=<replacestring>) returns <macroname>
#       with all occurrences of <findstring> changed to <replacestring>.
#   $(**: =+)  returns all dependents connected by pluses instead of spaces
#********************************************************************

!IFDEF BUILD_TOOLS

$(CAP_NAME): cougraph.obj disp.obj chtype.obj lstring.obj lstringe.obj card.obj d_edit.obj t_edit.obj n_edit.obj atoi.obj exists.obj \
             link.obj cathand.obj index.obj caputil.obj note.obj viewpt.obj find.obj arrange.obj showhour.obj drawtext.obj fontnorm.obj clock.obj grayrad.obj graychck.obj
        del $(CAP_NAME)
        lib @<<
$(CAP_NAME) /NOI +$(**: =&^
+);
<<NOKEEP


tsr.lib: mcsvc_hp.lib
        del tsr.lib
        lib @<<
tsr /NOI +$(**: =&^
+);
<<NOKEEP

bb.lib: mcsvc_hp.lib crt0.obj
        del bb.lib
        lib @<<
bb /NOI +$(**: =&^
+);
<<NOKEEP

!ENDIF

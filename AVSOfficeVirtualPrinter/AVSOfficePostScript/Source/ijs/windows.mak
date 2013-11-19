# MS-Windows
CC=cl
CFLAGS=-W3 -Zi
LDLIBS=
OBJ=.obj
EXE=.exe
FE=/Fe
IJS_EXEC_SERVER=ijs_exec_win$(OBJ)
RM=-del

!include "common.mak"

clean: common_clean
	$(RM) *.ilk *.pdb *.opt

# Linux
CC=gcc
CFLAGS=-g -Wall -ansi -pedantic -Wmissing-prototypes
LDLIBS=
OBJ=.o
EXE=
FE=-o 
IJS_EXEC_SERVER=ijs_exec_unix$(OBJ)
RM=rm -f

include common.mak

clean: common_clean
	$(RM) *~ gmon.out core ijs_spec.log ijs_spec.tex ijs_spec.aux

ijs_spec.pdf:	ijs_spec.sgml
	db2pdf ijs_spec.sgml

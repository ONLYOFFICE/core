# Unix makefile for the JBIG-KIT library
# $Id: Makefile,v 1.9 2004-06-08 15:40:06+01 mgk25 Exp $

# Select an ANSI/ISO C compiler here, GNU gcc is recommended
CC = gcc

# Options for the compiler: A high optimization level is suggested
CFLAGS = -O -Wall -ansi -pedantic

all: libjbig.a tstcodec

tstcodec: tstcodec.c jbig.c jbig.h jbig_tab.o
	$(CC) $(CFLAGS) -o tstcodec -DTEST_CODEC tstcodec.c jbig.c \
	jbig_tab.o

libjbig.a: jbig.o jbig_tab.o
	rm -f libjbig.a
	ar rc libjbig.a jbig.o jbig_tab.o
	-ranlib libjbig.a

jbig.o: jbig.c jbig.h

test: tstcodec
	./tstcodec

t82test.pbm: tstcodec
	./tstcodec $@

clean:
	rm -f *.o *~ core gmon.out dbg_d\=??.pbm tstcodec t82test.pbm

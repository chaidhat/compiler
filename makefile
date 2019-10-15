# thank you to  http://nuclear.mutantstargoat.com/articles/make
#               https://www.gnu.org/software/make/manual/make.html#Using-Implicit
# for the initial makefile template - this is my first time!

CC=gcc
CFLAGS=-Werror
ECC=$(BDIR)/./mcc
# TESTCMD= -v -D c 1 -I test/u.mh
TESTCMD = -v -D c 1 -E -fdast

_SRC := $(wildcard *.c)
_OBJ := $(_SRC:.c=.o) 

BDIR = bin
TDIR = test

$(BDIR)/mcc: $(_OBJ)
	$ mkdir -p bin 
	$(CC) -o $@ $^

# automatically makes, self-test and clean
all:
	$(MAKE) clean
	$(MAKE) 
	$(MAKE) clean
	$(MAKE) test1

test1:
	$(info ***test1***)
	@for f in $(wildcard $(TDIR)/*.minc); do echo A ; echo A ; echo A ; echo A NEXT FILE $${f} ; echo A ; echo A ; echo A ; $(ECC) $(TESTCMD) $${f} ; done
    
clean:
	rm -f *.o *~ core $(INCDIR)/*~ 

.PHONY: all test1 clean

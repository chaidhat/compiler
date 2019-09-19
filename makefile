# thank you to  http://nuclear.mutantstargoat.com/articles/make
#               https://www.gnu.org/software/make/manual/make.html#Using-Implicit
# for the initial makefile template - this is my first time!

CC=cc
ECC=$(BDIR)/./mcc

_SRC := $(wildcard *.c)
_OBJ := $(_SRC:.c=.o) 

BDIR = bin
TDIR = test

$(BDIR)/mcc: $(_OBJ)
	$(CC) -o $@ $^

# automatically makes, self-test and clean
all:
	$(MAKE) 
	$(MAKE) test1
	$(MAKE) clean

test1:
	$(info ***test1***)
	$(ECC) -v $(wildcard $(TDIR)/*.mc)
    
clean:
	rm -f *.o *~ core $(INCDIR)/*~ 

.PHONY: all test1 clean

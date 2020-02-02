# thank you to  http://nuclear.mutantstargoat.com/articles/make
#               https://www.gnu.org/software/make/manual/make.html#Using-Implicit
# for the initial makefile template - this is my first time!

CC=gcc
CFLAGS=-Werror -Os
ECC=$(BDIR)/mcc
TESTCMD = -g -S -a -D c 1

_SRC := $(wildcard *.c)
_OBJ := $(_SRC:.c=.o) 

BDIR = bin
TDIR = test

$(BDIR)/mcc: $(_OBJ)
	$(CC) -o $@ $^
	$(MAKE) clean

# automatically makes, self-test and clean
all:
	$(MAKE) clean
	$(MAKE) 
	$(MAKE) clean
	$(MAKE) test

run:
	$(MAKE) clean
	$(MAKE) 
	$(MAKE) clean
	$(foreach file,$(wildcard $(TDIR)/*.mc), \
		$(info FILE $(file)) \
		start powershell -command ""$(ECC)" $(TESTCMD) $(file) ; type "$(basename $(file)).s" ; pause"\
	)

test:
	start cmd /K "cd $(TDIR) && test.bat"
    
clean:
	del *.o

.PHONY: all run test clean

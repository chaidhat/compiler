# MinimalistiC Compiler - Makefile
# Target: ARM64 (Apple Silicon)

CC=gcc
CFLAGS=-Werror -Os -fcommon
BDIR=bin
TDIR=test

_SRC := $(filter-out gen_x86.c, $(wildcard *.c))
_OBJ := $(_SRC:.c=.o)

$(BDIR)/mcc: $(_OBJ) | $(BDIR)
	$(CC) -o $@ $^ -lm

$(BDIR):
	mkdir -p $(BDIR)

all: clean $(BDIR)/mcc

run: $(BDIR)/mcc
	@for f in $(TDIR)/*.mc $(TDIR)/*.mcc; do \
		if [ -f "$$f" ]; then \
			echo "=== $$f ==="; \
			$(BDIR)/mcc -g -S -a "$$f" || true; \
		fi; \
	done

test: $(BDIR)/mcc
	$(BDIR)/mcc -g -S -a $(TDIR)/4_scope.mcc

clean:
	rm -f *.o

.PHONY: all run test clean

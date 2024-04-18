IDIR=include
CC=gcc
CFLAGS=-g3 -I$(IDIR) -Wall -DDEBUG

ODIR=obj
SRCDIR=src

LIBS=-lm

_DEPS = cpu.h instruction.h parser.h ram.h regmap.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = cpu.o parser.o ram.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/main.o: main.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

riscv_emu: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	-rm -f $(ODIR)/*.o *~ riscv_emu $(INCDIR)/*~ 
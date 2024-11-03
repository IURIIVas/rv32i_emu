IDIR=include
CC=gcc
CFLAGS=-g3 -I$(IDIR) -Wall -DDEBUG

ODIR=obj
SRCDIR=src
UNITY_DIR = test/unity
TESTDIR = test

LIBS=-lm

SRC = $(foreach dirname, $(SRCDIR), $(wildcard $(dirname)/*.c))
DEPS = $(wildcard $(IDIR)/*.h)
OBJ = $(foreach filename, $(notdir $(SRC)), $(ODIR)/$(filename:.c=.o))

UNITY_SRC = $(foreach dirname, $(UNITY_DIR), $(wildcard $(dirname)/*.c))
UNITY_DEPS = $(wildcard test/unity/*.h)
UNITY_OBJ = $(foreach filename, $(notdir $(UNITY_SRC)), $(ODIR)/$(filename:.c=.o))

TEST_SRC = $(foreach dirname, $(TESTDIR), $(wildcard $(dirname)/*.c))

VPATH = $(sort $(SRCDIR) $(foreach srcfile,$(SRC),$(dir $(srcfile)))) 
VPATH += $(sort $(UNITY_DIR) $(foreach srcfile,$(UNITY_SRC),$(dir $(srcfile))))
VPATH += $(sort $(TESTDIR) $(foreach srcfile,$(TEST_SRC),$(dir $(srcfile))))

check:
	@echo $(VPATH)
	@echo $(OBJ)

$(ODIR)/%.o: %.c $(DEPS) $(UNITY_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir -p $@

test_rtype: $(OBJ) $(UNITY_OBJ) $(ODIR)/test_rtype_instr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test_itype: $(OBJ) $(UNITY_OBJ) $(ODIR)/test_itype_instr.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

riscv_emu: $(OBJ) main.o
	$(CC) $(CFLAGS) -o $@ $^  $(LIBS)

.PHONY: clean

clean:
	-rm -f $(ODIR)/*.o *~ riscv_emu $(INCDIR)/*~ 
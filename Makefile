CC       = g++
CCFLAGS   = -g -Wall -Werror -Wextra -pedantic -std=c++11 -Wno-unused-parameter

LFLAGS   = -lrt -lm

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
BINFILES  := $(SOURCES:$(SRCDIR)/%.c=$(BINDIR)/%)
rm       = rm -f

all: $(BINFILES)

$(BINFILES): $(BINDIR)/% : $(SRCDIR)/%.c
	$(CC) $(CCFLAGS) $(LFLAGS) $< -o $@

.PHONY: clean
clean:
	$(rm) $(BINFILES)

.PHONY: remove
remove: clean
	$(rm) $(BINDIR)

.PHONY: demo
demo:
	./${BINDIR}/${TARGET} -a -v && echo "\n" && \
	./${BINDIR}/${TARGET} -v

.PHONY: rle
rle: $(SRCDIR)/rle.c
	$(CC) $(CCFLAGS) $(LFLAGS) $< -o ./${BINDIR}/$@

.PHONY: mtf
mtf: $(SRCDIR)/mtf.c
	$(CC) $(CCFLAGS) $(LFLAGS) $< -o ./${BINDIR}/$@

.PHONY: bwt
bwt: $(SRCDIR)/bwt.c
	$(CC) $(CCFLAGS) $(LFLAGS) $< -o ./${BINDIR}/$@
